#include "MainScene.h"

using namespace param;

void MainScene::setup(HandCursor* hc) {
	ofSetBackgroundAuto(true);

	/* �A�C�R���𐶐� */
	const int x_unit = W / 7;
	const int y_unit = H / 5;

	this->icons.insert(make_pair("syoukousyu", Icon(x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/cooking_syoukousyu.png")));
	this->icons.insert(make_pair("energy", Icon(3*x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/drink_energy.png")));
	this->icons.insert(make_pair("vodka", Icon(5*x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/drink_vodka.png")));
	this->icons.insert(make_pair("juice", Icon(x_unit, 3*y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/juice_orange.png")));
	this->icons.insert(make_pair("tea", Icon(3*x_unit, 3*y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/petbottle_tea_koucha.png")));
	this->icons.insert(make_pair("suisosui", Icon(5*x_unit, 3*y_unit, x_unit, y_unit, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/water_bottle_suisosui.png")));
	
	this->hc = hc;

	for (auto &i : this->icons) {
		ofAddListener(i.second.transition_event, this, &MainScene::transition);
	}
}

void MainScene::update() {
	bool flag;
	for (auto &i : this->icons) {
		flag = false;
		for (auto &p : this->pointer_id) {
			if (i.second.is_inside(ofPoint(this->hc->track_data[p].current_pointer.x, this->hc->track_data[p].current_pointer.y))) {
				pair<string, int> id(i.first, p); // �A�C�R�����Ǝ�|�C���^id�̏��
				ofNotifyEvent(this->point_event, id);
				i.second.track_id = p;
				flag = true;
				break;
			}
		}
		/*
		for (auto &t : this->hc->track_data) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // �A�C�R�����Ǝ�|�C���^id�̏��
				ofNotifyEvent(this->point_event, id);
				i.second.track_id = t.first;
				flag = true;
				break;
			}
		}
		*/
		if (!flag) {
			i.second.change_state("None");
		}
		i.second.update();
	}
}

void MainScene::draw() {
	/* �A�C�R����`�� */
	for (auto &i : this->icons) {
		i.second.draw();
	}

	ofSetColor(ofColor::white);

	/* ��|�C���^�̕`�� */
	for (auto &p : this->pointer_id) {
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 50; ++i) {
			r += 0.6;
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

void MainScene::change_icon_state(string icon_id, string state) {
	this->icons.at(icon_id).change_state(state);
}

void MainScene::set_icon_pointer_id(string icon_id, int pointer_id) {
	this->icons.at(icon_id).set_pointer_id(pointer_id);
}