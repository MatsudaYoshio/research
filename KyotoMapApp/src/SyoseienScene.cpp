#include "SyoseienScene.h"

using namespace param;

void SyoseienScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 80);
	this->sentence_font.loadFont("meiryob.ttc", 50);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/syoseien_img.jpg");

	ofLoadImage(this->star_red, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/small_star8_red.png");
	ofLoadImage(this->star_orange, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/small_star6_orange.png");
	ofLoadImage(this->star_brown, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/small_star3_brown.png");

	this->stars_pos[0].set(30, 660, 100, 100);
	this->stars_pos[1].set(30, 800, 100, 100);
	this->stars_pos[2].set(30, 940, 100, 100);
}

void SyoseienScene::update() {

}

void SyoseienScene::draw() {
	ofSetColor(ofColor::white);
	this->img.draw(FRAME_SIZE, FRAME_SIZE, MAX_SUB_WINDOW_W - 2 * FRAME_SIZE, HALF_MAX_SUB_WINDOW_H - FRAME_SIZE);

	ofSetColor(ofColor::black);
	this->title_font.drawString(L"Â¬‰€(žkŠk“@)", 50, HALF_MAX_SUB_WINDOW_H + 100);

	ofSetColor(ofColor::white);
	this->star_red.draw(this->stars_pos[0]);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"”qŠÏEŠJŠÙŽžŠÔF", 130, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"9:00~17:00", 660, HALF_MAX_SUB_WINDOW_H + 200);

	ofSetColor(ofColor::white);
	this->star_orange.draw(this->stars_pos[1]);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\nZŠF", 130, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n‹ž“sŽs‰º‹ž‹æ‰ºŽì”‰®’¬’ÊŠÔ”V’¬“Œ“ü“Œ‹Ê…’¬", 330, HALF_MAX_SUB_WINDOW_H + 200);

	ofSetColor(ofColor::white);
	this->star_brown.draw(this->stars_pos[2]);
	ofSetColor(ofColor::black);
	this->sentence_font.drawString(L"\n\n\n\n“d˜b”Ô†F", 130, HALF_MAX_SUB_WINDOW_H + 200);
	ofSetColor(ofColor::red);
	this->sentence_font.drawString(L"\n\n\n\n075-371-9210", 470, HALF_MAX_SUB_WINDOW_H + 200);
}