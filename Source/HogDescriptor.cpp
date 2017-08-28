#include <vector>
#include <numeric>
#define _USE_MATH_DEFINES
#include <math.h>

#include <dlib/gui_widgets.h>

#include "HogDescriptor.h"

using namespace std;
using namespace dlib;

HogDescriptor::HogDescriptor(const array2d<unsigned char> &img, const int cell_size, const int block_size, const int orientation_size) {
	const int img_width = img.nc();
	const int img_height = img.nr();
	const int histogram_rows = img_height / cell_size;
	const int histogram_cols = img_width / cell_size;
	const int block_rows = histogram_rows - block_size + 1;
	const int block_cols = histogram_cols - block_size + 1;
	const int feature_size = block_rows*block_cols*pow(block_size, 2)*orientation_size;
	const double angle = 180.0 / orientation_size;

	std::vector<std::vector<std::vector<double>>> histogram(histogram_rows, std::vector<std::vector<double>>(histogram_cols, std::vector<double>(orientation_size, 0)));
	int dx, dy, p, q;
	double mag, grad;
	for (int y = 0; y < img_height; ++y) {
		for (int x = 0; x < img_width; ++x) {
			if (x == 0) {
				p = x;
				q = x + 1;
			}
			else if (x == img_width - 1) {
				p = x - 1;
				q = x;
			}
			else {
				p = x - 1;
				q = x + 1;
			}
			dx = img[y][p] - img[y][q];

			if (y == 0) {
				p = y;
				q = y + 1;
			}
			else if (y == img_width - 1) {
				p = y - 1;
				q = y;
			}
			else {
				p = y - 1;
				q = y + 1;
			}
			dy = img[p][x] - img[q][x];
			
			mag = sqrt(pow(dx, 2) + pow(dy, 2));
			grad = atan2(dy, dx)*180.0 / M_PI;
			if (grad < 0.0) {
				grad += 360.0;
			}
			if (grad > 180.0) {
				grad -= 180.0;
			}
			grad /= angle;
			histogram[y/cell_size][x/cell_size][(int)grad%orientation_size] += mag;
		}
	}

	std::vector<std::vector<double>> histogram_sum(histogram_rows, std::vector<double>(histogram_cols));
	for (int i = 0; i < histogram_rows; ++i) {
		for (int j = 0; j < histogram_cols; ++j) {
			histogram_sum[i][j] = accumulate(begin(histogram[i][j]), end(histogram[i][j]), 0.0);
		}
	}

	std::vector<std::vector<double>> block_sum(block_rows, std::vector<double>(block_cols, 0));
	for (int i = 0; i < block_size; ++i) {
		for (int j = 0; j < block_size; ++j) {
			block_sum[0][0] += pow(histogram_sum[i][j], 2);
		}
	}
	for (int i = 1; i < block_cols; ++i) {
		block_sum[0][i] = block_sum[0][i - 1];
		for (int j = 0; j < block_size; ++j) {
			block_sum[0][i] += pow(histogram_sum[j][i + block_size - 1] - histogram_sum[j][i - 1], 2);
		}
	}
	for (int i = 1; i < block_rows; ++i) {
		for (int j = 0; j < block_cols; ++j) {
			block_sum[i][j] = block_sum[i - 1][j];
			for (int k = 0; k < block_size; ++k) {
				block_sum[i][j] += pow(histogram_sum[i - 1][k] - histogram_sum[i + block_size - 1][k], 2);
			}
		}
	}

	std::vector<std::vector<double>> normalize_num(block_rows, std::vector<double>(block_cols));
	for (int i = 0; i < block_rows; ++i) {
		for (int j = 0; j < block_cols; ++j) {
			normalize_num[i][j] = sqrt(block_sum[i][j] + 1);
		}
	}

	this->feature_vector.resize(feature_size);
	for (int i = 0, j = 0; j < block_rows; ++j) {
		for (int k = 0; k < block_cols; ++k) {
			for (int l = 0; l < block_size; ++l) {
				for (int m = 0; m < block_size; ++m) {
					for (int n = 0; n < orientation_size; ++n) {
						this->feature_vector[i++] = histogram[j + l][k + m][n] / normalize_num[j][k];
					}
				}
			}
		}
	}
}

std::vector<double> HogDescriptor::get_feature_vector() const {
	return this->feature_vector;
}