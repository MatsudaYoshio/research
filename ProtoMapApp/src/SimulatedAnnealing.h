#pragma once
#ifndef ___Class_SimulatedAnnealing
#define ___Class_SimulatedAnnealing

#include "math.h"
#include "HandCursor.h"
#include "SubWindow.h"

class SimulatedAnnealing {
private:
	HandCursor* hc;

	unordered_map<long long int, SubWindow>* sub_windows;

	static constexpr int MAX_ITERATION{ 3000 };
	static constexpr int convergence_check_number{ 100 };

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_parameter; // 変化させるパラメータを選択する乱数
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数を返す乱数

	double past_cost, current_cost, next_cost, best_cost, area_cost, overlap_cost, distance_cost;
	int modify_window_num, modify_param;
	int convergence_count;
	std::unordered_map<long long int, ofRectangle> current_state, next_state;

	int file_index = 1;
	ofstream ofs;

	bool set_next_state();
	void calculate_cost();
public:
	void setup(HandCursor* hc, unordered_map<long long int, SubWindow>* sub_windows);
	void operator() (const unordered_map<long long int, ofRectangle>& start_state, unordered_map<long long int, ofRectangle>& best_state, double& best_cost);
};

#endif