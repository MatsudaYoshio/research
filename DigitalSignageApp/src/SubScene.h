#pragma once
#ifndef ___Class_SubScene
#define ___Class_SubScene

#include "ofMain.h"
#include "BaseScene.h"
#include "ofxSecondWindow.h"

class SubScene {
private:
	static constexpr double max_life = 100;
	BaseScene* scene;
	HandCursor* hc;
	ofRectangle view_rect, frame;
	unordered_map<string, ofTexture> cursor_texture;
	string cursor_state;
	const string window_name = "sub window";
	ofxSecondWindow sub_window;
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
	~SubScene();
private:
	void add_cursor_texture(const string cursor_type, const string texture_path);
};

#endif