#include <vector>
#include <numeric>
#define _USE_MATH_DEFINES
#include <math.h>

#include <dlib/gui_widgets.h>

#include "HogDescriptor.h"

using namespace std;
using namespace dlib;

HogDescriptor::HogDescriptor(array2d<unsigned char> &img, int cell_size, int block_size, int orientation_size) :cell_size(cell_size), block_size(block_size), orientation_size(orientation_size) {
	this->img_width = img.nc();
	this->img_height = img.nr();
	const int histogram_rows = this->img_height / this->cell_size;
	const int histogram_cols = this->img_width / this->cell_size;
	const int block_rows = histogram_rows - this->block_size + 1;
	const int block_cols = histogram_cols - this->block_size + 1;
	const double angle = 180.0 / this->orientation_size;
	this->feature_size = block_rows*block_cols*pow(this->block_size, 2)*this->orientation_size;

	std::vector<std::vector<std::vector<double>>> histogram = std::vector<std::vector<std::vector<double>>>(histogram_rows,std::vector<std::vector<double>>(histogram_cols,std::vector<double>(this->orientation_size, 0)));
	int dx, dy, n, m;
	double mag, grad;
	for (int y = 0; y < this->img_height; ++y) {
		for (int x = 0; x < this->img_width; ++x) {
			if (x == 0) {
				n = x;
				m = x + 1;
			}
			else if (x == this->img_width - 1) {
				n = x - 1;
				m = x;
			}
			else {
				n = x - 1;
				m = x + 1;
			}
			dx = img[y][n] - img[y][m];

			if (y == 0) {
				n = y;
				m = y + 1;
			}
			else if (y == this->img_width - 1) {
				n = y - 1;
				m = y;
			}
			else {
				n = y - 1;
				m = y + 1;
			}
			dy = img[n][x] - img[m][x];
			
			mag = sqrt(pow(dx, 2) + pow(dy, 2));
			grad = atan2(dy, dx)*180.0 / M_PI;
			if (grad < 0.0) {
				grad += 360.0;
			}
			if (grad > 180.0) {
				grad -= 180.0;
			}
			grad /= angle;
			histogram[y/this->cell_size][x/this->cell_size][(int)grad%this->orientation_size] += mag;
		}
	}

	std::vector<std::vector<double>> histogram_sum(histogram_rows, std::vector<double>(histogram_cols));
	for (int i = 0; i < histogram_rows; ++i) {
		for (int j = 0; j < histogram_cols; ++j) {
			histogram_sum[i][j] = accumulate(begin(histogram[i][j]), end(histogram[i][j]), 0.0);
		}
	}

	std::vector<std::vector<double>> block_sum(block_rows, std::vector<double>(block_cols, 0));
	for (int i = 0; i < this->block_size; ++i) {
		for (int j = 0; j < this->block_size; ++j) {
			block_sum[0][0] += histogram_sum[i][j];
		}
	}
	for (int i = 1; i < block_cols; ++i) {
		block_sum[0][i] = block_sum[0][i - 1];
		for (int j = 0; j < this->block_size; ++j) {
			block_sum[0][i] += histogram_sum[j][i + this->block_size - 1] - histogram_sum[j][i - 1];
		}
	}
	for (int i = 1; i < block_rows; ++i) {
		for (int j = 0; j < block_cols; ++j) {
			block_sum[i][j] = block_sum[i - 1][j];
			for (int k = 0; k < this->block_size; ++k) {
				block_sum[i][j] += histogram_sum[i - 1][k] - histogram_sum[i + this->block_size - 1][k];
			}
		}
	}

}