#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "GridForm.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 10; // 集団サイズ
	static constexpr double crossover_probablity = 0.8; // 交叉確率(交叉が発生する確率)
	static constexpr int crossover_pair_number = population_size*0.4; // 交叉を適応するペア数

	/* 乱数の準備 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0か1
	static std::uniform_int_distribution<int> random_indivisual; // 集団から個体を選ぶ乱数
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数

	vector<GridForm> population; // 集団
	pair<int, int> crossover_pair_index; // 交叉するペアのインデックス

	void initialize(); // 初期化
	void crossover(); // 交叉
public:
	GeneticAlgorithm();
	void operator()(GridForm& best_individual);
};

#endif