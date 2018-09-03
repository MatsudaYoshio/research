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

	this->sa.setup(this->hc, &this->sub_windows);
}

void SceneManager::update() {
	this->optimize(); // �œK��

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

		//if (i == 2) continue;

		for (const auto& p : this->pins[i]) {
			p.draw();
		}
	}

	//for (const auto& ud : this->hc->user_data) {
	//	ofSetColor(ud.second.cursor_color, 120);
	//	this->face_image.draw(ud.second.transformed_face_point.x(), ud.second.transformed_face_point.y(), 200, 200);
	//}

	// �T�u�E�B���h�E�̕`��
	for (auto&& w : this->sub_windows) {
		w.second.draw();
	}

	this->draw_cursor(); // ��J�[�\���̕`��
}

void SceneManager::optimize() {
	/* �Ă��Ȃ܂��@�ւ̓��͂̏���(���݂̃E�B���h�E��Ԃ��珉����Ԃ��쐬) */
	this->initial_rects.clear();
	for (const auto& s : this->sub_windows) {
		this->initial_rects.emplace(s.first, s.second.get_rect());
	}

	this->sa(this->initial_rects, this->best_rects, this->best_cost); // �Ă��Ȃ܂��@�ōœK��

	/* ��r����R�X�g���v�Z(���O�̃E�B���h�E��Ԃ����݂̃��[�U�����g����) */
	if (!this->previous_rects.empty() && !this->previous_sub_windows.empty()) {
		this->sa.calculate_cost(this->previous_rects, this->comparative_cost, this->previous_sub_windows);
	}

	cout << "comparative cost = " << this->comparative_cost << endl;
	cout << "best cost = " << this->best_cost << endl;
	cout << "-----------------------" << endl;

	if (this->comparative_cost > this->best_cost && this->comparative_cost - this->best_cost > this->transform_threshold) {
	//if (this->is_intersect_window_pointer() || this->is_intersect_window_window() || (this->comparative_cost > this->best_cost && this->comparative_cost - this->best_cost > this->transform_threshold)) {

		//cout << "comparative cost = " << this->comparative_cost << endl;
		//cout << "best cost = " << this->best_cost << endl;
		//cout << "-----------------------" << endl;

		this->previous_sub_windows = this->sub_windows;

		/* �œK�ȃp�����[�^���Z�b�g */
		for (const auto& id : this->sub_windows) {
			this->sub_windows[id.first].set_rect(this->best_rects[id.first]);
		}
		
		this->previous_rects = move(this->best_rects); // ���(�p�����[�^)���X�V
	}
}

void SceneManager::add_pin(pair<param::MENU_ITEM_ID, long long int>& id) {
	this->menu_item_user_id[static_cast<int>(id.first)] = id.second;
}

void SceneManager::make_sub_window(pair<param::CONTENT_ID, long long int>& id) {
	/* ���ɓ����R���e���c�̃T�u�E�B���h�E����������A�V���ɃT�u�E�B���h�E�𐶐����Ȃ� */
	if (this->sub_windows.find(static_cast<int>(id.first)) != end(this->sub_windows)) {
		return;
	}

	auto ite{ find_if(begin(this->sub_windows), end(this->sub_windows),
		[this, id](const auto& x) {return x.second.get_user_id() == id.second; }
	) };
	if (ite != end(this->sub_windows)) { // ���̃��[�U�����łɃT�u�E�B���h�E�𐶐����Ă�����
		/* ���̃T�u�E�B���h�E���폜���� */
		this->sub_windows.erase(ite->first);
	}

	this->sub_windows.emplace(static_cast<int>(id.first), SubWindow{ id.first, id.second });
}

void SceneManager::draw_cursor() {
	for (const auto& ud : this->hc->user_data) {
		if (ud.second.state == HandCursor::STATE::INACTIVE) {
			continue;
		}

		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white, ud.second.alpha);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 60);
		ofFill();
		ofSetColor(ud.second.cursor_color, ud.second.alpha);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 55);
	}
}

bool SceneManager::is_intersect_window_pointer() {
	// �T�u�E�B���h�E�ƃ|�C���^�̎��ӗ̈悪�d�����邩�ǂ����𒲂ׂ�
	for (const auto& sw : this->sub_windows) {
		for (const auto& ud : this->hc->user_data) {
			if (ud.second.state == HandCursor::STATE::INACTIVE) {
				continue;
			}
			if (sw.second.get_rect().intersects(ofRectangle(ofClamp(ud.second.cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight()))) {
				return true;
			}
		}
	}

	return false;
}

bool SceneManager::is_intersect_window_window() {
	// �T�u�E�B���h�E���m���d�����邩�ǂ����𒲂ׂ�
	for (const auto& sw1 : this->sub_windows) {
		for (const auto& sw2 : this->sub_windows) {
			if (sw1.first == sw2.first) {
				continue;
			}

			if (sw1.second.get_rect().intersects(sw2.second.get_rect())) {
				return true;
			}
		}
	}

	return false;
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->mb.add_pin_event, this, &SceneManager::add_pin);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		for (auto&& p : this->pins[i]) {
			ofRemoveListener(p.make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}
}