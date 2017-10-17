#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"

class Icon {
private:
	ofRectangle rect; // アイコンの矩形
	ofPoint tl, tr, bl, br; // 左上、右上、左下、右下の4点
	ofTexture texture; // アイコンに表示するテクスチャ
	static constexpr double frame_ratio = 0.2; // アイコンの矩形に対する枠の比率
	int frame_r, frame_g, frame_b;
	int alpha;
	string state; // アイコンの状態
	int selected_user_id; // アイコンを選択したユーザのid
public:
	ofEvent<int> transition_event;

	Icon(const int &x, const int &y, const int &width, const int &height, const string &img_path);
	void setup(const int &x, const int &y, const int &width, const int &height, const string &img_path);
	void update();
	void draw();

	bool is_inside(const ofPoint &p) const;
	void change_state(const string &state);
	void set_user_id(const int &user_id);
};

#endif