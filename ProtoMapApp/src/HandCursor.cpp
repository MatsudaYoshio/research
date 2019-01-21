#include "HandCursor.h"

using namespace std;
using namespace cv;
using namespace param;

const double HandCursor::display_operation_width_ratio{ DISPLAY_W / HandCursor::operation_width_rate };
const double HandCursor::display_operation_height_ratio{ DISPLAY_H / HandCursor::operation_heght_rate };

const Point HandCursor::invalid_point{ -100,-100 }; // ��ʏ�ɕ\������Ȃ��悤�ȍ��W�̓_(�����_)
const Point HandCursor::display_center_point{ HALF_DISPLAY_W, HALF_DISPLAY_H }; // �f�B�X�v���C�̒��S�̓_

/* �蓮�ŏ��������J�[�\���̐F */
const std::array<ofColor, HandCursor::initial_cursor_color_num> HandCursor::initial_cursor_colors = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange, ofColor::lightCoral, ofColor::goldenRod };

HandCursor::HandCursor():cursor_colors(this->initial_cursor_color_num), cursor_color_state(this->initial_cursor_color_num) {
	/* �蓮�ŏ��������J�[�\���̐F���Z�b�g */
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> random_color_id(0, this->initial_cursor_color_num - 1);

	for (const auto& c : this->initial_cursor_colors) {
		this->cg.add_color(c);
	}

	for (int i = 0; i < this->initial_cursor_color_num; ++i) {
		this->cursor_colors[i] = this->cg.used_colors[i].second;
	}

	shuffle(begin(this->cursor_colors), end(this->cursor_colors), mt);

	this->image_buffer.emplace_front(this->cap.get_image()); // �ŏ��̈ꖇ����荞��ł���

	/* �摜�擾�X���b�h */
	thread frame_thread(&HandCursor::get_frame, this);
	frame_thread.detach();

	//this->user_data[-100].alpha = 255;
	//this->user_data[-100].cursor_color = ofColor::orange;
	//this->user_data[-100].face_rect = Rect2d(200,200,200,200);
	//this->user_data[-100].cursor_point.x = 2937;
	//this->user_data[-100].cursor_point.y = 500;
	//this->user_data[-100].transformed_face_point.x = 200;
	//this->user_data[-100].transformed_face_point.y = 1500;
	//this->user_data[-100].state = STATE::ACTIVE;

	/* ����B�e */
	//this->writer.open("multi-user_interaction5.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 18, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	/* fps��\�� */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	//this->user_data[-100].latest_update_frame = this->frame_count;

	/* ��莞�Ԃ̊ԁA��񂪍X�V����Ă��Ȃ����[�U���폜���� */
	for (auto ite = begin(this->user_data); ite != end(this->user_data);) {
		if (this->frame_count - ite->second.latest_update_frame > 60) {
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

		if (user_id == this->new_user_id) { // �V�������[�U�ł����
			if (this->is_start_interaction_by_right_hand(i)) { // �E����グ�Ă����
				this->init_user_data(i, face_size, USING_HAND::RIGHT); // �E��ŃC���^���N�V�������J�n�����Ƃ��āA���[�U�f�[�^�̏�����
			}
			else if (this->is_start_interaction_by_left_hand(i)) { // ������グ�Ă����
				this->init_user_data(i, face_size, USING_HAND::LEFT); // ����ŃC���^���N�V�������J�n�����Ƃ��āA���[�U�f�[�^�̏�����
			}
		}
		else { // ���ɑ��삵�Ă��郆�[�U�ł����
			if (this->user_data[user_id].hand == USING_HAND::RIGHT && !this->is_start_interaction_by_right_hand(i)) { // �E��ő��삵�Ă������E�肪�オ���Ă��Ȃ����
				this->user_data[user_id].state = STATE::INACTIVE;
			}
			else if (this->user_data[user_id].hand == USING_HAND::LEFT && !this->is_start_interaction_by_left_hand(i)) { // ����ő��삵�Ă��������肪�オ���Ă��Ȃ����
				this->user_data[user_id].state = STATE::INACTIVE;
			}
			else {
				this->renew_user_data(i, face_size, user_id); // ���[�U�f�[�^�̍X�V
			}
		}
	}

	if (this->user_data.size() > this->initial_cursor_color_num) { // �蓮�ŏ��������J�[�\���̐F������Ȃ�������A�����I�ɐ������đ��₷
		this->cg();
		this->cursor_colors.emplace_back(this->cg.used_colors.back().second);
		this->cursor_color_state.emplace_back(false);
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

bool HandCursor::is_start_interaction_by_right_hand(const int personal_id) const {
	return this->pose_key_points[RIGHT_WRIST_Y(personal_id)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(personal_id)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(personal_id)] < (this->pose_key_points[RIGHT_SHOULDER_Y(personal_id)] + this->pose_key_points[MIDDLE_HIP_Y(personal_id)]) / 2;
}

bool HandCursor::is_start_interaction_by_left_hand(int personal_id) const {
	return this->pose_key_points[LEFT_WRIST_Y(personal_id)] != 0.0 && this->pose_key_points[LEFT_SHOULDER_Y(personal_id)] != 0.0 && this->pose_key_points[LEFT_WRIST_Y(personal_id)] < (this->pose_key_points[LEFT_SHOULDER_Y(personal_id)] + this->pose_key_points[MIDDLE_HIP_Y(personal_id)]) / 2;
}

void HandCursor::init_user_data(const int personal_id, const double face_size, USING_HAND hand) {
	const int cursor_color_id = distance(cbegin(this->cursor_color_state), find_if_not(cbegin(this->cursor_color_state), cend(this->cursor_color_state), [](auto x) {return x; })); // ���g�p�̐F����J�[�\���̐F��I��
	this->cursor_color_state[cursor_color_id] = true; // �I�񂾃J�[�\���̐F���g�p�ς݂Ƃ���

	this->cg();
	/* ���[�U�̏����ł��邾���l�߂ď����� */
	this->user_data.emplace(this->user_id, user_data_type{
		STATE::ACTIVE,
			hand,
			this->frame_count,
			this->frame_count,
			Rect2d(this->pose_key_points[NOSE_X(personal_id)] - face_size / 2, this->pose_key_points[NOSE_Y(personal_id)] - face_size / 2, face_size, face_size),
			Rect2d(this->pose_key_points[NECK_X(personal_id)] - face_size*this->display_operation_width_ratio, (this->pose_key_points[NECK_Y(personal_id)] + this->pose_key_points[NOSE_Y(personal_id)]) / 2 - face_size*this->display_operation_height_ratio / 2, face_size*this->display_operation_width_ratio, face_size*this->display_operation_height_ratio),
			Point(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)]),
			Point(this->pose_key_points[NECK_X(personal_id)] - face_size*this->display_operation_width_ratio / 2, (this->pose_key_points[NECK_Y(personal_id)] + this->pose_key_points[NOSE_Y(personal_id)]) / 2),
			face_size,
			cursor_color_id,
			this->cursor_colors[cursor_color_id],
			ofColor::limit(),
			this->invalid_point,
			this->invalid_point,
	});

	if (hand == USING_HAND::LEFT) {
		auto center_diff{ this->pose_key_points[NECK_X(personal_id)] - this->user_data[this->user_id].initial_point.x };
		this->user_data[this->user_id].initial_point.x += 2 * center_diff;
		this->user_data[this->user_id].operation_area.x += 2 * center_diff;
	}

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
	this->user_data[user_id].alpha = MAX_ALFHA;

	this->user_data[user_id].face_size = face_size;
	this->user_data[user_id].face_point.x = this->pose_key_points[NOSE_X(personal_id)];
	this->user_data[user_id].face_point.y = this->pose_key_points[NOSE_Y(personal_id)];

	/* �������W����̍����v�Z */
	if (this->user_data[user_id].hand == USING_HAND::RIGHT) {
		this->dx = this->pose_key_points[RIGHT_WRIST_X(personal_id)] - this->user_data[user_id].initial_point.x;
		this->dy = this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - this->user_data[user_id].initial_point.y;
	}
	else {
		this->dx = this->pose_key_points[LEFT_WRIST_X(personal_id)] - this->user_data[user_id].initial_point.x;
		this->dy = this->pose_key_points[LEFT_WRIST_Y(personal_id)] - this->user_data[user_id].initial_point.y;
	}

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
	dst_point.x = DISPLAY_W - RESOLUTION_RATIO_W*src_point.x;
	dst_point.y = RESOLUTION_RATIO_H * src_point.y;
}

void HandCursor::overlap_window(const long long int user_id) {
	this->user_data[user_id].state = STATE::OVERLAP;
	this->user_data[user_id].alpha = HALF_MAX_ALFHA;
}

void HandCursor::show_detect_window() {
	this->view_frame = this->image_buffer.front();

	const int people_num{ this->pose_key_points.getSize(0) };
	Concurrency::parallel_for(0, people_num, [&](int i) {
		// ���i�̐�����`��
		if (this->pose_key_points[NOSE_X(i)] != 0.0 && this->pose_key_points[NOSE_Y(i)] != 0.0 && this->pose_key_points[NECK_X(i)] != 0.0 && this->pose_key_points[NECK_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)]), Point(this->pose_key_points[NECK_X(i)], this->pose_key_points[NECK_Y(i)]), Scalar(ofColor::mediumVioletRed.b, ofColor::mediumVioletRed.g, ofColor::mediumVioletRed.r), 10);
		}
		if (this->pose_key_points[NECK_X(i)] != 0.0 && this->pose_key_points[NECK_Y(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[NECK_X(i)], this->pose_key_points[NECK_Y(i)]), Point(this->pose_key_points[RIGHT_SHOULDER_X(i)], this->pose_key_points[RIGHT_SHOULDER_Y(i)]), Scalar(ofColor::orangeRed.b, ofColor::orangeRed.g, ofColor::orangeRed.r), 10);
		}
		if (this->pose_key_points[NECK_X(i)] != 0.0 && this->pose_key_points[NECK_Y(i)] != 0.0 && this->pose_key_points[LEFT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[LEFT_SHOULDER_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[NECK_X(i)], this->pose_key_points[NECK_Y(i)]), Point(this->pose_key_points[LEFT_SHOULDER_X(i)], this->pose_key_points[LEFT_SHOULDER_Y(i)]), Scalar(ofColor::chartreuse.b, ofColor::chartreuse.g, ofColor::chartreuse.r), 10);
		}
		if (this->pose_key_points[NECK_X(i)] != 0.0 && this->pose_key_points[NECK_Y(i)] != 0.0 && this->pose_key_points[MIDDLE_HIP_X(i)] != 0.0 && this->pose_key_points[MIDDLE_HIP_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[NECK_X(i)], this->pose_key_points[NECK_Y(i)]), Point(this->pose_key_points[MIDDLE_HIP_X(i)], this->pose_key_points[MIDDLE_HIP_Y(i)]), Scalar(0, 0, 255), 10);
		}
		if (this->pose_key_points[RIGHT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(i)] != 0.0 && this->pose_key_points[RIGHT_ELBOW_X(i)] != 0.0 && this->pose_key_points[RIGHT_ELBOW_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[RIGHT_SHOULDER_X(i)], this->pose_key_points[RIGHT_SHOULDER_Y(i)]), Point(this->pose_key_points[RIGHT_ELBOW_X(i)], this->pose_key_points[RIGHT_ELBOW_Y(i)]), Scalar(ofColor::goldenRod.b, ofColor::goldenRod.g, ofColor::goldenRod.r), 10);
		}
		if (this->pose_key_points[RIGHT_ELBOW_X(i)] != 0.0 && this->pose_key_points[RIGHT_ELBOW_Y(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_X(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[RIGHT_ELBOW_X(i)], this->pose_key_points[RIGHT_ELBOW_Y(i)]), Point(this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]), Scalar(0, 255, 255), 10);
		}
		if (this->pose_key_points[LEFT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[LEFT_SHOULDER_Y(i)] != 0.0 && this->pose_key_points[LEFT_ELBOW_X(i)] != 0.0 && this->pose_key_points[LEFT_ELBOW_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[LEFT_SHOULDER_X(i)], this->pose_key_points[LEFT_SHOULDER_Y(i)]), Point(this->pose_key_points[LEFT_ELBOW_X(i)], this->pose_key_points[LEFT_ELBOW_Y(i)]), Scalar(ofColor::lime.b, ofColor::lime.g, ofColor::lime.r), 10);
		}
		if (this->pose_key_points[LEFT_ELBOW_X(i)] != 0.0 && this->pose_key_points[LEFT_ELBOW_Y(i)] != 0.0 && this->pose_key_points[LEFT_WRIST_X(i)] != 0.0 && this->pose_key_points[LEFT_WRIST_Y(i)] != 0.0) {
			line(this->view_frame, Point(this->pose_key_points[LEFT_ELBOW_X(i)], this->pose_key_points[LEFT_ELBOW_Y(i)]), Point(this->pose_key_points[LEFT_WRIST_X(i)], this->pose_key_points[LEFT_WRIST_Y(i)]), Scalar(0, 255, 0), 10);
		}

		// ���i�_��`��
		if (this->pose_key_points[NOSE_X(i)] != 0.0 && this->pose_key_points[NOSE_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)]), 12, Scalar(ofColor::mediumVioletRed.b, ofColor::mediumVioletRed.g, ofColor::mediumVioletRed.r), -1);
		}
		if (this->pose_key_points[NECK_X(i)] != 0.0 && this->pose_key_points[NECK_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[NECK_X(i)], this->pose_key_points[NECK_Y(i)]), 12, Scalar(0, 0, 255), -1);
		}
		if (this->pose_key_points[RIGHT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_SHOULDER_X(i)], this->pose_key_points[RIGHT_SHOULDER_Y(i)]), 12, Scalar(ofColor::orangeRed.b, ofColor::orangeRed.g, ofColor::orangeRed.r), -1);
		}
		if (this->pose_key_points[RIGHT_ELBOW_X(i)] != 0.0 && this->pose_key_points[RIGHT_ELBOW_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_ELBOW_X(i)], this->pose_key_points[RIGHT_ELBOW_Y(i)]), 12, Scalar(ofColor::goldenRod.b, ofColor::goldenRod.g, ofColor::goldenRod.r), -1);
		}
		if (this->pose_key_points[RIGHT_WRIST_X(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]), 12, Scalar(0, 255, 255), -1);
		}
		if (this->pose_key_points[LEFT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[LEFT_SHOULDER_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[LEFT_SHOULDER_X(i)], this->pose_key_points[LEFT_SHOULDER_Y(i)]), 12, Scalar(ofColor::chartreuse.b, ofColor::chartreuse.g, ofColor::chartreuse.r), -1);
		}
		if (this->pose_key_points[LEFT_ELBOW_X(i)] != 0.0 && this->pose_key_points[LEFT_ELBOW_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[LEFT_ELBOW_X(i)], this->pose_key_points[LEFT_ELBOW_Y(i)]), 12, Scalar(ofColor::lime.b, ofColor::lime.g, ofColor::lime.r), -1);
		}
		if (this->pose_key_points[LEFT_WRIST_X(i)] != 0.0 && this->pose_key_points[LEFT_WRIST_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[LEFT_WRIST_X(i)], this->pose_key_points[LEFT_WRIST_Y(i)]), 12, Scalar(0, 255, 0), -1);
		}
		if (this->pose_key_points[MIDDLE_HIP_X(i)] != 0.0 && this->pose_key_points[MIDDLE_HIP_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[MIDDLE_HIP_X(i)], this->pose_key_points[MIDDLE_HIP_Y(i)]), 12, Scalar(0, 0, 255), -1);
		}
	});

	for (const auto& t : this->user_data) {
		cv::rectangle(this->view_frame, t.second.operation_area, Scalar(255, 0, 0), 10);
	}

	imshow("detect window", this->view_frame);

	/* ����B�e */
	this->writer << this->view_frame;
}