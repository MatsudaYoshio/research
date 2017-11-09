#include "DigitalSignageApp.h"

void DigitalSignageApp::setup() {
	//ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���

	this->selected_user_num = 0;

	/* ���C���V�[���̏��� */
	ofAddListener(this->ms.point_event, this, &DigitalSignageApp::pointed);
	ofAddListener(this->ms.make_sub_window_event, this, &DigitalSignageApp::make_sub_window);
	this->ms.setup(&this->hc);

	this->ga.setup(&this->hc, &this->selected_user_num); // ��`�I�A���S���Y���̏���
}

void DigitalSignageApp::update() {
	this->hc.update(); // ��J�[�\���̍X�V
	this->ms.update(); // ���C���V�[���̍X�V
	if (this->selected_user_num > 0) {
		this->ga(this->g, this->g, this->user_assignment);
	}	
}

void DigitalSignageApp::draw() {

	this->ms.draw(); // ���C���V�[���̕`��

	if (this->selected_user_num > 0) {
		this->ga.draw_rectangles(this->g);
	}
	
	///* ��J�[�\���̕`�� */
	//for (const auto& td : this->hc.track_data) {
	//	int alpha = 255;
	//	double r = 1;
	//	for (int i = 0; i < 100; ++i) {
	//		r += 3;
	//		alpha -= 12;
	//		ofSetColor(td.second.cursor_color, alpha);
	//		ofCircle(param::W - td.second.current_pointer.x, td.second.current_pointer.y, r);
	//	}
	//}
}

void DigitalSignageApp::exit() {
	this->hc.exit();
}

void DigitalSignageApp::pointed(pair<int, int> &id) {
	this->ms.point_icon(id.first, id.second);
}

void DigitalSignageApp::make_sub_window(pair<int, int>& id) {
	++this->selected_user_num;
	this->user_assignment[0].emplace(id.second);
	this->g.set(0);
}