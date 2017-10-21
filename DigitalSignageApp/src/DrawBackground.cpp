#include "DrawBackground.h"

void DrawBackground::setup(const ofColor& color1, const ofColor& color2, const int& g) {
	this->granularity = g;
	this->color_grad.resize(g);

	this->n = 0;

	const int x = g / 2;
	const int y = g / 4;
	const int z = y * 3;

	this->color_grad[0].first = this->color_grad[x].first = color1;
	this->color_grad[0].second = this->color_grad[x].second = color2;
	this->color_grad[y].first = color1 - y;
	this->color_grad[y].second = color2 - y;
	this->color_grad[z].first = color1 + y;
	this->color_grad[z].second = color2 + y;
	for (int i = 1; i < y; ++i) {
		this->color_grad[i].first = this->color_grad[x - i].first = color1 - i;
		this->color_grad[i].second = this->color_grad[x - i].second = color2 - i;
		this->color_grad[i + x].first = this->color_grad[g - i].first = color1 + i;
		this->color_grad[i + x].second = this->color_grad[g - i].second = color2 + i;
	}
}

void DrawBackground::draw() {
	ofBackgroundGradient(this->color_grad[this->n].first, this->color_grad[this->n].second, OF_GRADIENT_CIRCULAR);
	++this->n %= this->granularity;
}