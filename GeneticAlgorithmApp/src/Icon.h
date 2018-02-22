#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"
#include "ProgressBar.h"

class Icon {
private:
	static constexpr double expansion_ratio{ 1.5 };

	int content_id;
	ofRectangle default_rect, big_rect, rect; // アイコンの矩形
	ofTexture texture; // アイコンに表示するテクスチャ
	int alpha;
	int selected_user_id; // アイコンを選択したユーザのid
	ProgressBar pb; // プログレスバー
	double angle;
public:
	ofEvent<pair<int, long long int>> select_event;
	int state; // アイコンの状態
	const enum class STATE {
		INACTIVE, POINT
	};

	void setup(int x, int y, int width, int height, const string& img_path, int content_id);
	void update();
	void draw();

	bool is_inside(const ofPoint &p) const;
	void change_state(const STATE& state);
	void set_user_id(long long int user_id);
	int get_content_id() const;
};

#endif