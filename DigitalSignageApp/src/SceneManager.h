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
	map<long long int, bool> pointer_log;
public:
	void setup(HandPointer* hp);
	void update();
	void draw();
	void pointed(pair<string, int> &id);
	void transition(int &pointer_id);
	void make_sub_window(int &pointer_id);
	~SceneManager();
};

#endif