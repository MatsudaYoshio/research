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

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"東本願寺（真宗本廟）", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"■開門・閉門時間：", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	//this->sentence_font.drawString(L"5:50~17:30(3月~10月)、6:20~16:30(11月~2月)", 640, HALF_MAX_SUB_WINDOW_H + 200);
	this->sentence_font.drawString(L"6:20~16:30(11月~2月)", 640, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n■住所：", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n京都市下京区烏丸通七条上る常葉町754", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n■TEL：", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9181", 300, HALF_MAX_SUB_WINDOW_H + 200);
}