// 手カーソルの検出、情報更新のクラス

#pragma once
#ifndef ___Class_HandCursor
#define ___Class_HandCursor

#include <iostream>
#include <cmath>
#include <algorithm>
#include <thread>
#include <cstdio>
#include <map>
#include <deque>
#include <random>

#include <ppl.h>

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "AppParameters.h"
#include "OneEuroFilter.h"
#include "BodyPartExtractor.h"

/* OpenPoseの骨格点参照のための定義(COCOモデル) */
#define NOSE_X(i) {i,0,0}
#define NOSE_Y(i) {i,0,1}
#define NECK_X(i) {i,1,0}
#define NECK_Y(i) {i,1,1}
#define RIGHT_SHOULDER_X(i) {i,2,0}
#define RIGHT_SHOULDER_Y(i) {i,2,1}
#define RIGHT_ELBOW_X(i) {i,3,0}
#define RIGHT_ELBOW_Y(i) {i,3,1}
#define RIGHT_WRIST_X(i) {i,4,0}
#define RIGHT_WRIST_Y(i) {i,4,1}
#define LEFT_SHOULDER_X(i) {i,5,0}
#define LEFT_SHOULDER_Y(i) {i,5,1}
#define LEFT_ELBOW_X(i) {i,6,0}
#define LEFT_ELBOW_Y(i) {i,6,1}
#define LEFT_WRIST_X(i) {i,7,0}
#define LEFT_WRIST_Y(i) {i,7,1}
#define RIGHT_HIP_X(i) {i,8,0}
#define RIGHT_HIP_Y(i) {i,8,1}
#define RIGHT_KNEE_X(i) {i,9,0}
#define RIGHT_KNEE_Y(i) {i,9,1}
#define RIGHT_ANKLE_X(i) {i,10,0}
#define RIGHT_ANKLE_Y(i) {i,10,1}
#define LEFT_HIP_X(i) {i,11,0}
#define LEFT_HIP_Y(i) {i,11,1}
#define LEFT_KNEE_X(i) {i,12,0}
#define LEFT_KNEE_Y(i) {i,12,1}
#define LEFT_ANKLE_X(i) {i,13,0}
#define LEFT_ANKLE_Y(i) {i,13,1}
#define RIGHT_EYE_X(i) {i,14,0}
#define RIGHT_EYE_Y(i) {i,14,1}
#define LEFT_EYE_X(i) {i,15,0}
#define LEFT_EYE_Y(i) {i,15,1}
#define RIGHT_EAR_X(i) {i,16,0}
#define RIGHT_EAR_Y(i) {i,16,1}
#define LEFT_EAR_X(i) {i,17,0}
#define LEFT_EAR_Y(i) {i,17,1}
#define BACKGROUND_X(i) {i,18,0}
#define BACKGROUND_Y(i) {i,18,1}

class HandCursor {
public:
	const enum class STATE { INACTIVE, ACTIVE };
private:
	/* ユーザ情報の型 */
	using user_data_type = struct {
		STATE state;
		long long int start_frame, latest_update_frame;
		cv::Rect2d face_rect, operation_area;
		cv::Point face_point, initial_point;
		double face_size;
		int cursor_color_id;
		ofColor cursor_color;
		int cursor_image_id;
		ofImage cursor_image;
		cv::Point transformed_face_point, cursor_point;
		std::unique_ptr<OneEuroFilter> dx_filter, dy_filter;
	};

	/* 定数 */
	static constexpr int resize_size{ 80 };
	static constexpr double default_face_size{ 60 };
	static constexpr double face_error{ 150 };
	static constexpr int cursor_color_num{ 10 };
	static constexpr int cursor_image_num{ 3 };
	static constexpr long long int new_user_id{ 0 };
	static constexpr double operation_width_rate{ 1000 };
	static constexpr double operation_heght_rate{ 1000 };
	static constexpr double moving_rate{ 1.4 };
	static constexpr double filter_freq{ 120 };
	// mincutoff: 0.00129    beta: 0.00129
	// mincutoff: 0.00000065 beta: 0.0038
	static constexpr double filter_mincutoff{ 0.00000065 };
	static constexpr double filter_beta{ 0.0038 };
	static const double display_operation_width_ratio;
	static const double display_operation_height_ratio;
	static const cv::Point invalid_point, display_center_point;
	static const std::array<ofColor, cursor_color_num> cursor_colors;
	static const cv::Scalar CV_RED;
	static const cv::Scalar CV_BLUE;
	static const cv::Scalar CV_GREEN;
	static const cv::Scalar CV_ORANGE;
	static const cv::Scalar CV_PURPLE;

	std::array<ofImage, cursor_image_num> cursor_images;

	std::array<bool, cursor_color_num> cursor_color_state;
	std::array<bool, cursor_image_num> cursor_image_state;

	bool stop_flag{ false };

	long long int frame_count{ 0 };
	long long int user_id{ 1 };

	cv::Mat view_frame;

	UEyeVideoCapture cap;
	FrameRateCounter frc;

	std::deque<cv::Mat> image_buffer; // 画像データのバッファ

	double dx, dy;

	cv::VideoWriter writer;

	BodyPartExtractor body_part_extractor;

	double estimate_face_size(int personal_id) const;
	int decide_user_id(int personal_id) const;
	void init_user_data(int personal_id, double face_size);
	void renew_user_data(int personal_id, double face_size, long long int user_id);
	void get_frame();
	void transform_point(const cv::Point& src_point, cv::Point& dst_point) const;
	void show_detect_window();
public:
	op::Array<float> pose_key_points;
	std::unordered_map<long long int, user_data_type> user_data;

	HandCursor();
	void update();
	void exit();
};

#endif