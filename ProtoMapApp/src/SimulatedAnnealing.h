#pragma once
#ifndef ___Class_SimulatedAnnealing
#define ___Class_SimulatedAnnealing

#include "math.h"
#include "HandCursor.h"
#include "SubWindow.h"
#include "HistoryPlotterSecondWindow.h"

class SimulatedAnnealing {
private:
	HandCursor* hc;
	//HistoryPlotterSecondWindow cost_plotter{ "cost function", ofRectangle(0,0,1920,1080), 50 };

	unordered_map<long long int, SubWindow>* sub_windows;

	static constexpr int MAX_ITERATION{ 1000 };
	static constexpr double sigma{ 170 };

	static constexpr int convergence_check_number{ 100 };

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_parameter; // 変化させるパラメータを選択する乱数
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数を返す乱数

	double current_cost, next_cost, best_cost, area_cost, overlap_cost, distance_cost;
	int modify_window_num, modify_param;
	std::unordered_map<long long int, ofRectangle> current_state, next_state;

	int file_index = 1;
	ofstream ofs;

	void set_next_state();
public:
	void setup(HandCursor* hc, unordered_map<long long int, SubWindow>* sub_windows);
	void calculate_cost(const unordered_map<long long int, ofRectangle>& state, double& cost, const unordered_map<long long int, SubWindow>& sub_windows);
	void operator() (const unordered_map<long long int, ofRectangle>& initial_state, unordered_map<long long int, ofRectangle>& best_state, double& best_cost);
};

#endif