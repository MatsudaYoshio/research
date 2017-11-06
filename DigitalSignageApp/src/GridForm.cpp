#include "GridForm.h"

using namespace param;

GridForm::GridForm() {
	for (int x = 0, x_pos = 0, i = 0; x < FORM_W; ++x, x_pos += GRID_W) {
		for (int y = 0, y_pos = 0; y < FORM_H; ++y, y_pos += GRID_H) {
			this->grid_rects[x][y] = ofRectangle(x_pos, y_pos, GRID_W, GRID_H);
			this->index_table[x][y] = BITS_SIZE-i++-1;
		}
	}
}

void GridForm::set_bit(const int &pos_x, const int &pos_y, const int &bit) {
	this->grid_bits[this->index_table[pos_x][pos_y]] = bit;
}

void GridForm::draw() const {
	ofSetColor(ofColor::blue);
	ofFill();
	for (int x = 0; x < FORM_W; ++x) {
		for (int y = 0; y < FORM_H; ++y) {
			if(this->grid_bits.test(this->index_table[x][y])) {
				ofDrawRectangle(this->grid_rects[x][y]);
			}
		}
	}
	ofNoFill();
}