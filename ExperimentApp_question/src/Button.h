#pragma once
#ifndef ___Class_Button
#define ___Class_Button

#include "ofMain.h"

class Button {
private:
	float alpha{ ofColor::limit() };
	ofColor color{ ofColor::silver };
	ofTrueTypeFont font;
	string text;
	ofPoint text_position;
public:
	ofRectangle rect;
	const enum class STATE {
		NO_PRESSED, PRESSED
	};

	void setup(const ofRectangle& rect, const string& text, const ofPoint& text_position, int text_size);
	void setup(const ofRectangle& rect, const string& text, const ofPoint& text_position, int text_size, const ofColor& color);
	void update();
	void draw() const;

	void switch_state();
private:
	STATE state{ STATE::NO_PRESSED };
};

#endif