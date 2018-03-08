#include "SceneManager.h"

using namespace cv;
using namespace param;

random_device SceneManager::rd;
mt19937 SceneManager::mt(SceneManager::rd());

void SceneManager::transform(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects) {
	this->transform_thread_flag = true;

	const double change_rate{ 1.0 / SubWindow::track_rects_num };

	int x_sign, y_sign, w_sign, h_sign;
	double x_change_val, y_change_val, w_change_val, h_change_val;

	for (const auto& id : this->active_scene_id_list_tmp) {
		w_sign = (new_rects[id].width > old_rects[id].width) ? +1 : -1;
		h_sign = (new_rects[id].height > old_rects[id].height) ? +1 : -1;
		w_change_val = change_rate*abs(new_rects[id].width - old_rects[id].width)*w_sign;
		h_change_val = change_rate*abs(new_rects[id].height - old_rects[id].height)*h_sign;
		x_sign = (new_rects[id].x > old_rects[id].x) ? +1 : -1;
		y_sign = (new_rects[id].y > old_rects[id].y) ? +1 : -1;
		x_change_val = change_rate*abs(new_rects[id].x - old_rects[id].x)*x_sign;
		y_change_val = change_rate*abs(new_rects[id].y - old_rects[id].y)*y_sign;
		try {
			for (int i = 0; i < SubWindow::track_rects_num; ++i) {
				this->sub_windows.at(id).track_rects[i] = old_rects[id];
				old_rects[id].setWidth(old_rects[id].width + w_change_val);
				old_rects[id].setHeight(old_rects[id].height + h_change_val);
				old_rects[id].setX(old_rects[id].x + x_change_val);
				old_rects[id].setY(old_rects[id].y + y_change_val);
			}
			this->sub_windows.at(id).track_index = 0;
		}
		catch (std::out_of_range&) {}
	}

	this->transform_thread_flag = false;
}

void SceneManager::setup(HandCursor* hc) {
	/* メインシーンの準備 */
	this->main_scene.setup(hc);
	ofAddListener(this->main_scene.point_event, this, &SceneManager::pointed); // ときどき、ここでなぜかmutex関係のエラーが出る
	ofAddListener(this->main_scene.make_sub_window_event, this, &SceneManager::make_sub_window);

	//this->sa.setup(hc, &this->active_scene_id_list, &this->main_scene.user_id_list, &this->sub_windows);

	this->hc = hc;

	this->ga.setup(this->hc);

	thread optimize_thread(&SceneManager::optimize, this);
	optimize_thread.detach();
}

void SceneManager::update() {
	//	if (!this->transform_thread_flag && !this->sub_windows.empty() && !this->active_scene_id_list.empty()) {
	//		for (const auto& s : this->sub_windows) {
	//			if (s.second.track_index != SubWindow::TRACK_READY) {
	//				goto THROUGH_OPT;
	//			}
	//		}
	//
	//		this->rects_tmp.clear();
	//
	//		for (const auto& s : this->sub_windows) {
	//			this->rects_tmp.insert(make_pair(s.first, s.second.get_rect()));
	//		}
	//
	//		this->active_scene_id_list_tmp = this->active_scene_id_list;
	//
	//		this->old_rects = this->rects_tmp;
	//
	//		this->sa(this->rects_tmp, this->best_rects);
	//
	//		void(SceneManager::*funcp)(unordered_map<long long int, ofRectangle>& old_rects, unordered_map<long long int, ofRectangle>& new_rects) = &SceneManager::transform;
	//		thread th(funcp, this, this->old_rects, this->best_rects);
	//		th.detach();
	//	}
	//THROUGH_OPT:
		/* 新しく検出したカーソルがあればメインシーンのユーザidリストに追加する */
	for (const auto& t : this->hc->user_data) {
		if (this->cursor_log.find(t.first) == end(this->cursor_log)) {
			this->cursor_log.emplace(t.first);
			this->main_scene.user_id_list.emplace(t.first);
		}
	}

	/* 消滅したカーソルがあればメインシーンのユーザidリストから消す */
	for (auto id = begin(this->main_scene.user_id_list); id != end(this->main_scene.user_id_list);) {
		if (this->hc->user_data.find(*id) == end(this->hc->user_data)) {
			id = this->main_scene.user_id_list.erase(id);
		}
		else {
			++id;
		}
	}

	this->main_scene.update(); // メインシーンの更新

	/* 消滅したカーソルがあれば選択しているユーザリストから消す */
	for (auto id = begin(this->selected_users); id != end(this->selected_users);) {
		if (this->hc->user_data.find(*id) == end(this->hc->user_data)) {
			this->selected_users.erase(id++);
		}
		else {
			++id;
		}
	}

	//if (!this->optimize_flag) {
	//	thread optimize_thread(&SceneManager::optimize, this);
	//	optimize_thread.detach();
	//}

	//this->ga(this->selected_users, this->main_scene.user_id_list);

	/*for (auto &ss : this->sub_windows) {
		ss.second.update();
	}

	while (!this->delete_scene_list.empty()) {
		this->sub_windows[this->delete_scene_list.front()].exit();
		this->sub_windows.erase(this->delete_scene_list.front());
		this->delete_scene_list.pop();
	}*/
}

void SceneManager::draw() {
	this->main_scene.draw();

	if (!this->selected_users.empty()) {
		this->ga.draw();
	}

	//for (auto &ss : this->sub_windows) {
	//	ss.second.draw();
	//}
}

void SceneManager::exit() {
	this->stop_flag = true;
}

void SceneManager::pointed(pair<int, long long int>& id) {
	this->main_scene.point_icon(id.first, id.second);
}

void SceneManager::make_sub_window(pair<int, long long int>& id) {
	this->selected_users.emplace(id.second);
	//this->main_scene.user_id_list.erase(id.second);

	/*SubWindow sub_window;
	switch (id.first) {
	case static_cast<int>(CONTENT_ID::KYOTO_TOWER) :
		this->sub_window_scene.reset(new KyotoTowerScene());
		sub_window.setup(move(this->sub_window_scene), this->hc, id.second, this->scene_id, ofRectangle(ofClamp(2000 - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(1560 - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)));
		break;
	case static_cast<int>(CONTENT_ID::HIGASHIHONGANJI) :
		this->sub_window_scene.reset(new HigashihonganjiScene());
		sub_window.setup(move(this->sub_window_scene), this->hc, id.second, this->scene_id, ofRectangle(ofClamp(2000 - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(1100 - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)));
		break;
	case static_cast<int>(CONTENT_ID::SYOSEIEN) :
		this->sub_window_scene.reset(new SyoseienScene());
		sub_window.setup(move(this->sub_window_scene), this->hc, id.second, this->scene_id, ofRectangle(ofClamp(2700 - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(1100 - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)));
		break;
	case static_cast<int>(CONTENT_ID::NISHIHONGANJI) :
		this->sub_window_scene.reset(new NishihonganjiScene());
		sub_window.setup(move(this->sub_window_scene), this->hc, id.second, this->scene_id, ofRectangle(ofClamp(880 - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(1100 - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)));
		break;
	case static_cast<int>(CONTENT_ID::RYUKOKU_MUSEUM) :
		this->sub_window_scene.reset(new RyukokuMuseumScene());
		sub_window.setup(move(this->sub_window_scene), this->hc, id.second, this->scene_id, ofRectangle(ofClamp(1280 - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(700 - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)));
		break;
	case static_cast<int>(CONTENT_ID::KYOTO_AQUARIUM) :
		this->sub_window_scene.reset(new KyotoAquariumScene());
		sub_window.setup(move(this->sub_window_scene), this->hc, id.second, this->scene_id, ofRectangle(ofClamp(100 - HALF_MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(1510 - HALF_MAX_SUB_WINDOW_H, 0, DISPLAY_H), ofClamp(MAX_SUB_WINDOW_W, 0, DISPLAY_W), ofClamp(MAX_SUB_WINDOW_H, 0, DISPLAY_H)));
		break;
	}

	ofAddListener(sub_window.delete_sub_window_event, this, &SceneManager::delete_sub_window);

	this->sub_windows.emplace(this->scene_id, move(sub_window));
	this->main_scene.user_id_list.erase(remove(begin(this->main_scene.user_id_list), end(this->main_scene.user_id_list), id.second), end(this->main_scene.user_id_list));

	this->active_scene_id_list.emplace_back(this->scene_id++);*/
}

void SceneManager::delete_sub_window(long long int& scene_id) {
	auto ite = find(cbegin(this->active_scene_id_list), cend(this->active_scene_id_list), scene_id);
	if (ite != cend(this->active_scene_id_list)) {
		this->active_scene_id_list.erase(ite);
	}

	this->delete_scene_list.push(scene_id);
}

void SceneManager::optimize() {
	while (!this->stop_flag) {
		//this->optimize_flag = true;
		if (!this->selected_users.empty() && !this->main_scene.user_id_list.empty()) {
			this->ga(this->selected_users, this->main_scene.user_id_list);
		}
		//this->optimize_flag = false;
	}
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->main_scene.point_event, this, &SceneManager::pointed);
	ofRemoveListener(this->main_scene.make_sub_window_event, this, &SceneManager::make_sub_window);

	for (auto &s : this->sub_windows) {
		s.second.exit();
	}
}