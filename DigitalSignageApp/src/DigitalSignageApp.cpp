#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	//ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

	//this->sm.setup(&this->hc);
	this->optf.setup(1920, 1080, 24, 9);
}

void DigitalSignageApp::update() {
	//this->hc.update(); // ��J�[�\���̍X�V
	//this->sm.update();
	this->optf.update(ofPoint(mouseX, mouseY));
}

void DigitalSignageApp::draw() {
	//ofBackground(255);
	
	//this->sm.draw();
	this->optf.draw();
}

void DigitalSignageApp::exit() {
	//this->hc.exit();
}