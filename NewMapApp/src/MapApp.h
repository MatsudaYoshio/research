#pragma once
#ifndef ___Class_MapApp
#define ___Class_MapApp

#include "ofMain.h"
#include "AppParameters.h"
#include "SceneManager.h"

class MapApp : public ofBaseApp {
private:
	SceneManager sm;
public:
	void setup();
	void update();
	void draw();
};

#endif
