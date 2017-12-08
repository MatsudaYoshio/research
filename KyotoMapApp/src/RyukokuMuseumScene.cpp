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

	ofSetColor(ofColor::black); // •¶ŽšF‚ðŽw’è
	this->title_font.drawString(L"—´’Jƒ~ƒ…[ƒWƒAƒ€", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"¡ŠJŠÙŽžŠÔF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"10:00~17:00(–ˆTŒŽ—j“ú‚Í‹xŠÙ)", 430, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n¡ZŠF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ–xì’Ê³–Ê‰º‚é", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n¡TELF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-351-2500", 300, HALF_MAX_SUB_WINDOW_H + 200);
}