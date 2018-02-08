#include "HandCursor.h"
#include <stdexcept>
#include <math.h>
#include <ctime>

using namespace std;
using namespace dlib;
using namespace cv;
using namespace param;

const char* HandCursor::model_path = "C:/Users/matsuda/workspace/machine_learning_data/hand/20170813/linear_svm_function.dat"; // 学習モデルのパス

random_device HandCursor::rd;
std::mt19937 HandCursor::mt(HandCursor::rd());

/* カーソルの色 */
const ofColor HandCursor::cursor_color_list[] = { ofColor::deepPink, ofColor::mediumPurple, ofColor::cyan, ofColor::blue, ofColor::red, ofColor::green, ofColor::black, ofColor::orange };
uniform_int_distribution<int> HandCursor::random_color(0, 7);

/* Scalar型の色 */
const Scalar HandCursor::CV_RED{ Scalar(0, 0, 255) };
const Scalar HandCursor::CV_BLUE{ Scalar(255, 0, 0) };
const Scalar HandCursor::CV_ORANGE{ Scalar(76, 183, 255) };

HandCursor::HandCursor() {
	deserialize(this->model_path) >> df; // ファイルから学習済みのモデルを読み込む

	//this->frame = Mat(Size(CAMERA_W, CAMERA_H), CV_8UC3);

	/* 最初の一枚を取り込んでおく */
	this->mat_org_image_buffer.get_push_position() = this->cap.get_image();
	this->mat_org_image_buffer.forward_offset();

	thread frame_thread(&HandCursor::get_frame, this);
	frame_thread.detach();

	/* 動画撮影 */
	//this->writer.open("hand_tracking_openPose.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), 30, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	/* fpsを表示 */
	//frc.NewFrame();
	//printf("fps : %lf\n", frc.GetFrameRate());

	if ((this->frame_count % 20) == 0) {
		this->body_part_extractor(this->pose_key_points, this->mat_org_image_buffer.get_read_position()); // openPoseによる骨格推定

		const auto people_num{ this->pose_key_points.getSize(0) }; // 検出された人数
		for (int i = 0; i < people_num; ++i) {
			if (this->pose_key_points[NOSE_X(i)] == 0.0 || this->pose_key_points[RIGHT_WRIST_X(i)] == 0.0) { // 顔か右手が検出されなければ
				continue;
			}

			if (ofDist(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)], this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]) > this->face_hand_distance) { // 顔と右手の距離が一定以上だったら
				continue;
			}

			auto face_size{ this->estimate_face_size(i) }; // 顔の大きさを推定

			auto user_id{ this->decide_user_id(i) };

			if (!user_id) {
				if (this->hand_detect(i, face_size)) {
					this->start_track(i, face_size);
				}
				else {
					continue;
				}
			}
			else {
				this->renew_user_data(i, face_size, user_id);
			}
		}
	}

	this->show_detect_window(); // 動作確認用のウィンドウを表示
}

void HandCursor::exit() {
	this->stop_flag = true;
}

double HandCursor::estimate_face_size(const int personal_id) {
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

int HandCursor::decide_user_id(const int personal_id) {
	int user_id{ 0 };
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

bool HandCursor::hand_detect(const int personal_id, const int face_size) {
	std::vector<std::vector<dlib::rectangle>> sliding_windows(3);
	SlidingWindows sw(face_size, face_size / 5, std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)]) - 200, 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)]) + 200, CAMERA_W), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)]) - 200, 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)]) + 100, CAMERA_H));
	SlidingWindows sw2(face_size + 50, face_size / 5 + 10, std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)]) - 200, 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)]) + 200, CAMERA_W), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)]) - 200, 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)]) + 100, CAMERA_H));
	SlidingWindows sw3(face_size + 100, face_size / 5 + 20, std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)]) - 200, 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_X(personal_id)]) + 200, CAMERA_W), std::max(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)]) - 200, 0), std::min(static_cast<int>(this->pose_key_points[RIGHT_WRIST_Y(personal_id)]) + 100, CAMERA_H));
	sliding_windows[0] = move(sw.get_windows());
	sliding_windows[1] = move(sw2.get_windows());
	sliding_windows[2] = move(sw3.get_windows());

	this->hand_dets_tmp.clear();
	for (const auto& sw : sliding_windows) {
		for (const auto& w : sw) {
			extract_image_chip(this->gs_image_buffer.get_read_position(), w, this->roi);
			if (this->is_hand(this->roi)) {
				this->hand_dets_tmp.emplace_back(w);
			}
		}
	}

	if (this->hand_dets_tmp.empty()) { // 検出された手がなければ
		return false;
	}

	this->hand_dets.clear();
	this->nms(this->hand_dets_tmp, this->hand_dets);

	return true;
}

/* 追跡時用の手検出 */
void HandCursor::hand_detect(const std::vector<dlib::rectangle>& sliding_windows, const int user_id) {
	for (const auto& w : sliding_windows) {
		extract_image_chip(this->gs_image_buffer.get_read_position(), w, this->roi_tracking);
		if (this->is_hand(this->roi_tracking)) {
			this->user_data[user_id].track_hand_dets.emplace_back(make_pair(this->frame_count, w));
		}
	}
}

void HandCursor::start_track(const int personal_id, const int face_size) {
	/* カーソルの色をかぶらないように選ぶ */
	int color_id;
SAME_COLOR:
	color_id = this->random_color(this->mt);
	for (const auto& t : this->user_data) {
		if (t.second.cursor_color_id == color_id) {
			goto SAME_COLOR;
		}
	}

	this->user_data.emplace_hint(cend(this->user_data), this->user_id, user_data_type{this->hand_dets[0], center(this->hand_dets[0]), point(this->pose_key_points[NOSE_X(personal_id)], this->pose_key_points[NOSE_Y(personal_id)]), face_size, move(color_id), this->cursor_color_list[color_id], });

	this->transform_point(this->user_data[this->user_id].face_point, this->user_data[this->user_id].transformed_face_point);
	this->transform_point(this->user_data[this->user_id].cursor_point, this->user_data[this->user_id].transformed_cursor_point);

	correlation_tracker ct;
	ct.start_track(this->org_image_buffer.get_read_position(), this->user_data[this->user_id].hand);

	thread th(&HandCursor::tracking, this, ct, user_id++);
	th.detach();
}

void HandCursor::renew_user_data(const int personal_id, const int face_size, const int user_id) {
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

void HandCursor::tracking(correlation_tracker &ct, const int user_id) {
	int m;
	int dx, dy;
	drectangle past_pos, current_pos;
	const double dx_rate = static_cast<double>(CAMERA_W) / this->user_data[user_id].face_size;
	const double dy_rate = static_cast<double>(CAMERA_H) / this->user_data[user_id].face_size;
	OneEuroFilter dx_filter(120, 0.5, 1.6, 1.0), dy_filter(120, 0.5, 1.6, 1.0);

	while (1) {
		m = 0;
		for (const auto &d : this->user_data[user_id].track_hand_dets) {
			if (d.first < this->frame_count - 30) {
				++m;
				continue;
			}
			break;
		}
		this->user_data[user_id].track_hand_dets.erase(begin(this->user_data[user_id].track_hand_dets), begin(this->user_data[user_id].track_hand_dets) + m);

		past_pos = ct.get_position(); // 直近フレームの手の位置を得る

		ct.update(this->org_image_buffer.get_read_position()); // 追跡位置の更新

		/* 現在の追跡位置(矩形)を得る */
		current_pos = ct.get_position();
		this->user_data[user_id].hand = current_pos;

		/* 現在の追跡位置の周辺のスライディングウィンドウを作成して手を検出 */
		/* 周辺とは追跡している手の矩形1個分周辺の範囲 */
		SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max(static_cast<int>(current_pos.left() - this->user_data[user_id].hand.width()), 0), std::min(static_cast<int>(current_pos.right() + this->user_data[user_id].hand.width()), CAMERA_W), std::max(static_cast<int>(current_pos.top() - this->user_data[user_id].hand.height()), 0), std::min(static_cast<int>(current_pos.bottom() + this->user_data[user_id].hand.height()), CAMERA_H));
		this->hand_detect(local_sw.get_windows(), user_id);

		/* 現在の追跡位置と直前の追跡位置の差 */
		dx = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x方向
		dy = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y方向

		dx = dx_filter.filter(dx);
		dy = dy_filter.filter(dy);

		/* カーソルの位置を更新 */
		this->user_data[user_id].cursor_point = point(ofClamp(this->user_data[user_id].cursor_point.x() + dx_rate * dx, 0, CAMERA_W), ofClamp(this->user_data[user_id].cursor_point.y() + dy_rate * dy, 0, CAMERA_H)); // 現在の追跡位置から相対的にカーソルの位置を決定
		this->transform_point(this->user_data[user_id].cursor_point, this->user_data[user_id].transformed_cursor_point);

		if (this->user_data[user_id].track_hand_dets.empty() || this->stop_flag) { // 直近フレームで手が検出されなかったら追跡をやめる
			this->user_data.erase(user_id);
			break;
		}
	}
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

void HandCursor::fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog) {
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.emplace_hint(end(feature_vector), j++, fhog[r][c](i));
			}
		}
	}
}

void HandCursor::transform_point(const point& src_point, point& dst_point) {
	dst_point.x() = DISPLAY_W - RESOLUTION_RATIO_W*src_point.x();
	dst_point.y() = RESOLUTION_RATIO_H * src_point.y();
}

void HandCursor::inverse_transform_point(const point& src_point, point& dst_point) {
	dst_point.x() = (DISPLAY_W - src_point.x()) / RESOLUTION_RATIO_W;
	dst_point.y() = src_point.y() / RESOLUTION_RATIO_H;
}

void HandCursor::modulate_cursor(const int& user_id) {
	try {
		this->inverse_transform_point(this->user_data.at(user_id).transformed_cursor_point, this->user_data.at(user_id).cursor_point);
	}
	catch (std::out_of_range&) {}
}

void HandCursor::show_detect_window() {
	this->view_frame = this->mat_org_image_buffer.get_read_position();

	const auto numberPeopleDetected = this->pose_key_points.getSize(0);
	for (int i = 0; i < numberPeopleDetected; ++i) {
		if (this->pose_key_points[NOSE_X(i)] != 0.0 && this->pose_key_points[NOSE_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[NOSE_X(i)], this->pose_key_points[NOSE_Y(i)]), 15, this->CV_BLUE, -1);
		}

		if (this->pose_key_points[RIGHT_WRIST_X(i)] != 0.0 && this->pose_key_points[RIGHT_WRIST_Y(i)] != 0.0) {
			cv::circle(this->view_frame, Point(this->pose_key_points[RIGHT_WRIST_X(i)], this->pose_key_points[RIGHT_WRIST_Y(i)]), 15, this->CV_RED, -1);
			//cv::rectangle(this->view_frame, Point(std::max(static_cast<int>(this->pose_key_points[{i, 4, 0}]) - 200, 0), std::max(static_cast<int>(this->pose_key_points[{i, 4, 1}]) - 200, 0)), Point(std::min(static_cast<int>(this->pose_key_points[{i, 4, 0}]) + 200, CAMERA_W), std::min(static_cast<int>(this->pose_key_points[{i, 4, 1}]) + 200, CAMERA_H)), this->CV_RED, 10);
		}
		//#undef NOSE_X
		//#undef NOSE_Y
		//#undef RIGHT_WRIST_X
		//#undef RIGHT_WRIST_X
		//if (this->pose_key_points[{i, 7, 0}] != 0.0 && this->pose_key_points[{i, 7, 1}] != 0.0) {
		//	cv::circle(this->view_frame, Point(this->pose_key_points[{ i, 7, 0 }], this->pose_key_points[{ i, 7, 1 }]), 15, this->CV_RED, -1);
		//}

		//const auto numberPeopleDetected = this->pose_key_points.get_read_position().getSize(0);
		////cout << "detect num : " << numberPeopleDetected << endl;
		//for (int i = 0; i < numberPeopleDetected; ++i) {
		//	if (this->pose_key_points.get_read_position()[{i, 0, 0}] != 0.0 && this->pose_key_points.get_read_position()[{i, 0, 1}] != 0.0) {
		//		cv::circle(this->view_frame, Point(this->pose_key_points.get_read_position()[{ i, 0, 0 }], this->pose_key_points.get_read_position()[{ i, 0, 1 }]), 15, this->CV_BLUE, -1);
		//	}

		//	if (this->pose_key_points.get_read_position()[{i, 4, 0}] != 0.0 && this->pose_key_points.get_read_position()[{i, 4, 1}] != 0.0) {
		//		cv::circle(this->view_frame, Point(this->pose_key_points.get_read_position()[{ i, 4, 0 }], this->pose_key_points.get_read_position()[{ i, 4, 1 }]), 15, this->CV_RED, -1);
		//	}

		//	if (this->pose_key_points.get_read_position()[{i, 7, 0}] != 0.0 && this->pose_key_points.get_read_position()[{i, 7, 1}] != 0.0) {
		//		cv::circle(this->view_frame, Point(this->pose_key_points.get_read_position()[{ i, 7, 0 }], this->pose_key_points.get_read_position()[{ i, 7, 1 }]), 15, this->CV_RED, -1);
		//	}

		/*try {
		cv::circle(this->view_frame, Point(this->pose_key_points.at({ i, 4, 0 }), this->pose_key_points.at({ i, 4, 1 })), 20, this->CV_RED, -1);
		}
		catch (std::out_of_range&) {}

		try {
		cv::circle(this->view_frame, Point(this->pose_key_points.at({ i, 7, 0 }), this->pose_key_points.at({ i, 7, 1 })), 20, this->CV_RED, -1);
		}
		catch (std::out_of_range&) {}*/

		//this->pose_key_points.at({ i, 0, 0 });
		//cv::circle(this->view_frame, Point(this->pose_key_points[{i, 0, 0}], this->pose_key_points[{i, 0, 1}]), 20, this->CV_BLUE, -1);
		//cv::circle(this->view_frame, Point(this->pose_key_points[{i, 4, 0}], this->pose_key_points[{i, 4, 1}]), 20, this->CV_RED, -1);
		//cv::circle(this->view_frame, Point(this->pose_key_points[{i, 7, 0}], this->pose_key_points[{i, 7, 1}]), 20, this->CV_RED, -1);

		//cout << this->pose_key_points[{i, 0, 0}] << " " << this->pose_key_points[{i, 0, 1}] << " " << this->pose_key_points[{i, 4, 0}] << " " << this->pose_key_points[{i, 4, 1}] << endl;
	}

	//parallel_for(0, this->hand_dets.size(), [&](int i) {
	//	cv::rectangle(view_frame, Point(this->hand_dets[i].left(), this->hand_dets[i].top()), Point(this->hand_dets[i].right(), this->hand_dets[i].bottom()), this->CV_RED, 5);
	//});

	//parallel_for(0, this->face_dets.size(), [&](int i) {
	//	cv::rectangle(view_frame, Point(this->face_dets[i].left(), this->face_dets[i].top()), Point(this->face_dets[i].right(), this->face_dets[i].bottom()), this->CV_BLUE, 5);
	//});

	for (const auto &t : this->user_data) {
		cv::rectangle(view_frame, Point(t.second.hand.left(), t.second.hand.top()), Point(t.second.hand.right(), t.second.hand.bottom()), this->CV_ORANGE, 5);
	}

	imshow("detect window", view_frame);

	//this->writer << this->view_frame;
}