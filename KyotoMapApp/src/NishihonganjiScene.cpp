#include "NishihonganjiScene.h"

using namespace param;

void NishihonganjiScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 80);
	this->sentence_font.loadFont("meiryob.ttc", 50);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/nishihonganji.jpg");
}

void NishihonganjiScene::update() {

}

void NishihonganjiScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"西本願寺", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"■開門・閉門時間：", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"5:50~17:00", 640, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n■住所：", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n京都市下京区門前町堀川通花屋町下る", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n■TEL：", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-5181", 300, HALF_MAX_SUB_WINDOW_H + 200);
}