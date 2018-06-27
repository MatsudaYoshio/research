#include "BodyPartExtractor.h"

using namespace std;
using namespace op;
using namespace cv;
using namespace param;

const string BodyPartExtractor::MODEL_POSE{ "COCO" };
const string BodyPartExtractor::MODEL_FOLDER{ "C:/openpose-1.2.1/models/" };
const string BodyPartExtractor::NET_RESOLUTION{ "-1x224" };
const string BodyPartExtractor::OUTPUT_RESOLUTION{ "-1x-1" };

const op::Point<int> BodyPartExtractor::output_size{ flagsToPoint(BodyPartExtractor::OUTPUT_RESOLUTION, BodyPartExtractor::OUTPUT_RESOLUTION) };
const op::Point<int> BodyPartExtractor::net_input_size{ flagsToPoint(BodyPartExtractor::NET_RESOLUTION, BodyPartExtractor::NET_RESOLUTION) };
const op::Point<int> BodyPartExtractor::image_size{ CAMERA_W, CAMERA_H };
const PoseModel BodyPartExtractor::pose_model{ flagsToPoseModel(BodyPartExtractor::MODEL_POSE) };

BodyPartExtractor::BodyPartExtractor() :scale_and_size_extractor(this->net_input_size, this->output_size, this->SCALE_NUMBER, this->SCALE_GAP), pose_extractor_caffe{ this->pose_model, this->MODEL_FOLDER, this->NUM_GPU_START } {
	this->pose_extractor_caffe.initializationOnThread();

	tie(this->scale_input_to_net_inputs, this->net_input_sizes, this->scale_input_to_output, this->output_resolution) = this->scale_and_size_extractor.extract(this->image_size);
}

void BodyPartExtractor::operator()(Array<float>& pose_key_points, Mat& image) {
	this->net_input_array = cv_mat_to_op_input.createArray(image, this->scale_input_to_net_inputs, this->net_input_sizes);

	this->pose_extractor_caffe.forwardPass(this->net_input_array, this->image_size, this->scale_input_to_net_inputs);

	pose_key_points = this->pose_extractor_caffe.getPoseKeypoints();
}