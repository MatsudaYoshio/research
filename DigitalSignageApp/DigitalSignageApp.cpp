#include "DigitalSignageApp.h"
#include "MainScene.h"
#include "DetailScene.h"

void DigitalSignageApp::setup() {
	ofSetFrameRate(60);
	// ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���
	this->sm.setup(&this->hp); // �V�[���̏���
}

void DigitalSignageApp::update() {
	this->hp.update(); // ��|�C���^�̍X�V
	this->sm.update(); // �V�[���̍X�V
}

void DigitalSignageApp::draw() {
	ofSetColor(255, 255, 255);
	
	this->sm.draw(); // �V�[���̕`��
	
	/* ��|�C���^�̕`�� */
	for (auto t : this->hp.track_data) {
		ofSetColor(t.second.pointer_color);
		ofCircle(t.second.current_pointer.x, t.second.current_pointer.y, 10);
	}

}

void DigitalSignageApp::exit() {
	this->hp.exit();
}