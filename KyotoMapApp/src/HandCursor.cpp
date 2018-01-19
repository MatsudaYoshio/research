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
const Scalar HandCursor::CV_RED = Scalar(0, 0, 255);
const Scalar HandCursor::CV_BLUE = Scalar(255, 0, 0);
const Scalar HandCursor::CV_ORANGE = Scalar(76, 183, 255);

HandCursor::HandCursor() :nms(this->overlap_ratio), face_thread_flag(false), hand_thread_flag(false), stop_flag(false), frame_count(0), track_id(0) {
	this->face_detector = get_frontal_face_detector();

	deserialize(this->model_path) >> df; // ファイルから学習済みのモデルを読み込む

	this->frame = Mat(Size(CAMERA_W, CAMERA_H), CV_8UC3);

	/* 渉成園 */
	this->track_data[-1].cursor_point.x() = 550;
	this->track_data[-1].cursor_point.y() = 650;
	/* 京都タワー */
	//this->track_data[-1].cursor_point.x() = 800;
	//this->track_data[-1].cursor_point.y() = 900;
	/* 龍谷ミュージアム */
	//this->track_data[-1].cursor_point.x() = 1200;
	//this->track_data[-1].cursor_point.y() = 450;
	/* 東本願寺 */
	//this->track_data[-1].cursor_point.x() = 900;
	//this->track_data[-1].cursor_point.y() = 600;
	/* 西本願寺 */
	//this->track_data[-1].cursor_point.x() = 1350;
	//this->track_data[-1].cursor_point.y() = 600;
	/* 京都水族館 */
	//this->track_data[-1].cursor_point.x() = 1800;
	//this->track_data[-1].cursor_point.y() = 900;

	this->transform_point(this->track_data[-1].cursor_point, this->track_data[-1].transformed_cursor_point);
	this->track_data[-1].face_rect = dlib::rectangle(HALF_DISPLAY_W * 3 / 4 + 300, HALF_DISPLAY_H / 2 - 300, 300, 300);
	this->track_data[-1].face_point = center(this->track_data[-1].face_rect);
	this->transform_point(this->track_data[-1].face_point, this->track_data[-1].transformed_face_point);
	this->track_data[-1].cursor_color_id = 0;
	this->track_data[-1].cursor_color = ofColor::deepPink;

	/* 西本願寺 */
	//this->track_data[-2].cursor_point.x() = 1350;
	//this->track_data[-2].cursor_point.y() = 600;

	//this->transform_point(this->track_data[-2].cursor_point, this->track_data[-2].transformed_cursor_point);
	//this->track_data[-2].face_rect = dlib::rectangle(HALF_DISPLAY_W * 3 / 4, HALF_DISPLAY_H / 2 + 300, 300, 300);
	//this->track_data[-2].face_point = center(this->track_data[-2].face_rect);
	//this->transform_point(this->track_data[-2].face_point, this->track_data[-2].transformed_face_point);
	//this->track_data[-2].cursor_color_id = 1;
	//this->track_data[-2].cursor_color = ofColor::mediumPurple;

	//this->writer.open("hand_detect4.avi", CV_FOURCC_DEFAULT, 12, Size(CAMERA_W, CAMERA_H), true);
}

void HandCursor::update() {
	while (!this->stop_flag) {
		++this->frame_count;

		/* fpsを表示 */
		//frc.NewFrame();
		//printf("fps : %lf\n", frc.GetFrameRate());

		this->frame = this->cap.get_image(); // カメラから画像を取得

		assign_image(this->org_image_buffer.get_push_position(), cv_image<bgr_pixel>(this->frame));
		this->org_image_buffer.forward_offset();

		assign_image(this->gs_image_buffer.get_push_position(), this->org_image_buffer.get_read_position());
		this->gs_image_buffer.forward_offset();

		if (!this->face_thread_flag) {
			this->new_thread_face_detect();
		}

		if (!this->face_dets.empty() && !this->hand_thread_flag) {
			this->new_thread_hand_detect();
		}

		/* 検出された顔のデータを準備する */
		//this->detect_face_data.clear();
		//this->detect_face_data.resize(face_dets.size());
		//int i = 0;
		//for (const auto& fd : face_dets) {
		//	this->transform_point(fd.tl_corner(), this->detect_face_data[i++]);
		//}

		//printf("frame count : %ld\n", this->frame_count);
		//this->show_detect_window(); // 検出チェック用のウィンドウを表示
	}
}

void HandCursor::exit() {
	this->stop_flag = true;
}

void HandCursor::modulate_cursor(const int& user_id) {
	try {
		this->inverse_transform_point(this->track_data.at(user_id).transformed_cursor_point, this->track_data.at(user_id).cursor_point);
	}
	catch (std::out_of_range&) {}
}

void HandCursor::show_detect_window() {
	this->view_frame = this->frame;

	parallel_for(0, this->hand_dets.size(), [&](int i) {
		cv::rectangle(view_frame, Point(this->hand_dets[i].left(), this->hand_dets[i].top()), Point(this->hand_dets[i].right(), this->hand_dets[i].bottom()), this->CV_RED, 5);
	});

	parallel_for(0, this->face_dets.size(), [&](int i) {
		cv::rectangle(view_frame, Point(this->face_dets[i].left(), this->face_dets[i].top()), Point(this->face_dets[i].right(), this->face_dets[i].bottom()), this->CV_BLUE, 5);
	});

	for (const auto &t : this->track_data) {
		cv::rectangle(view_frame, Point(t.second.hand.left(), t.second.hand.top()), Point(t.second.hand.right(), t.second.hand.bottom()), this->CV_ORANGE, 5);
	}

	imshow("detect window", view_frame);

	//this->writer << view_frame;
}

/* 顔検出 */
void HandCursor::face_detect() {
	this->face_thread_flag = true;

	this->face_dets = this->face_detector(this->gs_image_buffer.get_read_position());

	if (!this->face_dets.empty()) { // 検出した顔があれば

		/* 既に追跡している顔の近くの顔は除く */
		for (const auto& td : this->track_data) {
			for (auto fd = begin(this->face_dets); fd != end(this->face_dets);) {
				if (this->euclid_distance(td.second.face_point, center(*fd)) < 300) {
					fd = this->face_dets.erase(fd);
				}
				else {
					++fd;
				}
			}
		}

		if (!this->hand_thread_flag) {
			this->new_thread_hand_detect(); // 手検出スレッドを起動
		}
	}

	this->face_thread_flag = false;
}

/* 手検出 */
void HandCursor::hand_detect() {
	this->hand_thread_flag = true;

	this->hand_dets.clear();

	std::vector<dlib::rectangle> face_dets_tmp = this->face_dets, hand_dets_tmp;
	std::vector<dlib::rectangle> windows; // スライディングウィンドウ用の矩形
	array2d<unsigned char> roi;

	/* 既に追跡している顔の近くの顔は除く */
	for (const auto &td : this->track_data) {
		for (auto fd = begin(face_dets_tmp); fd != end(face_dets_tmp);) {
			if (this->euclid_distance(td.second.face_point, center(*fd)) < 300) {
				fd = face_dets_tmp.erase(fd);
			}
			else {
				++fd;
			}
		}
	}

	for (const auto &fd : face_dets_tmp) {
		//* 顔の周辺のスライディングウィンドウを作成(ウィンドウサイズが顔より大きくて異なる3種類) */
		std::vector<std::vector<dlib::rectangle>> sliding_windows(3);
		SlidingWindows sw(fd.width(), fd.width() / 5, std::max(static_cast<int>(fd.left()) - 300, 0), std::min(static_cast<int>(fd.right()) + 300, CAMERA_W), std::max(static_cast<int>(fd.top()) - 300, 0), std::min(static_cast<int>(fd.bottom()) + 200, CAMERA_H));
		SlidingWindows sw2(fd.width() + 50, fd.width() / 5 + 10, std::max(static_cast<int>(fd.left()) - 300, 0), std::min(static_cast<int>(fd.right()) + 300, CAMERA_W), std::max(static_cast<int>(fd.top()) - 300, 0), std::min(static_cast<int>(fd.bottom()) + 200, CAMERA_H));
		SlidingWindows sw3(fd.width() + 100, fd.width() / 5 + 20, std::max(static_cast<int>(fd.left()) - 300, 0), std::min(static_cast<int>(fd.right()) + 300, CAMERA_W), std::max(static_cast<int>(fd.top()) - 300, 0), std::min(static_cast<int>(fd.bottom()) + 200, CAMERA_H));
		sliding_windows[0] = move(sw.get_windows());
		sliding_windows[1] = move(sw2.get_windows());
		sliding_windows[2] = move(sw3.get_windows());

		/* 顔の周辺のスライディングウィンドウで手検出 */
		for (const auto& sw : sliding_windows) {
			for (const auto& w : sw) {
				extract_image_chip(this->gs_image_buffer.get_read_position(), w, roi);
				if (this->is_hand(roi)) {
					hand_dets_tmp.emplace_back(w);
				}
			}
		}

		if (hand_dets_tmp.empty()) { // 検出された手がなければ
			continue;
		}

		/* 既に追跡している手の近くの手を除く */
		for (const auto& td : this->track_data) {
			for (auto hd = begin(hand_dets_tmp); hd != end(hand_dets_tmp);) {
				if (this->euclid_distance(center(td.second.hand), center(*hd)) < 300) {
					hd = hand_dets_tmp.erase(hd);
				}
				else {
					++hd;
				}
			}
		}

		if (hand_dets_tmp.empty()) { // すべての手候補が既に追跡している手の近くの手として削除されたら
			continue;
		}

		/* NonMaximumSuppressionにかけて重複を取り除く */
		this->nms(hand_dets_tmp, this->hand_dets);

		this->track_data[this->track_id].hand = this->hand_dets[0];
		this->track_data[this->track_id].face_rect = fd;
		this->track_data[this->track_id].face_point = center(fd);
		this->transform_point(this->track_data[this->track_id].face_point, this->track_data[this->track_id].transformed_face_point);

		correlation_tracker ct;
		ct.start_track(this->org_image_buffer.get_read_position(), this->hand_dets[0]);

		this->track_data[this->track_id].cursor_point = center(this->hand_dets[0]);
		this->transform_point(this->track_data[this->track_id].cursor_point, this->track_data[this->track_id].transformed_cursor_point);

		/* カーソルの色をかぶらないように選ぶ */
		int color_id;
	SAME_COLOR:
		color_id = this->random_color(this->mt);
		for (const auto& t : this->track_data) {
			if (t.second.cursor_color_id == color_id) {
				goto SAME_COLOR;
			}
		}
		this->track_data[this->track_id].cursor_color = this->cursor_color_list[color_id];
		this->track_data[this->track_id].cursor_color_id = move(color_id);

		this->new_thread_tracking(ct, this->track_id++);
	}

	this->hand_thread_flag = false;
}

/* 追跡時用の手検出 */
void HandCursor::hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int &user_id) {
	array2d<unsigned char> roi;
	for (const auto &w : sliding_windows) {
		extract_image_chip(this->gs_image_buffer.get_read_position(), w, roi);
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
	this->fhog_to_feature_vector(feature_vec, fhog);

	return (df(feature_vec) > this->decision_ratio);
}

void HandCursor::tracking(correlation_tracker &ct, const int user_id) {
	int m;
	int dx, dy;
	drectangle past_pos, current_pos;
	const double dx_rate = static_cast<double>(CAMERA_W) / this->track_data[user_id].face_rect.width();
	const double dy_rate = static_cast<double>(CAMERA_H) / this->track_data[user_id].face_rect.height();
	OneEuroFilter dx_filter(120, 0.5, 1.6, 1.0), dy_filter(120, 0.5, 1.6, 1.0);

	while (1) {
		/* 直近のフレームで検出した手以外を消す */
		m = 0;
		for (const auto &d : this->track_data[user_id].track_hand_dets) {
			if (d.first < this->frame_count - 30) {
				++m;
				continue;
			}
			break;
		}
		this->track_data[user_id].track_hand_dets.erase(begin(this->track_data[user_id].track_hand_dets), begin(this->track_data[user_id].track_hand_dets) + m);

		past_pos = ct.get_position(); // 直近フレームの手の位置を得る

		ct.update(this->org_image_buffer.get_read_position()); // 追跡位置の更新

		/* 現在の追跡位置(矩形)を得る */
		current_pos = ct.get_position();
		this->track_data[user_id].hand = current_pos;

		/* 現在の追跡位置の周辺のスライディングウィンドウを作成して手を検出 */
		/* 周辺とは追跡している手の矩形1個分周辺の範囲 */
		SlidingWindows local_sw(current_pos.width(), current_pos.width() / 5, std::max(static_cast<int>(current_pos.left() - this->track_data[user_id].hand.width()), 0), std::min(static_cast<int>(current_pos.right() + this->track_data[user_id].hand.width()), CAMERA_W), std::max(static_cast<int>(current_pos.top() - this->track_data[user_id].hand.height()), 0), std::min(static_cast<int>(current_pos.bottom() + this->track_data[user_id].hand.height()), CAMERA_H));
		this->hand_detect(local_sw.get_windows(), user_id);

		/* 現在の追跡位置と直前の追跡位置の差 */
		dx = (current_pos.left() + current_pos.right() - past_pos.left() - past_pos.right()) / 2; // x方向
		dy = (current_pos.top() + current_pos.bottom() - past_pos.top() - past_pos.bottom()) / 2; // y方向

		dx = dx_filter.filter(dx);
		dy = dy_filter.filter(dy);

		/* カーソルの位置を更新 */
		this->track_data[user_id].cursor_point = point(ofClamp(this->track_data[user_id].cursor_point.x() + dx_rate * dx, 0, CAMERA_W), ofClamp(this->track_data[user_id].cursor_point.y() + dy_rate * dy, 0, CAMERA_H)); // 現在の追跡位置から相対的にカーソルの位置を決定
		this->transform_point(this->track_data[user_id].cursor_point, this->track_data[user_id].transformed_cursor_point);

		if (this->track_data[user_id].track_hand_dets.empty() || this->stop_flag) { // 直近フレームで手が検出されなかったら追跡をやめる
			this->track_data.erase(user_id);
			return;
		}
	}
}

void HandCursor::new_thread_tracking(correlation_tracker &ct, const int track_id) {
	void(HandCursor::*funcp)(correlation_tracker&, const int) = &HandCursor::tracking;
	thread th(funcp, this, ct, track_id);
	th.detach();
}

void HandCursor::new_thread_hand_detect() {
	void(HandCursor::*funcp)() = &HandCursor::hand_detect;
	thread th(funcp, this);
	th.detach();
}

void HandCursor::new_thread_face_detect() {
	void(HandCursor::*funcp)() = &HandCursor::face_detect;
	thread th(funcp, this);
	th.detach();
}

void HandCursor::new_thread_update() {
	void(HandCursor::*funcp)() = &HandCursor::update;
	thread th(funcp, this);
	th.detach();
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

double HandCursor::euclid_distance(const point& p1, const point& p2) const {
	return ofDist(p1.x(), p1.y(), p2.x(), p2.y());
}

void HandCursor::transform_point(const point& src_point, point& dst_point) {
	dst_point.x() = DISPLAY_W - RESOLUTION_RATIO_W*src_point.x();
	dst_point.y() = RESOLUTION_RATIO_H * src_point.y();
}

void HandCursor::inverse_transform_point(const point& src_point, point& dst_point) {
	dst_point.x() = (DISPLAY_W - src_point.x()) / RESOLUTION_RATIO_W;
	dst_point.y() = src_point.y() / RESOLUTION_RATIO_H;
}