#include "HandCursor.h"

using namespace std;
using namespace cv;
using namespace param;

const double HandCursor::display_operation_width_ratio{ DISPLAY_W / HandCursor::operation_width_rate };
const double HandCursor::display_operation_height_ratio{ DISPLAY_H / HandCursor::operation_heght_rate };

const Point HandCursor::invalid_point{ -100,-100 }; // 画面上に表示されないような座標の点(無効点)
const Point HandCursor::display_center_point{ HALF_DISPLAY_W, HALF_DISPLAY_H }; // ディスプレイの中心の点

/* カーソルの色 */
const std::array<ofColor, HandCursor::cursor_color_num> HandCursor::cursor_colors = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange, ofColor::lightCoral, ofColor::goldenRod };

/* Scalar型の色 */
const Scalar HandCursor::CV_RED{ Scalar(0, 0, 255) };
const Scalar HandCursor::CV_BLUE{ Scalar(255, 0, 0) };
const Scalar HandCursor::CV_GREEN{ Scalar(0, 255, 0) };
const Scalar HandCursor::CV_ORANGE{ Scalar(76, 183, 255) };
const Scalar HandCursor::CV_PURPLE{ Scalar(204, 0, 196) };

HandCursor::HandCursor() {
	this->image_buffer.emplace_front(this->cap.get_image()); // 最初の一枚を取り込んでおく

	/* 画像取得スレッド */
	thread frame_thread(&HandCursor::get_frame, this);
	frame_thread.detach();

	//this->user_data[-100].alpha = 255;
	//this->user_data[-100].cursor_color = ofColor::orange;
	//this->user_data[-100].face_rect = Rect2d(200,200,200,200);
	//this->user_data[-100].cursor_point.x = 2937;
	//this->user_data[-100].cursor_point.y = 500;
	//this->user_data[-100].transformed_face_point.x = 200;
	//this->user_data[-100].transformed_face_point.y = 1000;
	//this->user_data[-100].state = STATE::ACTIVE;

	/* 動画撮影 */
	//this->writer.open("hand_tracking_openPose2.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 13, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	/* fpsを表示 */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	//this->user_data[-100].latest_update_frame = this->frame_count;

	/* 一定時間の間、情報が更新されていないユーザを削除する */
	for (auto ite = begin(this->user_data); ite != end(this->user_data);) {
		if (this->frame_count - ite->second.latest_update_frame > 60) {
			this->cursor_color_state[ite->second.cursor_color_id] = false; // 色を解放
			ite = this->user_data.erase(ite);
		}
		else {
			++ite;
		}
	}

	this->body_part_extractor(this->pose_key_points, this->image_buffer.front()); // openPoseによる骨格推定

	const int people_num{ this->pose_key_points.getSize(0) }; // 検出された人数

	for (int i = 0; i < people_num; ++i) {
		if (this->pose_key_points[NOSE_X(i)] == 0.0 || this->pose_key_points[NOSE_Y(i)] == 0.0) { // 顔が検出できなければ
			continue;
		}
		double face_size{ this->estimate_face_size(i) }; // 顔の大きさを推定

		long long int user_id{ this->decide_user_id(i) }; // user_idを決定

		auto interaction_threshold{ (this->pose_key_points[RIGHT_SHOULDER_Y(i)] + this->pose_key_points[MIDDLE_HIP_Y(i)]) / 2 }; // 腰と右肩の中間点をインタラクションの基準点とする

		if (this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] < interaction_threshold) {
			// 右手と右肩が検出され、右手が腰と右肩の中間点より上であれば
			if (user_id == this->new_user_id) { // 新しいユーザであれば
				this->init_user_data(i, face_size); // ユーザデータの初期化
			}
			else {
				this->renew_user_data(i, face_size, user_id); // ユーザデータの更新
			}
		}
		else {
			if (user_id != this->new_user_id) {
				this->user_data[user_id].state = STATE::INACTIVE;
			}
		}
	}

	//this->show_detect_window(); // 動作確認用のウィンドウを表示
}

void HandCursor::exit() {
	this->stop_flag = true;
}

double HandCursor::estimate_face_size(const int personal_id) const {
	if (this->pose_key_points[RIGHT_EAR_X(personal_id)] != 0.0 && this->pose_key_points[LEFT_EAR_X(personal_id)] != 0.0) { // 両耳の位置が取得できていれば
		return ofDist(this->pose_key_points[LEFT_EAR_X(personal_id)], this->pose_key_points[LEFT_EAR_Y(personal_id)], this->pose_key_points[RIGHT_EAR_X(personal_id)], this->pose_key_points[RIGHT_EAR_Y(personal_id)]); // 両耳間の距離を返す
	}

	if (this->pose_key_points[NECK_X(personal_id)] == 0.0) { // 首の位置が取得できなければ
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
	const int cursor_color_id = distance(cbegin(this->cursor_color_state), find_if_not(cbegin(this->cursor_color_state), cend(this->cursor_color_state), [](auto x) {return x; })); // 未使用の色からカーソルの色を選ぶ
	this->cursor_color_state[cursor_color_id] = true; // 選んだカーソルの色を使用済みとする

	/* ユーザの情報をできるだけ詰めて初期化 */
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
			ofColor::limit(),
			this->invalid_point,
			this->invalid_point,
	});
	this->user_data[this->user_id].dx_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));
	this->user_data[this->user_id].dy_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));

	this->transform_point(this->user_data[this->user_id].face_point, this->user_data[this->user_id].transformed_face_point); // 顔の座標を画面上の座標に変換

	++this->user_id;
}

void HandCursor::renew_user_data(const int personal_id, const double face_size, const long long int user_id) {
	this->user_data[user_id].latest_update_frame = this->frame_count;

	if (this->user_data[user_id].state == STATE::INACTIVE) { // カーソルが消えている状態から復帰したら
		this->user_data[user_id].cursor_point = this->invalid_point;
		this->user_data[user_id].start_frame = this->frame_count;
		this->user_data[user_id].state = STATE::ACTIVE;
		this->user_data[user_id].dx_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));
		this->user_data[user_id].dy_filter.reset(new OneEuroFilter(this->filter_freq, this->filter_mincutoff, this->filter_beta));
	}

	/* 初めの一定量フレームを捨てる(最初のカーソルの表示を見た目きれいにするため) */
	if (this->user_data[user_id].latest_update_frame - this->user_data[user_id].start_frame < 5) {
		return;
	}

	this->user_data[user_id].state = STATE::ACTIVE;
	this->user_data[user_id].alpha = MAX_ALFHA;

	this->user_data[user_id].face_size = face_size;
	this->user_data[user_id].face_point.x = this->pose_key_points[NOSE_X(personal_id)];
	this->user_data[user_id].face_point.y = this->pose_key_points[NOSE_Y(personal_id)];

	this->dx_rate = CAMERA_W / this->user_data[user_id].face_size;
	this->dy_rate = CAMERA_H / this->user_data[user_id].face_size;

	this->dx = this->pose_key_points[RIGHT_WRIST_X(personal_id)] - this->user_data[user_id].initial_point.x;
	this->dy = this->pose_key_points[RIGHT_WRIST_Y(personal_id)] - this->user_data[user_id].initial_point.y;

	this->transform_point(this->user_data[user_id].face_point, this->user_data[user_id].transformed_face_point); // 顔の座標を画面上の座標に変換

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
		if (this->pose_key_points[RIGHT_WRIST_X(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]), 12, this->CV_RED, -1);
		}
		if (this->pose_key_points[NOSE_X(i)] != 0.0 && this->pose_key_points[NOSE_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)]), 12, this->CV_BLUE, -1);
		}
		if (this->pose_key_points[RIGHT_EAR_X(i)] != 0.0 && this->pose_key_points[RIGHT_EAR_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_EAR_X(i)], this->pose_key_points[RIGHT_EAR_Y(i)]), 12, this->CV_ORANGE, -1);
		}
		if (this->pose_key_points[LEFT_EAR_X(i)] != 0.0 && this->pose_key_points[LEFT_EAR_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[LEFT_EAR_X(i)], this->pose_key_points[LEFT_EAR_Y(i)]), 12, this->CV_ORANGE, -1);
		}
		if (this->pose_key_points[RIGHT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[RIGHT_SHOULDER_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_SHOULDER_X(i)], this->pose_key_points[RIGHT_SHOULDER_Y(i)]), 12, this->CV_GREEN, -1);
		}
		if (this->pose_key_points[LEFT_SHOULDER_X(i)] != 0.0 && this->pose_key_points[LEFT_SHOULDER_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[LEFT_SHOULDER_X(i)], this->pose_key_points[LEFT_SHOULDER_Y(i)]), 12, this->CV_GREEN, -1);
		}
		if (this->pose_key_points[NECK_X(i)] != 0.0 && this->pose_key_points[NECK_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[NECK_X(i)], this->pose_key_points[NECK_Y(i)]), 12, this->CV_PURPLE, -1);
		}
		//cv::rectangle(this->view_frame, Point(std::max(static_cast<int>(this->pose_key_points[RIGHT_EAR_X(i)]) - 256, 0), std::max(static_cast<int>(this->pose_key_points[RIGHT_EAR_Y(i)]) - 72, 0)), Point(std::min(static_cast<int>(this->pose_key_points[RIGHT_EAR_X(i)]), CAMERA_W), std::min(static_cast<int>(this->pose_key_points[RIGHT_EAR_Y(i)]) + 72, CAMERA_H)), this->CV_RED, 10);
	});

	//for (const auto& t : this->user_data) {
	//	cv::rectangle(this->view_frame, t.second.operation_area, this->CV_RED, 10);
	//	//cout << t.second.operation_area.x << " " << t.second.operation_area.y << " " << t.second.operation_area.width << " " << t.second.operation_area.height << endl;
	//	//cv::rectangle(img, Point(t.second.hand.left(), t.second.hand.top()), Point(t.second.hand.right(), t.second.hand.bottom()), this->CV_RED, 5);
	//	//cv::rectangle(img, t.second.face_rect, this->CV_BLUE, 10);
	//}

	imshow("detect window", this->view_frame);

	/* 動画撮影 */
	//this->writer << this->view_frame;
}