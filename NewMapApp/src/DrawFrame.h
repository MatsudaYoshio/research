#pragma once
#ifndef ___Class_DrawFrame
#define ___Class_DrawFrame

#include "ofMain.h"

class DrawFrame {
private:
	ofRectangle frame_rect;
	int size;
	ofPath path;
public:
	void setup(const ofColor& color, int size);
	void update(ofRectangle& frame_rect);
	void draw();

	~DrawFrame();
};

#endif