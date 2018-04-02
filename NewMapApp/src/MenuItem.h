#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "ofMain.h"
#include "AppParameters.h"

class MenuItem {
private:
	static constexpr double texture_size_ratio{ 0.6 };
	static constexpr int font_size{ 30 };

	string item_name;
	ofColor color;
	ofTexture texture;
	ofRectangle item_rect, texture_rect;
	ofTrueTypeFont font;
	int font_x, font_y;
public:
	void setup(const ofColor color, const string& image_path, const ofRectangle& item_rect, const string& item_name, const int font_x, const int font_y);
	void draw();
};

#endif