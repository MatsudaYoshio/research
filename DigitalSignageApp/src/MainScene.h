#pragma once
#ifndef ___Class_MainScene
#define ___Class_MainScene

#include "ofMain.h"
#include "BaseScene.h"

class MainScene : public BaseScene {
private:
	HandPointer* hp;
public:
	ofImage img;

	void setup(HandPointer *hp);
	void update();
	void draw();
};

#endif