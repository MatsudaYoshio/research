#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include "Mole.h"

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 6 }; // 各行におけるモグラの数
	static constexpr int mole_num_col{ 8 }; // 各列におけるモグラの数
	static constexpr int mole_num_all{ mole_num_row * mole_num_col };
	static constexpr int mole_margin_row{ 400 }; // 各行におけるモグラ間の余白距離
	static constexpr int mole_margin_col{ 40 }; // 各列におけるモグラ間の余白距離
	static constexpr int information_view_size{ 200 }; // 情報ビューの大きさ(画面上部からの長さ)
	static const ofColor bg_color; // 背景の色
	
	array<Mole, mole_num_all> moles;

	void initialize_image() const;
	void initialize_moles();
public:
	void setup();
	void update();
	void draw();
};

#endif