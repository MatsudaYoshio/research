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
}

void WhacAMoleApp::update() {
	this->se.update();

	for (const auto& up : this->se.transformed_key_points) { // すべてのユーザを含めた骨格点のをループでまわす
		if (up.first.second == RIGHT_WRIST || up.first.second == LEFT_WRIST) { // 右手首か左手首の骨格点だったら
			for (int i = 0; i < this->mole_num_all; ++i) {
				if (this->moles[i].is_appear() && this->is_whacked(this->moles[i], up.second)) { // もしモグラを叩いたら
					++score; // スコアを加算
					this->moles[i].hide(this->random_hidden_time(this->mt)); // モグラに隠れ時間を与え、隠れてもらう
				}
			}
		}
	}

	if (this->se.transformed_key_points.empty()) { // もしユーザがいなかったら
		for (int i = 0; i < this->mole_num_all; ++i) {
			if (this->moles[i].is_idle()) { // モグラが暇だったら
				/* ランダムに出現時間または隠れ時間を与える */
				if (this->random_0or1(this->mt)) {
					this->moles[i].appear(this->random_appearance_time(this->mt));
				}
				else {
					this->moles[i].hide(this->random_hidden_time(this->mt));
				}
			}
		}
	}
	else { // もしユーザがいたらそのにはモグラを出現させない
		for (const auto& up : this->se.transformed_key_points) { // すべてのユーザを含めた骨格点のをループでまわす
			if (up.first.second == RIGHT_WRIST || up.first.second == LEFT_WRIST) { // 右手首か左手首の骨格点だったら
				for (int i = 0; i < this->mole_num_all; ++i) {
					if (!this->is_whacked(this->moles[i], up.second) && this->moles[i].is_idle()) { // モグラの位置に骨格点がなくて、かつ、モグラが暇していたら
						/* ランダムに出現時間または隠れ時間を与える */
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
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // 背景の描画

	/* 情報ビュー（スコアを表示するところ）の描画 */
	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, DISPLAY_WIDTH, this->information_view_size);

	/* スコアを描画 */
	ofSetColor(ofColor::white);
	this->font.drawString("Score :", this->font_draw_point.x, this->font_draw_point.y);
	this->font.drawString(to_string(this->score), this->font_draw_point.x + 300, this->font_draw_point.y);

	/* モグラを描画 */
	ofSetColor(ofColor::white);
	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].draw();
	}

	/* 骨格の描画 */
	this->kpr.draw(this->se.transformed_key_points);
}

void WhacAMoleApp::exit() {
	this->se.exit();
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

bool WhacAMoleApp::is_whacked(const Mole& mole, const ofPoint& key_point) const {
	return mole.get_rect().inside(key_point);
}