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

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "AppParameters.h"
#include "OneEuroFilter.h"
#include "BodyPartExtractor.h"

/* OpenPoseの骨格点参照のための定義 */
#define NOSE_X(i) {i,0,0}
#define NOSE_Y(i) {i,0,1}
#define NECK_X(i) {i,1,0}
#define NECK_Y(i) {i,1,1}
#define RIGHT_SHOULDER_X(i) {i,2,0}
#define RIGHT_SHOULDER_Y(i) {i,2,1}
#define RIGHT_WRIST_X(i) {i,4,0}
#define RIGHT_WRIST_Y(i) {i,4,1}
#define LEFT_SHOULDER_X(i) {i,5,0}
#define LEFT_SHOULDER_Y(i) {i,5,1}
#define MIDDLE_HIP_Y(i) {i,8,1}
#define RIGHT_EAR_X(i) {i,16,0}
#define RIGHT_EAR_Y(i) {i,16,1}
#define LEFT_EAR_X(i) {i,17,0}
#define LEFT_EAR_Y(i) {i,17,1}

class HandCursor {
public:
	const enum class STATE { INACTIVE, OVERLAP, ACTIVE };
private:
	using user_data_type = struct {
		STATE state;
		long long int latest_update_frame;
		cv::Rect2d face_rect, operation_area;
		cv::Point cursor_point, face_point, hand_point, initial_point;
		double face_size;
		int cursor_color_id;
		ofColor cursor_color;
		float alpha;
		cv::Point transformed_face_point, transformed_cursor_point;
		std::unique_ptr<OneEuroFilter> dx_filter, dy_filter;
	};

	/* 定数 */
	static constexpr int resize_size{ 80 };
	static constexpr double default_face_size{ 60 };
	static constexpr double face_error{ 150 };
	static constexpr int cursor_color_num{ 8 };
	static constexpr long long int new_user_id{ 0 };
	static const std::array<ofColor, cursor_color_num> cursor_colors;
	static const cv::Scalar CV_RED;
	static const cv::Scalar CV_BLUE;
	static const cv::Scalar CV_ORANGE;

	std::array<bool, cursor_color_num> cursor_color_state;

	bool stop_flag{ false };

	long long int frame_count{ 0 };
	long long int user_id{ 1 };

	cv::Mat view_frame;

	UEyeVideoCapture cap;
	FrameRateCounter frc;

	std::deque<cv::Mat> image_buffer; // 画像データのバッファ

	double dx, dy, dx_rate, dy_rate;

	//cv::VideoWriter writer;

	BodyPartExtractor body_part_extractor;

	double estimate_face_size(int personal_id) const;
	int decide_user_id(int personal_id) const;
	void init_user_data(int personal_id, double face_size);
	void renew_user_data(int personal_id, double face_size, long long int user_id);
	void get_frame();
	void transform_point(const cv::Point& src_point, cv::Point& dst_point) const;
	void inverse_transform_point(const cv::Point& src_point, cv::Point& dst_point) const;
	void show_detect_window();
public:
	op::Array<float> pose_key_points;
	std::unordered_map<long long int, user_data_type> user_data;

	HandCursor();
	void update();
	void exit();
	void modulate_cursor(long long int user_id);
	void overlap_window(long long int user_id);
};

#endif