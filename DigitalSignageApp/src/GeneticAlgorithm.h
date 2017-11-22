#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include "AppParameters.h"
#include "HandCursor.h"

class GeneticAlgorithm {
private:
	static constexpr int population_size = 20; // 集団サイズ(選択淘汰された後は必ずこの数)
	static constexpr double crossover_probability = 0; // 交叉確率(交叉が発生する確率)
	static constexpr int crossover_pair_number = 8; // 交叉を適応するペア数
	static constexpr double mutation_probability = 0.2; // 突然変異率(突然変異が発生する確率)
	/* 近傍探索用の方向ベクトル(インデックスを0～3まで使えば4近傍、全部使えば8近傍) */
	static constexpr int dx[] = { 1, 0, -1, 0, 1, -1, -1, 1 };
	static constexpr int dy[] = { 0, -1, 0, 1, -1, -1, 1, 1 };

	/* 乱数 */
	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<int> random_0or1; // 0か1を返す乱数
	static std::uniform_int_distribution<int> random_indivisual; // 集団から個体を選ぶ乱数
	static std::uniform_int_distribution<int> random_crossover_method; // 交叉手法を選ぶ乱数
	static std::uniform_int_distribution<int> random_mutation_method; // 突然変異手法を選ぶ乱数
	static std::uniform_int_distribution<int> random_block;
	static std::uniform_real_distribution<double> random_0to1; // 0から1の間の実数を返す乱数

	HandCursor* hc; // 手カーソル

	array<array<ofRectangle, param::FORM_H>, param::FORM_W> grid_rects; // 格子矩形
	vector<param::genome_type> initial_individuals; // 初期集団の個体候補
	param::genome_type base_individual;

	vector<param::genome_type> population; // 集団
	vector<double> fitness; // 適応度
	param::genome_type elite_individual; // エリート個体

	/* 初期化 */
	void initialize(const set<int>& users_id);
	void initialize(const param::genome_type& initial_individual);
	
	void crossover(); // 交叉
	void mutation(); // 突然変異
	void calculate_fitness(); // 適応度の計算
	void selection(); // 選択淘汰

	int block_bits_size; // 各ブロックのビット数
	int genetic_length; // 遺伝子長(総ビット数)
	array<array<int, param::FORM_H>, param::FORM_W> grid2block_table; // 座標からブロックへの変換表
	array<pair<int, int>, param::BLOCK_SIZE> block2grid_table; // ブロックから座標への変換表 
	vector<array<int, param::BLOCK_SIZE>> block_assignments; // 各ブロックに対するユーザの割り当て
	set<int> users_id; // ユーザIDリスト
	int users_num; // ユーザ数
	unordered_map<int, int> user_id_index; // ユーザIDに対するインデックス
	int population_size_tmp; // 交叉や突然変異によって増加した一時的な集団サイズ
	unordered_map<int, set<int>> user_block; // 各ユーザがもつブロック

	int elite_index;
	array<int, param::BLOCK_SIZE> elite_block_assignment;

	double euclid_distance(const double &x1, const double &y1, const double &x2, const double &y2) const; // ユークリッド距離
public:
	static constexpr int NOT_USER = INT_MIN;

	void setup(HandCursor* hc);
	void draw() const;
	void draw(const array<int, param::BLOCK_SIZE>& block_assignment) const;
	void operator()(const set<int>& users_id);

	
};

#endif