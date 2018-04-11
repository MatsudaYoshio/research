#pragma once
#ifndef ___Class_AdItem
#define ___Class_AdItem

#include "AppParameters.h"
#include "DrawFrame.h"

class AdItem {
private:
	static constexpr int frame_size{ 10 };

	DrawFrame df;
	ofRectangle position;
	param::CONTENT_ID content_id;
	ofImage image;
	ofTrueTypeFont font;
public:
	void setup(const ofRectangle& position, param::CONTENT_ID content_id);
	void draw();
};

#endif