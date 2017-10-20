#pragma once
#ifndef ___Class_MainScene
#define ___Class_MainScene

#include "ofMain.h"
#include "BaseScene.h"
#include "DrawBackground.h"

class MainScene : public BaseScene {
private:
	HandCursor* hc;
	DrawBackground db;
public:
	ofImage img;

	void setup(HandCursor *hc);
	void update();
	void draw();

	void transition(int &pointer_id);
	void select_icon(const string &icon_id, const int &user_id);
};

#endif