#pragma once
#ifndef ___Class_ofApp
#define ___Class_ofApp

#include "ofMain.h"
#include "HandCursor.h"

class HandCursorApp : public ofBaseApp {
private:
	HandCursor hc;
public:
	void setup();
	void update();
	void draw();
	void exit();
};

#endif