#include "HandCursor.h"
#include "AppParameters.h"

using namespace std;
using namespace dlib;
using namespace cv;
using namespace param;

HandCursor::HandCursor() :nms(this->overlap_ratio), face_thread_flag(false), hand_thread_flag(false), stop_flag(false), frame_count(0), track_id(0), mt(rd()) {
	this->face_detector = get_frontal_face_detector();

	deserialize("C:/Users/matsuda/workspace/machine_learning_data/hand/20170813/linear_svm_function.dat") >> df; // �t�@�C������w�K�ς݂̃��f����ǂݍ���

	this->frame = Mat(Size(W, H), CV_8UC3);

	this->cursor_color_list.emplace_back(ofColor::blue);
	this->cursor_color_list.emplace_back(ofColor::red);
	this->cursor_color_list.emplace_back(ofColor::green);
	this->cursor_color_list.emplace_back(ofColor::black);
	this->cursor_color_list.emplace_back(ofColor::pink);
	this->rn_color = uniform_int_distribution<int>(0, this->cursor_color_list.size() - 1);

	/*this->track_data[-1].current_pointer.x = 1000;
	this->track_data[-1].current_pointer.y = 900;
	this->track_data[-1].face = dlib::rectangle(0, 0, 50, 50);
	this->track_data[-1].cursor_color_id = 0;
	this->track_data[-1].cursor_color = ofColor::blue;*/

	//this->track_data[-2].current_pointer.x = 1100;
	//this->track_data[-2].current_pointer.y = 600;
	//this->track_data[-2].face = dlib::rectangle(450, 600, 50, 50);
	//this->track_data[-2].cursor_color_id = 1;
	//this->track_data[-2].cursor_color = ofColor::red;

}

void HandCursor::update() {
	++this->frame_count;

	/* fps��\�� */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	this->frame = this->cap.get_image(); // �J��������摜���擾

	assign_image(this->image_org, cv_image<bgr_pixel>(this->frame));
	assign_image(this->image_gs, this->image_org);

	this->new_thread_face_detect();
	if (!this->face_dets.empty()) {
		this->new_thread_hand_detect();
	}

	//this->show_detect_window(); // ���o�`�F�b�N�p�̃E�B���h�E��\��
}

void HandCursor::exit() {
	this->stop_flag = true;
}

void HandCursor::show_detect_window() {
	this->view_frame = this->frame;

	parallel_for(0, this->hand_dets.size(), [&](long i) {
		cv::rectangle(view_frame, Point(this->hand_dets[i].left(), this->hand_dets[i].top()), Point(this->hand_dets[i].right(), this->hand_dets[i].bottom()), this->RED, 5);
	});

	parallel_for(0, this->face_dets.size(), [&](long i) {
		cv::rectangle(view_frame, Point(this->face_dets[i].left(), this->face_dets[i].top()), Point(this->face_dets[i].right(), this->face_dets[i].bottom()), this->BLUE, 5);
	});

	for (const auto &t : this->track_data) {
		cv::rectangle(view_frame, Point(t.second.current_pos.left(), t.second.current_pos.top()), Point(t.second.current_pos.right(), t.second.current_pos.bottom()), this->ORANGE, 5);
	}

	imshow("detect window", view_frame);
}

/* �猟�o */
void HandCursor::face_detect() {
	this->face_thread_flag = true;

	this->face_dets = this->face_detector(this->image_gs);

	if (!this->face_dets.empty()) { // ���o�����炪�����

									/* ���ɒǐՂ��Ă����̋߂��̊�͏��� */
		for (const auto &td : this->track_data) {
			for (auto fd = begin(this->face_dets); fd != end(this->face_dets);) {
				if (this->euclid_distance((td.second.face.left() + td.second.face.right()) / 2, (td.second.face.top() + td.second.face.bottom()) / 2, (fd->left() + fd->right()) / 2, (fd->top() + fd->bottom()) / 2) < 500) {
					fd = this->face_dets.erase(fd);
				}
				else {
					++fd;
				}
			}
		}

		this->new_thread_hand_detect(); // �茟�o�X���b�h���N��
	}

	this->face_thread_flag = false;
}

/* �茟�o */
void HandCursor::hand_detect() {
	this->hand_thread_flag = true;

	this->hand_dets.clear();

	std::vector<dlib::rectangle> face_dets_tmp = this->face_dets, hand_dets_tmp;
	std::vector<dlib::rectangle> windows; // �X���C�f�B���O�E�B���h�E�p�̋�`
	array2d<unsigned char> roi;

	/* ���ɒǐՂ��Ă����̋߂��̊�͏��� */
	for (const auto &td : this->track_data) {
		for (auto fd = begin(face_dets_tmp); fd != end(face_dets_tmp);) {
			if (this->euclid_distance((td.second.face.left() + td.second.face.right()) / 2, (td.second.face.top() + td.second.face.bottom()) / 2, (fd->left() + fd->right()) / 2, (fd->top() + fd->bottom()) / 2) < 500) {
				fd = face_dets_tmp.erase(fd);
			}
			else {
				++fd;
			}
		}
	}

	for (const auto &fd : face_dets_tmp) {
		//* ��̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬(�E�B���h�E�T�C�Y������傫���ĈقȂ�3���) */
		std::vector<std::vector<dlib::rectangle>> sliding_windows(3);
		SlidingWindows sw(fd.width(), fd.width() / 5, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, W), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 200, H));
		SlidingWindows sw2(fd.width() + 50, fd.width() / 5 + 10, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, W), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 200, H));
		SlidingWindows sw3(fd.width() + 100, fd.width() / 5 + 20, std::max((int)fd.left() - 300, 0), std::min((int)fd.right() + 300, W), std::max((int)fd.top() - 300, 0), std::min((int)fd.bottom() + 200, H));
		sliding_windows[0] = sw.get_windows();
		sliding_windows[1] = sw2.get_windows();
		sliding_windows[2] = sw3.get_windows();

		for (const auto &sw : sliding_windows) {
			for (const auto &w : sw) {
				extract_image_chip(this->image_gs, w, roi);
				if (this->is_hand(roi)) {
					hand_dets_tmp.emplace_back(w);
				}
			}
		}

		if (!hand_dets_tmp.empty()) { // ���o���ꂽ�肪�����

			/* ���ɒǐՂ��Ă����̋߂��̎������ */
			for (const auto &td : this->track_data) {
				for (auto hd = begin(hand_dets_tmp); hd != end(hand_dets_tmp);) {
					if (this->euclid_distance((td.second.hand.left() + td.second.hand.right()) / 2, (td.second.hand.top() + td.second.hand.bottom()) / 2, (hd->left() + hd->right()) / 2, (hd->top() + hd->bottom()) / 2) < 400) {
						hd = hand_dets_tmp.erase(hd);
					}
					else {
						++hd;
					}
				}
			}

			/* NonMaximumSuppression�ɂ����ďd������菜�� */
			this->nms(hand_dets_tmp, this->hand_dets);

			this->track_data[this->track_id].hand = this->track_data[this->track_id].current_pos = this->hand_dets[0];
			this->track_data[this->track_id].face = fd;

			correlation_tracker ct;
			ct.start_track(this->image_org, this->hand_dets[0]);

			this->track_data[this->track_id].current_pointer = this->track_data[this->track_id].past_pointer = Point((this->hand_dets[0].left() + this->hand_dets[0].right()) / 2, (this->hand_dets[0].top() + this->hand_dets[0].bottom()) / 2);

			/* �J�[�\���̐F�����Ԃ�Ȃ��悤�ɑI�� */
			do {
			SAME:
				int color_id = this->rn_color(this->mt);
				for (const auto &t : this->track_data) {
					if (t.second.cursor_color_id == color_id) {
						goto SAME;
					}
				}
				this->track_data[this->track_id].cursor_color_id = color_id;
				this->track_data[this->track_id].cursor_color = this->cursor_color_list[color_id];
				break;
			} while (1);

			this->new_thread_tracking(ct, this->track_id++);
		}

	}

	this->hand_thread_flag = false;
}

/* �ǐՎ��p�̎茟�o */
void HandCursor::hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int &user_id) {
	array2d<unsigned char> roi;
	for (const auto &w : sliding_windows) {
		extract_image_chip(this->image_gs, w, roi);
		if (this->is_hand(roi)) {
			this->track_data[user_id].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
		}
	}
}

bool HandCursor::is_hand(array2d<unsigned char> &img) {
	array2d<unsigned char> img_resize(this->resize_size, this->resize_size);
	resize_image(img, img_resize);

	fhog_type fhog;
	extract_fhog_features(img_resize, fhog);

	X_type feature_vec;
	fhog_to_feature_vector(feature_vec, fhog);

	return (df(feature_vec) > this->decision_ratio);
}

void HandCursor::tracking(correlation_tracker &ct, const int user_id) {
	int m;
	int dx, dy;
	drectangle past_pos, current_pos;
	const double dx_rate = W / this->track_data[user_id].face.width();
	const double dy_rate = H / this->track_data[user_id].face.height();

	while (1) {
		/* ���߂̃t���[���Ō��o������ȊO������ */
		m = 0;
		for (const auto &d : this->track_data[user_id].track_hand_dets) {
			if (d.first < this->frame_count - 30) {
				++m;
				continue;
			}
			break;
		}
		this->track_data[user_id].track_hand_dets.erase(begin(this->track_data[user_id].track_hand_dets), begin(this->track_data[user_id].track_hand_dets) + m);

		past_pos = ct.get_position(); // ���߃t���[���̎�̈ʒu�𓾂�

		ct.update(this->image_org); // �ǐՈʒu�̍X�V

		/* ���݂̒ǐՈʒu(��`)�𓾂� */
		current_pos = ct.get_position();
		this->track_data[user_id].hand = this->track_data[user_id].current_pos = current_pos;

		/* ���݂̒ǐՈʒu�̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬���Ď�����o */
		/* ���ӂƂ͒ǐՂ��Ă����̋�`1�����ӂ͈̔� */
		SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max(static_cast<int>(current_pos.left() - this->track_data[user_id].hand.width()), 0), std::min(static_cast<int>(current_pos.right() + this->track_data[user_id].hand.width()), W), std::max(static_cast<int>(current_pos.top() - this->track_data[user_id].hand.height()), 0), std::min(static_cast<int>(current_pos.bottom() + this->track_data[user_id].hand.height()), H));
		this->hand_detect(local_sw.get_windows(), user_id);

		/* ���݂̒ǐՈʒu�ƒ��O�̒ǐՈʒu�̍� */
		dx = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x����
		dy = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y����

		Point2f cp(std::max(std::min(this->track_data[user_id].past_pointer.x + dx_rate * dx, static_cast<double>(W)), 0.0), std::max(std::min((this->track_data[user_id].past_pointer.y + dy_rate * dy), static_cast<double>(H)), 0.0)); // ���݂̒ǐՈʒu���瑊�ΓI�Ƀ|�C���^�̈ʒu������

		this->track_data[user_id].current_pointer = this->track_data[user_id].past_pointer = cp; // �|�C���^�̈ʒu���X�V

		if (this->track_data[user_id].track_hand_dets.empty() || this->stop_flag) { // ���߃t���[���Ŏ肪���o����Ȃ�������ǐՂ���߂�
			this->track_data.erase(user_id);
			break;
		}
	}
}

void HandCursor::new_thread_tracking(correlation_tracker &ct, const int track_id) {
	void(HandCursor::*funcp)(correlation_tracker&, const int) = &HandCursor::tracking;
	thread th(funcp, this, ct, track_id);
	th.detach();
}

void HandCursor::new_thread_hand_detect() {
	if (!this->hand_thread_flag) {
		void(HandCursor::*funcp)() = &HandCursor::hand_detect;
		thread th(funcp, this);
		th.detach();
	}
}

void HandCursor::new_thread_face_detect() {
	if (!this->face_thread_flag) {
		void(HandCursor::*funcp)() = &HandCursor::face_detect;
		thread th(funcp, this);
		th.detach();
	}
}

void HandCursor::fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog) {
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.emplace_hint(end(feature_vector), j++, fhog[r][c](i));
			}
		}
	}
}

double HandCursor::euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}