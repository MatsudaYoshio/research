#include "HigashihonganjiScene.h"

using namespace param;

void HigashihonganjiScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/higashihonganji.jpg");
}

void HigashihonganjiScene::update() {

}

void HigashihonganjiScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"���{�莛�i�^�@�{�_�j", 30, H / 4 + 50);

	//this->sentence_font.drawString(L"���J��E�厞�ԁF5:50~17:30(3��~10��)�A6:20~16:30(11��~2��)\n\n���Z���F���s�s������G�ےʎ������\n\n��TEL�F075-371-9181", 50, H / 4 + 100);
	this->sentence_font.drawString(L"���J��E�厞�ԁF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"5:50~17:30(3��~10��)�A6:20~16:30(11��~2��)", 280, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n���Z���F", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n���s�s������G�ےʎ�������t��754", 150, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n��TEL�F", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9181", 150, H / 4 + 100);
}