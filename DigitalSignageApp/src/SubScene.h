#pragma once
#ifndef ___Class_SubScene
#define ___Class_SubScene

#include "ofMain.h"
#include "BaseScene.h"
#include "ofxSecondWindow.h"

class SubScene {
private:
	static constexpr int main_window_width = 1920;
	static constexpr int main_window_height = 1080;
	static constexpr double max_life = 100;
	BaseScene* scene;
	HandCursor* hc;
	ofRectangle view_rect;
	unordered_map<string, ofTexture> cursor_texture;
	string cursor_state;
	int alpha = 255;
	const string window_name = "sub_window";
	ofxSecondWindow sub_window;
	int user_id;
	int scene_id;
	double life;
	int tmp_width, tmp_height;
public:
	vector<int> track_id;
	void setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, int x, int y, int w, int h);
	void setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, ofRectangle rect);
	void update();
	void draw();
	bool is_inside(const ofPoint &p) const;
	ofRectangle get_rect() const;
	int get_user_id() const;
	ofEvent<int> delete_sub_window_event;
	ofEvent<int> user_leave_event;
	void exit();
	~SubScene();
private:
	void add_cursor_texture(const string cursor_type, const string texture_path);
};

#endif