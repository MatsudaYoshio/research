#include "WhacAMoleApp.h"

using namespace param;

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };
const double WhacAMoleApp::mole_width{ (DISPLAY_WIDTH - WhacAMoleApp::mole_margin_row) / WhacAMoleApp::mole_num_row - WhacAMoleApp::mole_margin_row };
const double WhacAMoleApp::mole_height{ (DISPLAY_HEIGHT - WhacAMoleApp::mole_margin_col - WhacAMoleApp::information_view_size) / WhacAMoleApp::mole_num_col - WhacAMoleApp::mole_margin_col };
const string WhacAMoleApp::top_score_path{ "C:/of_v0.9.8_vs_release/apps/myApps/WhacAMoleApp_opencampus/bin/data/top_score.txt" };

random_device WhacAMoleApp::rd;
mt19937 WhacAMoleApp::mt(WhacAMoleApp::rd());
uniform_int_distribution<int> WhacAMoleApp::random_0or1(0, 1);
uniform_int_distribution<int> WhacAMoleApp::random_appearance_time(20, 100);
uniform_int_distribution<int> WhacAMoleApp::random_hidden_time(20, 100);

void WhacAMoleApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化

	ifstream ifs(this->top_score_path);
	string str;
	ifs >> str;
	this->top_score = stoll(str);
	ifs.close();

	this->small_font.load("./font/SFNSText.ttf", 100); // フォントのロード
	this->big_font.load("./font/SFNSText.ttf", 200); // フォントのロード

	this->initialize_image(); // 画像の初期化
	this->initialize_moles(); // モグラオブジェクトの初期化

	this->overall_timer.start();
}

void WhacAMoleApp::update() {
	this->se.update();

	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].update();
	}

	for (const auto& up : this->se.transformed_key_points) { // すべてのユーザを含めた骨格点のをループでまわす
		if (up.first.second == RIGHT_WRIST || up.first.second == LEFT_WRIST) { // 右手首か左手首の骨格点だったら
			for (int i = 0; i < this->mole_num_all; ++i) {
				if (this->moles[i].is_appear() && this->is_whacked(this->moles[i], up.second)) { // もしモグラが出現していて、それを叩いたら
					this->moles[i].salvage();
					this->playing_score += log(this->overall_timer.get_milliseconds()) * this->moles[i].get_score(); // 時間によってスコアにかかる比率を変える
				}
			}
		}
	}

	this->playing_star = Mole::current_star;

	if (this->se.transformed_key_points.empty()) { // もしユーザがいなかったら
		for (int i = 0; i < this->mole_num_all; ++i) {
			if (this->moles[i].is_idle()) { // モグラが暇だったら{
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
	else { // もしユーザがいたらその場所にはモグラを出現させない
		for (int i = 0; i < this->mole_num_all; ++i) {
			if (this->moles[i].is_idle()) { // モグラが暇していたら
				for (const auto& up : this->se.transformed_key_points) { // すべてのユーザを含めた骨格点のをループでまわす
					if ((up.first.second == RIGHT_WRIST || up.first.second == LEFT_WRIST) && this->is_whacked(this->moles[i], up.second)) {
						goto THROUGH_MOLE;
					}
				}

				/* ランダムに出現時間または隠れ時間を与える */
				if (this->random_0or1(this->mt)) {
					this->moles[i].appear(this->random_appearance_time(this->mt));
				}
				else {
					this->moles[i].hide(this->random_hidden_time(this->mt));
				}
			}

		THROUGH_MOLE:;
		}
	}
}


void WhacAMoleApp::draw() {
	switch (this->state) {
	case STATE::PLAY:
		ofBackground(this->bg_color); // 背景の描画

		/* モグラを描画 */
		ofSetColor(ofColor::white);
		for (int i = 0; i < this->mole_num_all; ++i) {
			this->moles[i].draw();
		}

		/* 星を描画*/
		ofSetColor(ofColor::white);
		HOSHI_IMAGE.draw(200, 20, 150, 150);
		KAKERU_IMAGE.draw(325, 20, 150, 160);
		ofSetColor(ofColor::gold);
		this->small_font.drawString(to_string(this->playing_star), 500, 140);

		/* 現在のスコアを描画*/
		ofSetColor(ofColor::black);
		this->small_font.drawString("Your Score : ", 1000, 140);
		ofSetColor(ofColor::blue);
		this->small_font.drawString(to_string(this->playing_score), 1800, 140);

		/* トップスコアを描画*/
		ofSetColor(ofColor::black);
		this->small_font.drawString("Top Score : ", 2500, 140);
		ofSetColor(ofColor::deepPink);
		this->small_font.drawString(to_string(max(this->playing_score, this->top_score)), 3300, 140);

		/* 骨格の描画 */
		this->kpr.draw(this->se.transformed_key_points);

		break;
	case STATE::GAMEOVER:
		const auto t{ this->gameover_timer.get_seconds() };
		if (t < this->bang_time) {
			ofSetColor(ofColor::white);
			BANG_IMAGE.draw(this->bang_position, this->mole_width, this->mole_height);

			this->reset_game();
		}
		else if (t >= this->bang_time && t <= this->bang_time + this->result_time) {
			const float info_alpha = min(ofColor::limit(), ofColor::limit() / 2 + (t - 3) * 16);
			const float bg_alpha = max(ofColor::limit() / 4, ofColor::limit() / 2 - (t - 3) * 16);

			const ofColor _bg_color(this->bg_color, bg_alpha);
			ofBackground(_bg_color); // 背景の描画

			/* モグラを描画 */
			ofSetColor(ofColor::white, bg_alpha);
			for (int i = 0; i < this->mole_num_all; ++i) {
				this->moles[i].draw();
			}

			/* 白枠を描画 */
			ofSetColor(ofColor::white, info_alpha / 1.5);
			ofDrawRectRounded(DISPLAY_WIDTH / 4, DISPLAY_HEIGHT / 4, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 4, 40);

			ofSetColor(ofColor::black, info_alpha);
			this->big_font.drawString("GAME OVER", 1125, 500);

			ofSetColor(ofColor::black, info_alpha);
			this->small_font.drawString("Total Star : ", DISPLAY_WIDTH / 4 + 100, DISPLAY_HEIGHT / 4 + 225);
			ofSetColor(ofColor::gold, info_alpha);
			this->small_font.drawString(to_string(this->result_star), DISPLAY_WIDTH / 4 + 900, DISPLAY_HEIGHT / 4 + 225);

			ofSetColor(ofColor::black, info_alpha);
			this->small_font.drawString("Total Score : ", DISPLAY_WIDTH / 4 + 100, DISPLAY_HEIGHT / 4 + 425);
			ofSetColor(ofColor::blue, info_alpha);
			this->small_font.drawString(to_string(this->result_score), DISPLAY_WIDTH / 4 + 1000, DISPLAY_HEIGHT / 4 + 425);

			/* 骨格の描画 */
			this->kpr.draw(this->se.transformed_key_points);
		}
		else if (t > this->bang_time + this->result_time) {
			this->state = STATE::PLAY;
			this->reset_game();
		}

		break;
	}
}

void WhacAMoleApp::exit() {
	for (int i = 0; i < this->mole_num_all; ++i) {
		ofRemoveListener(this->moles[i].explosion_event, this, &WhacAMoleApp::gameover);
	}

	this->se.exit();
}

void WhacAMoleApp::initialize_image() const {
	for (int i = 0; i < MOLE_STATE_NUM; ++i) {
		MOLE_IMAGE[i].load("./fig/mole" + to_string(i) + ".png");
	}

	HOSHI_IMAGE.load("./fig/star.png");
	KAKERU_IMAGE.load("./fig/kakeru.png");
	BANG_IMAGE.load("./fig/bang.png");
}

void WhacAMoleApp::initialize_moles() {
	for (int i = 0, x = this->mole_margin_row; x < DISPLAY_WIDTH; x += this->mole_width + this->mole_margin_row) {
		for (int y = this->information_view_size + this->mole_margin_col; y < DISPLAY_HEIGHT && i < this->mole_num_all; y += this->mole_height + this->mole_margin_col) {
			this->moles[i++].setup(ofRectangle(x, y, this->mole_width, this->mole_height), this->random_hidden_time(this->mt));
		}
	}

	for (int i = 0; i < this->mole_num_all; ++i) {
		ofAddListener(this->moles[i].explosion_event, this, &WhacAMoleApp::gameover);
	}
}

bool WhacAMoleApp::is_whacked(const Mole& mole, const ofPoint& key_point) const {
	return mole.get_rect().inside(key_point);
}

void WhacAMoleApp::gameover(pair<int, int>& position) {
	if (this->state == STATE::PLAY) {
		this->state = STATE::GAMEOVER;

		this->bang_position.x = position.first;
		this->bang_position.y = position.second;

		this->result_score = this->playing_score;
		this->result_star = this->playing_star;

		if (this->result_score > this->top_score) {
			this->ofs.open(this->top_score_path);
			this->ofs << to_string(this->result_score) << endl;
			this->ofs.close();

			this->top_score = this->result_score;
		}

		this->gameover_timer.start();
	}
}

void WhacAMoleApp::reset_game() {
	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].hide(this->random_hidden_time(this->mt));
	}

	this->playing_score = 0;
	this->playing_star = 0;
	Mole::current_star = 0;

	this->overall_timer.start();
}