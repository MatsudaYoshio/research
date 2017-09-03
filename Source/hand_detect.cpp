#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <thread>
#include <mutex>
#include <cstdio>
#include <random>

/* dlib */
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/opencv.h>
#include <dlib/svm.h>
#include <dlib/dir_nav.h>
#include <dlib/svm_threaded.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "SlidingWindows.h"
#include "NonMaximumSuppression.h"
#include "ColorParam.h"

using namespace std;
using namespace dlib;
using namespace cv;

class Action {
private:
	using X_type = std::map<unsigned long, double>; // 特徴ベクトルの型
	using kernel_type = sparse_linear_kernel<X_type>; // カーネルの型
	using fhog_type = array2d<matrix<double, 31, 1>>; // fhog特徴量の型
	using track_data_type = struct {
		dlib::rectangle hand, face;
		Point past_pointer, current_pointer;
		drectangle current_pos;
		bool tracking_flag;
		std::vector<pair<int, dlib::rectangle>> track_hand_dets;
	};

	static constexpr int resize_size = 80;
	static constexpr int window_width = 640;
	static constexpr int window_height = 480;
	static constexpr double decision_ratio = 0.5;
	static constexpr double overlap_ratio = 0.1;

	track_data_type track_assignment[5];
	std::vector<dlib::rectangle> face_dets_buf, face_dets, hand_dets, hand_track_rects;
	frontal_face_detector face_detector; // 正面顔検出器
	decision_function<kernel_type> df; // 決定境界の関数
	array2d<unsigned char> image_gs;
	array2d<bgr_pixel> image_org;
	bool face_thread_flag;
	bool hand_thread_flag;
	bool stop_flag;
	long long int frame_count; // オーバーフローに気を付ける
	Mat frame, view_frame;
	NonMaximumSuppression nms;
	VideoCapture cap;
public:
	Action() :nms(this->overlap_ratio), face_thread_flag(false), hand_thread_flag(false), stop_flag(false), frame_count(0) {
		this->face_detector = get_frontal_face_detector();

		deserialize("D:/machine_learning_data/hand/20170813/linear_svm_function.dat") >> df; // ファイルから学習済みのモデルを読み込む

		for (int i = 0; i < 5; ++i) {
			this->track_assignment[i].tracking_flag = false;
			this->track_assignment[i].hand.set_left(-1);
		}

		this->frame = Mat(Size(this->window_width, this->window_height), CV_8UC3);
		
		cap.open(0);
		if (!cap.isOpened()) {
			cerr << "Unable to open the device 0" << endl;
			exit(-1);
		}
	}

	void loop() {
		while (1) {

			++this->frame_count;

			this->cap >> this->frame; // カメラから画像を取得

			assign_image(this->image_org, cv_image<bgr_pixel>(this->frame));
			assign_image(this->image_gs, this->image_org);
			
			this->new_thread_face_detect();
			if (!this->face_dets.empty()) {
				this->new_thread_hand_detect();
			}

			this->show_detect_window();

			if (cv::waitKey(1) == 'q') break;
		}

		this->stop_flag = true;
	}

private:
	void show_detect_window() {

		this->view_frame = this->frame;

		parallel_for(0, this->hand_dets.size(), [&](long i) {
			cv::rectangle(view_frame, Point(this->hand_dets[i].left(), this->hand_dets[i].top()), Point(this->hand_dets[i].right(), this->hand_dets[i].bottom()), ColorParam::RED, 5);
		});

		parallel_for(0, this->face_dets_buf.size(), [&](long i) {
			cv::rectangle(view_frame, Point(this->face_dets_buf[i].left(), this->face_dets_buf[i].top()), Point(this->face_dets_buf[i].right(), this->face_dets_buf[i].bottom()), ColorParam::BLUE, 5);
		});

		parallel_for(0, 5, [&](long i) {
			if (this->track_assignment[i].tracking_flag) {
				cv::rectangle(view_frame, Point(this->track_assignment[i].current_pos.left(), this->track_assignment[i].current_pos.top()), Point(this->track_assignment[i].current_pos.right(), this->track_assignment[i].current_pos.bottom()), ColorParam::ORANGE, 5);
			}
		});
		imshow("detect window", view_frame);
	}

	/* 顔検出 */
	void face_detect() {
		this->face_thread_flag = true;
		this->face_dets.clear();

		this->face_dets_buf = this->face_detector(this->image_gs);

		/* 既に追跡している顔の近くの顔を除く */
		for (auto r : this->face_dets_buf) {
			bool flag = false;
			for (int i = 0; i < 5; ++i) {
				if (this->track_assignment[i].tracking_flag && this->euclid_distance((this->track_assignment[i].face.left() + this->track_assignment[i].face.right()) / 2, (this->track_assignment[i].face.top() + this->track_assignment[i].face.bottom()) / 2, (r.left() + r.right()) / 2, (r.top() + r.bottom()) / 2) < 400) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				this->face_dets.emplace_back(r);
			}
		}

		if (!this->face_dets.empty()) { // 検出した顔があれば
			this->new_thread_hand_detect(); // 手検出スレッドを起動
		}

		this->face_thread_flag = false;
	}

	/* 手検出 */
	void hand_detect() {
		this->hand_thread_flag = true;
		
		this->hand_dets.clear();

		std::vector<dlib::rectangle> face_dets_tmp = this->face_dets;
		const int face_num = face_dets_tmp.size(); // 検出された顔の数
		std::vector<dlib::rectangle> windows; // スライディングウィンドウ用の矩形
		array2d<unsigned char> roi;
		std::vector<dlib::rectangle> hand_dets_tmp, hand_dets_tmp2;
		
		for (int i = 0; i < face_num; ++i) {
			/* 顔の周辺のスライディングウィンドウを作成 */
			SlidingWindows sw(200, 40, std::max((int)face_dets_tmp[i].left() - 200, 0), std::min((int)face_dets_tmp[i].right() + 200, this->window_width), std::max((int)face_dets_tmp[i].top() - 200, 0), std::min((int)face_dets_tmp[i].bottom() + 200, this->window_height));
			windows = sw.get_windows();

			/* スライディングウィンドウで検出 */
			for (auto w : windows) {
				extract_image_chip(this->image_gs, w, roi);
				if (this->is_hand(roi)) {
					hand_dets_tmp.emplace_back(w);
				}
			}

			/* 既に追跡している手の近くの手を除く */
			for (auto r : hand_dets_tmp) {
				bool flag = false;
				for (int i = 0; i < 5; ++i) {
					if (this->track_assignment[i].tracking_flag && this->euclid_distance((this->track_assignment[i].hand.left() + this->track_assignment[i].hand.right()) / 2, (this->track_assignment[i].hand.top() + this->track_assignment[i].hand.bottom()) / 2, (r.left() + r.right()) / 2, (r.top() + r.bottom()) / 2) < 400) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					hand_dets_tmp2.emplace_back(r);
				}
			}

			if (!hand_dets_tmp.empty()) { // 検出された手があれば

				/* NonMaximumSuppressionにかけて重複を取り除く */
				this->nms(hand_dets_tmp2, this->hand_dets);

				/* 顔と手を対応させて追跡を開始する */
				for (int j = 0; j < 5; ++j) {
					if (!this->track_assignment[j].tracking_flag) {
						this->track_assignment[j].tracking_flag = true;
						this->track_assignment[j].hand = this->hand_dets[0];
						this->track_assignment[j].face = face_dets_tmp[i];

						correlation_tracker ct;
						ct.start_track(this->image_org, this->hand_dets[0]);

						this->track_assignment[i].past_pointer = Point((this->hand_dets[0].left() + this->hand_dets[0].right()) / 2, (this->hand_dets[0].top() + this->hand_dets[0].bottom()) / 2);

						this->new_thread_tracking(ct, i);

						break;
					}
				}
			}
			
		}

		this->hand_thread_flag = false;
	}

	/* 追跡時用の手検出 */
	void hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int n) {
		array2d<unsigned char> roi;
		for (auto w : sliding_windows) {
			extract_image_chip(this->image_gs, w, roi);
			if (this->is_hand(roi)) {
				this->track_assignment[n].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
			}
		}
	}

	bool is_hand(array2d<unsigned char> &img) {
		array2d<unsigned char> img_resize(this->resize_size, this->resize_size);
		resize_image(img, img_resize);

		fhog_type fhog;
		extract_fhog_features(img_resize, fhog);

		X_type feature_vec;
		fhog_to_feature_vector(feature_vec, fhog);

		return (df(feature_vec) > this->decision_ratio);
	}

	void tracking(correlation_tracker &ct, int n) {
		while (1) {

			/* 直近のフレームで検出した手以外を消す */
			int m = 0;
			for (auto d : this->track_assignment[n].track_hand_dets) {
				if (d.first < this->frame_count - 30) {
					++m;
					continue;
				}
				break;
			}
			this->track_assignment[n].track_hand_dets.erase(begin(this->track_assignment[n].track_hand_dets), begin(this->track_assignment[n].track_hand_dets) + m);

			drectangle past_pos = ct.get_position(); // 直近フレームの手の位置を得る

			ct.update(this->image_org); // 追跡位置の更新
			
			/* 現在の追跡位置(矩形)を得る */
			drectangle current_pos = ct.get_position();
			this->track_assignment[n].hand = this->track_assignment[n].current_pos = current_pos;

			/* 現在の追跡位置の周辺のスライディングウィンドウを作成して手を検出 */
			SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max((int)current_pos.left() - 100, 0), std::min((int)current_pos.right() + 100, this->window_width), std::max((int)current_pos.top() - 100, 0), std::min((int)current_pos.bottom() + 100, this->window_height));
			this->hand_detect(local_sw.get_windows(), n);

			/* 現在の追跡位置(矩形の中心座標)を得る */
			double x = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x座標
			double y = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y座標

			Point2f cp(this->track_assignment[n].past_pointer.x - 10 * x, this->track_assignment[n].past_pointer.y + 10 * y); // 現在の追跡位置から相対的にポインタの位置を決定

			this->track_assignment[n].current_pointer = this->track_assignment[n].past_pointer = cp; // ポインタの位置を更新

			if (this->track_assignment[n].track_hand_dets.empty() || this->stop_flag) { // 直近フレームで手が検出されなかったら追跡をやめる
				this->track_assignment[n].tracking_flag = false;
				break;
			}
		}

	}

	void new_thread_tracking(correlation_tracker &ct, int n) {
		void(Action::*funcp)(correlation_tracker&, int) = &Action::tracking;
		thread th(funcp, this, ct, n);
		th.detach();
	}

	void new_thread_hand_detect() {
		if (!this->hand_thread_flag) {
			void(Action::*funcp)() = &Action::hand_detect;
			thread th(funcp, this);
			th.detach();
		}
	}

	void new_thread_face_detect() {
		if (!this->face_thread_flag) {
			void(Action::*funcp)() = &Action::face_detect;
			thread th(funcp, this);
			th.detach();
		}
	}

	void fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog) {
		for (int r = 0, j = 0; r < fhog.nr(); ++r) {
			for (int c = 0; c < fhog.nc(); ++c) {
				for (int i = 0; i < 31; ++i) {
					feature_vector.insert(make_pair(j++, fhog[r][c](i)));
				}
			}
		}
	}

	double euclid_distance(const double x1, const double y1, const double x2, const double y2) const {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}
};

int main() {

	Action act;

	act.loop();

	return 0;
}