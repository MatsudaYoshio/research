#include <vector>

#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>

#include <opencv2/opencv.hpp>

#include "NonMaximumSuppression.h"

using namespace std;
using namespace cv;
using namespace dlib;

NonMaximumSuppression::NonMaximumSuppression(const double threshold) :threshold(threshold) {}

void NonMaximumSuppression::operator() (const std::vector<dlib::rectangle> &src_rects, std::vector<dlib::rectangle> &dst_rects) const {
	for (auto r : src_rects) {
		if (this->overlaps_any_rect(dst_rects, r)) {
			continue;
		}
		dst_rects.emplace_back(r);
	}
}

void NonMaximumSuppression::operator() (const std::vector<Rect> &src_rects, std::vector<Rect> &dst_rects) const {
	for (auto r : src_rects) {
		if (this->overlaps_any_rect(dst_rects, r)) {
			continue;
		}
		dst_rects.emplace_back(r);
	}
}

bool NonMaximumSuppression::overlaps_any_rect(const std::vector<dlib::rectangle> &rects, const dlib::rectangle &rect) const {
	test_box_overlap tester(this->threshold);
	for (auto r : rects) {
		if (r != rect && tester(r, rect)) {
			return true;
		}
	}
	return false;
}

bool NonMaximumSuppression::overlaps_any_rect(const std::vector<Rect> &rects, const Rect &rect) const {
	SimilarRects sr(this->threshold);
	for (auto r : rects) {
		if (r != rect && sr(r, rect)) {
			return true;
		}
	}
	return false;
}