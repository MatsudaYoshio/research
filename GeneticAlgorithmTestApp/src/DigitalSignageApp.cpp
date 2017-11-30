#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	//ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

	/* ���C���V�[���̏��� */
	ofAddListener(this->ms.point_event, this, &DigitalSignageApp::pointed);
	ofAddListener(this->ms.make_sub_window_event, this, &DigitalSignageApp::make_sub_window);
	this->ms.setup(&this->hc);

	this->ga.setup(&this->hc); // ��`�I�A���S���Y���̏���
}

void DigitalSignageApp::update() {
	this->hc.update(); // ��J�[�\���̍X�V

	this->ms.update(); // ���C���V�[���̍X�V

	/* ���ł����J�[�\��������ΑI�����Ă��郆�[�U���X�g������� */
	for (auto id = begin(this->selected_users); id != end(this->selected_users);) {
		if (this->hc.track_data.find(*id) == end(this->hc.track_data)) {
			this->selected_users.erase(id++);
		}
		else {
			++id;
		}
	}
	this->ga(this->selected_users, this->ms.user_id_list);
}

void DigitalSignageApp::draw() {
	this->ms.draw(); // ���C���V�[���̕`��

	if (this->selected_users.size() > 0) {
		this->ga.draw();
	}
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}

void DigitalSignageApp::pointed(pair<int, int> &id) {
	this->ms.point_icon(id.first, id.second);
}

void DigitalSignageApp::make_sub_window(pair<int, int>& id) {
	this->selected_users.emplace(id.second);
}