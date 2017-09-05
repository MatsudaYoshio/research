#ifndef ___Class_HandPointer
#define ___Class_HandPointer

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

class HandPointer {

private:
	using X_type = std::map<unsigned long, double>; // “Á’¥ƒxƒNƒgƒ‹‚ÌŒ^
	using kernel_type = dlib::sparse_linear_kernel<X_type>; // ƒJ[ƒlƒ‹‚ÌŒ^
	using fhog_type = dlib::array2d<dlib::matrix<double, 31, 1 >> ; // fhog“Á’¥—Ê‚ÌŒ^
	using track_data_type = struct {
		dlib::rectangle hand, face;
		cv::Point past_pointer, current_pointer;
		int pointer_color_id;
		ofColor pointer_color;
		dlib::drectangle current_pos;
		std::vector<std::pair<int, dlib::rectangle>> track_hand_dets;
	};

	static constexpr int resize_size = 80;
	static constexpr int window_width = 1920;
	static constexpr int window_height = 1080;
	static constexpr double decision_ratio = 0.5;
	static constexpr double overlap_ratio = 0.1;

	std::vector<dlib::rectangle> face_dets, hand_dets;
	dlib::frontal_face_detector face_detector; // ³–ÊŠçŒŸoŠí
	dlib::decision_function<kernel_type> df; // Œˆ’è‹«ŠE‚ÌŠÖ”
	dlib::array2d<unsigned char> image_gs;
	dlib::array2d<dlib::bgr_pixel> image_org;
	bool face_thread_flag;
	bool hand_thread_flag;
	long long int frame_count;
	long long int track_id;
	cv::Mat frame, view_frame;
	NonMaximumSuppression nms;
	UEyeVideoCapture cap;
	FrameRateCounter frc;
	std::vector<ofColor> pointer_color_list;
	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> rn_color;
public:
	std::map<long long int, track_data_type> track_data;

public:
	HandPointer();
	void loop();
private:
	void show_detect_window();
	void face_detect();
	void hand_detect();
	void hand_detect(const std::vector<dlib::rectangle> &sliding_windows, const int track_id);
	bool is_hand(dlib::array2d<unsigned char> &img);
	void tracking(dlib::correlation_tracker &ct, int track_id);
	void new_thread_tracking(dlib::correlation_tracker &ct, int n);
	void new_thread_hand_detect();
	void new_thread_face_detect();
	void fhog_to_feature_vector(X_type &feature_vector, const fhog_type &fhog);
	double euclid_distance(const double x1, const double y1, const double x2, const double y2) const;
};

#endif