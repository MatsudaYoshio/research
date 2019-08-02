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
	static const cv::Scalar CV_RED; // openCV�̐F

	std::deque<cv::Mat> image_buffer; // �摜�f�[�^�̃o�b�t�@
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
	void transform_points(); // ���o���ꂽ���i�_���J�������W����f�B�X�v���C���W�֕ϊ�����֐�
public:
	map<pair<int, int>, ofPoint> transformed_key_points; // �ϊ���̍��i�_�i�f�B�X�v���C���W�j

	SkeltonEstimator();
	void update();
	void exit();
};

#endif