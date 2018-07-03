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
	if (!this->transform_thread_flag && all_of(cbegin(this->sub_windows), cend(this->sub_windows), [](const auto& x) {return x.second.track_index == SubWindow::TRACK_READY; })) { // �œK���̌��ʂ𔽉f���Ă���Œ��łȂ��Ƃ�
		if (this->hc->user_data.size() == 1 && this->sub_windows.size() == 1) { // ���[�U�̈�l�ň�l�̃T�u�E�B���h�E��\�����Ă���Ƃ�
			ofRectangle best_rect(max(this->hc->user_data.begin()->second.transformed_face_point.x - HALF_MAX_SUB_WINDOW_W, 0), max(this->hc->user_data.begin()->second.transformed_face_point.y - HALF_MAX_SUB_WINDOW_H, 0), MAX_SUB_WINDOW_W, MAX_SUB_WINDOW_H);

			if (this->hc->user_data.begin()->second.state == HandCursor::STATE::INACTIVE || !best_rect.intersects(ofRectangle(ofClamp(this->hc->user_data.begin()->second.transformed_cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(this->hc->user_data.begin()->second.transformed_cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight()))) { // ���[�U�̃|�C���^���Ȃ����A���[�U�̊�̐��ʂɕ\�����Ă��d�����Ȃ��Ȃ��
				this->old_rects.clear();
				this->old_rects.emplace(this->sub_windows.begin()->first, this->sub_windows.begin()->second.get_rect());
				this->best_rects.clear();
				this->best_rects.emplace(this->sub_windows.begin()->first, best_rect);

				this->make_optimize_thread();
			}
			else {
				this->optimize();
			}
		}
		else if (this->is_intersect_window_pointer() || this->is_intersect_window_window()) { // �E�B���h�E�Ԃ������̓E�B���h�E�ƃ|�C���^�Ԃŏd���������
			this->optimize();
		}
	}

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

	// �s���̍X�V
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_user_id[i] == NOT_USER) {
			continue;
		}
		for (auto&& p : this->pins[i]) {
			for (const auto& ud : this->hc->user_data) {
				if (ud.second.state == HandCursor::STATE::INACTIVE) {
					continue;
				}
				if (p.is_inside(ud.second.transformed_cursor_point.x, ud.second.transformed_cursor_point.y)) {
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
		//if (this->menu_item_user_id[i] == NOT_USER) {
		//	continue;
		//}
		if (i == 2) continue;
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

void SceneManager::transform(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects) {
	this->transform_thread_flag = true;

	const double change_rate{ 1.0 / SubWindow::track_rects_num };

	int x_sign, y_sign, w_sign, h_sign;
	double x_change_val, y_change_val, w_change_val, h_change_val;

	for (const auto& id : this->sub_windows) {
		w_sign = (new_rects[id.first].width > old_rects[id.first].width) ? +1 : -1;
		h_sign = (new_rects[id.first].height > old_rects[id.first].height) ? +1 : -1;
		w_change_val = change_rate*abs(new_rects[id.first].width - old_rects[id.first].width)*w_sign;
		h_change_val = change_rate*abs(new_rects[id.first].height - old_rects[id.first].height)*h_sign;
		x_sign = (new_rects[id.first].x > old_rects[id.first].x) ? +1 : -1;
		y_sign = (new_rects[id.first].y > old_rects[id.first].y) ? +1 : -1;
		x_change_val = change_rate*abs(new_rects[id.first].x - old_rects[id.first].x)*x_sign;
		y_change_val = change_rate*abs(new_rects[id.first].y - old_rects[id.first].y)*y_sign;
		try {
			for (int i = 0; i < SubWindow::track_rects_num; ++i) {
				this->sub_windows.at(id.first).track_rects[i] = old_rects[id.first];
				old_rects[id.first].setWidth(old_rects[id.first].width + w_change_val);
				old_rects[id.first].setHeight(old_rects[id.first].height + h_change_val);
				old_rects[id.first].setX(old_rects[id.first].x + x_change_val);
				old_rects[id.first].setY(old_rects[id.first].y + y_change_val);
			}
			this->sub_windows.at(id.first).track_index = 0;
		}
		catch (std::out_of_range&) {}
	}

	this->transform_thread_flag = false;
}

void SceneManager::make_optimize_thread() {
	void(SceneManager::*funcp)(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects) = &SceneManager::transform;
	thread th(funcp, this, this->old_rects, this->best_rects);
	th.detach();
}

void SceneManager::optimize() {
	this->rects_tmp.clear();

	for (const auto& s : this->sub_windows) {
		this->rects_tmp.emplace(s.first, s.second.get_rect());
	}

	this->sa(this->rects_tmp, this->best_rects);

	this->old_rects = move(this->rects_tmp);

	this->make_optimize_thread();
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

		auto alpha{ ofColor::limit() };

		for (const auto& sw : this->sub_windows) {
			if (sw.second.get_rect().inside(ud.second.transformed_cursor_point.x, ud.second.transformed_cursor_point.y)) {
				alpha = 100;
				break;
			}
		}

		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white, alpha);
		ofDrawCircle(ud.second.transformed_cursor_point.x, ud.second.transformed_cursor_point.y, 60);
		ofFill();
		ofSetColor(ud.second.cursor_color, alpha);
		ofDrawCircle(ud.second.transformed_cursor_point.x, ud.second.transformed_cursor_point.y, 55);
	}
}

bool SceneManager::is_intersect_window_pointer() {
	// �T�u�E�B���h�E�ƃ|�C���^�̎��ӗ̈悪�d�����邩�ǂ����𒲂ׂ�
	for (const auto& sw : this->sub_windows) {
		for (const auto& ud : this->hc->user_data) {
			if (ud.second.state == HandCursor::STATE::INACTIVE) {
				continue;
			}
			if (sw.second.get_rect().intersects(ofRectangle(ofClamp(ud.second.transformed_cursor_point.x - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.transformed_cursor_point.y - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight()))) {
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