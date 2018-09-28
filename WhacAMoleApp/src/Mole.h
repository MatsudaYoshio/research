// モグラクラス

#pragma once
#ifndef ___Class_Mole
#define ___Class_Mole

#include "ofMain.h"

#include "AppParameters.h"

class Mole {
private:
	int appearance_time{ 0 }; // 出現時間
	int hidden_time{ 0 }; // 隠れ時間
	ofRectangle draw_rect; // モグラ画像を描画する位置・大きさを保持する矩形
public:
	void setup(const ofRectangle& draw_rect, int hidden_time);
	void draw();

	ofRectangle get_rect() const;
	bool is_idle() const;
	bool is_appear() const;
	void appear(int appearance_time);
	void hide(int hidden_time);
};

#endif