#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"
#include "OneEuroFilter.h"

class SubWindow {
private:
	static constexpr int number_font_size{ 60 };
	static constexpr int number_circle_size{ 60 };

	DrawFrame df;
	ofRectangle view_rect;
	ofTrueTypeFont font;
	param::CONTENT_ID content_id;
	long long int user_id;
public:
	SubWindow();
	SubWindow(param::CONTENT_ID content_id, long long int user_id, const ofColor& user_color);

	void draw();

	ofRectangle get_rect() const;
	long long int get_user_id() const;
};

#endif