#pragma once
#ifndef ___Class_SceneManager
#define ___Class_SceneManager

#include "AppParameters.h"
#include "MenuBar.h"

class SceneManager {
private:
	MenuBar mb;
	const ofImage map_image{ "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_bg.png" };
public:
	void setup();
	void update();
	void draw();
};

#endif