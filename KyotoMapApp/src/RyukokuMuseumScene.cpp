#include "RyukokuMuseumScene.h"

using namespace param;

void RyukokuMuseumScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 80);
	this->sentence_font.loadFont("meiryob.ttc", 50);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/ryukoku_museum.jpg");
}

void RyukokuMuseumScene::update() {

}

void RyukokuMuseumScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"���J�~���[�W�A��", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"���J�َ��ԁF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"10:00~17:00(���T���j���͋x��)", 430, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n���Z���F", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n���s�s������x��ʐ��ʉ���", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n��TEL�F", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-351-2500", 300, HALF_MAX_SUB_WINDOW_H + 200);
}