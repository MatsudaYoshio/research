#ifndef ___Class_HogDescriptor
#define ___Class_HogDescriptor

#include <vector>

#include <dlib/gui_widgets.h>

class HogDescriptor {
private:
	std::vector<double> feature_vector;
public:
	HogDescriptor(const dlib::array2d<unsigned char> &img, const int cell, const int block, const int orientation);
	std::vector<double> get_feature_vector() const;
};

#endif