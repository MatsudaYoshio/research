#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"

class SubWindow {
private:
	static constexpr int frame_size{ 10 };
	static constexpr int defalut_height{ 400 };
	static constexpr int defalut_width{ static_cast<int>(defalut_height * 1.618) };

	DrawFrame df;
	ofRectangle view_rect;
	param::CONTENT_ID content_id;
	long long int user_id;
public:
	static constexpr int track_rects_num{ 20 };
	static constexpr int TRACK_READY{ -1 };

	ofRectangle track_rects[track_rects_num];
	int track_index;

	SubWindow(param::CONTENT_ID content_id, long long int user_id);

	void update();
	void draw();

	ofRectangle get_rect() const;
	long long int get_user_id() const;
};

#endif