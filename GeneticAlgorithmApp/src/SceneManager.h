#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "BaseScene.h"
#include "HandCursor.h"
#include "SubWindow.h"
#include "MainScene.h"
#include "SimulatedAnnealing.h"
#include "KyotoTowerScene.h"
#include "windows.h"
#include "HigashihonganjiScene.h"
#include "SyoseienScene.h"
#include "NishihonganjiScene.h"
#include "RyukokuMuseumScene.h"
#include "KyotoAquariumScene.h"
#include "AppParameters.h"
#include "GeneticAlgorithm.h"

#include <opencv2/opencv.hpp>

class SceneManager {
private:
	HandCursor* hc;
	//SimulatedAnnealing sa;
	MainScene main_scene;
	GeneticAlgorithm ga;

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;

	unordered_map<long long int, SubWindow> sub_windows;
	set<long long int> cursor_log;
	long long int scene_id{ 0 }; // オーバーフローする可能性あり
	unordered_map<long long int, ofRectangle> rects_tmp, best_rects, old_rects;
	bool transform_thread_flag{ false };
	unordered_map<long long int, string> cursor_assignment;
	vector<long long int> active_scene_id_list, active_scene_id_list_tmp;
	queue<long long int> delete_scene_list;
	unique_ptr<BaseScene> sub_window_scene;
	set<long long int> selected_users;
public:
	void setup(HandCursor* hc);
	void update();
	void draw();
	void pointed(pair<int, long long int>& id);
	void make_sub_window(pair<int, long long int>& id);
	void delete_sub_window(long long int& scene_id);
	void transform(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects);
	~SceneManager();
};

#endif