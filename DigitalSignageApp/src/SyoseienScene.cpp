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

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"渉成園(枳殻邸)", 50, H / 4 + 50);

	this->sentence_font.drawString(L"「お東さん」と京都市民に親しまれている東本願寺の庭園。", 50, H / 4 + 100);
}