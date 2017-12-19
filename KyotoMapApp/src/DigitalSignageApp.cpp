#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

	this->hc.new_thread_update();
	
	this->sm.setup(&this->hc);
}

void DigitalSignageApp::update() {
	//this->hc.update(); // ��J�[�\���̍X�V

	this->sm.update();
}

void DigitalSignageApp::draw() {
	this->sm.draw();
}

void DigitalSignageApp::exit() {
	this->stop_flag = true;
	this->hc.exit();
}