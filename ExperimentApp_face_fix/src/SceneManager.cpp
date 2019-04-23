#include "SceneManager.h"

using namespace param;
using namespace cv;

const ofColor SceneManager::default_face_color{ ofColor::gray };

void SceneManager::setup(HandCursor* const hc) {
	this->hc = hc;

	this->mb.setup(hc);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->pins[i].resize(MENU_ITEM_CONTENTS[i].size());
		for (int j = 0; j < this->pins[i].size(); ++j) {
			this->pins[i][j].setup(MENU_ITEM_CONTENTS[i][j]);
			ofAddListener(this->pins[i][j].make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}
}

void SceneManager::update() {
	/* いなくなったユーザのサブウィンドウを削除 */
	for (auto ite = begin(this->sub_windows); ite != end(this->sub_windows);) {
		if (this->hc->user_data.find(ite->second.get_user_id()) == end(this->hc->user_data)) {
			ite = this->sub_windows.erase(ite);
		}
		else {
			++ite;
		}
	}

	this->mb.update(); // メニューバーの更新

	/* カーソルがウィンドウと重複するかどうかを調べて、カーソルの状態を変更する */
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

	// ピンの更新
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (!this->mb.is_activated(i)) {
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
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H); // マップの表示

	this->mb.draw(); // メニューバーの表示

	/* ピンの描画 */
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (!this->mb.is_activated(i)) {
			continue;
		}

		for (const auto& p : this->pins[i]) {
			p.draw();
		}
	}

	// サブウィンドウの描画
	for (auto&& w : this->sub_windows) {
		w.second.draw();
	}

	this->draw_face(); // 顔アイコンの描画

	this->draw_cursor(); // 手カーソルの描画
}

void SceneManager::make_sub_window(pair<param::CONTENT_ID, long long int>& id) {
	this->make_sub_window_flag = true;

	/* 既に同じコンテンツのサブウィンドウがあったら、新たにサブウィンドウを生成しない */
	if (this->sub_windows.find(static_cast<int>(id.first)) != end(this->sub_windows)) {
		return;
	}

	++this->pin_selected_count[static_cast<int>(id.first) / cbegin(pins)->size()];

	const auto ite{ find_if(cbegin(this->sub_windows), cend(this->sub_windows),
		[this, id](const auto& x) {return x.second.get_user_id() == id.second; }
	) };
	if (ite != cend(this->sub_windows)) { // そのユーザがすでにサブウィンドウを生成していたら
		/* そのサブウィンドウを削除する */
		this->sub_windows.erase(ite->first);
	}

	this->sub_windows.emplace(static_cast<int>(id.first), SubWindow{ id.first, id.second, this->hc->user_data[id.second].cursor_color, ofPoint(this->hc->user_data[id.second].first_face_point.x, this->hc->user_data[id.second].first_face_point.y) });
}

void SceneManager::switch_menu_item(const int n) {
	if (this->mb.is_activated(n)) {
		this->mb.deactivate_menu_item(n);
	}
	else {
		this->mb.activate_menu_item(n);
	}
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

SceneManager::~SceneManager() {
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		for (auto&& p : this->pins[i]) {
			ofRemoveListener(p.make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}
}