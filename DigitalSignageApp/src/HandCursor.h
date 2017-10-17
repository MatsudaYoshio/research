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
#include <dlib/image_processing/frontal_face_detector.h>

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "SlidingWindows.h"
#include "NonMaximumSuppression.h"
#include "ColorParam.h"
#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "ofColor.h"

class HandCursor {

private:
	using X_type = std::map<unsigned long, double>; // �����x�N�g���̌^
	using kernel_type = dlib::sparse_linear_kernel<X_type>; // �J�[�l���̌^
	using fhog_type = dlib::array2d<dlib::matrix<double, 31, 1 >>; // fhog�����ʂ̌^
	using track_data_type = struct {
		dlib::rectangle hand, face;
		cv::Point past_pointer, current_pointer;
		int cursor_color_id;
		ofColor cursor_color;
		dlib::drectangle current_pos;
		std::vector<std::pair<int, dlib::rectangle>> track_hand_dets;
	};

	static constexpr int resize_size = 80;
	static constexpr double decision_ratio = 0.5;
	static constexpr double overlap_ratio = 0.1;

	std::vector<dlib::rectangle> face_dets, hand_dets;
	dlib::frontal_face_detector face_detector; // ���ʊ猟�o��
	dlib::decision_function<kernel_type> df; // ���苫�E�̊֐�
	dlib::array2d<unsigned char> image_gs;
	dlib::array2d<dlib::bgr_pixel> image_org;
	bool face_thread_flag;
	bool hand_thread_flag;

	/* frame_count��track_id�̓C���N�������g�ɂ���ăI�[�o�t���[����\�������� */
	long long int frame_count;
	long long int track_id; // track_data�����ʂ���id�ŁA���user_id
	
	cv::Mat frame, view_frame;
	NonMaximumSuppression nms;
	UEyeVideoCapture cap;
	FrameRateCounter frc;
	std::vector<ofColor> cursor_color_list;
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> rn_color;
	bool stop_flag;
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