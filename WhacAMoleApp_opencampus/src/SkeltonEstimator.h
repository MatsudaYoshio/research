#pragma once
#ifndef ___Class_SkeltonEstimator
#define ___Class_SkeltonEstimator

#include <iostream>
#include <cmath>
#include <algorithm>
#include <thread>
#include <cstdio>
#include <map>
#include <deque>

#include <ppl.h>

/* OpenCV */
#include <opencv2/opencv.hpp>

#include "UEyeVideoCapture.h"
#include "FrameRateCounter.h"
#include "AppParameters.h"
#include "BodyPartExtractor.h"

class SkeltonEstimator {
private:
	static const cv::Scalar CV_RED; // openCVの色

	std::deque<cv::Mat> image_buffer; // 画像データのバッファ
	int people_num;
	op::Array<float> raw_pose_key_points;
	bool stop_flag{ false };
	cv::Mat view_frame;
	
	UEyeVideoCapture cap;
	//FrameRateCounter frc;

	//cv::VideoWriter writer;

	BodyPartExtractor body_part_extractor;

	void get_frame();
	void show_detect_window();
	void transform_points(); // 検出された骨格点をカメラ座標からディスプレイ座標へ変換する関数
public:
	map<pair<int, int>, ofPoint> transformed_key_points; // 変換後の骨格点（ディスプレイ座標）

	SkeltonEstimator();
	void update();
	void exit();
};

#endif