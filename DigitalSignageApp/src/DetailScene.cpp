#include "DetailScene.h"

void DetailScene::setup(HandPointer *hp) {
	this->icons.insert(make_pair("return", Icon(this->window_width-200, 100, 100, 100, "C:/Users/matsuda/Downloads/return.jpeg")));

	this->hp = hp;
}

void DetailScene::update() {
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

void DetailScene::draw() {
	for (auto i : this->icons) {
		i.second.draw();
	}
}