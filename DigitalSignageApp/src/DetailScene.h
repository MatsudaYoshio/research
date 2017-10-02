#pragma once
#ifndef ___Class_DetailScene
#define ___Class_DetailScene

#include "ofMain.h"
#include "BaseScene.h"
#include "../../../addons/ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h"

class DetailScene : public BaseScene {
private:
	HandCursor* hc;
	ofxTrueTypeFontUC font;
public:
	void setup(HandCursor *hc);
	void update();
	void draw();
};

#endif