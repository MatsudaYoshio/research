#include "NishihonganjiScene.h"

using namespace param;

void NishihonganjiScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/nishihonganji.jpg");
}

void NishihonganjiScene::update() {

}

void NishihonganjiScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, HALF_DISPLAY_W - 2 * FRAME_SIZE, (HALF_DISPLAY_H >> 1) - 2 * FRAME_SIZE);

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"西本願寺", 50, DISPLAY_H / 4 + 50);

	this->sentence_font.drawString(L"■開門・閉門時間：", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"5:50~17:00", 280, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n■住所：", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n京都市下京区門前町堀川通花屋町下る", 150, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n■TEL：", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-5181", 150, DISPLAY_H / 4 + 100);
}