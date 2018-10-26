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

// Result for BODY_25 (25 body parts consisting of COCO + foot)
// const std::map<unsigned int, std::string> POSE_BODY_25_BODY_PARTS {
//     {0,  "Nose"},
//     {1,  "Neck"},
//     {2,  "RShoulder"},
//     {3,  "RElbow"},
//     {4,  "RWrist"},
//     {5,  "LShoulder"},
//     {6,  "LElbow"},
//     {7,  "LWrist"},
//     {8,  "MidHip"},
//     {9,  "RHip"},
//     {10, "RKnee"},
//     {11, "RAnkle"},
//     {12, "LHip"},
//     {13, "LKnee"},
//     {14, "LAnkle"},
//     {15, "REye"},
//     {16, "LEye"},
//     {17, "REar"},
//     {18, "LEar"},
//     {19, "LBigToe"},
//     {20, "LSmallToe"},
//     {21, "LHeel"},
//     {22, "RBigToe"},
//     {23, "RSmallToe"},
//     {24, "RHeel"},
//     {25, "Background"}
// };

/* OpenPoseの骨格点参照のための定義（上記を参考） */
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
#define RIGHT_EAR_X(i) {i,17,0}
#define RIGHT_EAR_Y(i) {i,17,1}
#define LEFT_EAR_X(i) {i,18,0}
#define LEFT_EAR_Y(i) {i,18,1}

class HandCursor {
public:
	const enum class STATE { INACTIVE, OVERLAP, ACTIVE };
private:
	using user_data_type = struct {
		STATE state;
		long long int start_frame, latest_update_frame;
		cv::Rect2d face_rect, operation_area;
		cv::Point face_point, initial_point;
		double face_size;
		int cursor_color_id;
		ofColor cursor_color;
		float alpha;
		cv::Point transformed_face_point, cursor_point;
		std::unique_ptr<OneEuroFilter> dx_filter, dy_filter;
	};

	/* 定数 */
	static constexpr int resize_size{ 80 };
	static constexpr double default_face_size{ 60 };
	static constexpr double face_error{ 150 };
	static constexpr int cursor_color_num{ 10 };
	static constexpr long long int new_user_id{ 0 };
	static constexpr double operation_width_rate{ 1000 };
	static constexpr double operation_heght_rate{ 1000 };
	static constexpr double moving_rate{ 1.45 };
	static constexpr double filter_freq{ 120 };
	// mincutoff: 0.00129    beta: 0.00129
	// mincutoff: 0.00000065 beta: 0.0038
	// mincutoff: 0.000001 beta: 0.004
	static constexpr double filter_mincutoff{ 0.000001 };
	static constexpr double filter_beta{ 0.004 };
	static const double display_operation_width_ratio;
	static const double display_operation_height_ratio;
	static const cv::Point invalid_point, display_center_point;
	static const std::array<ofColor, cursor_color_num> cursor_colors;
	static const cv::Scalar CV_RED;
	static const cv::Scalar CV_BLUE;
	static const cv::Scalar CV_GREEN;
	static const cv::Scalar CV_ORANGE;
	static const cv::Scalar CV_PURPLE;

	std::array<bool, cursor_color_num> cursor_color_state;

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
	void overlap_window(long long int user_id);
};

#endif