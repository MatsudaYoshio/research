#pragma once
#ifndef ___Class_DigitalSignageApp
#define ___Class_DigitalSignageApp

#include "ofMain.h"
#include "HandCursor.h"
#include "SceneManager.h"
#include "OptimizedForm.h"

class DigitalSignageApp : public ofBaseApp {
	void setup();
	void update();
	void draw();
	void exit();
private:
	HandCursor hc;
	SceneManager sm;
	OptimizedForm optf;
};

#endif