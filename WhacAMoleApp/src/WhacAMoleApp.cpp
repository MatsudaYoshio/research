#include "WhacAMoleApp.h"

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // �t���[���X�V���ɓh��Ԃ��𖳌���
	ofHideCursor(); // �J�[�\����\��
}

void WhacAMoleApp::update() {
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // �w�i�̕`��

	this->hammer_image.draw(mouseX, mouseY);
}