#include "MainScene.h"

using namespace param;

void MainScene::setup(HandCursor* hc) {
	this->hc = hc;

	this->font.loadFont("meiryob.ttc", 40);

	this->curve_vertices.resize(this->curve_vertices_num);
	this->curve_vertices[0].set(W, -0.185*H);
	this->curve_vertices[1].set(0.936*W, 0.185*H);
	this->curve_vertices[2].set(0.936*W, 0.185 * 2 * H);
	this->curve_vertices[3].set(0.936*W, 0.185 * 3 * H);
	this->curve_vertices[4].set(0.936*W, 0.185 * 4 * H);
	this->curve_vertices[5].set(0.964*W, 0.926*H);
	this->curve_vertices[6].set(1.042*W, H);

	this->icons.resize(4);
	this->icons[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].setup(800, 780, 200, 200, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/kyoto_tower.png", static_cast<int>(CONTENT_ID::KYOTO_TOWER));
	this->icons[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].setup(750, 450, 250, 250, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/simple_temple.png", static_cast<int>(CONTENT_ID::HIGASHIHONGANJI));
	this->icons[static_cast<int>(CONTENT_ID::SYOSEIEN)].setup(1300, 500, 200, 200, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/syoseien_t.png", static_cast<int>(CONTENT_ID::SYOSEIEN));
	this->icons[static_cast<int>(CONTENT_ID::NISHIHONGANJI)].setup(200, 500, 200, 200, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/simple_temple2.png", static_cast<int>(CONTENT_ID::NISHIHONGANJI));

	for (auto &i : this->icons) {
		ofAddListener(i.select_event, this, &MainScene::select_icon);
	}

	this->db.setup(ofColor::whiteSmoke, ofColor::silver, 80);
}

void MainScene::update() {
	/* �V�������o�����J�[�\��������΃��C���V�[���̃��[�Uid���X�g�ɒǉ����� */
	for (const auto& td : this->hc->track_data) {
		this->user_id_list.emplace(td.first);
	}

	/* ���ł����J�[�\��������΃��C���V�[���̃��[�Uid���X�g������� */
	for (auto id = begin(this->user_id_list); id != end(this->user_id_list);) {
		if (this->hc->track_data.find(*id) == end(this->hc->track_data)) {
			this->user_id_list.erase(id++);
		}
		else {
			++id;
		}
	}

	for (auto &i : this->icons) {
		switch (i.state) {
		case static_cast<int>(Icon::STATE::INACTIVE) :
		case static_cast<int>(Icon::STATE::POINT) :
			for (auto u = begin(this->user_id_list); u != end(this->user_id_list); ++u) {
				try {
					if (i.is_inside(ofPoint(W - this->hc->track_data.at(*u).current_pointer.x, this->hc->track_data.at(*u).current_pointer.y))) {
						pair<int, int> id(i.get_content_id(), *u); // �R���e���cid�ƃ��[�Uid�̏��
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
	this->db.draw();

	ofSetColor(ofColor::black);
	for (int x = -6; x < 6; ++x) {
		for (int y = -6; y < 6; ++y) {
			this->font.drawString(L"���s�}�b�v", 70+x, 70+y);
		}
	}
	ofSetColor(ofColor::white);
	this->font.drawString(L"���s�}�b�v", 70, 70);

	/* ��̕`�� */
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

	/* ���̕`�� */
	ofSetColor(ofColor::gold);
	ofRectRounded(0.026*W, 0.185*H, 1.042*W, 0.026*W, 20);
	ofRectRounded(0.026*W, 0.648*H, 1.042*W, 0.026*W, 20);

	ofRectRounded(0.052*W, 0.093*H, 0.026*W, 0.815*H, 20);
	ofRectRounded(0.208*W, 0.093*H, 0.026*W, 0.815*H, 20);
	ofRectRounded(0.52*W, 0.093*H, 0.026*W, 0.815*H, 20);
	ofRectRounded(0.781*W, 0.093*H, 0.026*W, 0.815*H, 20);

	/* �A�C�R���̕`�� */
	for (auto &i : this->icons) {
		i.draw();
	}

	ofSetColor(ofColor::white);

	/* ��J�[�\���̕`�� */
	try {
		for (auto &id : this->user_id_list) {
			int alpha = 255;
			double r = 1;
			for (int i = 0; i < 100; ++i) {
				r += 3;
				alpha -= 12;
				ofSetColor(this->hc->track_data.at(id).cursor_color, alpha);
				ofCircle(W - this->hc->track_data.at(id).current_pointer.x, this->hc->track_data.at(id).current_pointer.y, r);
			}
		}
	}
	catch (std::out_of_range&) {

	}
}

void MainScene::select_icon(pair<int, int>& id) {
	ofNotifyEvent(this->make_sub_window_event, id); // �T�u�E�B���h�E�����C�x���g����
}

void MainScene::point_icon(const int &content_id, const int &user_id) {
	this->icons.at(content_id).change_state(Icon::STATE::POINT);
	this->icons.at(content_id).set_user_id(user_id);
}