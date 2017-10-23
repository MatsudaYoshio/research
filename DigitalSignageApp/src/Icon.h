#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"
#include "ProgressBar.h"

class Icon {
private:
	ofRectangle rect; // アイコンの矩形
	ofTexture texture; // アイコンに表示するテクスチャ
	int alpha;
	string state; // アイコンの状態
	int selected_user_id; // アイコンを選択したユーザのid
	ProgressBar pb; // プログレスバー
	double angle = 0.0, rotate_speed = 10.0;
	int resize_speed = 4;
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