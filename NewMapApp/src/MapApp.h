#pragma once
#ifndef ___Class_MapApp
#define ___Class_MapApp

#include "HandCursor.h"
#include "SceneManager.h"

class MapApp : public ofBaseApp {
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