#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "ofMain.h"
#include "AppParameters.h"

class MenuItem {
private:
	static constexpr double texture_size_ratio{ 0.6 };

	ofColor color;
	ofTexture texture;
	ofRectangle item_rect, texture_rect;
public:
	void setup(const ofColor color, const char* image_path, const ofRectangle& item_rect);
	void update();
	void draw();
};

#endif