#pragma once
#ifndef ___Class_ColorGenerator
#define ___Class_ColorGenerator

#define _USE_MATH_DEFINES

#include "ofMain.h"

#include <cmath>
#include <unordered_map>
#include <opencv2/opencv.hpp>

class ColorGenerator {
public:
	using Lab_color_type = struct { int L, a, b; };
	using color_info = pair<Lab_color_type, ofColor>;

	vector<color_info> used_colors;

	ColorGenerator::ColorGenerator();
	void add_color(const ofColor& color);
	void operator() (int n = 1);
private:
	static constexpr int hue_max{ 181 };
	static constexpr int value_max{ 256 };
	static constexpr int target_colors_max_num{ hue_max*value_max };

	array<color_info, target_colors_max_num> target_colors;
	array<bool, target_colors_max_num> target_colors_state;

	void rgb2Lab(int r, int g, int b, Lab_color_type& Lab);
	double ciede2000(Lab_color_type Lab1, Lab_color_type Lab2) const;
};

#endif