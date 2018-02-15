#include "MapApp.h"

void MapApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

	this->sm.setup(&this->hc);
}

void MapApp::update() {
	this->hc.update();
	this->sm.update();
}

void MapApp::draw() {
	this->sm.draw();
}

void MapApp::exit() {
	this->hc.exit();
}