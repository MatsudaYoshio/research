#pragma once
#ifndef ___Class_Pin
#define ___Class_Pin

#include "ofMain.h"
#include "AppParameters.h"

class Pin {
private:
	static constexpr double size_ratio{ 0.35 };
	static constexpr double radius{ 45 };
	static constexpr int font_size{ 40 };

	int pin_number;
	param::CONTENT_ID content_id;
	int x, y, tx1, ty1, tx2, ty2, tx3, ty3, font_x, font_y;
	ofTrueTypeFont font;
	ofColor color;
public:
	void setup(param::CONTENT_ID content_id);
	void update();
	void draw() const;
};

#endif