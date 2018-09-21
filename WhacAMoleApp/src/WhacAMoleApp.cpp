#include "WhacAMoleApp.h"

using namespace param;

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };

random_device WhacAMoleApp::rd;
mt19937 WhacAMoleApp::mt(WhacAMoleApp::rd());
uniform_int_distribution<int> WhacAMoleApp::random_0or1(0, 1);
uniform_int_distribution<int> WhacAMoleApp::random_appearance_time(200, 800);
uniform_int_distribution<int> WhacAMoleApp::random_hidden_time(200, 800);

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化
	ofHideCursor(); // カーソル非表示

	this->initialize_image(); // 画像の初期化

	this->initialize_moles();
}

void WhacAMoleApp::update() {
	ofRectangle hammer_rect(mouseX - this->hammer_width / 2, mouseY - this->hammer_height / 2, this->hammer_width, this->hammer_height);

	for (int i = 0; i < this->mole_num_all; ++i) {
		if (!this->is_whacked(this->moles[i], hammer_rect) && this->moles[i].is_idle()) {
			if (this->random_0or1(this->mt)) {
				this->moles[i].appear(this->random_appearance_time(this->mt));
			}
			else {
				this->moles[i].hide(this->random_hidden_time(this->mt));
			}
		}
	}
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // 背景の描画

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, DISPLAY_WIDTH, this->information_view_size);

	ofSetColor(ofColor::white);
	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].draw();
	}

	HAMMER_IMAGE.draw(mouseX - this->hammer_width / 2, mouseY - this->hammer_height / 2, this->hammer_width, this->hammer_height);
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
			this->moles[i++].setup(ofRectangle(x, y, mole_width, mole_height), this->random_hidden_time(this->mt));
		}
	}
}

bool WhacAMoleApp::is_whacked(const Mole& mole, const ofRectangle& hammer_rect) const {
	return mole.get_rect().intersects(hammer_rect);
}