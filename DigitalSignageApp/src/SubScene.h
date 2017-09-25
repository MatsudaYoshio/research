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

	BaseScene* scene;
	HandPointer* hp;
	ofFbo fbo;
	ofPixels pixels;
	ofImage img;
	ofRectangle window_rect, view_rect;
	unordered_map<string, ofTexture> cursor_texture;
	string cursor_state;
	int alpha = 255;
	const string window_name = "sub_window";
	ofxSecondWindow sub_window;
	int pointer_id;
	int scene_id;
	int life = 100;
public:
	vector<int> track_id;
	void setup(BaseScene* scene, HandPointer* hp, int pointer_id, int scene_id);
	void update();
	void draw();
	bool is_inside(const ofPoint &p) const;
	ofEvent<int> delete_sub_window_event;
	void exit();
	~SubScene();
};

#endif