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
	int selected_user_num; // �I�𑀍�����Ă��郆�[�U��(�T�u�E�B���h�E���o���Ă��郆�[�U��)
	array<unordered_set<int>, param::BITS_SIZE> user_assignment; // �i�q��`(�r�b�g)�ɑ΂��郆�[�U�̊��蓖��
public:
	void setup();
	void update();
	void draw();
	void exit();

	void pointed(pair<int, int> &id);
	void make_sub_window(pair<int, int>& id);
};

#endif