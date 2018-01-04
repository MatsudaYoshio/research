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
	static constexpr double max_life = 100;

	BaseScene* scene;
	HandCursor* hc;
	ofRectangle view_rect;
	ofxSecondWindow window;
	DrawFrame df;

	int cursor_state;
	int user_id, scene_id;
	int tmp_width, tmp_height, tmp_id;
	double life;
public:
	static constexpr int track_rects_num = 25;
	static constexpr int TRACK_READY = -1;

	ofRectangle track_rects[track_rects_num];
	int track_index;

	void setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, int x, int y, int w, int h);
	void setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, ofRectangle rect);
	void update();
	void draw();

	const enum class CURSOR_STATE {
		INACTIVE, POINT
	};

	bool is_inside(const ofPoint &p) const;
	ofRectangle get_rect() const;
	void set_rect(const ofRectangle &rect);
	int get_user_id() const;

	ofEvent<int> delete_sub_window_event;
	ofEvent<pair<int, int>> user_leave_event;
	ofEvent<int> cursor_disappear_event;
	void exit();
private:
};

#endif