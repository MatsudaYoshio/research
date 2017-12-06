#include "MainScene.h"

using namespace param;

void MainScene::setup(HandCursor* hc) {
	ofSetBackgroundAuto(true);

	this->hc = hc;

	this->title_font.loadFont("meiryob.ttc", 80);
	this->kamogawa_font.loadFont("meiryob.ttc", 100);

	ofLoadImage(this->hotel_texture, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/building_hotel_small.png");

	this->curve_vertices.resize(this->curve_vertices_num);
	this->curve_vertices[0].set(DISPLAY_W, -0.185*DISPLAY_H);
	this->curve_vertices[1].set(0.936*DISPLAY_W, 0.185*DISPLAY_H);
	this->curve_vertices[2].set(0.936*DISPLAY_W, 0.185 * 2 * DISPLAY_H);
	this->curve_vertices[3].set(0.936*DISPLAY_W, 0.185 * 3 * DISPLAY_H);
	this->curve_vertices[4].set(0.936*DISPLAY_W, 0.185 * 4 * DISPLAY_H);
	this->curve_vertices[5].set(0.964*DISPLAY_W, 0.926*DISPLAY_H);
	this->curve_vertices[6].set(1.042*DISPLAY_W, DISPLAY_H);

	this->icons.resize(5);
	this->icons[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].setup(2000, 1560, 300, 300, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/kyoto_tower.png", static_cast<int>(CONTENT_ID::KYOTO_TOWER));
	this->icons[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].setup(2000, 1100, 300, 300, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/simple_temple.png", static_cast<int>(CONTENT_ID::HIGASHIHONGANJI));
	this->icons[static_cast<int>(CONTENT_ID::SYOSEIEN)].setup(2700, 1100, 300, 300, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/syoseien_t.png", static_cast<int>(CONTENT_ID::SYOSEIEN));
	this->icons[static_cast<int>(CONTENT_ID::NISHIHONGANJI)].setup(500, 1100, 300, 300, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/simple_temple2.png", static_cast<int>(CONTENT_ID::NISHIHONGANJI));
	this->icons[static_cast<int>(CONTENT_ID::RYUKOKU_MUSEUM)].setup(900, 700, 300, 300, "C:/of_v0.9.8_vs_release/apps/myApps/KyotoMap/fig/tatemono_hakubutsukan.png", static_cast<int>(CONTENT_ID::RYUKOKU_MUSEUM));

	for (auto& i : this->icons) {
		ofAddListener(i.select_event, this, &MainScene::select_icon);
	}

	this->db.setup(ofColor::whiteSmoke, ofColor::silver, 80);
}

void MainScene::update() {
	for (auto& i : this->icons) {
		switch (i.state) {
		case static_cast<int>(Icon::STATE::INACTIVE) :
		case static_cast<int>(Icon::STATE::POINT) :
			for (auto u = begin(this->user_id_list); u != end(this->user_id_list); ++u) {
				try {
					if (i.is_inside(ofPoint(this->hc->track_data.at(*u).transformed_cursor_point.x(), this->hc->track_data.at(*u).transformed_cursor_point.y()))) {
						pair<int, int> id(i.get_content_id(), *u); // コンテンツidとユーザidの情報
						ofNotifyEvent(this->point_event, id);
						goto CONTINUE_LOOP;
					}
				}
				catch (std::out_of_range&) {
					this->user_id_list.erase(u);
					return;
				}
			}
													 break;
		}

		i.change_state(Icon::STATE::INACTIVE);

	CONTINUE_LOOP:
		i.update();
	}
}

void MainScene::draw() {
	this->db.draw(); // 背景(グラデーション)を描く

	/* 左上に縁がある文字を描く */
	ofSetColor(ofColor::black);
	for (int x = -10; x < 10; ++x) {
		for (int y = -10; y < 10; ++y) {
			this->title_font.drawString(L"京都マップ", 140 + x, 140 + y);
		}
	}
	ofSetColor(ofColor::white);
	this->title_font.drawString(L"京都マップ", 140, 140);

	/* 川の描画 */
	ofFill();
	ofSetColor(ofColor::lightSkyBlue);

	ofBeginShape();

	for (int i = 0; i < this->curve_vertices_num; ++i) {
		if (i == 0) {
			ofCurveVertex(this->curve_vertices[0]);
			ofCurveVertex(this->curve_vertices[0]);
		}
		else if (i == this->curve_vertices_num - 1) {
			ofCurveVertex(this->curve_vertices[i]);
			ofCurveVertex(this->curve_vertices[0]);
			ofCurveVertex(this->curve_vertices[0]);
		}
		else {
			ofCurveVertex(this->curve_vertices[i]);
		}
	}

	ofEndShape();

	ofSetColor(ofColor::black);
	this->kamogawa_font.drawString(L"鴨", 3640, 800);
	this->kamogawa_font.drawString(L"川", 3640, 1300);

	/* 道の描画 */
	ofSetColor(ofColor::gold);
	ofRectRounded(0.026*DISPLAY_W, 0.185*DISPLAY_H, 1.042*DISPLAY_W, 0.026*DISPLAY_W, 20);
	ofRectRounded(0.026*DISPLAY_W, 0.648*DISPLAY_H, 1.042*DISPLAY_W, 0.026*DISPLAY_W, 20);

	ofRectRounded(0.052*DISPLAY_W, 0.093*DISPLAY_H, 0.026*DISPLAY_W, 0.815*DISPLAY_H, 20);
	ofRectRounded(0.208*DISPLAY_W, 0.093*DISPLAY_H, 0.026*DISPLAY_W, 0.815*DISPLAY_H, 20);
	ofRectRounded(0.6*DISPLAY_W, 0.093*DISPLAY_H, 0.026*DISPLAY_W, 0.815*DISPLAY_H, 20);
	ofRectRounded(0.781*DISPLAY_W, 0.093*DISPLAY_H, 0.026*DISPLAY_W, 0.815*DISPLAY_H, 20);

	/* アイコンの描画 */
	for (auto &i : this->icons) {
		i.draw();
	}

	/* ホテルテクスチャの描画 */
	ofSetColor(ofColor::white);
	this->hotel_texture.draw(600, 700, 180, 180);
	this->hotel_texture.draw(1600, 1700, 180, 180);
	this->hotel_texture.draw(2100, 700, 180, 180);

	/* 手カーソルの描画 */
	try {
		for (auto& id : this->user_id_list) {
			this->alpha = 255;
			this->r = 1;
			for (int i = 0; i < 100; ++i) {
				this->r += 3;
				this->alpha -= 12;
				ofSetColor(this->hc->track_data.at(id).cursor_color, this->alpha);
				ofCircle(this->hc->track_data.at(id).transformed_cursor_point.x(), this->hc->track_data.at(id).transformed_cursor_point.y(), this->r);
			}
		}
	}
	catch (std::out_of_range&) {}
}

void MainScene::select_icon(pair<int, int>& id) {
	ofNotifyEvent(this->make_sub_window_event, id); // サブウィンドウ生成イベント発火
}

void MainScene::point_icon(const int &content_id, const int &user_id) {
	this->icons.at(content_id).change_state(Icon::STATE::POINT);
	this->icons.at(content_id).set_user_id(user_id);
}

MainScene::~MainScene() {
	for (auto& i : this->icons) {
		ofRemoveListener(i.select_event, this, &MainScene::select_icon);
	}
}