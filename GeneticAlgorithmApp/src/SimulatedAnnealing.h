#pragma once
#ifndef ___Class_SimulatedAnnealing
#define ___Class_SimulatedAnnealing

#include "math.h"
#include "ofMain.h"
#include "HandCursor.h"
#include "SubWindow.h"
#include "AppParameters.h"

class SimulatedAnnealing {
private:
	HandCursor* hc;

	vector<long long int>* active_window_list;
	vector<long long int>* main_window_user_list;
	unordered_map<long long int, SubWindow>* sub_windows;

	static constexpr int MAX_ITERATION{ 2000 };

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_parameter; // 変化させるパラメータを選択する乱数
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数を返す乱数

	double current_cost, next_cost, best_cost, area_cost, overlap_cost, shape_cost, distance_cost;
	int modify_window_num, modify_param;
	unordered_map<long long int, ofRectangle> current_state, next_state;

	//int file_index = 1;
	//ofstream ofs;

	bool set_next_state();
	void calculate_cost();
public:
	void setup(HandCursor* hc, vector<long long int>* active_window_list, vector<long long int>* main_window_user_list, unordered_map<long long int, SubWindow>* sub_windows);
	void operator() (const unordered_map<long long int, ofRectangle>& start_state, unordered_map<long long int, ofRectangle>& best_state);
};

#endif