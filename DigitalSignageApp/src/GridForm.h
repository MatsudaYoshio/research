#pragma once
#ifndef ___Class_GridForm
#define ___Class_GridForm

#include "ofMain.h"
#include "AppParameters.h"

class GridForm {
private:
	array<array<bool, param::FORM_H>, param::FORM_W> grid_bits;
	array<array<ofRectangle, param::FORM_H>, param::FORM_W> grid_rects;
public:
	GridForm();
	int get_area();
	void draw() const;
};

#endif