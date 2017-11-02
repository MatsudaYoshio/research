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
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"西本願寺", 50, H / 4 + 50);

	this->sentence_font.drawString(L"堀川通に面して広がる浄土真宗本願寺派本山。", 50, H / 4 + 100);
}