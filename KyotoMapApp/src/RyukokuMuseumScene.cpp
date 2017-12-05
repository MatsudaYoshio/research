#include "RyukokuMuseumScene.h"

using namespace param;

void RyukokuMuseumScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/ryukoku_museum.jpg");
}

void RyukokuMuseumScene::update() {

}

void RyukokuMuseumScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, DISPLAY_W / 2, DISPLAY_H / 4);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"���J�~���[�W�A��", 50, DISPLAY_H / 4 + 50);

	this->sentence_font.drawString(L"���J�َ��ԁF", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"10:00~17:00(���T���j���͋x��)", 200, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n���Z���F", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n���s�s������x��ʐ��ʉ���", 150, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n��TEL�F", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-351-2500", 150, DISPLAY_H / 4 + 100);
}