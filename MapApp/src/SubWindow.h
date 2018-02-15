#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "ofMain.h"
#include "BaseScene.h"
#include "ofxSecondWindow.h"
#include "HandCursor.h"
#include "DrawFrame.h"

class SubWindow {
private:
	unique_ptr<BaseScene> scene;
	HandCursor* hc;
	ofRectangle view_rect;
	ofxSecondWindow window;
	DrawFrame df;

	int cursor_state;
	long long int user_id, scene_id, tmp_id;
public:
	static constexpr int track_rects_num{ 25 };
	static constexpr int TRACK_READY{ -1 };

	ofRectangle track_rects[track_rects_num];
	int track_index;

	void setup(unique_ptr<BaseScene>&& scene, HandCursor* hc, long long int user_id, long long int scene_id, int x, int y, int w, int h);
	void setup(unique_ptr<BaseScene>&& scene, HandCursor* hc, long long int user_id, long long int scene_id, ofRectangle rect);
	void update();
	void draw();

	const enum class CURSOR_STATE {
		INACTIVE, POINT
	};

	bool is_inside(const ofPoint& p) const;
	ofRectangle get_rect() const;
	void set_rect(const ofRectangle& rect);
	int get_user_id() const;

	ofEvent<long long int> delete_sub_window_event;
	void exit();
};

#endif