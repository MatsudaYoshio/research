#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "AppParameters.h"
#include "DrawFrame.h"
#include "OneEuroFilter.h"

class SubWindow {
private:
	static constexpr int frame_size{ 12 };
	static constexpr double defalut_height{ 600 };
	static constexpr double defalut_width{ defalut_height * 1.618 }; // ‰©‹à”ä
	static constexpr double track_rects_num{ 12 };
	static constexpr auto change_rate{ 1 / track_rects_num };
	static constexpr int number_font_size{ 60 };
	static constexpr int number_circle_size{ 60 };

	DrawFrame df;
	ofRectangle view_rect;
	ofTrueTypeFont font;
	param::CONTENT_ID content_id;
	long long int user_id;
	queue<ofRectangle> next_rects;
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