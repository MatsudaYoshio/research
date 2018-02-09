#ifndef ___Class_SlidingWindows
#define ___Class_SlidingWindows

#include <vector>

#include <dlib/opencv.h>

class SlidingWindows {
public:
	void operator() (std::vector<dlib::rectangle>& windows, int window_size, int step, int min_width, int max_width, int min_height, int max_height) const;
	const std::vector<dlib::rectangle> operator() (int window_size, int step, int min_width, int max_width, int min_height, int max_height) const;
};

#endif