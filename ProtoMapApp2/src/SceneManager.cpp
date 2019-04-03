#include "SceneManager.h"

using namespace param;
using namespace cv;

const ofColor SceneManager::default_face_color{ ofColor::gray };

void SceneManager::setup(HandCursor* const hc) {
	this->hc = hc;

	this->mb.setup(hc);
	ofAddListener(this->mb.add_pin_event, this, &SceneManager::update_pin);

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
	if (!this->sub_windows.empty()) {
		this->optimize(); // �œK��
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
			this->release_menu_item(i);
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

	this->draw_face(); // ��A�C�R���̕`��

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

	if (this->make_sub_window_flag || (this->comparative_cost > this->best_cost && this->comparative_cost - this->best_cost > this->transform_threshold)) {
		this->make_sub_window_flag = false;

		this->previous_sub_windows = this->sub_windows;

		/* �œK�ȃp�����[�^���Z�b�g */
		for (const auto& id : this->sub_windows) {
			this->sub_windows[id.first].set_rect(this->best_rects[id.first]);
		}

		this->previous_rects = move(this->best_rects); // ���(�p�����[�^)���X�V
	}
}

void SceneManager::update_pin(pair<param::MENU_ITEM_ID, long long int>& id) {
	auto ite{ find(cbegin(this->menu_item_user_id), cend(this->menu_item_user_id), id.second) };
	if (ite != end(this->menu_item_user_id)) { // ���ɂǂ����̃��j���[���ڂ�I�����Ă�����
		this->release_menu_item(distance(cbegin(this->menu_item_user_id), ite)); // �I������Ă������j���[���ڂ�����i�s�����\���ɂ���j
	}

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

void SceneManager::draw_face() const {
	const auto people_num{ this->hc->pose_key_points.getSize(0) };
	for (int i = 0; i < people_num; ++i) {
		if (this->hc->pose_key_points[NOSE_X(i)] == 0.0 || this->hc->pose_key_points[NOSE_Y(i)] == 0.0) {
			continue;
		}

		Point transformed_point;
		this->hc->transform_point(Point(this->hc->pose_key_points[NOSE_X(i)], this->hc->pose_key_points[NOSE_Y(i)]), transformed_point);
		if (this->hc->personal_id2user_id[i] == NOT_USER) {
			ofSetColor(this->default_face_color, HALF_MAX_ALFHA);
			const auto face_size = this->face_rate*this->hc->estimate_face_size(i);
			this->face_image.draw(transformed_point.x - face_size / 2, transformed_point.y - face_size / 2, face_size, face_size);
		}
		else {
#define user(i) this->hc->user_data[this->hc->personal_id2user_id[i]]
			if (user(i).face_blink_count != 0) {
				ofSetColor(user(i).cursor_color, THREE_QUARTER_MAX_ALFHA*user(i).face_blink_count / FACE_BLINK_INTERVAL);

				this->face_image.draw(transformed_point.x - this->face_rate * user(i).face_size / 2, transformed_point.y - this->face_rate * user(i).face_size / 2, this->face_rate * user(i).face_size, this->face_rate * user(i).face_size);

				--user(i).face_blink_count;
			}
			else {
				user(i).face_blink_count = FACE_BLINK_INTERVAL;
			}
#undef user(i)
		}
	}
}

void SceneManager::draw_cursor() const {
	for (const auto& ud : this->hc->user_data) {
		if (ud.second.state == HandCursor::STATE::INACTIVE) {
			continue;
		}

		ofSetColor(ud.second.cursor_color, ud.second.cursor_alpha);
		if (ud.second.hand == HandCursor::USING_HAND::RIGHT) {
			this->right_hand_image.draw(ud.second.cursor_point.x - this->cursor_size / 2, ud.second.cursor_point.y - this->cursor_size / 2, this->cursor_size, this->cursor_size);
		}
		else {
			this->left_hand_image.draw(ud.second.cursor_point.x - this->cursor_size / 2, ud.second.cursor_point.y - this->cursor_size / 2, this->cursor_size, this->cursor_size);
		}
	}
}

void SceneManager::release_menu_item(const int menu_item_id) {
	this->menu_item_life[menu_item_id] = this->max_menu_item_life;
	this->menu_item_user_id[menu_item_id] = NOT_USER;
	this->mb.deactivate_menu_item(menu_item_id);
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->mb.add_pin_event, this, &SceneManager::update_pin);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		for (auto&& p : this->pins[i]) {
			ofRemoveListener(p.make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}
}