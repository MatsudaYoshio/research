#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include <random>

#include "SkeltonEstimator.h"
#include "KeyPointsRenderer.h"
#include "Mole.h"

/* OpenPoseの骨格点参照のための定義(BODY25モデル) */
#define NOSE 0
#define NECK 1
#define RIGHT_SHOULDER 2
#define RIGHT_ELBOW 3
#define RIGHT_WRIST 4
#define LEFT_SHOULDER 5
#define LEFT_ELBOW 6
#define LEFT_WRIST 7
#define MIDDLE_HIP 8
#define RIGHT_HIP 9
#define RIGHT_KNEE 10
#define RIGHT_ANKLE 11
#define LEFT_HIP 12
#define LEFT_KNEE 13
#define LEFT_ANKLE 14
#define RIGHT_EYE 15
#define LEFT_EYE 16
#define RIGHT_EAR 17
#define LEFT_EAR 18
#define LEFT_BIG_TOE 19
#define LEFT_SMALL_TOE 20
#define LEFT_HEEL 21
#define RIGHT_BIG_TOE 22
#define RIGHT_SMALL_TOE 23
#define RIGHT_HEEL 24

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 6 }; // 各行におけるモグラの数
	static constexpr int mole_num_col{ 8 }; // 各列におけるモグラの数
	static constexpr int mole_num_all{ mole_num_row * mole_num_col };
	static constexpr double mole_margin_row{ 400 }; // 各行におけるモグラ間の余白距離
	static constexpr double mole_margin_col{ 40 }; // 各列におけるモグラ間の余白距離
	static constexpr double information_view_size{ 200 }; // 情報ビューの大きさ(画面上部からの長さ)
	static const ofColor bg_color; // 背景の色
	static const ofPoint font_draw_point; // フォントを描画する座標(基準)

	/* 乱数 */
	static random_device rd;
	static mt19937 mt;
	static uniform_int_distribution<int> random_0or1; // 0か1を返す乱数
	static uniform_int_distribution<int> random_appearance_time; // モグラの出現時間を決めるための乱数
	static uniform_int_distribution<int> random_hidden_time; // モグラの隠れ時間を決めるための乱数

	SkeltonEstimator se;
	KeyPointsRenderer kpr;

	long long int score{ 0 };
	ofTrueTypeFont font;
	array<Mole, mole_num_all> moles;

	void initialize_image() const;
	void initialize_moles();
	bool is_whacked(const Mole& mole, const ofPoint& key_point) const; // モグラを叩いているかどうかを判定する関数
public:
	void setup();
	void update();
	void draw();
	void exit();
};

#endif