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

	this->ab.setup(&this->menu_item_user_id);

	this->sa.setup(this->hc, &this->sub_windows);
}

void SceneManager::update() {
	if (!this->transform_thread_flag && (this->is_intersect_window_pointer() || this->is_intersect_window_window()) && all_of(begin(this->sub_windows), end(this->sub_windows), [](const auto& x) {return x.second.track_index == SubWindow::TRACK_READY; })) {
		this->rects_tmp.clear();

		for (const auto& s : this->sub_windows) {
			this->rects_tmp.emplace(s.first, s.second.get_rect());
		}

		this->sa(this->rects_tmp, this->best_rects);

		this->old_rects = move(this->rects_tmp);

		void(SceneManager::*funcp)(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects) = &SceneManager::transform;
		thread th(funcp, this, this->old_rects, this->best_rects);
		th.detach();
	}

	/* サブウィンドウの更新 */
	/* いなくなったユーザのサブウィンドウを削除 */
	for (auto ite = begin(this->sub_windows); ite != end(this->sub_windows);) {
		if (this->hc->user_data.find(ite->second.get_user_id()) == end(this->hc->user_data)) {
			ite->second.exit();
			ite = this->sub_windows.erase(ite);
		}
		else {
			++ite;
		}
	}

	for (auto&& w : this->sub_windows) {
		w.second.update();
	}

	this->mb.update(); // メニューバーの更新

	// ピンの更新
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_user_id[i] == NOT_USER) {
			continue;
		}
		for (auto&& p : this->pins[i]) {
			for (const auto& ud : this->hc->user_data) {
				if (ud.second.state == HandCursor::STATE::INACTIVE) {
					continue;
				}
				if (p.is_inside(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y())) {
					p.point(ud.first);
					goto CONTINUE_LOOP;
				}
			}
			p.reset_state();

		CONTINUE_LOOP:
			p.update();
		}
	}
	this->ab.update(); // 広告バーの更新
}

void SceneManager::draw() {
	ofSetColor(ofColor::white);
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H); // マップの表示

	this->mb.draw(); // メニューバーの表示

	/* ピンの描画 */
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_user_id[i] == NOT_USER) {
			continue;
		}
		for (const auto& p : this->pins[i]) {
			p.draw();
		}
	}

	this->ab.draw(); // 広告バーの描画

	this->draw_cursor(); // 手カーソルの描画

	// サブウィンドウの描画
	for (auto&& w : this->sub_windows) {
		w.second.draw();
	}
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

void SceneManager::add_pin(pair<param::MENU_ITEM_ID, long long int>& id) {
	this->menu_item_user_id[static_cast<int>(id.first)] = id.second;
}

void SceneManager::make_sub_window(pair<param::CONTENT_ID, long long int>& id) {
	/* 既に同じコンテンツのサブウィンドウがあったら、新たにサブウィンドウを生成しない */
	if (this->sub_windows.find(static_cast<int>(id.first)) != end(this->sub_windows)) {
		return;
	}

	auto ite{ find_if(begin(this->sub_windows), end(this->sub_windows),
		[this, id](const auto& x) {return x.second.get_user_id() == id.second; }
	) };
	if (ite != end(this->sub_windows)) { // そのユーザがすでにサブウィンドウを生成していたら
		/* そのサブウィンドウを終了・削除する */
		ite->second.exit();
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
		ofSetColor(ofColor::white);
		ofFill();
		ofSetColor(ud.second.cursor_color);
		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 55);
	}
}

bool SceneManager::is_intersect_window_pointer() {
	// サブウィンドウとポインタの周辺領域が重複するかどうかを調べる
	for (const auto& sw : this->sub_windows) {
		for (const auto& ud : this->hc->user_data) {
			if (ud.second.state == HandCursor::STATE::INACTIVE) {
				continue;
			}
			if (sw.second.get_rect().intersects(ofRectangle(ofClamp(ud.second.transformed_cursor_point.x() - USER_CERTAIN_WINDOW.getX(), 0, DISPLAY_W), ofClamp(ud.second.transformed_cursor_point.y() - USER_CERTAIN_WINDOW.getY(), 0, DISPLAY_H), USER_CERTAIN_WINDOW.getWidth(), USER_CERTAIN_WINDOW.getHeight()))) {
				return true;
			}
		}
	}

	return false;
}

bool SceneManager::is_intersect_window_window() {
	// サブウィンドウ同士が重複するかどうかを調べる
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