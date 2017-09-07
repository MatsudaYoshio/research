#include "HandPointer.h"

using namespace std;
using namespace dlib;
using namespace cv;

HandPointer::HandPointer() :nms(this->overlap_ratio), face_thread_flag(false), hand_thread_flag(false), stop_flag(false), frame_count(0), track_id(0), mt(rd()) {
	this->face_detector = get_frontal_face_detector();

	deserialize("C:/Users/matsuda/Desktop/20170813/linear_svm_function.dat") >> df; // ファイルから学習済みのモデルを読み込む

	this->frame = Mat(Size(this->window_width, this->window_height), CV_8UC3);

	this->pointer_color_list.emplace_back(ofColor::blue);
	this->pointer_color_list.emplace_back(ofColor::red);
	this->pointer_color_list.emplace_back(ofColor::green);
	this->pointer_color_list.emplace_back(ofColor::orange);
	this->pointer_color_list.emplace_back(ofColor::yellow);
	this->pointer_color_list.emplace_back(ofColor::black);
	this->pointer_color_list.emplace_back(ofColor::pink);
	this->pointer_color_list.emplace_back(ofColor::purple);
	this->rn_color = uniform_int_distribution<int>(0, this->pointer_color_list.size());
}

void HandPointer::update() {
	++this->frame_count;

	frc.NewFrame();
	printf("fps : %lf\n", frc.GetFrameRate());

	this->frame = this->cap.get_image(); // カメラから画像を取得

	assign_image(this->image_org, cv_image<bgr_pixel>(this->frame));
	assign_image(this->image_gs, this->image_org);

	this->new_thread_face_detect();
	if (!this->face_dets.empty()) {
		this->new_thread_hand_detect();
	}

	this->show_detect_window();
}

void HandPointer::exit() {
	this->stop_flag = true;
}

void HandPointer::show_detect_window() {
	this->view_frame = this->frame;

	parallel_for(0, this->hand_dets.size(), [&](long i) {
		cv::rectangle(view_frame, Point(this->hand_dets[i].left(), this->hand_dets[i].top()), Point(this->hand_dets[i].right(), this->hand_dets[i].bottom()), ColorParam::RED, 5);
	});

	parallel_for(0, this->face_dets.size(), [&](long i) {
		cv::rectangle(view_frame, Point(this->face_dets[i].left(), this->face_dets[i].top()), Point(this->face_dets[i].right(), this->face_dets[i].bottom()), ColorParam::BLUE, 5);
	});

	for (auto t : this->track_data) {
		cv::rectangle(view_frame, Point(t.second.current_pos.left(), t.second.current_pos.top()), Point(t.second.current_pos.right(), t.second.current_pos.bottom()), ColorParam::ORANGE, 5);
	}

	imshow("detect window", view_frame);
}

/* 顔検出 */
void HandPointer::face_detect() {
	this->face_thread_flag = true;

	this->face_dets = this->face_detector(this->image_gs);

	if (!this->face_dets.empty()) { // 検出した顔があれば

		/* 既に追跡している顔の近くの顔は除く */
		for (auto td : this->track_data) {
			for (auto fd = begin(this->face_dets); fd != end(this->face_dets);) {
				if (this->euclid_distance((td.second.face.left() + td.second.face.right()) / 2, (td.second.face.top() + td.second.face.bottom()) / 2, (fd->left() + fd->right()) / 2, (fd->top() + fd->bottom()) / 2) < 500) {
					fd = this->face_dets.erase(fd);
				}
				else {
					++fd;
				}
			}
		}

		this->new_thread_hand_detect(); // 手検出スレッドを起動
	}

	this->face_thread_flag = false;
}

/* 手検出 */
void HandPointer::hand_detect() {
	this->hand_thread_flag = true;

	this->hand_dets.clear();

	std::vector<dlib::rectangle> face_dets_tmp = this->face_dets, hand_dets_tmp;
	const int face_num = face_dets_tmp.size(); // 検出された顔の数
	std::vector<dlib::rectangle> windows; // スライディングウィンドウ用の矩形
	array2d<unsigned char> roi;

	/* 既に追跡している顔の近くの顔は除く */
	for (auto td : this->track_data) {
		for (auto fd = begin(face_dets_tmp); fd != end(face_dets_tmp);) {
			if (this->euclid_distance((td.second.face.left() + td.second.face.right()) / 2, (td.second.face.top() + td.second.face.bottom()) / 2, (fd->left() + fd->right()) / 2, (fd->top() + fd->bottom()) / 2) < 500) {
				fd = face_dets_tmp.erase(fd);
			}
			else {
				++fd;
			}
		}
	}

	for (auto fd : face_dets_tmp) {

		///* 顔の周辺のスライディングウィンドウを作成 */
		//SlidingWindows sw(150, 30, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, this->window_width), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 300, this->window_height));
		//windows = sw.get_windows();

		///* スライディングウィンドウで検出 */
		//for (auto w : windows) {
		//	extract_image_chip(this->image_gs, w, roi);
		//	if (this->is_hand(roi)) {
		//		hand_dets_tmp.emplace_back(w);
		//	}
		//}

		std::vector<std::vector<dlib::rectangle>> sliding_windows(3);
		SlidingWindows sw(100, 20, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, this->window_width), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 200, this->window_height));
		SlidingWindows sw2(150, 30, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, this->window_width), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 200, this->window_height));
		SlidingWindows sw3(200, 40, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, this->window_width), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 200, this->window_height));
		sliding_windows[0] = sw.get_windows();
		sliding_windows[1] = sw2.get_windows();
		sliding_windows[2] = sw3.get_windows();

		for (auto sw : sliding_windows) {
			for (auto w : sw) {
				extract_image_chip(this->image_gs, w, roi);
				if (this->is_hand(roi)) {
					hand_dets_tmp.emplace_back(w);
				}
			}
		}

		if (!hand_dets_tmp.empty()) { // 検出された手があれば

			/* 既に追跡している手の近くの手を除く */
			for (auto td : this->track_data) {
				for (auto hd = begin(hand_dets_tmp); hd != end(hand_dets_tmp);) {
					if (this->euclid_distance((td.second.hand.left() + td.second.hand.right()) / 2, (td.second.hand.top() + td.second.hand.bottom()) / 2, (hd->left() + hd->right()) / 2, (hd->top() + hd->bottom()) / 2) < 400) {
						hd = hand_dets_tmp.erase(hd);
					}
					else {
						++hd;
					}
				}
			}

			/* NonMaximumSuppressionにかけて重複を取り除く */
			this->nms(hand_dets_tmp, this->hand_dets);
			
			this->track_data[this->track_id].hand = this->track_data[this->track_id].current_pos = this->hand_dets[0];
			this->track_data[this->track_id].face = fd;

			correlation_tracker ct;
			ct.start_track(this->image_org, this->hand_dets[0]);

			this->track_data[this->track_id].current_pointer = this->track_data[this->track_id].past_pointer = Point((this->hand_dets[0].left() + this->hand_dets[0].right()) / 2, (this->hand_dets[0].top() + this->hand_dets[0].bottom()) / 2);

			do {
				int color_id = this->rn_color(this->mt);
				bool flag = false;
				for (auto t : this->track_data) {
					if (t.second.pointer_color_id == color_id) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					this->track_data[this->track_id].pointer_color_id = color_id;
					this->track_data[this->track_id].pointer_color = this->pointer_color_list[color_id];
					break;
				}
			} while (1);

			this->new_thread_tracking(ct, this->track_id++);
		}

	}

	this->hand_thread_flag = false;
}

/* 追跡時用の手検出 */
void HandPointer::hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int track_id) {
	array2d<unsigned char> roi;
	for (auto w : sliding_windows) {
		extract_image_chip(this->image_gs, w, roi);
		if (this->is_hand(roi)) {
			this->track_data[track_id].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
		}
	}
}

bool HandPointer::is_hand(array2d<unsigned char> &img) {
	array2d<unsigned char> img_resize(this->resize_size, this->resize_size);
	resize_image(img, img_resize);

	fhog_type fhog;
	extract_fhog_features(img_resize, fhog);

	X_type feature_vec;
	fhog_to_feature_vector(feature_vec, fhog);

	return (df(feature_vec) > this->decision_ratio);
}

void HandPointer::tracking(correlation_tracker &ct, int track_id) {
	while (1) {
		/* 直近のフレームで検出した手以外を消す */
		int m = 0;
		for (auto d : this->track_data[track_id].track_hand_dets) {
			if (d.first < this->frame_count - 30) {
				++m;
				continue;
			}
			break;
		}
		this->track_data[track_id].track_hand_dets.erase(begin(this->track_data[track_id].track_hand_dets), begin(this->track_data[track_id].track_hand_dets) + m);

		drectangle past_pos = ct.get_position(); // 直近フレームの手の位置を得る

		ct.update(this->image_org); // 追跡位置の更新

									/* 現在の追跡位置(矩形)を得る */
		drectangle current_pos = ct.get_position();
		this->track_data[track_id].hand = this->track_data[track_id].current_pos = current_pos;

		/* 現在の追跡位置の周辺のスライディングウィンドウを作成して手を検出 */
		SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max((int)current_pos.left() - 100, 0), std::min((int)current_pos.right() + 100, this->window_width), std::max((int)current_pos.top() - 100, 0), std::min((int)current_pos.bottom() + 100, this->window_height));
		this->hand_detect(local_sw.get_windows(), track_id);

		/* 現在の追跡位置(矩形の中心座標)を得る */
		double x = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x座標
		double y = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y座標

		Point2f cp(this->track_data[track_id].past_pointer.x - 10 * x, this->track_data[track_id].past_pointer.y + 10 * y); // 現在の追跡位置から相対的にポインタの位置を決定

		this->track_data[track_id].current_pointer = this->track_data[track_id].past_pointer = cp; // ポインタの位置を更新

		if (this->track_data[track_id].track_hand_dets.empty() || this->stop_flag) { // 直近フレームで手が検出されなかったら追跡をやめる
			this->track_data.erase(track_id);
			break;
		}
	}

}

void HandPointer::new_thread_tracking(correlation_tracker &ct, int n) {
	void(HandPointer::*funcp)(correlation_tracker&, int) = &HandPointer::tracking;
	thread th(funcp, this, ct, n);
	th.detach();
}

void HandPointer::new_thread_hand_detect() {
	if (!this->hand_thread_flag) {
		void(HandPointer::*funcp)() = &HandPointer::hand_detect;
		thread th(funcp, this);
		th.detach();
	}
}

void HandPointer::new_thread_face_detect() {
	if (!this->face_thread_flag) {
		void(HandPointer::*funcp)() = &HandPointer::face_detect;
		thread th(funcp, this);
		th.detach();
	}
}

void HandPointer::fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog) {
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.insert(make_pair(j++, fhog[r][c](i)));
			}
		}
	}
}

double HandPointer::euclid_distance(const double x1, const double y1, const double x2, const double y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}