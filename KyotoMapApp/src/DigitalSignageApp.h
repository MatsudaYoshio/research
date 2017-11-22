#pragma once
#ifndef ___Class_DigitalSignageApp
#define ___Class_DigitalSignageApp

#include "ofMain.h"
#include "HandCursor.h"
#include "SceneManager.h"

class DigitalSignageApp : public ofBaseApp {
private:
	HandCursor hc;
	SceneManager sm;
public:
	void setup();
	void update();
	void draw();
	void exit();
};

#endif