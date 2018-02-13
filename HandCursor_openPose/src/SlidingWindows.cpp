#include "SlidingWindows.h"

using namespace std;
using namespace dlib;

void SlidingWindows::operator()(std::vector<rectangle>& windows, const int window_size, const int step, const int min_width, const int max_width, const int min_height, const int max_height) const {
	const int max_i{ max_height - window_size };
	const int max_j{ max_width - window_size };
	if (step == 0) {
		cout << "step is 0!!!" << endl;
	}
	windows.resize(((max_i - min_height) / step + 1)*((max_j - min_width) / step + 1));

	for (int i = min_height, x = 0; i <= max_i; i += step) {
		for (int j = min_width; j <= max_j; j += step) {
			windows[x++] = dlib::rectangle(j, i, j + window_size, i + window_size);
		}
	}
}

const std::vector<rectangle> SlidingWindows::operator()(const int window_size, const int step, const int min_width, const int max_width, const int min_height, const int max_height) const {
	std::vector<rectangle> windows;
	const int max_i{ max_height - window_size };
	const int max_j{ max_width - window_size };
	if (step == 0) {
		cout << "step is 0!!!!!!!!!" << endl;
	}
	windows.resize(((max_i - min_height) / step + 1)*((max_j - min_width) / step + 1));

	for (int i = min_height, x = 0; i <= max_i; i += step) {
		for (int j = min_width; j <= max_j; j += step) {
			windows[x++] = dlib::rectangle(j, i, j + window_size, i + window_size);
		}
	}

	return windows;
}