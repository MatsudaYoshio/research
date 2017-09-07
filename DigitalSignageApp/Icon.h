#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"

class Icon {
private:
	ofRectangle rect;
	ofPoint tl, tr, bl, br; // ����A�E��A�����A�E����4�_
	ofTexture texture;
public:
	Icon(const int x, const int y, const int width, const int height, const string img_path);
	void setup(const int x, const int y, const int width, const int height, const string img_path);
	void update();
	void draw();
	bool is_inside(const ofPoint &p) const;
};

#endif