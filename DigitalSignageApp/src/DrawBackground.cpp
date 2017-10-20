#include "DrawBackground.h"

void DrawBackground::setup(const ofColor& color1, const ofColor& color2) {
	this->n = 0;

	this->color_grad[0].first = this->color_grad[50].first = color1;
	this->color_grad[0].second = this->color_grad[50].second = color2;
	this->color_grad[25].first = color1 - 25;
	this->color_grad[25].second = color2 - 25;
	this->color_grad[75].first = color1 + 25;
	this->color_grad[75].second = color2 + 25;
	for (int i = 1; i < 25; ++i) {
		this->color_grad[i].first = this->color_grad[50 - i].first = color1 - i;
		this->color_grad[i].second = this->color_grad[50 - i].second = color2 - i;
		this->color_grad[i + 50].first = this->color_grad[100 - i].first = color1 + i;
		this->color_grad[i + 50].second = this->color_grad[100 - i].second = color2 + i;
	}
}

void DrawBackground::draw() {
	ofBackgroundGradient(this->color_grad[this->n].first, this->color_grad[this->n].second, OF_GRADIENT_CIRCULAR);
	this->n++;
	this->n %= 100;
}