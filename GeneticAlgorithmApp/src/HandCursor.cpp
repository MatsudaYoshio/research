#include "HandCursor.h"

using namespace std;
using namespace dlib;
using namespace cv;
using namespace param;

const char* HandCursor::model_path{ "C:/Users/matsuda/workspace/machine_learning_data/hand/20170813/linear_svm_function.dat" }; // �w�K���f���̃p�X

/* �J�[�\���̐F */
const std::array<ofColor, HandCursor::cursor_color_num> HandCursor::cursor_colors = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange };

/* Scalar�^�̐F */
const Scalar HandCursor::CV_RED{ Scalar(0, 0, 255) };
const Scalar HandCursor::CV_BLUE{ Scalar(255, 0, 0) };
const Scalar HandCursor::CV_ORANGE{ Scalar(76, 183, 255) };

HandCursor::HandCursor() {
	deserialize(this->model_path) >> this->df; // �t�@�C������w�K�ς݂̃��f����ǂݍ���

	/* �ŏ��̈ꖇ����荞��ł��� */
	this->mat_org_image_buffer.get_push_position() = this->cap.get_image();
	this->mat_org_image_buffer.forward_offset();

	thread frame_thread(&HandCursor::get_frame, this);
	frame_thread.detach();

	/* ���z���[�U */
	this->user_data[-1].cursor_point.x() = 550;
	this->user_data[-1].cursor_point.y() = 650;
	this->transform_point(this->user_data[-1].cursor_point, this->user_data[-1].transformed_cursor_point);
	this->user_data[-1].cursor_color = ofColor::mediumPurple;
	this->user_data[-1].cursor_color_id = 1;
	this->cursor_color_state[1] = true;
	this->user_data[-1].face_point = center(dlib::rectangle(HALF_DISPLAY_W * 3 / 4 + 300, HALF_DISPLAY_H / 2 - 300, 300, 300));
	this->transform_point(this->user_data[-1].face_point, this->user_data[-1].transformed_face_point);

	/* ����B�e */
	//this->writer.open("hand_tracking_openPose6.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 35, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	/* fps��\�� */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	if ((this->frame_count % 30) == 0) {
		this->body_part_extractor(this->pose_key_points, this->mat_org_image_buffer.get_read_position()); // openPose�ɂ�鍜�i����

		const int people_num{ this->pose_key_points.getSize(0) }; // ���o���ꂽ�l��
		for (int i = 0; i < people_num; ++i) {

			if (this->pose_key_points[NOSE_X(i)] == 0.0 || this->pose_key_points[RIGHT_WRIST_X(i)] == 0.0) { // �炩�E�肪���o����Ȃ����
				continue;
			}

			if (ofDist(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)], this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]) > this->face_hand_distance) { // ��ƉE��̋��������ȏゾ������
				continue;
			}

			double face_size{ this->estimate_face_size(i) }; // ��̑傫���𐄒�

			long long int user_id{ this->decide_user_id(i) }; // user_id������

			if (user_id == 0) { // user_id��0(�V���ȃ��[�U)�ł����
				if (this->hand_detect(i, face_size)) { // �肪���o������
					this->start_track(i, face_size); // �ǐՊJ�n
				}
				else {
					continue;
				}
			}
			else {
				this->renew_user_data(i, face_size, user_id); // ���[�U�̏����X�V
			}
		}
	}
	//this->show_detect_window(); // ����m�F�p�̃E�B���h�E��\��
}

void HandCursor::exit() {
	this->stop_flag = true;
}

double HandCursor::estimate_face_size(const int personal_id) const {
	if (this->pose_key_points[RIGHT_EAR_X(personal_id)] != 0.0 && this->pose_key_points[LEFT_EAR_X(personal_id)] != 0.0) { // �����̈ʒu���擾�ł��Ă����
		return ofDist(this->pose_key_points[LEFT_EAR_X(personal_id)], this->pose_key_points[LEFT_EAR_Y(personal_id)], this->pose_key_points[RIGHT_EAR_X(personal_id)], this->pose_key_points[RIGHT_EAR_Y(personal_id)]); // �����Ԃ̋�����Ԃ�
	}

	if (this->pose_key_points[NECK_X(personal_id)] == 0.0) { // ��̈ʒu���擾�ł��Ȃ����
		return this->default_face_size;
	}
	else {
		if (this->pose_key_points[RIGHT_SHOULDER_X(personal_id)] == 0.0) {
			if (this->pose_key_points[LEFT_SHOULDER_X(personal_id)] == 0.0) {
				return this->default_face_size;
			}
			else {
				return ofDist(this->pose_key_points[NECK_X(personal_id)], this->pose_key_points[NECK_Y(personal_id)], this->pose_key_points[LEFT_SHOULDER_X(personal_id)], this->pose_key_points[LEFT_SHOULDER_Y(personal_id)]);
			}
		}
		else {
			if (this->pose_key_points[LEFT_SHOULDER_X(personal_id)] == 0.0) {
				return ofDist(this->pose_key_points[NECK_X(personal_id)], this->pose_key_points[NECK_Y(personal_id)], this->pose_key_points[RIGHT_SHOULDER_X(personal_id)], this->pose_key_points[RIGHT_SHOULDER_Y(personal_id)]);
			}
			else {
				return (ofDist(this->pose_key_points[NECK_X(personal_id)], this->pose_key_points[NECK_Y(personal_id)], this->pose_key_points[LEFT_SHOULDER_X(personal_id)], this->pose_key_points[LEFT_SHOULDER_Y(personal_id)]) + ofDist(this->pose_key_points[NECK_X(personal_id)], this->pose_key_points[NECK_Y(personal_id)], this->pose_key_points[RIGHT_SHOULDER_X(personal_id)], this->pose_key_points[RIGHT_SHOULDER_Y(personal_id)])) / 2;
			}
		}
	}
}

int HandCursor::decide_user_id(const int personal_id) const {
	long long int user_id{ 0 };
	double best_d{ DBL_MAX };
	for (const auto& ud : this->user_data) {
		const double d{ ofDist(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)], ud.second.face_point.x(), ud.second.face_point.y()) };
		if (d < this->face_error && d < best_d) {
			user_id = ud.first;
			best_d = d;
		}
	}

	return user_id;
}

bool HandCursor::hand_detect(const int personal_id, const double face_size) {
	/* 3��ނ̃X���C�f�B���O�E�B���h�E���쐬 */
	this->generate_sliding_windows(this->sliding_windows[0], face_size, face_size*this->sliding_window_step_rate, std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)] - face_size*this->windows_range_rate_left), 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)] + face_size*this->windows_range_rate_right), CAMERA_W), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - face_size*this->windows_range_rate_top), 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)] + face_size*this->windows_range_rate_bottom), CAMERA_H));

	this->generate_sliding_windows(this->sliding_windows[1], face_size*1.2, face_size*1.2*this->sliding_window_step_rate, std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)] - face_size*this->windows_range_rate_left), 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)] + face_size*this->windows_range_rate_right), CAMERA_W), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - face_size*this->windows_range_rate_top), 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)] + face_size*this->windows_range_rate_bottom), CAMERA_H));

	this->generate_sliding_windows(this->sliding_windows[2], face_size*1.8, face_size*1.8*this->sliding_window_step_rate, std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)] - face_size*this->windows_range_rate_left), 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)] + face_size*this->windows_range_rate_right), CAMERA_W), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - face_size*this->windows_range_rate_top), 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)] + face_size*this->windows_range_rate_bottom), CAMERA_H));

	this->hand_dets_tmp.clear();
	for (const auto& sw : sliding_windows) {
		for (const auto& w : sw) {
			extract_image_chip(this->gs_image_buffer.get_read_position(), w, this->roi);
			if (this->is_hand(this->roi)) {
				this->hand_dets_tmp.emplace_back(w);
			}
		}
	}

	if (this->hand_dets_tmp.empty()) { // ���o���ꂽ�肪�Ȃ����
		return false;
	}

	this->hand_dets.clear();
	this->nms(this->hand_dets_tmp, this->hand_dets);

	return true;
}

/* �ǐՎ��p�̎茟�o */
void HandCursor::hand_detect(const std::vector<dlib::rectangle>& sliding_windows, const long long int user_id) {
	for (const auto& w : sliding_windows) {
		array2d<unsigned char> roi;
		extract_image_chip(this->gs_image_buffer.get_read_position(), w, roi);
		if (this->is_hand(roi)) {
			this->user_data[user_id].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
		}
	}
}

void HandCursor::start_track(const int personal_id, const double face_size) {
	int cursor_color_id = distance(cbegin(this->cursor_color_state), find_if_not(cbegin(this->cursor_color_state), cend(this->cursor_color_state), [](auto x) {return x; })); // ���g�p�̐F����J�[�\���̐F��I��

	this->cursor_color_state[cursor_color_id] = true; // �I�񂾃J�[�\���̐F���g�p�ς݂Ƃ���

	this->user_data.emplace(this->user_id, user_data_type{this->hand_dets[0], center(this->hand_dets[0]), point(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)]), face_size, cursor_color_id, this->cursor_colors[cursor_color_id], }); // ���[�U�̏����ł��邾���l�߂�

	this->transform_point(this->user_data[this->user_id].face_point, this->user_data[this->user_id].transformed_face_point); // ��̍��W����ʏ�̍��W�ɕϊ�
	this->transform_point(this->user_data[this->user_id].cursor_point, this->user_data[this->user_id].transformed_cursor_point); // �J�[�\���̍��W����ʏ�̍��W�ɕϊ�

	/* �ǐՂ��J�n���� */
	correlation_tracker ct;
	ct.start_track(this->org_image_buffer.get_read_position(), this->user_data[this->user_id].hand);
	thread th(&HandCursor::tracking, this, ct, this->user_id++);
	th.detach();
}

void HandCursor::renew_user_data(const int personal_id, const double face_size, const long long int user_id) {
	/* ��̍��W�Ƒ傫�����X�V���� */
	try {
		this->user_data.at(user_id).face_size = face_size;

		this->user_data.at(user_id).face_point.x() = this->pose_key_points[NOSE_X(personal_id)];
		this->user_data.at(user_id).face_point.y() = this->pose_key_points[NOSE_Y(personal_id)];
	}
	catch (std::out_of_range&) {}
}

void HandCursor::get_frame() {
	while (!this->stop_flag) {
		this->mat_org_image_buffer.get_push_position() = this->cap.get_image();
		this->mat_org_image_buffer.forward_offset();

		assign_image(this->org_image_buffer.get_push_position(), cv_image<bgr_pixel>(this->mat_org_image_buffer.get_read_position()));
		this->org_image_buffer.forward_offset();

		assign_image(this->gs_image_buffer.get_push_position(), this->org_image_buffer.get_read_position());
		this->gs_image_buffer.forward_offset();

		++this->frame_count;
	}
}

void HandCursor::tracking(correlation_tracker& ct, const long long int user_id) {
	int m;
	int dx, dy;
	drectangle past_pos, current_pos;
	const double dx_rate{ static_cast<double>(CAMERA_W) / this->user_data[user_id].face_size };
	const double dy_rate{ static_cast<double>(CAMERA_H) / this->user_data[user_id].face_size };
	OneEuroFilter dx_filter(120, 0.5, 1.6, 1.0), dy_filter(120, 0.5, 1.6, 1.0);

	while (1) {
		m = 0;
		for (const auto& d : this->user_data[user_id].track_hand_dets) {
			if (d.first < this->frame_count - 30) {
				++m;
				continue;
			}
			break;
		}
		this->user_data[user_id].track_hand_dets.erase(cbegin(this->user_data[user_id].track_hand_dets), cbegin(this->user_data[user_id].track_hand_dets) + m);

		past_pos = ct.get_position(); // ���߃t���[���̎�̈ʒu�𓾂�

		ct.update(this->org_image_buffer.get_read_position()); // �ǐՈʒu�̍X�V

		/* ���݂̒ǐՈʒu(��`)�𓾂� */
		current_pos = ct.get_position();

		this->user_data[user_id].hand = current_pos;

		/* ���݂̒ǐՈʒu�̎��ӂ̃X���C�f�B���O�E�B���h�E���쐬���Ď�����o */
		/* ���ӂƂ͒ǐՂ��Ă����̋�`1�����ӂ͈̔� */
		this->hand_detect(this->generate_sliding_windows(current_pos.width(), current_pos.width()*this->sliding_window_step_rate, std::max(static_cast<int>(current_pos.left() - this->user_data[user_id].hand.width()), 0), std::min(static_cast<int>(current_pos.right() + this->user_data[user_id].hand.width()), CAMERA_W), std::max(static_cast<int>(current_pos.top() - this->user_data[user_id].hand.height()), 0), std::min(static_cast<int>(current_pos.bottom() + this->user_data[user_id].hand.height()), CAMERA_H)), user_id);

		/* ���݂̒ǐՈʒu�ƒ��O�̒ǐՈʒu�̍� */
		dx = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x����
		dy = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y����

		/* ���ꂼ��̕����̍��Ƀt�B���^�������� */
		dx = dx_filter.filter(dx);
		dy = dy_filter.filter(dy);

		/* �J�[�\���̈ʒu���X�V */
		this->user_data[user_id].cursor_point = point(ofClamp(this->user_data[user_id].cursor_point.x() + dx_rate * dx, 0, CAMERA_W), ofClamp(this->user_data[user_id].cursor_point.y() + dy_rate * dy, 0, CAMERA_H)); // ���݂̒ǐՈʒu���瑊�ΓI�ɃJ�[�\���̈ʒu������

		this->transform_point(this->user_data[user_id].cursor_point, this->user_data[user_id].transformed_cursor_point);

		if (this->user_data[user_id].track_hand_dets.empty() || this->stop_flag) { // ���߃t���[���Ŏ肪���o����Ȃ�������ǐՂ���߂�
			this->cursor_color_state[this->user_data[user_id].cursor_color_id] = false;
			this->user_data.erase(user_id);
			break;
		}
	}
}

void HandCursor::generate_sliding_windows(std::vector<dlib::rectangle>& windows, const int window_size, const int step, const int min_width, const int max_width, const int min_height, const int max_height) {
	if (step == 0) {
		return;
	}

	const auto max_i{ max_height - window_size };
	const auto max_j{ max_width - window_size };

	windows.resize(((max_i - min_height) / step + 1)*((max_j - min_width) / step + 1));

	for (int i = min_height, x = 0; i <= max_i; i += step) {
		for (int j = min_width; j <= max_j; j += step) {
			windows[x++] = dlib::rectangle(j, i, j + window_size, i + window_size);
		}
	}
}

std::vector<dlib::rectangle> HandCursor::generate_sliding_windows(const int window_size, const int step, const int min_width, const int max_width, const int min_height, const int max_height) {
	std::vector<dlib::rectangle> windows;

	this->generate_sliding_windows(windows, window_size, step, min_width, max_width, min_height, max_height);

	return windows;
}

bool HandCursor::is_hand(array2d<unsigned char>& img) {
	array2d<unsigned char> img_resize(this->resize_size, this->resize_size);
	resize_image(img, img_resize);

	fhog_type fhog;
	extract_fhog_features(img_resize, fhog);

	X_type feature_vec;
	this->fhog_to_feature_vector(feature_vec, fhog);

	return (df(feature_vec) > this->decision_ratio);
}

void HandCursor::fhog_to_feature_vector(X_type& feature_vector, const fhog_type& fhog) const {
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.emplace_hint(cend(feature_vector), j++, fhog[r][c](i));
			}
		}
	}
}

void HandCursor::transform_point(const point& src_point, point& dst_point) const {
	dst_point.x() = DISPLAY_W - RESOLUTION_RATIO_W*src_point.x();
	dst_point.y() = RESOLUTION_RATIO_H * src_point.y();
}

void HandCursor::inverse_transform_point(const point& src_point, point& dst_point) const {
	dst_point.x() = (DISPLAY_W - src_point.x()) / RESOLUTION_RATIO_W;
	dst_point.y() = src_point.y() / RESOLUTION_RATIO_H;
}

void HandCursor::modulate_cursor(const long long int& user_id) {
	try {
		this->inverse_transform_point(this->user_data.at(user_id).transformed_cursor_point, this->user_data.at(user_id).cursor_point);
	}
	catch (std::out_of_range&) {}
}

void HandCursor::show_detect_window() {
	this->view_frame = this->mat_org_image_buffer.get_read_position();

	Mat img = this->view_frame;

	//const int people_num{ this->pose_key_points.getSize(0) };
	//parallel_for(0, people_num, [&](int i) {
	//	if (this->pose_key_points[NOSE_X(i)] != 0.0 && this->pose_key_points[NOSE_Y(i)] != 0.0) {
	//		cv::circle(this->view_frame, Point(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)]), 15, this->CV_BLUE, -1);
	//	}

	//	if (this->pose_key_points[RIGHT_WRIST_X(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0) {
	//		cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]), 15, this->CV_RED, -1);
	//	}

	//	cv::rectangle(this->view_frame, Point(std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(i)]) - 200, 0), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(i)]) - 200, 0)), Point(std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(i)]) + 200, CAMERA_W), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(i)]) + 100, CAMERA_H)), this->CV_RED, 10);
	//});

	for (const auto& t : this->user_data) {
		cv::rectangle(img, Point(t.second.hand.left(), t.second.hand.top()), Point(t.second.hand.right(), t.second.hand.bottom()), this->CV_RED, 5);
	}

	imshow("detect window", img);

	/* ����B�e */
	//this->writer << img;
}