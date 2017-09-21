#pragma once
#ifndef ___Class_SubScene
#define ___Class_SubScene

#include "ofMain.h"
#include "BaseScene.h"
#include "ofxSecondWindow.h"

class SubScene {
private:
	static constexpr int window_width = 1920;
	static constexpr int window_height = 1080;

	BaseScene* scene;
	HandPointer* hp;
	ofFbo fbo;
	ofPixels pixels;
	ofImage img;
	ofRectangle rect;

	const string window_name = "sub_window";
	ofxSecondWindow sub_window;
	int pointer_id;
public:
	vector<int> track_id;
	void setup(BaseScene* scene, HandPointer* hp, int pointer_id);
	void update();
	void draw();
	bool is_inside(const ofPoint &p) const;
	~SubScene();
};

#endif