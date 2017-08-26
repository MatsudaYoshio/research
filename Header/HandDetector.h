#ifndef ___Class_HandDetector
#define ___Class_HandDetector

#include <map>
#include <vector>

#include <dlib/gui_widgets.h>

#include "NonMaximumSuppression.h"

class HandDetector {
private:
	using X_type = std::map<unsigned long, double>; // �����x�N�g���̌^
	using kernel_type = dlib::sparse_linear_kernel<X_type>; // �J�[�l���̌^
	using fhog_type = dlib::array2d<dlib::matrix<double, 31, 1>>; // fhog�����ʂ̌^

	dlib::decision_function<kernel_type> df; // ���苫�E�̊֐�
	NonMaximumSuppression nms;
	static constexpr int resize_size = 80;
	static constexpr double decision_ratio = 0.5;
	static constexpr double overlap_ratio = 0.1;
public:
	HandDetector();
	void operator() (dlib::array2d<unsigned char> &image, std::vector<dlib::rectangle> &sliding_windows, std::vector<dlib::rectangle> &result_rects, bool nms_flag = true);
private:
	bool is_hand(dlib::array2d<unsigned char> &image);
	void fhog_to_feature_vector(X_type &feature_vector, fhog_type &fhog);
};

#endif