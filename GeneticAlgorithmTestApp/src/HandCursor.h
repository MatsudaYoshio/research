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
#include <mutex>

/* dlib */
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/opencv.h>
#include <dlib/svm.h>
#include <dlib/dir_nav.h>
#include <dlib/svm_threaded.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "SlidingWindows.h"
#include "NonMaximumSuppression.h"
#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "ofColor.h"
#include "AppParameters.h"
#include "RingBuffer.cpp"

class HandCursor {

private:
	using X_type = std::map<unsigned long, double>; // 特徴ベクトルの型
	using kernel_type = dlib::sparse_linear_kernel<X_type>; // カーネルの型
	using fhog_type = dlib::array2d<dlib::matrix<double, 31, 1 >>; // fhog特徴量の型
	using track_data_type = struct {
		dlib::rectangle hand, face;
		cv::Point past_pointer, current_pointer;
		int cursor_color_id = -1;
		ofColor cursor_color;
		dlib::drectangle current_pos;
		std::vector<std::pair<int, dlib::rectangle>> track_hand_dets;
	};

	static constexpr int resize_size = 80;
	static constexpr double decision_ratio = 0.6;
	static constexpr double overlap_ratio = 0.1;
	static constexpr int image_buffer_max_size = 10;
	static const string model_path;
	static const ofColor cursor_color_list[];
	static const cv::Scalar CV_RED;
	static const cv::Scalar CV_BLUE;
	static const cv::Scalar CV_ORANGE;

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_color;

	std::vector<dlib::rectangle> face_dets, hand_dets;
	dlib::frontal_face_detector face_detector; // 正面顔検出器
	dlib::decision_function<kernel_type> df; // 決定境界の関数
	
	bool face_thread_flag;
	bool hand_thread_flag;
	bool stop_flag;

	/* frame_countとtrack_idはインクリメントによってオーバフローする可能性がある */
	long long int frame_count;
	long long int track_id; // track_dataを識別するidで、後のuser_id

	cv::Mat frame, view_frame;
	NonMaximumSuppression nms;
	UEyeVideoCapture cap;
	FrameRateCounter frc;
	
	std::mutex mtx;

	RingBuffer<dlib::array2d<dlib::bgr_pixel>> org_image_buffer;
	RingBuffer<dlib::array2d<unsigned char>> gs_image_buffer;
public:
	std::map<long long int, track_data_type> track_data;

public:
	HandCursor();
	void update();
	void exit();
private:
	void show_detect_window();
	void face_detect();
	void hand_detect();
	void hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int &user_id);
	bool is_hand(dlib::array2d<unsigned char> &img);
	void tracking(dlib::correlation_tracker &ct, const int user_id);
	void new_thread_tracking(dlib::correlation_tracker &ct, const int track_id);
	void new_thread_hand_detect();
	void new_thread_face_detect();
	void fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog);
	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const;
};

#endif