#include "SceneManager.h"

using namespace param;

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

	//this->cost_change_plotter.set_line_color(ofColor::lightGreen);
	//this->cost_change_plotter.set_window_position(DISPLAY_W - 1024, 0);
	//this->cost_change_plotter.set_range(-10000, 10000);

	this->kpr.setup(this->hc);
}

void SceneManager::update() {
	if (!this->sub_windows.empty()) {
		this->optimize(); // 最適化
	}

	//this->cost_change_plotter.update(this->comparative_cost - this->best_cost);

	/* サブウィンドウの更新 */
	/* いなくなったユーザのサブウィンドウを削除 */
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

	/* メニュー項目の更新 */
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

	//for (const auto& ud : this->hc->user_data) {
	//	ofSetColor(ud.second.cursor_color, 120);
	//	this->face_image.draw(ud.second.transformed_face_point.x(), ud.second.transformed_face_point.y(), 200, 200);
	//}

	// サブウィンドウの描画
	for (auto&& w : this->sub_windows) {
		w.second.draw();
	}

	this->draw_cursor(); // 手カーソルの描画

	//this->cost_change_plotter.draw();

	//ofSetColor(ofColor::deepPink);
	//this->hand_image.draw(500, 500, 200, 200); // マップの表示
	//ofSetColor(ofColor::white);

	this->kpr.draw();
}

void SceneManager::optimize() {
	/* 焼きなまし法への入力の準備(現在のウィンドウ状態から初期状態を作成) */
	this->initial_rects.clear();
	for (const auto& s : this->sub_windows) {
		this->initial_rects.emplace(s.first, s.second.get_rect());
	}

	this->sa(this->initial_rects, this->best_rects, this->best_cost); // 焼きなまし法で最適化

	/* 比較するコストを計算(直前のウィンドウ状態かつ現在のユーザ情報を使って) */
	if (!this->previous_rects.empty() && !this->previous_sub_windows.empty()) {
		this->sa.calculate_cost(this->previous_rects, this->comparative_cost, this->previous_sub_windows);
	}

	if (this->make_sub_window_flag || (this->comparative_cost > this->best_cost && this->comparative_cost - this->best_cost > this->transform_threshold)) {
		this->make_sub_window_flag = false;

		this->previous_sub_windows = this->sub_windows;

		/* 最適なパラメータをセット */
		for (const auto& id : this->sub_windows) {
			this->sub_windows[id.first].set_rect(this->best_rects[id.first]);
		}

		this->previous_rects = move(this->best_rects); // 状態(パラメータ)を更新
	}
}

void SceneManager::update_pin(pair<param::MENU_ITEM_ID, long long int>& id) {
	auto ite{ find(cbegin(this->menu_item_user_id), cend(this->menu_item_user_id), id.second) };
	if (ite != end(this->menu_item_user_id)) { // 既にどこかのメニュー項目を選択していたら
		this->release_menu_item(distance(cbegin(this->menu_item_user_id), ite)); // 選択されていたメニュー項目を解放（ピンを非表示にする）
	}

	this->menu_item_user_id[static_cast<int>(id.first)] = id.second;
}

void SceneManager::make_sub_window(pair<param::CONTENT_ID, long long int>& id) {
	this->make_sub_window_flag = true;

	/* 既に同じコンテンツのサブウィンドウがあったら、新たにサブウィンドウを生成しない */
	if (this->sub_windows.find(static_cast<int>(id.first)) != end(this->sub_windows)) {
		return;
	}

	const auto ite{ find_if(cbegin(this->sub_windows), cend(this->sub_windows),
		[this, id](const auto& x) {return x.second.get_user_id() == id.second; }
	) };
	if (ite != cend(this->sub_windows)) { // そのユーザがすでにサブウィンドウを生成していたら
		/* そのサブウィンドウを削除する */
		this->sub_windows.erase(ite->first);
	}

	this->sub_windows.emplace(static_cast<int>(id.first), SubWindow{ id.first, id.second, this->hc->user_data[id.second].cursor_color });
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

		//ofSetColor(ud.second.cursor_color, ud.second.alpha);
		//if (ud.second.hand == HandCursor::USING_HAND::RIGHT) {
		//	this->right_hand_image.draw(ud.second.cursor_point.x - 125, ud.second.cursor_point.y - 125, 250, 250);
		//}
		//else {
		//	this->left_hand_image.draw(ud.second.cursor_point.x - 125, ud.second.cursor_point.y - 125, 250, 250);
		//}


		//if (ud.second.cursor_point.x >= 800 || ud.second.cursor_point.y >= 800) {
		//this->hand_image.draw(ud.second.cursor_point.x-125, ud.second.cursor_point.y-125, 250, 250);
		//}
		//else {
			//this->hand_grab_image.draw(ud.second.cursor_point.x+50-100, ud.second.cursor_point.y+50-100, 200, 200);
		//}
		
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