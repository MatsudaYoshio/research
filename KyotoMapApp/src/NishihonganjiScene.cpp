#include "NishihonganjiScene.h"

using namespace param;

void NishihonganjiScene::setup() {
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

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/nishihonganji.jpg");

	ofLoadImage(this->clock_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/tokei.png");
	ofLoadImage(this->phone_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/denwa.png");
}

void NishihonganjiScene::update() {

}

void NishihonganjiScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	switch (LANGUAGE) {
	case static_cast<int>(LANGUAGE_ID::JAPANENE) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"���{�莛", 50, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"�J��E�厞�ԁF", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"5:50~17:00", 660, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::blue);
		this->earth_texture.draw(this->earth_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n�Z���F", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n���s�s�������O���x��ʉԉ�������", 330, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\n�d�b�ԍ��F", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-371-5181", 470, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	case static_cast<int>(LANGUAGE_ID::ENGLISH) :
		ofSetColor(ofColor::black);
		this->title_font.drawString(L"Nishi-Hongwan-ji", 30, HALF_MAX_SUB_WINDOW_H + 100);
		this->title_font.drawString(L"Temple", 980, HALF_MAX_SUB_WINDOW_H + 100);

		ofSetColor(ofColor::white);
		this->clock_texture.draw(this->clock_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"Visiting", 130, HALF_MAX_SUB_WINDOW_H + 200);
		this->sentence_font.drawString(L"Hours:", 400, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"5:50AM-17:00PM", 630, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->money_texture.draw(this->money_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nEntrance", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\nFees:", 440, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\nNot", 630, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\nspecified", 780, HALF_MAX_SUB_WINDOW_H + 200);

		ofSetColor(ofColor::white);
		this->phone_texture.draw(this->phone_rect);
		ofSetColor(ofColor::black);
		this->sentence_font.drawString(L"\n\n\n\nTEL:", 130, HALF_MAX_SUB_WINDOW_H + 200);
		ofSetColor(ofColor::red);
		this->sentence_font.drawString(L"\n\n\n\n075-371-5181", 290, HALF_MAX_SUB_WINDOW_H + 200);

		break;
	}
}