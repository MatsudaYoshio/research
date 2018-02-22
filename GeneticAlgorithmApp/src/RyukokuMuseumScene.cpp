#include "RyukokuMuseumScene.h"

using namespace param;

void RyukokuMuseumScene::setup() {
	switch (LANGUAGE) {
	case static_cast<int>(LANGUAGE_ID::JAPANENE) :
		this->title_font.loadFont("meiryob.ttc", 80);
		this->sentence_font.loadFont("meiryob.ttc", 50);

		ofLoadImage(this->earth_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/earth.png");

		break;
	case static_cast<int>(LANGUAGE_ID::ENGLISH) :
		this->title_font.loadFont("arialbd.ttf", 80);
		this->sentence_font.loadFont("arialbd.ttf", 50);

		ofLoadImage(this->money_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/money.jpeg");

		break;
	}

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/ryukoku_museum.jpg");

	ofLoadImage(this->clock_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/tokei.png");
	ofLoadImage(this->phone_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/denwa.png");
}

void RyukokuMuseumScene::update() {

}

void RyukokuMuseumScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	switch (LANGUAGE) {
	case static_cast<int>(LANGUAGE_ID::JAPANENE) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"—´’Jƒ~ƒ…[ƒWƒAƒ€", 50, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"ŠJŠÙŽžŠÔF", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"10:00~17:00(–ˆTŒŽ—j“ú‚Í‹xŠÙ)", 470, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::blue);
		this->earth_texture.draw(this->earth_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nZŠF", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ–xì’Ê³–Ê‰º‚é", 330, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\n“d˜b”Ô†F", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-351-2500", 470, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	case static_cast<int>(LANGUAGE_ID::ENGLISH) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"Ryukoku", 30, HALF_MAX_SUB_WINDOW_H + 100);
		this->title_font.drawString(L"Museum", 530, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"Visiting", 130, HALF_MAX_SUB_WINDOW_H + 200);
		this->sentence_font.drawString(L"Hours:", 400, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"10:00AM-17:00PM", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->money_texture.draw(this->money_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nEntrance", 130, HALF_MAX_SUB_WINDOW_H + 200);
		this->sentence_font.drawString(L"\n\nFees:", 440, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n500yen(General)", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\nTEL:", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-351-2500", 290, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	}



	
}