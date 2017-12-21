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
	ofTexture clock_texture, earth_texture, phone_texture, money_texture;
	ofRectangle clock_rect = ofRectangle(40, 673, 80, 80), earth_rect = ofRectangle(30, 805, 100, 100), phone_rect = ofRectangle(40, 953, 80, 80), money_rect = ofRectangle(40, 805, 90, 90);

	virtual ~BaseScene() {};
};

#endif