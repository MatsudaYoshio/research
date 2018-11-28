#include "SceneManager.h"

using namespace param;

void SceneManager::setup(HandCursor* const hc) {
	this->hc = hc;

	this->mb.setup(hc);
	ofAddListener(this->mb.add_pin_event, this, &SceneManager::add_pin);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->pins[i].resize(MENU_ITEM_CONTENTS[i].size());
		for (int j = 0; j < this->pins[i].size(); ++j) {
			this->pins[i][j].setup(MENU_ITEM_CONTENTS[i][j]);
			ofAddListener(this->pins[i][j].make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}

	fill_n(begin(this->menu_item_user_id), MENU_ITEM_NUM, NOT_USER);
	fill_n(begin(this->menu_item_life), MENU_ITEM_NUM, this->max_menu_item_life);
}

void SceneManager::update() {
	/* �T�u�E�B���h�E�̍X�V */
	/* ���Ȃ��Ȃ������[�U�̃T�u�E�B���h�E���폜 */
	for (auto ite = begin(this->sub_windows); ite != end(this->sub_windows);) {
		if (this->hc->user_data.find(ite->second.get_user_id()) == end(this->hc->user_data)) {
			ite = this->sub_windows.erase(ite);
		}
		else {
			++ite;
		}
	}

	for (auto&& w : this->sub_windows) {
		w.second.update();
	}

	/* ���j���[���ڂ̍X�V */
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_user_id[i] == NOT_USER) {
			continue;
		}

		if (this->hc->user_data.find(this->menu_item_user_id[i]) != end(this->hc->user_data)) {
			this->menu_item_life[i] = this->max_menu_item_life;
		}
		else {
			--this->menu_item_life[i];
		}

		if (this->menu_item_life[i] == this->min_menu_item_life) {
			this->menu_item_life[i] = this->max_menu_item_life;
			this->menu_item_user_id[i] = NOT_USER;
		}
	}

	this->mb.update(); // ���j���[�o�[�̍X�V

	/* �J�[�\�����E�B���h�E�Əd�����邩�ǂ����𒲂ׂāA�J�[�\���̏�Ԃ�ύX���� */
	for (const auto& ud : this->hc->user_data) {
		if (ud.second.state == HandCursor::STATE::INACTIVE) {
			continue;
		}

		for (const auto& sw : this->sub_windows) {
			if (sw.second.get_rect().inside(ud.second.cursor_point.x, ud.second.cursor_point.y)) {
				this->hc->overlap_window(ud.first);
				break;
			}
		}
	}

	// �s���̍X�V
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_user_id[i] == NOT_USER) {
			continue;
		}
		for (auto&& p : this->pins[i]) {
			for (const auto& ud : this->hc->user_data) {
				if (ud.second.state != HandCursor::STATE::ACTIVE) {
					continue;
				}
				if (p.is_inside(ud.second.cursor_point.x, ud.second.cursor_point.y)) {
					p.point(ud.first);
					goto CONTINUE_LOOP;
				}
			}
			p.reset_state();

		CONTINUE_LOOP:
			p.update();
		}
	}
}

void SceneManager::draw() {
	ofSetColor(ofColor::white);
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H); // �}�b�v�̕\��

	this->mb.draw(); // ���j���[�o�[�̕\��

	/* �s���̕`�� */
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_user_id[i] == NOT_USER) {
			continue;
		}

		for (const auto& p : this->pins[i]) {
			p.draw();
		}
	}

	// �T�u�E�B���h�E�̕`��
	for (auto&& w : this->sub_windows) {
		w.second.draw();
	}

	this->draw_cursor(); // ��J�[�\���̕`��
}

void SceneManager::add_pin(pair<param::MENU_ITEM_ID, long long int>& id) {
	this->menu_item_user_id[static_cast<int>(id.first)] = id.second;
}

void SceneManager::make_sub_window(pair<param::CONTENT_ID, long long int>& id) {
	this->make_sub_window_flag = true;

	/* ���ɓ����R���e���c�̃T�u�E�B���h�E����������A�V���ɃT�u�E�B���h�E�𐶐����Ȃ� */
	if (this->sub_windows.find(static_cast<int>(id.first)) != end(this->sub_windows)) {
		return;
	}

	const auto ite{ find_if(cbegin(this->sub_windows), cend(this->sub_windows),
		[this, id](const auto& x) {return x.second.get_user_id() == id.second; }
	) };
	if (ite != cend(this->sub_windows)) { // ���̃��[�U�����łɃT�u�E�B���h�E�𐶐����Ă�����
		/* ���̃T�u�E�B���h�E���폜���� */
		this->sub_windows.erase(ite->first);
	}

	this->sub_windows.emplace(static_cast<int>(id.first), SubWindow{ id.first, id.second, this->hc->user_data[id.second].cursor_color });
}

void SceneManager::draw_cursor() {
	for (const auto& ud : this->hc->user_data) {
		//if (ud.second.state == HandCursor::STATE::INACTIVE) {
		//	continue;
		//}

		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white, ud.second.alpha);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 60);
		ofFill();
		ofSetColor(ud.second.cursor_color, ud.second.alpha);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 55);
	}
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->mb.add_pin_event, this, &SceneManager::add_pin);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		for (auto&& p : this->pins[i]) {
			ofRemoveListener(p.make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}
}