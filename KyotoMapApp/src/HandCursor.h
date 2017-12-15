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
#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "AppParameters.h"
#include "RingBuffer.cpp"

class HandCursor {
	// -----------------------------------------------------------------
	typedef double TimeStamp; // in seconds

	static constexpr TimeStamp UndefinedTime = -1.0;


	double duration = 10.0; // seconds

	double frequency = 120; // Hz
	double mincutoff = 1.0; // FIXME
	double beta = 1.1;      // FIXME
	double dcutoff = 1.0;   // this one should be ok
	TimeStamp timestamp = 0.0;

	class LowPassFilter {

		double y, a, s;
		bool initialized;

		void setAlpha(double alpha) {
			if (alpha <= 0.0 || alpha>1.0)
				throw std::range_error("alpha should be in (0.0., 1.0]");
			a = alpha;
		}

	public:

		LowPassFilter(double alpha, double initval = 0.0) {
			y = s = initval;
			setAlpha(alpha);
			initialized = false;
		}

		double filter(double value) {
			double result;
			if (initialized)
				result = a*value + (1.0 - a)*s;
			else {
				result = value;
				initialized = true;
			}
			y = value;
			s = result;
			return result;
		}

		double filterWithAlpha(double value, double alpha) {
			setAlpha(alpha);
			return filter(value);
		}

		bool hasLastRawValue(void) {
			return initialized;
		}

		double lastRawValue(void) {
			return y;
		}

	};

	// -----------------------------------------------------------------

	class OneEuroFilter {

		double freq;
		double mincutoff;
		double beta_;
		double dcutoff;
		LowPassFilter *x;
		LowPassFilter *dx;
		TimeStamp lasttime;

		double alpha(double cutoff) {
			double te = 1.0 / freq;
			double tau = 1.0 / (2 * PI*cutoff);
			return 1.0 / (1.0 + tau / te);
		}

		void setFrequency(double f) {
			if (f <= 0) throw std::range_error("freq should be >0");
			freq = f;
		}

		void setMinCutoff(double mc) {
			if (mc <= 0) throw std::range_error("mincutoff should be >0");
			mincutoff = mc;
		}

		void setBeta(double b) {
			beta_ = b;
		}

		void setDerivateCutoff(double dc) {
			if (dc <= 0) throw std::range_error("dcutoff should be >0");
			dcutoff = dc;
		}

	public:

		OneEuroFilter(double freq,
			double mincutoff = 1.0, double beta_ = 0.0, double dcutoff = 1.0) {
			setFrequency(freq);
			setMinCutoff(mincutoff);
			setBeta(beta_);
			setDerivateCutoff(dcutoff);
			x = new LowPassFilter(alpha(mincutoff));
			dx = new LowPassFilter(alpha(dcutoff));
			lasttime = UndefinedTime;
		}

		double filter(double value, TimeStamp timestamp = UndefinedTime) {
			// update the sampling frequency based on timestamps
			if (lasttime != UndefinedTime && timestamp != UndefinedTime)
				freq = 1.0;
			lasttime = timestamp;
			// estimate the current variation per second 
			double dvalue = x->hasLastRawValue() ? (value - x->lastRawValue())*freq : 0.0; // FIXME: 0.0 or value?
			double edvalue = dx->filterWithAlpha(dvalue, alpha(dcutoff));
			// use it to update the cutoff frequency
			double cutoff = mincutoff + beta_*fabs(edvalue);
			// filter the given value
			return x->filterWithAlpha(value, alpha(cutoff));
		}

		~OneEuroFilter(void) {
			delete x;
			delete dx;
		}

	};

	OneEuroFilter f, f2;

private:
	using X_type = std::map<unsigned long, double>; // 特徴ベクトルの型
	using kernel_type = dlib::sparse_linear_kernel<X_type>; // カーネルの型
	using fhog_type = dlib::array2d<dlib::matrix<double, 31, 1 >>; // fhog特徴量の型
	using track_data_type = struct {
		dlib::rectangle hand, face_rect;
		dlib::point cursor_point, face_point, transformed_face_point, transformed_cursor_point;
		int cursor_color_id = -1;
		ofColor cursor_color;
		std::vector<std::pair<int, dlib::rectangle>> track_hand_dets;
	};

	static constexpr int resize_size = 80;
	static constexpr double decision_ratio = 0.6;
	static constexpr double overlap_ratio = 0.1;
	static const char* model_path;
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

	/* 画像データのバッファ */
	RingBuffer<dlib::array2d<dlib::bgr_pixel>> org_image_buffer; // dlibのbgr型画像のバッファ
	RingBuffer<dlib::array2d<unsigned char>> gs_image_buffer; // dlibのグレースケール画像のバッファ

	//cv::VideoWriter writer;
public:
	std::map<long long int, track_data_type> track_data;
	std::vector<dlib::point> detect_face_data;

public:
	HandCursor();
	void update();
	void exit();
	void modulate_cursor(const int& user_id);
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
	double euclid_distance(const dlib::point& p1, const dlib::point& p2) const;
	void transform_point(const dlib::point& src_point, dlib::point& dst_point);
	void inverse_transform_point(const dlib::point& src_point, dlib::point& dst_point);
};

#endif