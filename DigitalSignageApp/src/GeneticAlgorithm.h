#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "GridForm.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 10; // 集団サイズ(選択淘汰された後は必ずこの数)
	static constexpr double crossover_probability = 0.8; // 交叉確率(交叉が発生する確率)
	static constexpr int crossover_pair_number = 4; // 交叉を適応するペア数
	static constexpr int mutation_probability = 0.01; // 突然変異率(突然変異が発生する確率)

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0か1
	static std::uniform_int_distribution<int> random_indivisual; // 集団から個体を選ぶ乱数
	static std::uniform_int_distribution<int> random_crossover_point; // 交叉点を選ぶ乱数
	static std::uniform_int_distribution<int> random_bit;
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数

	vector<GridForm> population; // 集団
	vector<double> fitness; // 適応度

	void initialize(); // 初期化
	void crossover(); // 交叉
	void mutation(); // 突然変異
	void calculate_fitness(); // 適応度の計算
	void selection(); // 選択淘汰
public:
	GeneticAlgorithm();
	void operator()(GridForm& best_individual);
};

#endif