#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"
#include "OneEuroFilter.h"

class SubWindow {
private:
	DrawFrame df;
	ofRectangle view_rect;
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