#include "MainScene.h"

using namespace param;

void MainScene::setup(HandCursor* hc) {
	ofSetBackgroundAuto(true);

	this->hc = hc;

	/* �A�C�R���𐶐� */
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

	this->db.setup(ofColor::aquamarine, ofColor::blueSteel, 120);
}

void MainScene::update() {
	for (auto &i : this->icons) {
		switch (i.second.state) {
		case static_cast<int>(Icon::STATE::FADEOUT) :
		case static_cast<int>(Icon::STATE::FADEIN) :
			goto CONTINUE_LOOP;
		case static_cast<int>(Icon::STATE::INACTIVE) :
			/* �����ł��܂ɃA�C�R����ύX���鏈�������� */
			if (0) {
				i.second.change_state(Icon::STATE::FADEOUT);
				goto CONTINUE_LOOP;
			}
		case static_cast<int>(Icon::STATE::POINT) :
			for (const auto &p : this->pointer_id) {
				if (i.second.is_inside(ofPoint(this->hc->track_data[p].current_pointer.x, this->hc->track_data[p].current_pointer.y))) {
					pair<string, int> id(i.first, p); // �A�C�R��id�ƃ��[�Uid�̏��
					ofNotifyEvent(this->point_event, id);
					goto CONTINUE_LOOP;
				}
			}
			break;
		}

		i.second.change_state(Icon::STATE::INACTIVE);

	CONTINUE_LOOP:
		i.second.update();
	}
}

void MainScene::draw() {
	this->db.draw();

	/* �A�C�R����`�� */
	for (auto &i : this->icons) {
		i.second.draw();
	}

	ofSetColor(ofColor::white);

	/* ��J�[�\���̕`�� */
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
	this->icons.at(icon_id).change_state(Icon::STATE::POINT);
	this->icons.at(icon_id).set_user_id(user_id);
}