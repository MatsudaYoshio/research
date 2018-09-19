#include "WhacAMoleApp.h"

using namespace param;

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化
	ofHideCursor(); // カーソル非表示

	this->initialize_image();

	this->initialize_moles();
}

void WhacAMoleApp::update() {
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // 背景の描画

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, DISPLAY_WIDTH, this->information_view_size);

	ofSetColor(ofColor::white);
	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].draw();
	}

	HAMMER_IMAGE.draw(mouseX, mouseY);
}

void WhacAMoleApp::initialize_image() const {
	HAMMER_IMAGE.load("fig/hammer.png");
	HOLE_IMAGE.load("fig/mole0.png");
	MOLE_IMAGE.load("fig/mole1.png");
}

void WhacAMoleApp::initialize_moles() {
	const auto mole_width{ (DISPLAY_WIDTH - this->mole_margin_row) / this->mole_num_row - this->mole_margin_row }; // 各モグラの幅
	const auto mole_height{ (DISPLAY_HEIGHT - this->mole_margin_col - this->information_view_size) / this->mole_num_col - this->mole_margin_col }; // 各モグラの高さ

	for (int i = 0, x = this->mole_margin_row; x < DISPLAY_WIDTH; x += mole_width + this->mole_margin_row) {
		for (int y = this->information_view_size + this->mole_margin_col; y < DISPLAY_HEIGHT && i < this->mole_num_all; y += mole_height + this->mole_margin_col) {
			this->moles[i++].setup(ofRectangle(x, y, mole_width, mole_height));
		}
	}
}