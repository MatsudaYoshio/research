#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"
#include "ProgressBar.h"

class Icon {
private:
	static constexpr double rotate_speed = 10.0;
	static constexpr int resize_speed = 4;

	ofRectangle rect; // アイコンの矩形
	int default_w, default_h;
	ofTexture texture; // アイコンに表示するテクスチャ
	int alpha;
	int selected_user_id; // アイコンを選択したユーザのid
	ProgressBar pb; // プログレスバー
	double angle;
public:
	ofEvent<int> transition_event;
	int state; // アイコンの状態
	static const enum class STATE {
		INACTIVE, POINT, FADEOUT, FADEIN
	};

	Icon(const int &x, const int &y, const int &width, const int &height, const string &img_path);
	void setup(const int &x, const int &y, const int &width, const int &height, const string &img_path);
	void update();
	void draw();

	bool is_inside(const ofPoint &p) const;
	void change_state(const STATE& state);
	void set_user_id(const int &user_id);
};

#endif