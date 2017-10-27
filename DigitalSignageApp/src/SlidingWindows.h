#ifndef ___Class_SlidingWindows
#define ___Class_SlidingWindows

#include <vector>
#include <utility>

#include <dlib/opencv.h>

class SlidingWindows {
private:
	int window_size, step, min_width, max_width, min_height, max_height;
	std::pair<double, double> center_point;
	std::vector<dlib::rectangle> windows;
public:
	SlidingWindows(const int &window_size, const int &step, const int &min_width, const int &max_width, const int &min_height, const int &max_height);
	const std::vector<dlib::rectangle>& get_windows();
	void center_sort();
private:
	double euclidean_distance(const double &x1, const double &y1, const double &x2, const double &y2) const;
	double center_distance(const dlib::rectangle& window) const;
};

#endif