#include "MainScene.h"

using namespace param;

void MainScene::setup(HandCursor* hc) {
	ofSetBackgroundAuto(true);

	this->hc = hc;

	/* アイコンを生成 */
	const int x_unit = W / 7;
	const int y_unit = H / 5;

	this->icons.insert(make_pair("syoukousyu", Icon(x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/cooking_syoukousyu.png")));
	this->icons.insert(make_pair("energy", Icon(3 * x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/drink_energy.png")));
	this->icons.insert(make_pair("vodka", Icon(5 * x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/drink_vodka.png")));
	this->icons.insert(make_pair("juice", Icon(x_unit, 3 * y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/juice_orange.png")));
	this->icons.insert(make_pair("tea", Icon(3 * x_unit, 3 * y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/petbottle_tea_koucha.png")));
	this->icons.insert(make_pair("suisosui", Icon(5 * x_unit, 3 * y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/water_bottle_suisosui.png")));

	for (auto &i : this->icons) {
		ofAddListener(i.second.transition_event, this, &MainScene::transition);
	}

	this->db.setup(ofColor::aquamarine, ofColor::blueSteel);
}

void MainScene::update() {
	for (auto &i : this->icons) {
		for (const auto &p : this->pointer_id) {
			if (i.second.is_inside(ofPoint(this->hc->track_data[p].current_pointer.x, this->hc->track_data[p].current_pointer.y))) {
				pair<string, int> id(i.first, p); // アイコンidとユーザidの情報
				ofNotifyEvent(this->point_event, id);
				goto DETECT;
			}
		}

		i.second.change_state("None");

	DETECT:
		i.second.update();
	}
}

void MainScene::draw() {
	this->db.draw();

	/* アイコンを描画 */
	for (auto &i : this->icons) {
		i.second.draw();
	}

	ofSetColor(ofColor::white);

	/* 手カーソルの描画 */
	for (auto &p : this->pointer_id) {
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 100; ++i) {
			r += 3;
			alpha -= 12;
			ofSetColor(this->hc->track_data[p].cursor_color, alpha);
			ofCircle(this->hc->track_data[p].current_pointer.x, this->hc->track_data[p].current_pointer.y, r);
		}
	}
}

void MainScene::transition(int &pointer_id) {
	if (hc->track_data.size() == 1) {
		ofNotifyEvent(this->make_sub_window_event, pointer_id);
		//ofNotifyEvent(this->transition_event, pointer_id);
	}
	else if (hc->track_data.size() >= 2) {
		ofNotifyEvent(this->make_sub_window_event, pointer_id);
	}
}

void MainScene::select_icon(const string &icon_id, const int &user_id) {
	this->icons.at(icon_id).change_state("point");
	this->icons.at(icon_id).set_user_id(user_id);
}