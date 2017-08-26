#include <map>
#include <vector>

#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>

#include "HandDetector.h"
#include "NonMaximumSuppression.h"

using namespace std;
using namespace dlib;

HandDetector::HandDetector() :nms(this->overlap_ratio) {
	deserialize("") >> this->df; // ファイルから学習済みのモデルを読み込む
}

void HandDetector::operator() (array2d<unsigned char> &image, std::vector<dlib::rectangle> &sliding_windows, std::vector<dlib::rectangle> &result_rects, bool nms_flag = true) {
	array2d<unsigned char> roi;
	std::vector<dlib::rectangle> rects;

	for (auto w : sliding_windows) {
		extract_image_chip(image, w, roi);
		if (this->is_hand(roi)) {
			rects.emplace_back(w);
		}
	}

	if (nms_flag) {
		this->nms(rects, result_rects);
	}
	else {
		result_rects = rects;
	}
}

bool HandDetector::is_hand(array2d<unsigned char> &image) {
	array2d<unsigned char> img_resize(this->resize_size, this->resize_size);
	resize_image(image, img_resize);

	fhog_type fhog;
	extract_fhog_features(img_resize, fhog);

	X_type feature_vector;
	this->fhog_to_feature_vector(feature_vector, fhog);
	
	return (this->df(feature_vector) > this->decision_ratio);
}

void HandDetector::fhog_to_feature_vector(X_type &feature_vector, fhog_type &fhog) {
	for (int r = 0, j = 0; r < fhog.nr(); ++r) {
		for (int c = 0; c < fhog.nc(); ++c) {
			for (int i = 0; i < 31; ++i) {
				feature_vector.insert(make_pair(j++, fhog[r][c](i)));
			}
		}
	}
}