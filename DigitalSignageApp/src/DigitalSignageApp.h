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
	param::genome_type g;
	int selected_user_num; // 選択操作をしているユーザ数(サブウィンドウを出しているユーザ数)
	array<unordered_set<int>, param::BITS_SIZE> user_assignment; // 格子矩形(ビット)に対するユーザの割り当て
public:
	void setup();
	void update();
	void draw();
	void exit();

	void pointed(pair<int, int> &id);
	void make_sub_window(pair<int, int>& id);
};

#endif