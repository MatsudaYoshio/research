#ifndef ___Class_HogDescriptor
#define ___Class_HogDescriptor

#include <vector>

#include <dlib/gui_widgets.h>

class HogDescriptor {
private:
	const int cell_size, block_size, orientation_size;
	int img_width, img_height, feature_size;
public:
	HogDescriptor(dlib::array2d<unsigned char> &img, int cell_size, int block_size, int orientation_size);
private:
};

#endif