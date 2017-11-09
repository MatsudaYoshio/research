#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "AppParameters.h"
#include "HandCursor.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 20; // 集団サイズ(選択淘汰された後は必ずこの数)
	static constexpr double crossover_probability = 0.8; // 交叉確率(交叉が発生する確率)
	static constexpr int crossover_pair_number = 8; // 交叉を適応するペア数
	static constexpr int mutation_probability = 0.01; // 突然変異率(突然変異が発生する確率)

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0か1
	static std::uniform_int_distribution<int> random_indivisual; // 集団から個体を選ぶ乱数
	static std::uniform_int_distribution<int> random_crossover_point; // 交叉点を選ぶ乱数
	static std::uniform_int_distribution<int> random_bit;
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数

	HandCursor* hc; // 手カーソル
	int* selected_user_num; // 選択操作をしているユーザ数(サブウィンドウを出しているユーザ数)
	array<unordered_set<int>, param::BITS_SIZE> user_assignment; // 格子矩形(ビット)に対するユーザの割り当て

	array<array<int, param::FORM_H>, param::FORM_W> grid2bit_table; // 座標からビットへの変換表
	array<pair<int, int>, param::BITS_SIZE> bit2grid_table; // ビットから座標への変換表
	array<array<ofRectangle, param::FORM_H>, param::FORM_W> grid_rects; // 格子矩形
	vector<param::genome_type> initial_individuals; // 初期集団の個体候補

	vector<param::genome_type> population; // 集団
	vector<double> fitness; // 適応度

	/* 初期化 */
	void initialize();
	void initialize(const param::genome_type& initial_individual);
	
	void crossover(); // 交叉
	void mutation(); // 突然変異
	void calculate_fitness(); // 適応度の計算
	void selection(); // 選択淘汰

	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const; // ユークリッド距離
public:
	void setup(HandCursor* hc, int* selected_user_num);
	void draw_rectangles(const param::genome_type& g) const;
	void operator()(param::genome_type& best_individual, const array<unordered_set<int>, param::BITS_SIZE>& user_assignment);
	void operator()(const param::genome_type& initial_individual, param::genome_type& best_individual, const array<unordered_set<int>, param::BITS_SIZE>& user_assignment);
};

#endif