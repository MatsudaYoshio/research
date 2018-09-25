#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include <random>

#include "HandCursor.h"
#include "Mole.h"

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 6 }; // 各行におけるモグラの数
	static constexpr int mole_num_col{ 8 }; // 各列におけるモグラの数
	static constexpr int mole_num_all{ mole_num_row * mole_num_col };
	static constexpr double mole_margin_row{ 400 }; // 各行におけるモグラ間の余白距離
	static constexpr double mole_margin_col{ 40 }; // 各列におけるモグラ間の余白距離
	static constexpr double information_view_size{ 200 }; // 情報ビューの大きさ(画面上部からの長さ)
	static constexpr double hammer_width{ 200 }; // ハンマー(画像)の幅
	static constexpr double hammer_height{ 200 }; // ハンマー(画像)の高さ
	static const ofColor bg_color; // 背景の色
	static const ofPoint font_draw_point; // フォントを描画する座標(基準)

	/* 乱数 */
	static random_device rd;
	static mt19937 mt;
	static uniform_int_distribution<int> random_0or1; // 0か1を返す乱数
	static uniform_int_distribution<int> random_appearance_time; // モグラの出現時間を決めるための乱数
	static uniform_int_distribution<int> random_hidden_time; // モグラの隠れ時間を決めるための乱数

	HandCursor hc;

	long long int score{ 0 };
	ofTrueTypeFont font;
	array<Mole, mole_num_all> moles;
	unordered_map<long long int, ofRectangle> hammer_rects;

	void initialize_image() const;
	void initialize_moles();
	bool is_whacked(const Mole& mole, const ofRectangle& hammer_rect) const; // モグラを叩いているかどうかを判定する関数
public:
	void setup();
	void update();
	void draw();
	void exit();
};

#endif