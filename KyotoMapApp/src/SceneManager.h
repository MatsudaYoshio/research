#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "BaseScene.h"
#include "HandCursor.h"
#include "SubWindow.h"
#include "MainScene.h"
#include "SimulatedAnnealing.h"

class SceneManager {
private:
	HandCursor* hc;
	SimulatedAnnealing sa;
	MainScene main_scene;

	/* óêêî */
	static std::random_device rd;
	static std::mt19937 mt;

	unordered_map<int, SubWindow> sub_windows;
	set<long long int> cursor_log;
	long long int scene_id = 0;
	unordered_map<int, ofRectangle> rects_tmp, best_rects, old_rects;
	bool transform_thread_flag = false;
	unordered_map<int, string> cursor_assignment;
	vector<int> active_scene_id_list, active_scene_id_list_tmp;
	queue<int> delete_scene_list;
public:
	void setup(HandCursor* hc);
	void update();
	void draw();
	void pointed(pair<int, int>& id);
	void make_sub_window(pair<int, int>& id);
	void delete_sub_window(int& scene_id);
	//void inactivate_sub_window(int& scene_id);
	void transform(unordered_map<int, ofRectangle>& old_rects, unordered_map<int, ofRectangle>& new_rects);
	~SceneManager();
};

#endif