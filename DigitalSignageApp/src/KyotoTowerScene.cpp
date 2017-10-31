#include "KyotoTowerScene.h"

using namespace param;

void KyotoTowerScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/kyoto_tower.jpg");
}

void KyotoTowerScene::update() {

}

void KyotoTowerScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"京都のランドマーク「京都タワー」", 50, H / 4 + 50);

	this->sentence_font.drawString(L"京都の玄関口に佇み、住人、観光客を温かく迎える京都タワー。", 50, H / 4 + 100);
}