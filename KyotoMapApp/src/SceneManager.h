#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "BaseScene.h"
#include "HandCursor.h"
#include "SubWindow.h"
#include "MainScene.h"
#include "SimulatedAnnealing.h"

#include <mutex>

class SceneManager {
private:
	HandCursor* hc;
	SimulatedAnnealing sa;
	MainScene main_scene;

	unordered_map<int, SubWindow> sub_windows;
	map<long long int, bool> cursor_log;
	long long int scene_id = 0;
	queue<int> erase_scene_id;
	unordered_map<int, ofRectangle> rects_tmp, best_rects, old_rects;
	bool transform_thread_flag = false;
	unordered_map<int, string> cursor_assignment;
	vector<int> active_scene_id_list, active_scene_id_list_tmp;
	std::mutex mtx;
public:
	void setup(HandCursor* hc);
	void update();
	void draw();
	void pointed(pair<int, int> &id);
	void make_sub_window(pair<int, int>& id);
	void delete_sub_window(int &scene_id);
	void inactivate_sub_window(int &scene_id);
	void transform(unordered_map<int, ofRectangle> &old_rects, unordered_map<int, ofRectangle> &new_rects);
	~SceneManager();
};

#endif