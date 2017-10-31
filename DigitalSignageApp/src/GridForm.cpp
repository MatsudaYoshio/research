#include "GridForm.h"

using namespace param;

GridForm::GridForm() {
	fill(&this->grid_bits[0][0], &this->grid_bits[param::FORM_W][0], false);

	for (int x = 0, x_pos = 0; x < FORM_W; ++x, x_pos += GRID_W) {
		for (int y = 0, y_pos = 0; y < FORM_H; ++y, y_pos += GRID_H) {
			this->grid_rects[x][y] = ofRectangle(x_pos, y_pos, GRID_W, GRID_H);
		}
	}
}

int GridForm::get_area() {
	return count(&this->grid_bits[0][0], &this->grid_bits[param::FORM_W][0], true);
}

void GridForm::draw() const {
	ofSetColor(ofColor::blue);
	ofFill();
	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if (this->grid_bits[x][y]) {
				ofDrawRectangle(this->grid_rects[x][y]);
			}
		}
	}
	ofNoFill();
}