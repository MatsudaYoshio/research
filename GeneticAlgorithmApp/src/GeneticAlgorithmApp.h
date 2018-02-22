#pragma once
#ifndef ___Class_GeneticAlgorithmApp
#define ___Class_GeneticAlgorithmApp

#include "ofMain.h"
#include "HandCursor.h"
#include "SceneManager.h"

class GeneticAlgorithmApp : public ofBaseApp {
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