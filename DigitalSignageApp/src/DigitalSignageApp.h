#pragma once
#ifndef ___Class_DigitalSignageApp
#define ___Class_DigitalSignageApp

#include <unordered_set>
#include "ofMain.h"
#include "HandCursor.h"
#include "MainScene.h"
#include "GeneticAlgorithm.h"

class DigitalSignageApp : public ofBaseApp {
private:
	HandCursor hc;
	MainScene ms;
	GeneticAlgorithm ga;
	set<int> selected_users; // �I�𑀍�����Ă��郆�[�U
public:
	void setup();
	void update();
	void draw();
	void exit();

	void pointed(pair<int, int> &id);
	void make_sub_window(pair<int, int>& id);
};

#endif