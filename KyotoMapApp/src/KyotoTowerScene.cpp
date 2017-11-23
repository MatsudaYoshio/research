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

	ofSetColor(ofColor::black); // •¶ŽšF‚ðŽw’è
	this->title_font.drawString(L"‹ž“sƒ^ƒ[", 30, H / 4 + 50);

	//this->sentence_font.drawString(L"‰c‹ÆŽžŠÔF9:00~12:00\n\nZŠF‹ž“sŽs‰º‹ž‹æ‰GŠÛ’ÊŽµð‰º‚é“Œ‰–¬˜H’¬721-1\n\nTELF075-361-3215", 50, H / 4 + 100);
	this->sentence_font.drawString(L"¡‰c‹ÆŽžŠÔF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"9:00~21:00", 200, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n¡ZŠF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ‰GŠÛ’ÊŽµð‰º‚é“Œ‰–¬˜H’¬721-1", 150, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n¡TELF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-361-3215", 150, H / 4 + 100);
}