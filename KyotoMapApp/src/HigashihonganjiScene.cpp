#include "HigashihonganjiScene.h"

using namespace param;

void HigashihonganjiScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/higashihonganji.jpg");
}

void HigashihonganjiScene::update() {

}

void HigashihonganjiScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // •¶ŽšF‚ðŽw’è
	this->title_font.drawString(L"“Œ–{ŠèŽ›i^@–{•_j", 30, H / 4 + 50);

	//this->sentence_font.drawString(L"¡ŠJ–åE•Â–åŽžŠÔF5:50~17:30(3ŒŽ~10ŒŽ)A6:20~16:30(11ŒŽ~2ŒŽ)\n\n¡ZŠF‹ž“sŽs‰º‹ž‹æ‰GŠÛ’ÊŽµðã‚é\n\n¡TELF075-371-9181", 50, H / 4 + 100);
	this->sentence_font.drawString(L"¡ŠJ–åE•Â–åŽžŠÔF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"5:50~17:30(3ŒŽ~10ŒŽ)A6:20~16:30(11ŒŽ~2ŒŽ)", 280, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n¡ZŠF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ‰GŠÛ’ÊŽµðã‚éí—t’¬754", 150, H / 4 + 100);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n¡TELF", 40, H / 4 + 100);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9181", 150, H / 4 + 100);
}