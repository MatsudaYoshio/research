#pragma once
#ifndef ___Class_DrawFrame
#define ___Class_DrawFrame

#include "ofMain.h"

/* ŽlŠp‚¢˜g‚ð•`‰æ‚·‚éƒNƒ‰ƒX */
class DrawFrame {
private:
	static constexpr int frame_size{ 12 };

	ofRectangle frame_rect;
	ofPath path;
public:
	void setup(const ofColor& color, const ofRectangle& frame_rect);
	void draw();

	~DrawFrame();
};

#endif