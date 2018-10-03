#include "ofApp.h"

using namespace param;

void ofApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofSetCircleResolution(256); // �~�̐����x��ݒ�
}

void ofApp::update() {
	this->se.update();
}

void ofApp::draw() {
	/* ���i�̕`�� */
	this->kpr.draw(this->se.transformed_key_points);

	/* �S���̍��i�_���R�}���h���C���ɏo�� */
	//this->show_key_points();
}

void ofApp::exit() {
	this->se.exit(); // �A�v���I�����Ɏ�J�[�\���N���X��exit�֐����Ăяo��
}

void ofApp::show_key_points() const {
	for (const auto& p : this->se.transformed_key_points) {
		printf("user%d's key point [%d] -> (%f,%f)\n", p.first.first, p.first.second, p.second.x, p.second.y);
	}
}