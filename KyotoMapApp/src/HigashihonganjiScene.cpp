#include "HigashihonganjiScene.h"

using namespace param;

void HigashihonganjiScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 80);
	this->sentence_font.loadFont("meiryob.ttc", 50);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/higashihonganji.jpg");
}

void HigashihonganjiScene::update() {

}

void HigashihonganjiScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"���{�莛�i�^�@�{�_�j", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"���J��E�厞�ԁF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	//this->sentence_font.drawString(L"5:50~17:30(3��~10��)�A6:20~16:30(11��~2��)", 640, HALF_MAX_SUB_WINDOW_H + 200);
	this->sentence_font.drawString(L"6:20~16:30(11��~2��)", 640, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n���Z���F", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n���s�s������G�ےʎ�������t��754", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n��TEL�F", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9181", 300, HALF_MAX_SUB_WINDOW_H + 200);
}