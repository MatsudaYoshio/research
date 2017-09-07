#include "MainScene.h"

void MainScene::setup(HandPointer* hp) {
	ofSetBackgroundAuto(true);

	/* �A�C�R���𐶐� */
	this->icons.insert(make_pair("test_icon1", Icon(200, 200, 200, 200, "C:/Users/matsuda/Desktop/car.jpg")));
	this->icons.insert(make_pair("test_icon2", Icon(1000, 600, 200, 200, "C:/Users/matsuda/Desktop/car.jpg")));
	
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