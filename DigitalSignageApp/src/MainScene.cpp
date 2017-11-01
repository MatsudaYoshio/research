#include "MainScene.h"

using namespace param;

void MainScene::setup(HandCursor* hc) {
	ofSetBackgroundAuto(true);

	this->hc = hc;

	this->curve_vertices.resize(this->curve_vertices_num);
	this->curve_vertices[0].set(W, -0.185*H);
	this->curve_vertices[1].set(0.936*W, 0.185*H);
	this->curve_vertices[2].set(0.936*W, 0.185 * 2 * H);
	this->curve_vertices[3].set(0.936*W, 0.185 * 3 * H);
	this->curve_vertices[4].set(0.936*W, 0.185 * 4 * H);
	this->curve_vertices[5].set(0.964*W, 0.926*H);
	this->curve_vertices[6].set(1.042*W, H);

	this->icons.resize(2);
	this->icons[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].setup(800, 780, 200, 200, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/kyoto_tower.png", static_cast<int>(CONTENT_ID::KYOTO_TOWER));
	this->icons[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].setup(700, 450, 250, 250, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/simple_temple.png", static_cast<int>(CONTENT_ID::HIGASHIHONGANJI));

	for (auto &i : this->icons) {
		ofAddListener(i.select_event, this, &MainScene::select_icon);
	}

	this->db.setup(ofColor::whiteSmoke, ofColor::silver, 80);
}

void MainScene::update() {
	for (auto &i : this->icons) {
		switch (i.state) {
		case static_cast<int>(Icon::STATE::INACTIVE) :
		case static_cast<int>(Icon::STATE::POINT) :
			for (const auto &u : this->user_id_list) {
				if (i.is_inside(ofPoint(W-this->hc->track_data[u].current_pointer.x, this->hc->track_data[u].current_pointer.y))) {
					pair<int, int> id(i.get_content_id(), u); // コンテンツidとユーザidの情報
					ofNotifyEvent(this->point_event, id);
					goto CONTINUE_LOOP;
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
	this->db.draw();

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

	/* 道の描画 */
	ofSetColor(ofColor::gold);
	ofRectRounded(0.026*W, 0.185*H, 1.042*W, 0.026*W, 20);
	ofRectRounded(0.026*W, 0.648*H, 1.042*W, 0.026*W, 20);

	ofRectRounded(0.052*W, 0.093*H, 0.026*W, 0.815*H, 20);
	ofRectRounded(0.208*W, 0.093*H, 0.026*W, 0.815*H, 20);
	ofRectRounded(0.52*W, 0.093*H, 0.026*W, 0.815*H, 20);
	ofRectRounded(0.781*W, 0.093*H, 0.026*W, 0.815*H, 20);

	/* アイコンの描画 */
	for (auto &i : this->icons) {
		i.draw();
	}

	ofSetColor(ofColor::white);

	/* 手カーソルの描画 */
	for (auto &id : this->user_id_list) {
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 100; ++i) {
			r += 3;
			alpha -= 12;
			ofSetColor(this->hc->track_data[id].cursor_color, alpha);
			ofCircle(W-this->hc->track_data[id].current_pointer.x, this->hc->track_data[id].current_pointer.y, r);
		}
	}
}

void MainScene::select_icon(pair<int, int>& id) {
	ofNotifyEvent(this->make_sub_window_event, id); // サブウィンドウ生成イベント発火
}

void MainScene::point_icon(const int &content_id, const int &user_id) {
	this->icons.at(content_id).change_state(Icon::STATE::POINT);
	this->icons.at(content_id).set_user_id(user_id);
}