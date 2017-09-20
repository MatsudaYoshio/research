#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "BaseScene.h"
#include "HandPointer.h"
#include "SubScene.h"

class SceneManager {
private:
	static constexpr int window_width = 1920;
	static constexpr int window_height = 1080;

	unordered_map<string, BaseScene*> scenes;
	vector<SubScene> sub_scenes;
	string current_scene;
	ofFbo fbo;
	ofPixels pixels;
	ofImage img;
	HandPointer* hp;
public:
	void setup(HandPointer* hp);
	void update();
	void draw();
	void pointed(pair<string, int> &id);
	void transition(string &str);
	void make_sub_window(string &str);
	~SceneManager();
};

#endif