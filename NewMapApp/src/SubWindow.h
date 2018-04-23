#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "ofMain.h"
#include "ofxSecondWindow.h"
#include "HandCursor.h"
#include "DrawFrame.h"

class SubWindow {
private:
	static constexpr int frame_size{ 20 };
	static constexpr int defalut_width{ 400 };
	static constexpr int defalut_height{ 400 };

	ofxSecondWindow window;
	DrawFrame df;
	ofRectangle view_rect;
	param::CONTENT_ID content_id;
	long long int user_id;
	ofImage image;
	ofTrueTypeFont font;
public:
	static constexpr int track_rects_num{ 25 };
	static constexpr int TRACK_READY{ -1 };

	ofRectangle track_rects[track_rects_num];
	int track_index;

	SubWindow(param::CONTENT_ID content_id, long long int user_id);

	void update();
	void draw();

	//bool is_inside(const ofPoint& p) const;
	//ofRectangle get_rect() const;
	//void set_rect(const ofRectangle& rect);
	//int get_user_id() const;

	ofEvent<long long int> delete_sub_window_event;
	void exit();
};

#endif