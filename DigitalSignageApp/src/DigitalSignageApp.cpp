#include "DigitalSignageApp.h"
#include "MainScene.h"
#include "DetailScene.h"

void DigitalSignageApp::setup() {
	//ofSetFrameRate(60);
	// ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���
	this->sm.setup(&this->hp); // �V�[���̏���

	ofSetWindowTitle("main window");

	//this->secondWindow.setup("second window", 50, 50, 500, 500, false);
	//this->secondWindow.setup("second window", ofGetScreenWidth(), 0, 1280, 800, true);
}

void DigitalSignageApp::update() {
	this->hp.update(); // ��|�C���^�̍X�V
	this->sm.update(); // �V�[���̍X�V
}

void DigitalSignageApp::draw() {
	ofBackground(255);
	this->sm.draw(); // �V�[���̕`��
	
	//ofSetColor(0, 255, 0);
	//ofDrawBitmapString("this is the\nfirst window", 30, 80);
	//ofRect(20, 20, 50, 40);

	//this->secondWindow.begin();
	//this->sm.draw(); // �V�[���̕`��
	/*
	ofBackground(255);
	ofSetColor(0, 0, 255);
	ofDrawBitmapString("this is the\nsecond window", 30, 65);
	ofEllipse(20, 20, 60, 30);
	*/
	//this->secondWindow.end();

	//this->secondWindow.show();
	
}

void DigitalSignageApp::exit() {
	this->hp.exit();
}