#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"

class SubWindow {
private:
	static constexpr int frame_size{ 12 };
	static constexpr double default_height{ 700 };
	static constexpr double default_width{ default_height * 1.618 }; // ‰©‹à”ä

	DrawFrame df;
	ofRectangle view_rect;
	param::CONTENT_ID content_id;
	long long int user_id;
public:
	SubWindow();
	SubWindow(param::CONTENT_ID content_id, long long int user_id, const ofColor& user_color);

	void update();
	void draw();

	ofRectangle get_rect() const;
	long long int get_user_id() const;
};

#endif