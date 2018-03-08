#pragma once
#ifndef ___Class_GeneticAlgorithm
#define ___Class_GeneticAlgorithm

#include <random>
#include <ppl.h>
#include "AppParameters.h"
#include "HandCursor.h"
#include "TimerBase.h"

class GeneticAlgorithm {
private:
	using genome_type = vector<bool>;

	static constexpr int population_size{ 400 }; // 集団サイズ(選択淘汰された後は必ずこの数)
	static constexpr double crossover_probability{ 0 }; // 交叉確率(交叉が発生する確率)
	static constexpr int crossover_pair_number{ 8 }; // 交叉を適応するペア数
	static constexpr double mutation_probability{ 1.0 }; // 突然変異率(突然変異が発生する確率)
	static constexpr double fitness_multiple{ 2 * population_size / 125 + 0.4 }; // スケーリング時のパラメータ(最良の個体が次世代に残す個体数の期待値を表し、集団数が50～100ならば1.2～2.0ぐらいがいい)
	static constexpr int max_iteration{ 400 };

	/* 近傍探索用の方向ベクトル(インデックスを0～3まで使えば4近傍、全部使えば8近傍) */
	static constexpr int dx[] = { 1, -1, 0, 0, 1, -1, -1, 1 };
	static constexpr int dy[] = { 0, 0, -1, 1, -1, -1, 1, 1 };

	static constexpr int grid_w{ 480 };
	static constexpr int grid_h{ 216 };
	static constexpr int form_w{ param::DISPLAY_W / grid_w };
	static constexpr int form_h{ param::DISPLAY_H / grid_h };
	static constexpr int block_size{ form_w*form_h };

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

	array<array<ofRectangle, form_h>, form_w> grid_rects; // 格子矩形
	vector<genome_type> initial_individuals; // 初期集団の個体候補

	vector<genome_type> population; // 集団
	vector<genome_type> population_new; // 選択淘汰によって新たにできる集団
	vector<double> fitness; // 適応度
	genome_type elite_individual; // エリート個体(プログラム起動時から現在までの最適な個体)
	double elite_fitness{ DBL_MIN }; // エリート個体の適応度

	void initialize(); // 初期化
	void crossover(); // 交叉
	void mutation(); // 突然変異
	void calculate_fitness(); // 適応度の計算
	void scaling(); // 適応度のスケーリング
	void selection(); // 選択淘汰

	int block_bits_size; // 各ブロックのビット数
	int genetic_length; // 遺伝子長(総ビット数)
	array<array<int, form_h>, form_w> grid2block_table; // 座標からブロックへの変換表
	array<pair<int, int>, block_size> block2grid_table; // ブロックから座標への変換表 
	vector<array<int, block_size>> block_assignments; // 各ブロックに対するユーザの割り当て
	set<long long int> selected_users_id, all_users_id; // ユーザIDリスト
	int selected_users_num; // ユーザ数
	unordered_map<int, int> user_id2user_index; // ユーザIDからユーザインデックスへの変換表
	int population_size_tmp; // 交叉や突然変異によって増加した一時的な集団サイズ
	vector<unordered_map<long long int, set<int>>> user_block; // 各ユーザがもつブロック
	array<int, block_size> elite_block_assignment{}; // エリート個体のブロック割り当て
	double fitness_sum; // 現在の集団の適応度の総和
	int best_fitness_index; // 現在の集団での最適個体のインデックス
	vector<vector<bool>> user_bits; // ユーザインデックスに対するビット

	ofstream ofs, ofs2, ofs3;
	TimerBase tb;
public:
	static constexpr int NOT_USER{ 0 };

	void setup(HandCursor* hc);
	void draw() const;
	void operator()(set<long long int> selected_users_id, set<long long int> all_users_id);
};

#endif