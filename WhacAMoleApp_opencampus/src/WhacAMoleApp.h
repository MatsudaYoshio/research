#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include "SkeltonEstimator.h"
#include "KeyPointsRenderer.h"
#include "Mole.h"
#include "Timer.h"

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 4 }; // 各行におけるモグラの数
	static constexpr int mole_num_col{ 4 }; // 各列におけるモグラの数
	static constexpr int mole_num_all{ mole_num_row * mole_num_col }; // モグラの総数
	static constexpr double mole_margin_row{ 250 }; // 各行におけるモグラ間の余白距離
	static constexpr double mole_margin_col{ 40 }; // 各列におけるモグラ間の余白距離
	static constexpr double information_view_size{ 200 }; // 情報ビューの大きさ(画面上部からの長さ)
	static constexpr int bang_time{ 3 };
	static constexpr int result_time{ 13 };
	static const ofColor bg_color; // 背景の色
	static const double mole_width; // 各モグラの幅
	static const double mole_height; // 各モグラの高さ
	static const string top_score_path;

	/* 乱数 */
	static random_device rd;
	static mt19937 mt;
	static uniform_int_distribution<int> random_0or1; // 0か1を返す乱数
	static uniform_int_distribution<int> random_appearance_time; // モグラの出現時間を決めるための乱数
	static uniform_int_distribution<int> random_hidden_time; // モグラの隠れ時間を決めるための乱数

	const enum class STATE { PLAY, GAMEOVER };

	SkeltonEstimator se;
	KeyPointsRenderer kpr;
	Timer overall_timer, gameover_timer;

	STATE state{ STATE::PLAY };

	ofTrueTypeFont small_font, big_font;
	ofPoint bang_position;
	array<Mole, mole_num_all> moles;
	long long int playing_score{ 0 }, result_score;
	long long int playing_star{ 0 }, result_star;
	long long int top_score;
	ofstream ofs;

	void setup();
	void update();
	void draw();
	void exit();

	void initialize_image() const;
	void initialize_moles();
	bool is_whacked(const Mole& mole, const ofPoint& key_point) const; // モグラを叩いているかどうかを判定する関数
	void gameover(pair<int, int>& position);
	void reset_game();
};

#endif