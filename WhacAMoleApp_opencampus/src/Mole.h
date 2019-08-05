// モグラクラス

#pragma once
#ifndef ___Class_Mole
#define ___Class_Mole

#include <random>

#include "ofMain.h"

#include "AppParameters.h"

class Mole {
private:
	static random_device rd;
	static mt19937 mt;
	static uniform_real_distribution<> random01; // 0~1を返す乱数
	static uniform_int_distribution<int> random_angry_time;

	static constexpr double star_probability{ 0.1 };
	static constexpr double angry_probability{ 0.1 };
	static constexpr int king_threshold{ 5 };
	static constexpr int normal_score{ 10 };
	static constexpr int star_score{ normal_score * 2 };
	static constexpr int king_score{ normal_score * 5 };
	const enum class STATE { HIDE, NORMAL, ANGRY, STAR, DAMEGE, KING };

	static long long int previous_star;

	int appearance_time{ 0 }; // 出現時間
	int hidden_time{ 0 }; // 隠れ時間
	int angry_time{ 0 }; // 怒り時間
	long long int score{ 0 }; // 溜まっているスコア
	ofRectangle draw_rect; // モグラ画像を描画する位置・大きさを保持する矩形
	STATE state{ STATE::HIDE };
public:
	static long long int current_star; // 溜まっているスター

	ofEvent<pair<int, int>> explosion_event;

	void setup(const ofRectangle& draw_rect, int hidden_time); // モグラを描画する位置・大きさに関する矩形と最初の隠れ時間を引数にセットアップ
	void update();
	void draw() const ;

	ofRectangle get_rect() const;
	int get_score();

	bool is_idle() const; // モグラの隠れ時間と出現時間がどっちもない状態かどうかを調べる（暇かどうか）
	bool is_appear() const; // モグラが出現しているかどうかを調べる

	void appear(int appearance_time); // モグラに対して指定した時間だけ出現時間を加算する
	void hide(int hidden_time); // モグラに対して指定した時間だけ隠れ時間を加算する
	void salvage();
};

#endif