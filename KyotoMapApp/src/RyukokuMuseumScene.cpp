#include "RyukokuMuseumScene.h"

using namespace param;

void RyukokuMuseumScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/ryukoku_museum.jpg");
}

void RyukokuMuseumScene::update() {

}

void RyukokuMuseumScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, DISPLAY_W / 2, DISPLAY_H / 4);

	ofSetColor(ofColor::black); // •¶ŽšF‚ðŽw’è
	this->title_font.drawString(L"—´’Jƒ~ƒ…[ƒWƒAƒ€", 50, DISPLAY_H / 4 + 50);

	this->sentence_font.drawString(L"¡ŠJŠÙŽžŠÔF", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"10:00~17:00(–ˆTŒŽ—j“ú‚Í‹xŠÙ)", 200, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n¡ZŠF", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ–xì’Ê³–Ê‰º‚é", 150, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n¡TELF", 40, DISPLAY_H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-351-2500", 150, DISPLAY_H / 4 + 100);
}