#pragma once
#ifndef ___Class_DigitalSignageApp
#define ___Class_DigitalSignageApp

#include "ofMain.h"
#include "HandPointer.h"
#include "Icon.h"
#include "BaseScene.h"
#include "SceneManager.h"

class DigitalSignageApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void exit();

	HandPointer hp;
	SceneManager sm;
};

#endif