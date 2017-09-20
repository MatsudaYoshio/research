#pragma once
#ifndef ___Class_DetailScene
#define ___Class_DetailScene

#include "ofMain.h"
#include "BaseScene.h"
#include "../../../addons/ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h"

class DetailScene : public BaseScene {
private:
	HandPointer* hp;
	ofxTrueTypeFontUC font;
public:
	void setup(HandPointer *hp);
	void update();
	void draw();
};

#endif