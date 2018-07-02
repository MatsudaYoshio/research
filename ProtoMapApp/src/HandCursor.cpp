#include "HandCursor.h"

using namespace std;
using namespace cv;
using namespace param;

/* �J�[�\���̐F */
const std::array<ofColor, HandCursor::cursor_color_num> HandCursor::cursor_colors = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange };

/* Scalar�^�̐F */
const Scalar HandCursor::CV_RED{ Scalar(0, 0, 255) };
const Scalar HandCursor::CV_BLUE{ Scalar(255, 0, 0) };
const Scalar HandCursor::CV_ORANGE{ Scalar(76, 183, 255) };

HandCursor::HandCursor() {
	this->image_buffer.emplace_front(this->cap.get_image()); // �ŏ��̈ꖇ����荞��ł���

	/* �摜�擾�X���b�h */
	thread frame_thread(&HandCursor::get_frame, this);
	frame_thread.detach();

	/* ����B�e */
	//this->writer.open("hand_tracking_openPose6.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 35, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	/* fps��\�� */
	frc.NewFrame();
	printf("fps : %lf\n", frc.GetFrameRate());

	/* ��莞�Ԃ̊ԁA��񂪍X�V����Ă��Ȃ����[�U���폜���� */
	for (auto ite = begin(this->user_data); ite != end(this->user_data);) {
		if (this->frame_count - ite->second.latest_update_frame > 80) {
			this->cursor_color_state[ite->second.cursor_color_id] = false; // �F�����
			ite = this->user_data.erase(ite);
		}
		else {
			++ite;
		}
	}

	this->body_part_extractor(this->pose_key_points, this->image_buffer.front()); // openPose�ɂ�鍜�i����

	const int people_num{ this->pose_key_points.getSize(0) }; // ���o���ꂽ�l��

	for (int i = 0; i < people_num; ++i) {
		if (this->pose_key_points[NOSE_X(i)] == 0.0 || this->pose_key_points[NOSE_Y(i)] == 0.0) { // �炪���o�ł��Ȃ����
			continue;
		}
		double face_size{ this->estimate_face_size(i) }; // ��̑傫���𐄒�

		long long int user_id{ this->decide_user_id(i) }; // user_id������

		auto interaction_threshold{ (this->pose_key_points[RIGHT_SHOULDER_Y(i)] + this->pose_key_points[MIDDLE_HIP_Y(i)]) / 2 }; // ���ƉE���̒��ԓ_���C���^���N�V�����̊�_�Ƃ���

		if (this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] < interaction_threshold) {
			// �E��ƉE�������o����A�E�肪���ƉE���̒��ԓ_����ł����
			if (user_id == this->new_user_id) { // �V�������[�U�ł����
				this->init_user_data(i, face_size); // ���[�U�f�[�^�̏�����
			}
			else {
				this->renew_user_data(i, face_size, user_id); // ���[�U�f�[�^�̍X�V
			}
		}
		else {
			if (user_id != this->new_user_id) {
				this->user_data[user_id].state = STATE::INACTIVE;
			}
		}
	}
	//}
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
	long long int user_id{ this->new_user_id };
	double best_d{ DBL_MAX };
	for (const auto& ud : this->user_data) {
		const double d{ ofDist(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)], ud.second.face_point.x, ud.second.face_point.y) };
		if (d < this->face_error && d < best_d) {
			user_id = ud.first;
			best_d = d;
		}
	}

	return user_id;
}

void HandCursor::init_user_data(const int personal_id, const double face_size) {
	const int cursor_color_id = distance(cbegin(this->cursor_color_state), find_if_not(cbegin(this->cursor_color_state), cend(this->cursor_color_state), [](auto x) {return x; })); // ���g�p�̐F����J�[�\���̐F��I��
	this->cursor_color_state[cursor_color_id] = true; // �I�񂾃J�[�\���̐F���g�p�ς݂Ƃ���

	/* ���[�U�̏����ł��邾���l�߂ď����� */
	this->user_data.emplace(this->user_id, user_data_type{
		STATE::ACTIVE,
			this->frame_count,
			Rect2d(this->pose_key_points[NOSE_X(personal_id)] - face_size / 2, this->pose_key_points[NOSE_Y(personal_id)] - face_size / 2, face_size, face_size),
			Point(this->pose_key_points[RIGHT_WRIST_X(personal_id)], this->pose_key_points[RIGHT_WRIST_Y(personal_id)]),
			Point(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)]),
			Point(this->pose_key_points[RIGHT_WRIST_X(personal_id)], this->pose_key_points[RIGHT_WRIST_Y(personal_id)]),
			face_size,
			cursor_color_id,
			this->cursor_colors[cursor_color_id],
	});
	this->user_data[this->user_id].dx_filter.reset(new OneEuroFilter(120, 0.25, 0.25, 1.0));
	this->user_data[this->user_id].dy_filter.reset(new OneEuroFilter(120, 0.25, 0.25, 1.0));

	this->transform_point(this->user_data[this->user_id].face_point, this->user_data[this->user_id].transformed_face_point); // ��̍��W����ʏ�̍��W�ɕϊ�
	this->transform_point(this->user_data[this->user_id].cursor_point, this->user_data[this->user_id].transformed_cursor_point); // �J�[�\���̍��W����ʏ�̍��W�ɕϊ�

	++this->user_id;
}

void HandCursor::renew_user_data(const int personal_id, const double face_size, const long long int user_id) {
	this->user_data[user_id].state = STATE::ACTIVE;

	this->user_data[user_id].face_size = face_size;
	this->user_data[user_id].face_point.x = this->pose_key_points[NOSE_X(personal_id)];
	this->user_data[user_id].face_point.y = this->pose_key_points[NOSE_Y(personal_id)];

	this->dx_rate = CAMERA_W / this->user_data[user_id].face_size;
	this->dy_rate = CAMERA_H / this->user_data[user_id].face_size;

	/* ���݂̒ǐՈʒu�ƒ��O�̒ǐՈʒu�̍� */
	this->dx = this->pose_key_points[RIGHT_WRIST_X(personal_id)] - this->user_data[user_id].hand_point.x; // x����
	this->dy = this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - this->user_data[user_id].hand_point.y; // y����
	
	if (this->dx < this->user_data[user_id].face_size) {
		this->dx *= 0.3;
	}
	if (this->dy < this->user_data[user_id].face_size) {
		this->dy *= 0.3;
	}

	/* ���ꂼ��̕����̍��Ƀt�B���^�������� */
	this->dx = this->user_data[user_id].dx_filter->filter(this->dx);
	this->dy = this->user_data[user_id].dy_filter->filter(this->dy);

	/* �J�[�\���̈ʒu���X�V(���݂̒ǐՈʒu���瑊�ΓI�ɃJ�[�\���̈ʒu������) */
	this->user_data[user_id].cursor_point = Point(ofClamp(this->user_data[user_id].cursor_point.x + this->dx_rate * this->dx, 0, CAMERA_W), ofClamp(this->user_data[user_id].cursor_point.y + this->dy_rate * this->dy, 0, CAMERA_H));

	this->user_data[user_id].hand_point.x = this->pose_key_points[RIGHT_WRIST_X(personal_id)];
	this->user_data[user_id].hand_point.y = this->pose_key_points[RIGHT_WRIST_Y(personal_id)];

	this->transform_point(this->user_data[user_id].face_point, this->user_data[user_id].transformed_face_point); // ��̍��W����ʏ�̍��W�ɕϊ�
	this->transform_point(this->user_data[user_id].cursor_point, this->user_data[user_id].transformed_cursor_point); // �J�[�\���̍��W����ʏ�̍��W�ɕϊ�

	this->user_data[user_id].latest_update_frame = this->frame_count;
}

void HandCursor::get_frame() {
	while (!this->stop_flag) {
		this->image_buffer.emplace_front(this->cap.get_image());

		++this->frame_count;
	}
}

void HandCursor::transform_point(const Point& src_point, Point& dst_point) const {
	dst_point.x = DISPLAY_W - RESOLUTION_RATIO_W*src_point.x;
	dst_point.y = RESOLUTION_RATIO_H * src_point.y;
}

void HandCursor::inverse_transform_point(const Point& src_point, Point& dst_point) const {
	dst_point.x = (DISPLAY_W - src_point.x) / RESOLUTION_RATIO_W;
	dst_point.y = src_point.y / RESOLUTION_RATIO_H;
}

void HandCursor::modulate_cursor(const long long int& user_id) {
	try {
		this->inverse_transform_point(this->user_data.at(user_id).transformed_cursor_point, this->user_data.at(user_id).cursor_point);
	}
	catch (std::out_of_range&) {}
}

void HandCursor::show_detect_window() {
	this->view_frame = this->image_buffer.front();

	Mat img = this->view_frame;

	const int people_num{ this->pose_key_points.getSize(0) };
	for (int i = 0; i < people_num; ++i) {
		if (this->pose_key_points[NOSE_X(i)] != 0.0 && this->pose_key_points[NOSE_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)]), 15, this->CV_BLUE, -1);
		}

		if (this->pose_key_points[RIGHT_WRIST_X(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]), 15, this->CV_RED, -1);
		}

		//cv::rectangle(this->view_frame, Point(std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(i)]) - 200, 0), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(i)]) - 200, 0)), Point(std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(i)]) + 200, CAMERA_W), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(i)]) + 100, CAMERA_H)), this->CV_RED, 10);
	}

	for (const auto& t : this->user_data) {
		//cv::rectangle(img, Point(t.second.hand.left(), t.second.hand.top()), Point(t.second.hand.right(), t.second.hand.bottom()), this->CV_RED, 5);
		//cv::rectangle(img, t.second.face_rect, this->CV_BLUE, 10);
	}

	imshow("detect window", img);

	/* ����B�e */
	//this->writer << img;
}