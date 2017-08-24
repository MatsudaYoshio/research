#ifndef ___Class_ApplicationItem
#define ___Class_ApplicationItem

#include <opencv2/opencv.hpp>

class ApplicationItem {
public:
	cv::Rect rect;
	cv::Scalar color;

	ApplicationItem(cv::Rect &rect, cv::Scalar color);
};

#endif
