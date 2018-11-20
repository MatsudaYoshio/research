#pragma once
#ifndef ___Class_SimulatedAnnealing
#define ___Class_SimulatedAnnealing

#include <ppl.h>

#include "math.h"
#include "HandCursor.h"
#include "SubWindow.h"

class SimulatedAnnealing {
private:
	HandCursor* hc;

	unordered_map<long long int, SubWindow>* sub_windows;

	static constexpr int MAX_ITERATION{ 1000 };
	static constexpr double sigma{ 150 };
	static constexpr double k{ 1000000 };
	static constexpr double b{ 1 / (2 * sigma*sigma) };
	static constexpr double alpha{ 1 };
	static constexpr double beta{ 1555200 };
	static constexpr double gamma{ 10 };

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_parameter; // 変化させるパラメータを選択する乱数
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数を返す乱数

	static array<double, MAX_ITERATION> kT;
	static const double a;

	double current_cost, next_cost, best_cost, area_cost, overlap_cost, distance_cost;
	int modify_window_num, modify_param;
	std::unordered_map<long long int, ofRectangle> current_state, next_state;

	//int file_index = 1;
	//ofstream ofs;

	void set_next_state();
	double minimum_distance(const ofRectangle& r, double x, double y) const;
public:
	void setup(HandCursor* hc, unordered_map<long long int, SubWindow>* sub_windows);
	void calculate_cost(const unordered_map<long long int, ofRectangle>& state, double& cost, const unordered_map<long long int, SubWindow>& sub_windows);
	void operator() (const unordered_map<long long int, ofRectangle>& initial_state, unordered_map<long long int, ofRectangle>& best_state, double& best_cost);
};

#endif