#include "SyoseienScene.h"

using namespace param;

void SyoseienScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 80);
	this->sentence_font.loadFont("meiryob.ttc", 50);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/syoseien_img.jpg");
}

void SyoseienScene::update() {

}

void SyoseienScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"����(�k�k�@)", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"���q�ρE�J�َ��ԁF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"9:00~17:00", 640, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n���Z���F", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n���s�s�����扺�쐔�����ʊԔV���������ʐ���", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n��TEL�F", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9210", 300, HALF_MAX_SUB_WINDOW_H + 200);
}