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

class SkeltonEstimator {
private:
	/* openCVの色 */
	static const cv::Scalar CV_RED;
	static const cv::Scalar CV_BLUE;
	static const cv::Scalar CV_GREEN;
	static const cv::Scalar CV_ORANGE;
	static const cv::Scalar CV_PURPLE;

	std::deque<cv::Mat> image_buffer; // 画像データのバッファ
	int people_num;
	op::Array<float> raw_pose_key_points;
	bool stop_flag{ false };
	cv::Mat view_frame;
	
	UEyeVideoCapture cap;
	FrameRateCounter frc;

	cv::VideoWriter writer;

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