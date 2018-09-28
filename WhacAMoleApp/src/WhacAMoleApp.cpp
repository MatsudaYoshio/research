#include "WhacAMoleApp.h"

using namespace param;

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };
const ofPoint WhacAMoleApp::font_draw_point{ 400, 120 };

random_device WhacAMoleApp::rd;
mt19937 WhacAMoleApp::mt(WhacAMoleApp::rd());
uniform_int_distribution<int> WhacAMoleApp::random_0or1(0, 1);
uniform_int_distribution<int> WhacAMoleApp::random_appearance_time(8, 90);
uniform_int_distribution<int> WhacAMoleApp::random_hidden_time(8, 90);

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化
	ofHideCursor(); // カーソル非表示

	this->font.load("./font/SFNSText.ttf", 60); // フォントのロード

	this->initialize_image(); // 画像の初期化

	this->initialize_moles(); // モグラオブジェクトの初期化

	this->kpr.setup(&this->hc.pose_key_points);
}

void WhacAMoleApp::update() {
	this->hc.update();

	this->hammer_rects.clear();
	for (const auto& ud : this->hc.user_data) {
		this->hammer_rects.emplace(ud.first, ofRectangle(ud.second.cursor_point.x - this->hammer_width / 2, ud.second.cursor_point.y - this->hammer_height / 2, this->hammer_width, this->hammer_height));
	}

	for (const auto& r : this->hammer_rects) {
		for (int i = 0; i < this->mole_num_all; ++i) {
			if (this->moles[i].is_appear() && this->is_whacked(this->moles[i], r.second)) {
				++score;
				this->moles[i].hide(this->random_hidden_time(this->mt));
			}
		}
	}

	if (this->hammer_rects.empty()) {
		for (int i = 0; i < this->mole_num_all; ++i) {
			if (this->moles[i].is_idle()) {
				if (this->random_0or1(this->mt)) {
					this->moles[i].appear(this->random_appearance_time(this->mt));
				}
				else {
					this->moles[i].hide(this->random_hidden_time(this->mt));
				}
			}
		}
	}
	else {
		for (const auto& r : this->hammer_rects) {
			for (int i = 0; i < this->mole_num_all; ++i) {
				if (!this->is_whacked(this->moles[i], r.second) && this->moles[i].is_idle()) {
					if (this->random_0or1(this->mt)) {
						this->moles[i].appear(this->random_appearance_time(this->mt));
					}
					else {
						this->moles[i].hide(this->random_hidden_time(this->mt));
					}
				}
			}
		}
	}
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // 背景の描画

	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, DISPLAY_WIDTH, this->information_view_size);

	ofSetColor(ofColor::white);
	this->font.drawString("Score :", this->font_draw_point.x, this->font_draw_point.y);
	this->font.drawString(to_string(this->score), this->font_draw_point.x + 300, this->font_draw_point.y);

	ofSetColor(ofColor::white);
	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].draw();
	}

	this->kpr.draw();

	ofSetColor(ofColor::white);
	for (const auto& ud : this->hc.user_data) {
		ud.second.cursor_image.draw(ud.second.cursor_point.x - this->hammer_width / 2, ud.second.cursor_point.y - this->hammer_height / 2, this->hammer_width, this->hammer_height);
	}
}

void WhacAMoleApp::exit() {
	this->hc.exit();
}

void WhacAMoleApp::initialize_image() const {
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