#pragma once
#ifndef ___Class_GridForm
#define ___Class_GridForm

#include "ofMain.h"
#include "AppParameters.h"

class GridForm {
private:
	array<array<int, param::FORM_H>, param::FORM_W> index_table;
	array<array<ofRectangle, param::FORM_H>, param::FORM_W> grid_rects;
public:
	bitset<param::BITS_SIZE> grid_bits;

	GridForm();
	void set_bit(const int &pos_x, const int &pos_y, const int &bit);
	int get_area();
	void draw() const;
};

#endif