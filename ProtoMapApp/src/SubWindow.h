#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"
#include "OneEuroFilter.h"

class SubWindow {
private:
	static constexpr int frame_size{ 10 };
	static constexpr int defalut_height{ 400 };
	static constexpr int defalut_width{ static_cast<int>(defalut_height * 1.618) };

	DrawFrame df;
	ofRectangle view_rect;
	param::CONTENT_ID content_id;
	long long int user_id;

	OneEuroFilter filter_x{ 120, 0.08, 0.08 };
	OneEuroFilter filter_y{ 120, 0.08, 0.08 };
public:
	SubWindow();
	SubWindow(param::CONTENT_ID content_id, long long int user_id);

	void update();
	void draw();

	ofRectangle get_rect() const;
	void set_rect(const ofRectangle& rect);
	long long int get_user_id() const;
};

#endif