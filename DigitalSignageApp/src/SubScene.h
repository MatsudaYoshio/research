#pragma once
#ifndef ___Class_SubScene
#define ___Class_SubScene

#include "ofMain.h"
#include "BaseScene.h"

class SubScene : public BaseScene {
private:
	HandPointer* hp;
public:
	void setup(HandPointer *hp);
	void update();
	void draw();
};

#endif