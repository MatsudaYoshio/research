#include "AdBar.h"

using namespace param;

const array<pair<int, int>, AdBar::ad_item_num> AdBar::ad_position{ make_pair(100, 50), make_pair(500, 50), make_pair(900, 50), make_pair(1300, 50) };

random_device AdBar::rd;
mt19937 AdBar::mt(AdBar::rd());

void AdBar::setup(array<bool, param::MENU_ITEM_NUM>* const menu_item_flag) {
	this->state = STATE::INACTIVE;
	this->menu_item_flag = menu_item_flag;
	this->df.setup(ofColor::gold, 10);
}

void AdBar::update() {
	switch (this->state) {
	case STATE::INACTIVE:
		for (int i = 0; i < MENU_ITEM_NUM; ++i) {
			if ((*this->menu_item_flag)[i]) {
				for (const auto& c : MENU_ITEM_CONTENTS[i]) {
					this->ads_tmp.emplace(c);
				}
			}
		}

		uniform_int_distribution<int> random_ad(0, this->ads_tmp.size() - 1);


		break;
	case STATE::ACTIVE:
	}
	this->ads_tmp.clear();
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if ((*this->menu_item_flag)[i]) {
			for (const auto& c : MENU_ITEM_CONTENTS[i]) {
				this->ads_tmp.emplace(c);
			}
		}
	}


	//this->df.update(ofRectangle(0, 0, 300, 300));
}

void AdBar::draw() {
	//ofFill();
	//ofSetColor(ofColor::white, 130);
	//ofDrawRectangle(0, 0, DISPLAY_W, 300);

	//this->df.draw();


	//ofImage img;
	//ofSetColor(ofColor::white);
	//img.load("C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/kyoto_tower.jpg");
	//img.draw(10, 10, 300 - 20, 300 - 20);

	//ofSetColor(ofColor::gold);
	//ofDrawCircle(50, 50, 35);

	//ofTrueTypeFont font;
	//font.loadFont("meiryob.ttc", 35);
	//ofSetColor(ofColor::black);
	//font.drawString("1", 33, 64);
}