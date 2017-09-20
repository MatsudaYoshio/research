#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"

class Icon {
private:
	ofRectangle rect;
	ofPoint tl, tr, bl, br; // 左上、右上、左下、右下の4点
	ofTexture texture;
	static constexpr double frame_ratio = 0.2;
	int frame_r, frame_g, frame_b;
	int alpha;
	string state;
	int pointer_id;
public:
	int track_id;

	Icon(const int x, const int y, const int width, const int height, const string img_path);
	void setup(const int x, const int y, const int width, const int height, const string img_path);
	void update();
	void draw();
	bool is_inside(const ofPoint &p) const;
	ofEvent<int> transition_event;
	void change_state(string state);
	void set_pointer_id(int pointer_id);
};

#endif