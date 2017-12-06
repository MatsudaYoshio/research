#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

	this->sm.setup(&this->hc);
}

void DigitalSignageApp::update() {
	this->hc.update(); // ��J�[�\���̍X�V

	this->sm.update();
}

void DigitalSignageApp::draw() {
	this->sm.draw();
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}