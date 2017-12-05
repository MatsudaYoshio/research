#include "SyoseienScene.h"

using namespace param;

void SyoseienScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/syoseien_img.jpg");
}

void SyoseienScene::update() {

}

void SyoseienScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, DISPLAY_W / 2, DISPLAY_H / 4);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"����(�k�k�@)", 50, DISPLAY_H / 4 + 50);

	this->sentence_font.drawString(L"���q�ρE�J�َ��ԁF", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"9:00~17:00", 280, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n���Z���F", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n���s�s�����扺�쐔�����ʊԔV���������ʐ���", 150, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n��TEL�F", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9210", 150, DISPLAY_H / 4 + 100);
}