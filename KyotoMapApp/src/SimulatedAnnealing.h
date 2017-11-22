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

	double current_cost, next_cost, best_cost;
	unordered_map<int, ofRectangle> current_state, next_state;
	int modify_window_num, modify_param;

	bool set_next_state();
	void calculate_cost();
	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const;
public:
	void setup(HandCursor* hc, vector<int>* active_window_list, vector<int>* main_window_user_list, unordered_map<int, SubWindow>* sub_windows);
	void operator() (const unordered_map<int, ofRectangle>& start_state, unordered_map<int, ofRectangle>& best_state);
};

#endif