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

	this->ab.setup(&this->menu_item_flag);

	this->sa.setup(this->hc, &this->sub_windows);
}

void SceneManager::update() {
	if (!this->transform_thread_flag && !this->sub_windows.empty()) {
		if (any_of(begin(this->sub_windows), end(this->sub_windows), [](const auto& x) {return x.second.track_index != SubWindow::TRACK_READY; })) {
			goto THROUGH_OPT;
		}

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
THROUGH_OPT:
	for (auto&& w : this->sub_windows) {
		w.second.update();
	}

	this->mb.update();

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_flag[i]) {
			for (auto&& p : this->pins[i]) {
				for (const auto& ud : this->hc->user_data) {
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
	}

	this->ab.update();
}

void SceneManager::draw() {
	ofSetColor(ofColor::white);
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H); // マップの表示

	this->mb.draw(); // メニューバーの表示

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		if (this->menu_item_flag[i]) {
			for (const auto& p : this->pins[i]) {
				p.draw();
			}
		}
	}

	this->ab.draw();

	/* 手カーソルの描画 */
	for (const auto& ud : this->hc->user_data) {
		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white);
		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 60);
		ofFill();
		ofSetColor(ud.second.cursor_color);
		ofDrawCircle(ud.second.transformed_cursor_point.x(), ud.second.transformed_cursor_point.y(), 55);
	}

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

void SceneManager::add_pin(param::MENU_ITEM_ID& item_id) {
	this->menu_item_flag[static_cast<int>(item_id)] = true;
}

void SceneManager::make_sub_window(pair<param::CONTENT_ID, long long int>& id) {
	this->sub_windows.emplace(static_cast<int>(id.first), SubWindow{ id.first, id.second });
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->mb.add_pin_event, this, &SceneManager::add_pin);

	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		for (auto&& p : this->pins[i]) {
			ofRemoveListener(p.make_sub_window_event, this, &SceneManager::make_sub_window);
		}
	}
}