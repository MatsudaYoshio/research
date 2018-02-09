#pragma once
#ifndef ___Class_HandCursor
#define ___Class_HandCursor

#include <iostream>
#include <cmath>
#include <algorithm>
#include <thread>
#include <cstdio>
#include <map>
#include <random>

/* dlib */
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/opencv.h>
#include <dlib/svm.h>
#include <dlib/dir_nav.h>
#include <dlib/svm_threaded.h>
#include <dlib/image_processing.h>

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "SlidingWindows.h"
#include "NonMaximumSuppression.h"
#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "AppParameters.h"
#include "RingBuffer.cpp"
#include "OneEuroFilter.h"
#include "BodyPartExtractor.h"

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
#define RIGHT_EAR_X(i) {i,16,0}
#define RIGHT_EAR_Y(i) {i,16,1}
#define LEFT_EAR_X(i) {i,17,0}
#define LEFT_EAR_Y(i) {i,17,1}

class HandCursor {
private:
	using X_type = std::map<unsigned long, double>; // 特徴ベクトルの型
	using kernel_type = dlib::sparse_linear_kernel<X_type>; // カーネルの型
	using fhog_type = dlib::array2d<dlib::matrix<double, 31, 1 >>; // fhog特徴量の型
	using user_data_type = struct {
		dlib::rectangle hand;
		dlib::point cursor_point, face_point;
		int face_size;
		int cursor_color_id;
		ofColor cursor_color;
		dlib::point transformed_face_point, transformed_cursor_point;
		std::vector<std::pair<int, dlib::rectangle>> track_hand_dets;
	};
	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_color;

	/* 定数 */
	static constexpr int resize_size{ 80 };
	static constexpr double decision_ratio{ 0.6 };
	static constexpr double overlap_ratio{ 0.1 };
	static constexpr double default_face_size{ 60 };
	static constexpr double face_hand_distance{ 300 };
	static constexpr double face_error{ 100 };
	static constexpr double windows_range_rate_top = { 2.0 }, windows_range_rate_bottom{ 0.5 }, windows_range_rate_left{ 1.5 }, windows_range_rate_right{ 1.5 };
	static const char* model_path;
	static const ofColor cursor_color_list[];
	static const cv::Scalar CV_RED;
	static const cv::Scalar CV_BLUE;
	static const cv::Scalar CV_ORANGE;

	dlib::decision_function<kernel_type> df; // 決定境界の関数
	std::vector<dlib::rectangle> hand_dets_tmp, hand_dets;
	dlib::array2d<unsigned char> roi, roi_tracking;
	std::vector<std::vector<dlib::rectangle>> sliding_windows;

	bool stop_flag{ false };

	long long int frame_count{ 0 };
	long long int user_id{ 1 };

	cv::Mat view_frame;

	SlidingWindows sw;
	NonMaximumSuppression nms{ overlap_ratio };
	UEyeVideoCapture cap;
	FrameRateCounter frc;

	/* 画像データのバッファ */
	RingBuffer<cv::Mat> mat_org_image_buffer; // Mat型画像のバッファ
	RingBuffer<dlib::array2d<dlib::bgr_pixel>> org_image_buffer; // dlibのbgr型画像のバッファ
	RingBuffer<dlib::array2d<unsigned char>> gs_image_buffer; // dlibのグレースケール画像のバッファ

	//cv::VideoWriter writer;

	BodyPartExtractor body_part_extractor;

	double estimate_face_size(int personal_id);
	int decide_user_id(int personal_id);
	bool hand_detect(int personal_id, int face_size);
	void hand_detect(const std::vector<dlib::rectangle>& sliding_windows, int user_id);
	void start_track(int personal_id, int face_size);
	void renew_user_data(int personal_id, int face_size, int user_id);
	void get_frame();
	void tracking(dlib::correlation_tracker& ct, const int user_id);
	bool is_hand(dlib::array2d<unsigned char>& img);
	void fhog_to_feature_vector(X_type& feature_vector, const fhog_type& fhog);
	void transform_point(const dlib::point& src_point, dlib::point& dst_point);
	void inverse_transform_point(const dlib::point& src_point, dlib::point& dst_point);
	void show_detect_window();
public:
	op::Array<float> pose_key_points;
	std::unordered_map<long long int, user_data_type> user_data;

	HandCursor();
	void update();
	void exit();
	void modulate_cursor(const int& user_id);
};

#endif