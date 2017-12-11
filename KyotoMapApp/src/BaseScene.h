#pragma once
#ifndef ___Class_BaseScene
#define ___Class_BaseScene

#include "ofMain.h"
#include "../../../addons/ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h"
#include "AppParameters.h"

class BaseScene {
public:
	virtual void setup() {};
	virtual void update() {};
	virtual void draw() {};

	ofImage img;
	ofxTrueTypeFontUC title_font, sentence_font;
	ofTexture star_red, star_orange, star_brown;
	ofRectangle stars_pos[3];
};

#endif