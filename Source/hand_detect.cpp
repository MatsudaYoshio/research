#include <iostream>
#include <cmath>
#include <algorithm>
#include <thread>
#include <cstdio>
#include <map>

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
	using X_type = std::map<unsigned long, double>; // �����x�N�g���̌^
	using kernel_type = sparse_linear_kernel<X_type>; // �J�[�l���̌^
	using fhog_type = array2d<matrix<double, 31, 1>>; // fhog�����ʂ̌^
	using track_data_type = struct {
		dlib::rectangle hand, face;
		Point past_pointer, current_pointer;
		drectangle current_pos;
		std::vector<pair<int, dlib::rectangle>> track_hand_dets;
	};

	static constexpr int resize_size = 80;
	static constexpr int window_width = 640;
	static constexpr int window_height = 480;
	static constexpr double decision_ratio = 0.5;
	static constexpr double overlap_ratio = 0.1;

	//track_data_type track_assignment[5];
	std::map<long long int, track_data_type> track_data;
	std::vector<dlib::rectangle> face_dets, hand_dets;
	frontal_face_detector face_detector; // ���ʊ猟�o��
	decision_function<kernel_type> df; // ���苫�E�̊֐�
	array2d<unsigned char> image_gs;
	array2d<bgr_pixel> image_org;
	bool face_thread_flag;
	bool hand_thread_flag;
	bool stop_flag;
	long long int frame_count;
	long long int track_id;
	Mat frame, view_frame;
	NonMaximumSuppression nms;
	VideoCapture cap;
public:
	Action() :nms(this->overlap_ratio), face_thread_flag(false), hand_thread_flag(false), stop_flag(false), frame_count(0), track_id(0) {
		this->face_detector = get_frontal_face_detector();

		deserialize("D:/machine_learning_data/hand/20170813/linear_svm_function.dat") >> df; // �t�@�C������w�K�ς݂̃��f����ǂݍ���

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

			this->cap >> this->frame; // �J��������摜���擾

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

		parallel_for(0, this->face_dets.size(), [&](long i) {
			cv::rectangle(view_frame, Point(this->face_dets[i].left(), this->face_dets[i].top()), Point(this->face_dets[i].right(), this->face_dets[i].bottom()), ColorParam::BLUE, 5);
		});

		for (auto t : this->track_data) {
			cv::rectangle(view_frame, Point(t.second.current_pos.left(), t.second.current_pos.top()), Point(t.second.current_pos.right(), t.second.current_pos.bottom()), ColorParam::ORANGE, 5);
		}

		imshow("detect window", view_frame);
	}

	/* �猟�o */
	void face_detect() {
		this->face_thread_flag = true;

		this->face_dets = this->face_detector(this->image_gs);

		/* ���ɒǐՂ��Ă����̋߂��̊�͏��� */
		for (auto td : this->track_data) {
			for (auto fd = begin(this->face_dets); fd != end(this->face_dets);) {
				if (this->euclid_distance((td.second.face.left() + td.second.face.right()) / 2, (td.second.face.top() + td.second.face.bottom()) / 2, (fd->left() + fd->right()) / 2, (fd->top() + fd->bottom()) / 2) < 400) {
					fd = this->face_dets.erase(fd);
				}
				else {
					++fd;
				}
			}
		}
		
		if (!this->face_dets.empty()) { // ���o�����炪�����
			this->new_thread_hand_detect(); // �茟�o�X���b�h���N��
		}

		this->face_thread_flag = false;
	}

	/* �茟�o */
	void hand_detect() {
		this->hand_thread_flag = true;
		
		this->hand_dets.clear();

		std::vector<dlib::rectangle> face_dets_tmp = this->face_dets, hand_dets_tmp;
		const int face_num = face_dets_tmp.size(); // ���o���ꂽ��̐�
		std::vector<dlib::rectangle> windows; // �X���C�f�B���O�E�B���h�E�p�̋�`
		array2d<unsigned char> roi;
		
		for (int i = 0; i < face_num; ++i) {
			/* ��̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬 */
			SlidingWindows sw(200, 40, std::max((int)face_dets_tmp[i].left() - 200, 0), std::min((int)face_dets_tmp[i].right() + 200, this->window_width), std::max((int)face_dets_tmp[i].top() - 200, 0), std::min((int)face_dets_tmp[i].bottom() + 200, this->window_height));
			windows = sw.get_windows();

			/* �X���C�f�B���O�E�B���h�E�Ō��o */
			for (auto w : windows) {
				extract_image_chip(this->image_gs, w, roi);
				if (this->is_hand(roi)) {
					hand_dets_tmp.emplace_back(w);
				}
			}

			/* ���ɒǐՂ��Ă����̋߂��̎������ */
			for (auto td : this->track_data) {
				for (auto hd = begin(hand_dets_tmp); hd != end(hand_dets_tmp);) {
					if (this->euclid_distance((td.second.face.left() + td.second.face.right()) / 2, (td.second.face.top() + td.second.face.bottom()) / 2, (hd->left() + hd->right()) / 2, (hd->top() + hd->bottom()) / 2) < 400) {
						hand_dets_tmp.erase(hd++);
					}
					else {
						++hd;
					}
				}
			}

			if (!hand_dets_tmp.empty()) { // ���o���ꂽ�肪�����

				/* NonMaximumSuppression�ɂ����ďd������菜�� */
				this->nms(hand_dets_tmp, this->hand_dets);

				this->track_data[this->track_id].hand = this->hand_dets[0];
				this->track_data[this->track_id].face = face_dets_tmp[i];

				correlation_tracker ct;
				ct.start_track(this->image_org, this->hand_dets[0]);

				this->track_data[this->track_id].past_pointer = Point((this->hand_dets[0].left() + this->hand_dets[0].right()) / 2, (this->hand_dets[0].top() + this->hand_dets[0].bottom()) / 2);

				this->new_thread_tracking(ct, this->track_id++);
			}
			
		}

		this->hand_thread_flag = false;
	}

	/* �ǐՎ��p�̎茟�o */
	void hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int track_id) {
		array2d<unsigned char> roi;
		for (auto w : sliding_windows) {
			extract_image_chip(this->image_gs, w, roi);
			if (this->is_hand(roi)) {
				this->track_data[track_id].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
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

	void tracking(correlation_tracker &ct, int track_id) {
		while (1) {

			/* ���߂̃t���[���Ō��o������ȊO������ */
			int m = 0;
			for (auto d : this->track_data[track_id].track_hand_dets) {
				if (d.first < this->frame_count - 30) {
					++m;
					continue;
				}
				break;
			}
			this->track_data[track_id].track_hand_dets.erase(begin(this->track_data[track_id].track_hand_dets), begin(this->track_data[track_id].track_hand_dets) + m);

			drectangle past_pos = ct.get_position(); // ���߃t���[���̎�̈ʒu�𓾂�

			ct.update(this->image_org); // �ǐՈʒu�̍X�V
			
			/* ���݂̒ǐՈʒu(��`)�𓾂� */
			drectangle current_pos = ct.get_position();
			this->track_data[track_id].hand = this->track_data[track_id].current_pos = current_pos;

			/* ���݂̒ǐՈʒu�̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬���Ď�����o */
			SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max((int)current_pos.left() - 100, 0), std::min((int)current_pos.right() + 100, this->window_width), std::max((int)current_pos.top() - 100, 0), std::min((int)current_pos.bottom() + 100, this->window_height));
			this->hand_detect(local_sw.get_windows(), track_id);

			/* ���݂̒ǐՈʒu(��`�̒��S���W)�𓾂� */
			double x = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x���W
			double y = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y���W

			Point2f cp(this->track_data[track_id].past_pointer.x - 10 * x, this->track_data[track_id].past_pointer.y + 10 * y); // ���݂̒ǐՈʒu���瑊�ΓI�Ƀ|�C���^�̈ʒu������

			this->track_data[track_id].current_pointer = this->track_data[track_id].past_pointer = cp; // �|�C���^�̈ʒu���X�V

			if (this->track_data[track_id].track_hand_dets.empty() || this->stop_flag) { // ���߃t���[���Ŏ肪���o����Ȃ�������ǐՂ���߂�
				this->track_data.erase(track_id);
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