#pragma once
#ifndef ___Class_Mole
#define ___Class_Mole

#include "ofMain.h"

#include "AppParameters.h"

class Mole {
private:
	int appearance_time{ 0 };
	int hidden_time{ 0 };
	ofRectangle draw_rect;
public:
	void setup(const ofRectangle& draw_rect, int hidden_time);
	void draw();

	ofRectangle get_rect() const;
	bool is_idle() const;
	bool is_appear() const;
	void appear(int appearance_time);
	void hide(int hidden_time);
};

#endif