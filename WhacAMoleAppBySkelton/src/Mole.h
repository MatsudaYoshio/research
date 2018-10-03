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
	void setup(const ofRectangle& draw_rect, int hidden_time); // モグラを描画する位置・大きさに関する矩形と最初の隠れ時間を引数にセットアップ
	void draw();

	ofRectangle get_rect() const;
	bool is_idle() const; // モグラの隠れ時間と出現時間がどっちもない状態かどうかを調べる（暇かどうか）
	bool is_appear() const; // モグラが出現しているかどうかを調べる
	void appear(int appearance_time); // モグラに対して指定した時間だけ出現時間を加算する
	void hide(int hidden_time); // モグラに対して指定した時間だけ隠れ時間を加算する
};

#endif