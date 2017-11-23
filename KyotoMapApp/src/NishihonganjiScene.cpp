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

	ofSetColor(ofColor::black); // ¶Fðwè
	this->title_font.drawString(L"¼{è", 50, H / 4 + 50);

	this->sentence_font.drawString(L"¡JåEÂåÔF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"5:50~17:00", 280, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n¡ZF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\nssºæåO¬xìÊÔ®¬ºé", 150, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n¡TELF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-5181", 150, H / 4 + 100);
}