#pragma once
#ifndef ___Class_BodyPartExtractor
#define ___Class_BodyPartExtractor

#include <opencv2/opencv.hpp>

#include <gflags/gflags.h>

#include <openpose/core/headers.hpp>
#include <openpose/filestream/headers.hpp>
#include <openpose/gui/headers.hpp>
#include <openpose/pose/headers.hpp>
#include <openpose/utilities/headers.hpp>

#include "AppParameters.h"

class BodyPartExtractor {
private:
	/* �萔 */
	static const string MODEL_POSE, MODEL_FOLDER, NET_RESOLUTION, OUTPUT_RESOLUTION;
	/* NET_RESOLUTION �� "320x160"�̂悤�ȕ�����^
	   �傫������΂���قǑ��x���ቺ���A���x���ǂ��Ȃ�
	   ���ƍ�����16�{���̂݉\�A�Ⴆ�΁A"128x96"�A"320x176"�A"432x368"�Ƃ�
	   ���ƍ����̂ǂ��炩��-1�ɂ���΁A���͉摜�T�C�Y�ɑ΂��đ��x�Ɛ��x�Ƃ��ɍő剻����炵���H
	   16�̔{���F16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,256,272,288,304,320,336,352,368,384,400,416,432,448,464,480,496,512
	*/

	static constexpr int NUM_GPU_START{ 0 };
	static constexpr int SCALE_NUMBER{ 1 };
	static constexpr double SCALE_GAP{ 0.3 };

	static const op::Point<int> output_size, net_input_size;
	static const op::PoseModel pose_model;
	static const op::Point<int> image_size;

	std::vector<double> scale_input_to_net_inputs;
	std::vector<op::Point<int>> net_input_sizes;
	double scale_input_to_output;
	op::Point<int> output_resolution;
	std::vector<op::Array<float>> net_input_array;
	op::Array<float> pose_key_points;
	op::Array<float> pose_net_output;

	op::CvMatToOpInput cv_mat_to_op_input;
	op::ScaleAndSizeExtractor scale_and_size_extractor;
	op::PoseExtractorCaffe pose_extractor_caffe;
public:
	BodyPartExtractor();
	void operator()(op::Array<float>& pose_key_points, cv::Mat& image);
};

#endif