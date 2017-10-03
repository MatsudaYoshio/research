#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "BaseScene.h"
#include "HandCursor.h"
#include "SubScene.h"

class SceneManager {
private:
	static constexpr int window_width = 1920;
	static constexpr int window_height = 1080;

	unordered_map<string, BaseScene*> scenes;
	unordered_map<int, SubScene> sub_scenes;
	string current_scene;
	HandCursor* hc;
	map<long long int, bool> cursor_log;
	long long int scene_id = 0;
	queue<long long int> erase_scene_id;

	double past_cost = DBL_MAX, current_cost, best_cost = DBL_MAX;
	unordered_map<int, ofRectangle> rects_tmp, best_rects, old_rects;
	bool flag = false;
	unordered_map<int, string> cursor_assignment;
	double calculate_cost();
	vector<int> active_scene_id_list;
public:
	void setup(HandCursor* hc);
	void update();
	void draw();
	void pointed(pair<string, int> &id);
	void transition(int &pointer_id);
	void make_sub_window(int &pointer_id);
	void delete_sub_window(int &scene_id);
	void change_cursor_to_main_window(pair<int, int> &id);
	void transform(unordered_map<int, ofRectangle> &old_rects, unordered_map<int, ofRectangle> &new_rects);
	~SceneManager();
};

#endif