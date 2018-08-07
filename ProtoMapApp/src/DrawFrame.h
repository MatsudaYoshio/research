#pragma once
#ifndef ___Class_DrawFrame
#define ___Class_DrawFrame

#include "ofMain.h"

/* �l�p���g��`�悷��N���X */
class DrawFrame {
private:
	ofRectangle frame_rect;
	int size;
	ofPath path;

	void make_path();
public:
	void setup(const ofColor& color, int size, const ofRectangle& frame_rect);
	void setup(const ofColor& color, int size);
	void update(ofRectangle& frame_rect);
	void draw();

	void change_color(const ofColor& color);

	~DrawFrame();
};

#endif