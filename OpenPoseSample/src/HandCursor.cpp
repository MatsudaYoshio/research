#include "HandCursor.h"

using namespace std;
using namespace cv;
using namespace param;

const double HandCursor::display_operation_width_ratio{ DISPLAY_WIDTH / HandCursor::operation_width_rate };
const double HandCursor::display_operation_height_ratio{ DISPLAY_HEIGHT / HandCursor::operation_heght_rate };

const Point HandCursor::invalid_point{ -100,-100 }; // ��ʏ�ɕ\������Ȃ��悤�ȍ��W�̓_(�����_)
const Point HandCursor::display_center_point{ HALF_DISPLAY_WIDTH, HALF_DISPLAY_HEIGHT }; // �f�B�X�v���C�̒��S�̓_

/* �J�[�\���̐F */
const array<ofColor, HandCursor::cursor_color_num> HandCursor::cursor_colors = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange, ofColor::lightCoral, ofColor::goldenRod };

/* Scalar�^�̐F */
const Scalar HandCursor::CV_RED{ Scalar(0, 0, 255) };
const Scalar HandCursor::CV_BLUE{ Scalar(255, 0, 0) };
const Scalar HandCursor::CV_GREEN{ Scalar(0, 255, 0) };
const Scalar HandCursor::CV_ORANGE{ Scalar(76, 183, 255) };
const Scalar HandCursor::CV_PURPLE{ Scalar(204, 0, 196) };

HandCursor::HandCursor() {
	// �J�[�\���̉摜�Q��������
	this->cursor_images[0].load("fig/hammer.png");
	this->cursor_images[1].load("fig/hammer2.png");
	this->cursor_images[2].load("fig/racket.png");

	this->image_buffer.emplace_front(this->cap.get_image()); // �ŏ��̈ꖇ����荞��ł���

	/* �摜�擾�X���b�h */
	thread frame_thread(&HandCursor::get_frame, this);
	frame_thread.detach();

	/* ����B�e */
	//this->writer.open("OpenPose_test.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 14, Size(CAMERA_WIDTH, CAMERA_HEIGHT), true);
}

void HandCursor::update() {
	///* fps���R�}���h���C���ɏo�� */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	/* ��莞�Ԃ̊ԁA��񂪍X�V����Ă��Ȃ����[�U���폜���� */
	for (auto ite = begin(this->user_data); ite != end(this->user_data);) {
		if (this->frame_count - ite->second.latest_update_frame > 60) {
			this->cursor_color_state[ite->second.cursor_color_id] = false; // �F�����
			this->cursor_image_state[ite->second.cursor_image_id] = false; // �摜�����
			ite = this->user_data.erase(ite);
		}
		else {
			++ite;
		}
	}

	this->body_part_extractor(this->pose_key_points, this->image_buffer.front()); // openPose�ɂ�鍜�i����(���ʂ�pose_key_points�ɕۑ�)

	const int people_num{ this->pose_key_points.getSize(0) }; // ���o���ꂽ�l��

	for (int i = 0; i < people_num; ++i) {
		if (this->pose_key_points[NOSE_X(i)] == 0.0 || this->pose_key_points[NOSE_Y(i)] == 0.0) { // �炪���o�ł��Ȃ����
			continue;
		}
		double face_size{ this->estimate_face_size(i) }; // ��̑傫���𐄒�

		long long int user_id{ this->decide_user_id(i) }; // user_id������

		auto interaction_threshold{ (this->pose_key_points[RIGHT_SHOULDER_Y(i)] + this->pose_key_points[RIGHT_HIP_Y(i)]) / 2 }; // �E���ƉE���̒��ԓ_���C���^���N�V�����̊�_�Ƃ���

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

	const int cursor_image_id = distance(cbegin(this->cursor_image_state), find_if_not(cbegin(this->cursor_image_state), cend(this->cursor_image_state), [](auto x) {return x; })); // ���g�p�̐F����J�[�\���̐F��I��
	this->cursor_image_state[cursor_image_id] = true; // �I�񂾉摜���g�p�ς݂Ƃ���

	/* ���[�U�̏����ł��邾���l�߂ď����� */
	this->user_data.emplace(this->user_id, user_data_type{
		STATE::ACTIVE,
			this->frame_count,
			this->frame_count,
			Rect2d(this->pose_key_points[NOSE_X(personal_id)] - face_size / 2, this->pose_key_points[NOSE_Y(personal_id)] - face_size / 2, face_size, face_size),
			Rect2d(this->pose_key_points[NECK_X(personal_id)] - face_size*this->display_operation_width_ratio, (this->pose_key_points[NECK_Y(personal_id)] + this->pose_key_points[NOSE_Y(personal_id)]) / 2 - face_size*this->display_operation_height_ratio / 2, face_size*this->display_operation_width_ratio, face_size*this->display_operation_height_ratio),
			Point(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)]),
			Point(this->pose_key_points[NECK_X(personal_id)] - face_size*this->display_operation_width_ratio / 2, (this->pose_key_points[NECK_Y(personal_id)] + this->pose_key_points[NOSE_Y(personal_id)]) / 2),
			face_size,
			cursor_color_id,
			this->cursor_colors[cursor_color_id],
			cursor_image_id,
			this->cursor_images[cursor_image_id],
			this->invalid_point,
			this->invalid_point,
	});

	/* �t�B���^�̏����� */
	this->user_data[this->user_id].dx_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));
	this->user_data[this->user_id].dy_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));

	this->transform_point(this->user_data[this->user_id].face_point, this->user_data[this->user_id].transformed_face_point); // ��̍��W����ʏ�̍��W�ɕϊ�

	++this->user_id;
}

void HandCursor::renew_user_data(const int personal_id, const double face_size, const long long int user_id) {
	this->user_data[user_id].latest_update_frame = this->frame_count;

	if (this->user_data[user_id].state == STATE::INACTIVE) { // �J�[�\���������Ă����Ԃ��畜�A������
		this->user_data[user_id].cursor_point = this->invalid_point;
		this->user_data[user_id].start_frame = this->frame_count;
		this->user_data[user_id].state = STATE::ACTIVE;
		this->user_data[user_id].dx_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));
		this->user_data[user_id].dy_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));
	}

	/* ���߂̈��ʃt���[�����̂Ă�(�ŏ��̃J�[�\���̕\���������ڂ��ꂢ�ɂ��邽��) */
	if (this->user_data[user_id].latest_update_frame - this->user_data[user_id].start_frame < 5) {
		return;
	}

	this->user_data[user_id].state = STATE::ACTIVE;

	this->user_data[user_id].face_size = face_size;
	this->user_data[user_id].face_point.x = this->pose_key_points[NOSE_X(personal_id)];
	this->user_data[user_id].face_point.y = this->pose_key_points[NOSE_Y(personal_id)];

	/* �������W����̍����v�Z */
	this->dx = this->pose_key_points[RIGHT_WRIST_X(personal_id)] - this->user_data[user_id].initial_point.x;
	this->dy = this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - this->user_data[user_id].initial_point.y;

	this->transform_point(this->user_data[user_id].face_point, this->user_data[user_id].transformed_face_point); // ��̍��W����ʏ�̍��W�ɕϊ�

	this->user_data[user_id].cursor_point.x = this->display_center_point.x - this->moving_rate * this->operation_width_rate / this->user_data[user_id].face_size * dx;
	this->user_data[user_id].cursor_point.y = this->display_center_point.y + this->moving_rate * this->operation_heght_rate / this->user_data[user_id].face_size * dy;

	this->user_data[user_id].cursor_point.x = this->user_data[user_id].dx_filter->filter(this->user_data[user_id].cursor_point.x);
	this->user_data[user_id].cursor_point.y = this->user_data[user_id].dy_filter->filter(this->user_data[user_id].cursor_point.y);
}

void HandCursor::get_frame() {
	while (!this->stop_flag) {
		this->image_buffer.emplace_front(this->cap.get_image());

		++this->frame_count;
	}
}

void HandCursor::transform_point(const Point& src_point, Point& dst_point) const {
	dst_point.x = DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*src_point.x;
	dst_point.y = RESOLUTION_RATIO_HEIGHT * src_point.y;
}

void HandCursor::show_detect_window() {
	this->view_frame = this->image_buffer.front(); // �ŐV�t���[�����R�s�[

	const int people_num{ this->pose_key_points.getSize(0) }; // ���o���ꂽ�l��
	Concurrency::parallel_for(0, people_num, [&](int i) {
		Concurrency::parallel_for(0, 18, [&](int j) {
			if (this->pose_key_points[{i, j, 0}] != 0.0 && this->pose_key_points[{i, j, 1}] != 0.0) {
				cv::circle(this->view_frame, Point(this->pose_key_points[{i, j, 0}], this->pose_key_points[{i, j, 1}]), 12, this->CV_RED, -1);
			}
		});
	});

	imshow("detect window", this->view_frame);

	/* ����B�e */
	//this->writer << this->view_frame;
}