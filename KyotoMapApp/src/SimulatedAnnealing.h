#pragma once
#ifndef ___Class_SimulatedAnnealing
#define ___Class_SimulatedAnnealing

#include "ofMain.h"
#include "HandCursor.h"
#include "SubWindow.h"
#include "AppParameters.h"

class SimulatedAnnealing {
private:
	HandCursor* hc;

	vector<int>* active_window_list;
	vector<int>* main_window_user_list;
	unordered_map<int, SubWindow>* sub_windows;

	static constexpr int MAX_ITERATION = 3000;

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_parameter; // 変化させるパラメータを選択する乱数

	double current_cost, next_cost, best_cost;
	unordered_map<int, ofRectangle> current_state, next_state;
	int modify_window_num, modify_param;

	bool set_next_state();
	void calculate_cost();
public:
	void setup(HandCursor* hc, vector<int>* active_window_list, vector<int>* main_window_user_list, unordered_map<int, SubWindow>* sub_windows);
	void operator() (const unordered_map<int, ofRectangle>& start_state, unordered_map<int, ofRectangle>& best_state);
};

#endif