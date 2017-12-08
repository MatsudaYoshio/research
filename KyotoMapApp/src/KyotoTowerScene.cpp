#include "KyotoTowerScene.h"

using namespace param;

void KyotoTowerScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 80);
	this->sentence_font.loadFont("meiryob.ttc", 50);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/kyoto_tower.jpg");
}

void KyotoTowerScene::update() {

}

void KyotoTowerScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	ofSetColor(ofColor::black); // •¶ŽšF‚ðŽw’è
	this->title_font.drawString(L"‹ž“sƒ^ƒ[", 50, HALF_MAX_SUB_WINDOW_H + 100);

	this->sentence_font.drawString(L"¡‰c‹ÆŽžŠÔF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"9:00~21:00", 430, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n¡ZŠF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ‰GŠÛ’ÊŽµð‰º‚é“Œ‰–¬˜H’¬721-1", 300, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n¡TELF", 40, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-361-3215", 300, HALF_MAX_SUB_WINDOW_H + 200);
}