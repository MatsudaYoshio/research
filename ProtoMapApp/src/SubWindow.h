#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"
#include "OneEuroFilter.h"
#include "MovingAverageFilter.h"

class SubWindow {
private:
	static constexpr int frame_size{ 10 };
	static constexpr int defalut_height{ 400 };
	static constexpr int defalut_width{ static_cast<int>(defalut_height * 1.618) };
	static constexpr double track_rects_num{ 10 };
	static constexpr auto change_rate{ 1 / track_rects_num };

	DrawFrame df;
	ofRectangle view_rect;
	param::CONTENT_ID content_id;
	long long int user_id;
	queue<ofRectangle> next_rects;

	MovingAverageFilter filter_x{ 2 };
	MovingAverageFilter filter_y{ 2 };
	//OneEuroFilter filter_x{ 120, 0.05, 0.05 };
	//OneEuroFilter filter_y{ 120, 0.05, 0.05 };
public:
	SubWindow();
	SubWindow(param::CONTENT_ID content_id, long long int user_id, const ofColor& user_color);

	void update();
	void draw();

	ofRectangle get_rect() const;
	void set_rect(ofRectangle rect);
	long long int get_user_id() const;
};

#endif