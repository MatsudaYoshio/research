#include "KyotoTowerScene.h"

using namespace param;

void KyotoTowerScene::setup() {
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

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/kyoto_tower.jpg");

	ofLoadImage(this->clock_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/tokei.png");
	ofLoadImage(this->phone_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/denwa.png");
}

void KyotoTowerScene::update() {

}

void KyotoTowerScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	switch (LANGUAGE) {
	case static_cast<int>(LANGUAGE_ID::JAPANENE) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"‹ž“sƒ^ƒ[", 50, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"‰c‹ÆŽžŠÔF", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"9:00~21:00", 470, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::blue);
		this->earth_texture.draw(this->earth_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nZŠF", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ‰GŠÛ’ÊŽµð‰º‚é“Œ‰–¬˜H’¬721-1", 330, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\n“d˜b”Ô†F", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-361-3215", 470, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	case static_cast<int>(LANGUAGE_ID::ENGLISH) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"Kyoto", 30, HALF_MAX_SUB_WINDOW_H + 100);
		this->title_font.drawString(L"Tower", 360, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"Visiting", 130, HALF_MAX_SUB_WINDOW_H + 200);
		this->sentence_font.drawString(L"Hours:", 400, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"9:00AM-21:00PM", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->money_texture.draw(this->money_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nEntrance", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nFees:", 440, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n770yen(Adults)", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\nTEL:", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-361-3215", 290, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	}

	
}