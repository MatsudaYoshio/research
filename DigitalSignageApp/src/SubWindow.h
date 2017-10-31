#pragma once
#ifndef ___Class_SubWindow
#define ___Class_SubWindow

#include "ofMain.h"
#include "BaseScene.h"
#include "ofxSecondWindow.h"
#include "HandCursor.h"

class SubWindow {
private:
	static constexpr double max_life = 100;
	BaseScene* scene;
	HandCursor* hc;
	ofRectangle view_rect, frame;
	string cursor_state;
	ofxSecondWindow window;
	int user_id;
	int scene_id;
	double life;
	int tmp_width, tmp_height;
public:
	void setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, int x, int y, int w, int h);
	void setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, ofRectangle rect);
	void update();
	void draw();
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