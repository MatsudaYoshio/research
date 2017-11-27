#ifndef ___Class_NonMaximumSuppression
#define ___Class_NonMaximumSuppression

#include <vector>

#include <dlib/opencv.h>

#include <opencv2/opencv.hpp>

class NonMaximumSuppression {
private:
	const double threshold;
public:
	NonMaximumSuppression(const double threshold);
	void operator() (const std::vector<dlib::rectangle> &src_rects, std::vector<dlib::rectangle> &dst_rects) const;
	void operator() (const std::vector<cv::Rect> &src_rects, std::vector<cv::Rect> &dst_rects) const;
private:
	bool overlaps_any_rect(const std::vector<dlib::rectangle> &rects, const dlib::rectangle &rect) const;
	bool overlaps_any_rect(const std::vector<cv::Rect> &rects, const cv::Rect &rect) const;
};

#endif