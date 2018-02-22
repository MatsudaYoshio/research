#include "GeneticAlgorithmApp.h"

void GeneticAlgorithmApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

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