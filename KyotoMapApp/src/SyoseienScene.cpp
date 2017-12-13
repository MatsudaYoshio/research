#include "SyoseienScene.h"

using namespace param;

void SyoseienScene::setup() {
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

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/syoseien_img.jpg");

	ofLoadImage(this->clock_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/tokei.png");
	ofLoadImage(this->phone_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/denwa.png");
}

void SyoseienScene::update() {

}

void SyoseienScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	switch (LANGUAGE) {
	case static_cast<int>(LANGUAGE_ID::JAPANENE) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"渉成園(枳殻邸)", 50, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"拝観・開館時間：", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"9:00~17:00", 660, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::blue);
		this->earth_texture.draw(this->earth_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n住所：", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n京都市下京区下珠数屋町通間之町東入東玉水町", 330, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\n電話番号：", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-371-9210", 470, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	case static_cast<int>(LANGUAGE_ID::ENGLISH) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"Shosei-en", 30, HALF_MAX_SUB_WINDOW_H + 100);
		this->title_font.drawString(L"Garden", 570, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"Visiting", 130, HALF_MAX_SUB_WINDOW_H + 200);
		this->sentence_font.drawString(L"Hours:", 400, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"9:00AM-17:00PM", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->money_texture.draw(this->money_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nEntrance", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nFees:", 440, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n500yen", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\nTEL:", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-371-9210", 290, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	}
}