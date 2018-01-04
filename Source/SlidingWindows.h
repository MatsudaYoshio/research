#ifndef ___Class_SlidingWindows
#define ___Class_SlidingWindows

#include <vector>
#include <utility>

#include <dlib/gui_widgets.h>

class SlidingWindows {
private:
	int window_size, step, min_width, max_width, min_height, max_height;
	std::pair<double, double> center_point;
	std::vector<dlib::rectangle> windows;
public:
	SlidingWindows(int window_size, int step, int min_width, int max_width, int min_height, int max_height);
	std::vector<dlib::rectangle> get_windows() const;
	void center_sort();
private:
	double euclidean_distance(double x1, double y1, double x2, double y2) const;
	double center_distance(dlib::rectangle window) const;
};

#endif