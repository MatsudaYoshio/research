#include "GeneticAlgorithmApp.h"

void GeneticAlgorithmApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	this->sm.setup(&this->hc);
}

void GeneticAlgorithmApp::update() {
	this->hc.update();
	this->sm.update();
}

void GeneticAlgorithmApp::draw() {
	this->sm.draw();
}

void GeneticAlgorithmApp::exit() {
	this->hc.exit();
}