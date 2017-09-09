#include "MainScene.h"

void MainScene::setup(HandPointer* hp) {
	ofSetBackgroundAuto(true);

	/* �A�C�R���𐶐� */
	const int x_unit = this->window_width / 7;
	const int y_unit = this->window_height / 5;

	this->icons.insert(make_pair("syoukousyu", Icon(x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.2_vs_release/apps/myApps/DigitalSignage/fig/cooking_syoukousyu.png")));
	this->icons.insert(make_pair("energy", Icon(3*x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.2_vs_release/apps/myApps/DigitalSignage/fig/drink_energy.png")));
	this->icons.insert(make_pair("vodka", Icon(5*x_unit, y_unit, x_unit, y_unit, "C:/of_v0.9.2_vs_release/apps/myApps/DigitalSignage/fig/drink_vodka.png")));
	this->icons.insert(make_pair("juice", Icon(x_unit, 3*y_unit, x_unit, y_unit, "C:/of_v0.9.2_vs_release/apps/myApps/DigitalSignage/fig/juice_orange.png")));
	this->icons.insert(make_pair("tea", Icon(3*x_unit, 3*y_unit, x_unit, y_unit, "C:/of_v0.9.2_vs_release/apps/myApps/DigitalSignage/fig/petbottle_tea_koucha.png")));
	this->icons.insert(make_pair("suisosui", Icon(5*x_unit, 3*y_unit, x_unit, y_unit, "C:/of_v0.9.2_vs_release/apps/myApps/DigitalSignage/fig/water_bottle_suisosui.png")));
	
	this->hp = hp;
}

void MainScene::update() {
	/* ��|�C���^���A�C�R���ɐG�ꂽ��C�x���g�𔭉� */
	for (auto t : this->hp->track_data) {
		for (auto i : this->icons) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // �A�C�R�����Ǝ�|�C���^id�̏��
				ofNotifyEvent(this->point_event, id);
			}
		}
	}
}

void MainScene::draw() {
	/* �A�C�R����`�� */
	for (auto i : this->icons) {
		i.second.draw();
	}
}