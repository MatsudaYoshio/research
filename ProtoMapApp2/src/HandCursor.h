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
#include "ColorGenerator.h"
#include "TimerBase.h"

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
#define MIDDLE_HIP_X(i) {i,8,0}
#define MIDDLE_HIP_Y(i) {i,8,1}
#define RIGHT_KNEE_X(i) {i,10,0}
#define RIGHT_KNEE_Y(i) {i,10,1}
#define RIGHT_ANKLE_X(i) {i,11,0}
#define RIGHT_ANKLE_Y(i) {i,11,1}
#define LEFT_KNEE_X(i) {i,13,0}
#define LEFT_KNEE_Y(i) {i,13,1}
#define LEFT_ANKLE_X(i) {i,14,0}
#define LEFT_ANKLE_Y(i) {i,14,1}
#define RIGHT_EYE_X(i) {i,15,0}
#define RIGHT_EYE_Y(i) {i,15,1}
#define LEFT_EYE_X(i) {i,16,0}
#define LEFT_EYE_Y(i) {i,16,1}
#define RIGHT_EAR_X(i) {i,17,0}
#define RIGHT_EAR_Y(i) {i,17,1}
#define LEFT_EAR_X(i) {i,18,0}
#define LEFT_EAR_Y(i) {i,18,1}

class HandCursor {
public:
	const enum class STATE { INACTIVE, OVERLAP, ACTIVE };
	const enum class USING_HAND { RIGHT, LEFT };
private:
	using user_data_type = struct {
		STATE state;
		USING_HAND hand;
		bool cursor_appear_flag;
		long long int personal_id;
		long long int start_frame, latest_update_frame;
		cv::Rect2d face_rect, operation_area;
		cv::Point face_point, initial_point;
		double face_size;
		int cursor_color_id;
		ofColor cursor_color;
		float cursor_alpha;
		int face_blink_count;
		cv::Point transformed_face_point, cursor_point;
		std::unique_ptr<OneEuroFilter> dx_filter, dy_filter;
	};

	/* 定数 */
	static constexpr int resize_size{ 80 };
	static constexpr double default_face_size{ 60 };
	static constexpr double minimum_face_size{ default_face_size*0.8 };
	static constexpr double face_error{ 100 };
	static constexpr int initial_cursor_color_num{ 8 };
	static constexpr long long int new_user_id{ 0 };
	static constexpr double operation_width_rate{ 1000 };
	static constexpr double operation_heght_rate{ 1000 };
	static constexpr double moving_rate{ 1.45 };
	static constexpr double filter_freq{ 120 };
	// mincutoff: 0.00129    beta: 0.00129
	// mincutoff: 0.00000065 beta: 0.0038
	// mincutoff: 0.000001 beta: 0.004
	static constexpr double filter_mincutoff{ 0.00000065 };
	static constexpr double filter_beta{ 0.004 };
	static const double display_operation_width_ratio;
	static const double display_operation_height_ratio;
	static const cv::Point invalid_point, display_center_point;
	static const std::array<ofColor, initial_cursor_color_num> initial_cursor_colors;

	vector<ofColor> cursor_colors;
	vector<bool> cursor_color_state;

	bool stop_flag{ false };

	long long int frame_count{ 0 };
	long long int user_id{ 1 };

	cv::Mat view_frame;

	UEyeVideoCapture cap;
	FrameRateCounter frc;

	std::deque<cv::Mat> image_buffer; // 画像データのバッファ

	double dx, dy;

	//cv::VideoWriter writer;

	BodyPartExtractor body_part_extractor;

	ColorGenerator cg;

	//TimerBase tm;

	int decide_user_id(int personal_id) const;
	bool is_start_interaction_by_right_hand(int personal_id) const;
	bool is_start_interaction_by_left_hand(int personal_id) const;
	void init_user_data(int personal_id, double face_size, USING_HAND hand);
	void renew_user_data(int personal_id, double face_size, long long int user_id);
	void deactivate_user(long long int user_id);
	void get_frame();
	void show_detect_window();
public:
	op::Array<float> pose_key_points;
	std::unordered_map<long long int, user_data_type> user_data;
	vector<long long int> personal_id2user_id;

	HandCursor();
	void update();
	void exit();
	void overlap_window(long long int user_id);
	double estimate_face_size(int personal_id) const;
	void transform_point(const cv::Point& src_point, cv::Point& dst_point) const;
};

#endif