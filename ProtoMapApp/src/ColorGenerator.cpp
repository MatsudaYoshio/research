#include "ColorGenerator.h"

using namespace cv;

ColorGenerator::ColorGenerator() {
	Mat hsv_tmp{ 1, 1, CV_8UC3 ,{ 0,255,0 } }, rgb_tmp{ 1, 1, CV_8UC3 };
	Lab_color_type Lab_data;
	for (int i = 0, h = 0; h < this->hue_max; ++h) {
		for (int v = 0; v < this->value_max; ++v) {
			hsv_tmp.data[0] = h;
			hsv_tmp.data[2] = v;

			cvtColor(hsv_tmp, rgb_tmp, COLOR_HSV2RGB);
			Scalar rgb_data(rgb_tmp.data[0], rgb_tmp.data[1], rgb_tmp.data[2]);
			this->rgb2Lab(rgb_data[0], rgb_data[1], rgb_data[2], Lab_data);

			this->target_colors[i].first = Lab_data;
			this->target_colors[i].second = ofColor(rgb_data[0], rgb_data[1], rgb_data[2]);

			++i;
		}
	}
}

void ColorGenerator::add_color(const ofColor& color) {
	Mat rgb_tmp{ 1, 1, CV_8UC3 };
	rgb_tmp.data[0] = color.r;
	rgb_tmp.data[1] = color.g;
	rgb_tmp.data[2] = color.b;

	Scalar rgb_data(rgb_tmp.data[0], rgb_tmp.data[1], rgb_tmp.data[2]);

	Lab_color_type Lab_data;
	this->rgb2Lab(rgb_data[0], rgb_data[1], rgb_data[2], Lab_data);

	for (int i = 0; i < this->target_colors_max_num; ++i) {
		if (this->target_colors[i].first.L == Lab_data.L && this->target_colors[i].first.a == Lab_data.a && this->target_colors[i].first.b == Lab_data.b) {
			this->target_colors_state[i] = true;
			break;
		}
	}

	this->used_colors.emplace_back(make_pair(Lab_data, color));
}

void ColorGenerator::operator() (const int n) {
	double min_dist, opt_dist;
	int opt_color_index;

	for (int i = 0; i < n; ++i) {
		opt_dist = DBL_MIN;
		for (int j = 0; j < this->target_colors_max_num; ++j) {
			if (this->target_colors_state[j]) continue;

			min_dist = DBL_MAX;
			for (const auto& uc : this->used_colors) {
				min_dist = min(min_dist, this->ciede2000(this->target_colors[j].first, uc.first));
			}

			if (min_dist > opt_dist) {
				opt_dist = min_dist;
				opt_color_index = j;
			}
		}

		this->used_colors.emplace_back(this->target_colors[opt_color_index]);
		this->target_colors_state[opt_color_index] = true;
	}
}

void ColorGenerator::rgb2Lab(const int r, const int g, const int b, Lab_color_type& Lab) {
	double sr = r, sg = g, sb = b;

	sr /= 255; sg /= 255; sb /= 255;

	auto f = [](double& v) { v = v > 0.04045 ? pow((v + 0.055) / 1.055, 2.4) : v / 12.92; };
	f(sr); f(sg); f(sb);

	auto x{ sr*0.4124 + sg*0.3576 + sb*0.1805 };
	auto y{ sr*0.2136 + sg*0.7152 + sb*0.0722 };
	auto z{ sr*0.0193 + sg*0.1192 + sb*0.9505 };

	x *= 1.0521;
	z *= 0.9184;

	auto h = [](double& v) { v = v > 0.008856 ? pow(v, 1.0 / 3) : 7.787*v + 0.1379; };
	h(x); h(y); h(z);

	Lab.L = 116 * y - 16;
	Lab.a = 500 * (x - y);
	Lab.b = 200 * (y - z);
}

double ColorGenerator::ciede2000(const Lab_color_type Lab1, const Lab_color_type Lab2) const {
	const double delta_Lp = Lab2.L - Lab1.L;
	const double L_ = (Lab1.L + Lab2.L) / 2;
	const double C1 = sqrt(Lab1.a*Lab1.a + Lab1.b*Lab1.b);
	const double C2 = sqrt(Lab2.a*Lab2.a + Lab2.b*Lab2.b);
	const double C_ = (C1 + C2) / 2;
	const double ap1 = Lab1.a + 0.5*Lab1.a*(1 - sqrt(pow(C_, 7) / (pow(C_, 7) + 6103515625)));
	const double ap2 = Lab2.a + 0.5*Lab2.a*(1 - sqrt(pow(C_, 7) / (pow(C_, 7) + 6103515625)));
	const double Cp1 = sqrt(ap1*ap1 + Lab1.b*Lab1.b);
	const double Cp2 = sqrt(ap2*ap2 + Lab2.b*Lab2.b);
	const double Cp_ = (Cp1 + Cp2) / 2;
	const double delta_Cp = Cp2 - Cp1;

	double hp1;
	if (Lab1.b == 0 && ap1 == 0) {
		hp1 = 0;
	}
	else {
		hp1 = atan2(Lab1.b, ap1) * 180 / M_PI;
		if (hp1 < 0) hp1 += 360;
	}

	double hp2;
	if (Lab2.b == 0 && ap2 == 0) {
		hp2 = 0;
	}
	else {
		hp2 = atan2(Lab2.b, ap2) * 180 / M_PI;
		if (hp2 < 0) hp2 += 360;
	}

	double delta_hp;
	if (C1 == 0 || C2 == 0) {
		delta_hp = 0;
	}
	else if (fabs(hp1 - hp2) <= 180) {
		delta_hp = hp2 - hp1;
	}
	else if (hp2 <= hp1) {
		delta_hp = hp2 - hp1 + 360;
	}
	else {
		delta_hp = hp2 - hp1 - 360;
	}

	const double delta_Hp = 2 * sqrt(Cp1*Cp2)*sin(delta_hp*M_PI / 180 / 2);

	double Hp_;
	if (fabs(hp1 - hp2) > 180) {
		Hp_ = (hp1 + hp2 + 360) / 2;
	}
	else {
		Hp_ = (hp1 + hp2) / 2;
	}

	const double T = 1 - 0.17*cos((Hp_ - 30)*M_PI / 180) + 0.24*cos(Hp_ * 2 * M_PI / 180) + 0.32*cos((3 * Hp_ + 6)*M_PI / 180) - 0.2*cos((4 * Hp_ - 63)*M_PI / 180);
	const double S_L = 1 + (0.015*pow(L_ - 50, 2)) / sqrt(20 + pow(L_ - 50, 2));
	const double S_C = 1 + 0.045*Cp_;
	const double S_H = 1 + 0.015*Cp_*T;
	const double R_T = -2 * sqrt(pow(Cp_, 7) / (pow(Cp_, 7) + 6103515625))*sin(60 * exp(-pow((Hp_ - 275) / 25, 2))*M_PI / 180);

	return sqrt(pow(delta_Lp / S_L, 2) + pow(delta_Cp / S_C, 2) + pow(delta_Hp / S_H, 2) + R_T*(delta_Cp / S_C)*(delta_Hp / S_H));
}