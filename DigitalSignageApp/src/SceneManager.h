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
	unordered_map<int, SubScene> sub_scenes;
	string current_scene;
	ofFbo fbo;
	ofPixels pixels;
	ofImage img;
	HandPointer* hp;
	map<long long int, bool> pointer_log;
	long long int scene_id = 0;
	queue<long long int> erase_scene_id;

	ofxSecondWindow secondWindow;
	int x = 50, y = 50, w = 500, h = 500;
	double past_cost, current_cost;
	int t = 2;
	int f = 0;
	bool flag = false;
public:
	void setup(HandPointer* hp);
	void update();
	void draw();
	void pointed(pair<string, int> &id);
	void transition(int &pointer_id);
	void make_sub_window(int &pointer_id);
	void delete_sub_window(int &scene_id);
	void change_cursor_to_main_window(int &user_id);
	void test(int old_x, int old_y, int old_w, int old_h, int new_x, int new_y, int new_w, int new_h);
	~SceneManager();
};

#endif