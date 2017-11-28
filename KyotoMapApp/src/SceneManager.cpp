#include "SceneManager.h"
#include "KyotoTowerScene.h"
#include "windows.h"
#include "RectangleOptimization.h"
#include "HigashihonganjiScene.h"
#include "SyoseienScene.h"
#include "NishihonganjiScene.h"
#include "RyukokuMuseumScene.h"
#include "AppParameters.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace param;

void SceneManager::transform(unordered_map<int, ofRectangle> &old_rects, unordered_map<int, ofRectangle> &new_rects) {
	this->transform_thread_flag = true;

	const double change_rate = 0.05;
	const int change_times = 20;

	int x_sign, y_sign, w_sign, h_sign;
	double x_change_val, y_change_val, w_change_val, h_change_val;

	for (const auto &id : this->active_scene_id_list_tmp) {
		w_sign = (new_rects[id].width > old_rects[id].width) ? +1 : -1;
		h_sign = (new_rects[id].height > old_rects[id].height) ? +1 : -1;
		w_change_val = change_rate*abs(new_rects[id].width - old_rects[id].width)*w_sign;
		h_change_val = change_rate*abs(new_rects[id].height - old_rects[id].height)*h_sign;

		for (int i = 0; i < change_times; ++i) {
			if (find(begin(this->active_scene_id_list), end(this->active_scene_id_list), id) == end(this->active_scene_id_list)) {
				goto FINISH;
			}
			this->mtx.lock();
			this->sub_windows[id].set_rect(old_rects[id]);
			this->sub_windows[id].set_frame();
			this->mtx.unlock();
			old_rects[id].setWidth(old_rects[id].width + w_change_val);
			old_rects[id].setHeight(old_rects[id].height + h_change_val);
		}

		x_sign = (new_rects[id].x > old_rects[id].x) ? +1 : -1;
		y_sign = (new_rects[id].y > old_rects[id].y) ? +1 : -1;
		x_change_val = change_rate*abs(new_rects[id].x - old_rects[id].x)*x_sign;
		y_change_val = change_rate*abs(new_rects[id].y - old_rects[id].y)*y_sign;

		for (int i = 0; i < change_times; ++i) {
			if (find(begin(this->active_scene_id_list), end(this->active_scene_id_list), id) == end(this->active_scene_id_list)) {
				goto FINISH;
			}
			this->mtx.lock();
			this->sub_windows[id].set_rect(old_rects[id]);
			this->sub_windows[id].set_frame();
			this->mtx.unlock();
			old_rects[id].setX(old_rects[id].x + x_change_val);
			old_rects[id].setY(old_rects[id].y + y_change_val);
		}

	}
	FINISH:
	this->transform_thread_flag = false;
}

void SceneManager::setup(HandCursor* hc) {
	/* メインシーンの準備 */
	this->main_scene.setup(hc);
	ofAddListener(this->main_scene.point_event, this, &SceneManager::pointed);
	ofAddListener(this->main_scene.make_sub_window_event, this, &SceneManager::make_sub_window);

	this->sa.setup(hc, &this->active_scene_id_list, &this->main_scene.user_id_list, &this->sub_windows);

	this->hc = hc;
}

void SceneManager::update() {
	if (!this->transform_thread_flag) {
		while (!this->erase_scene_id.empty()) {
			this->sub_windows.erase(this->erase_scene_id.front());
			this->active_scene_id_list.erase(find(begin(this->active_scene_id_list), end(this->active_scene_id_list), this->erase_scene_id.front()));
			this->erase_scene_id.pop();
		}
	}

	if (!this->transform_thread_flag && !this->sub_windows.empty() && !this->active_scene_id_list.empty()) {
		this->rects_tmp.clear();

		for (const auto &s : this->sub_windows) {
			this->rects_tmp.insert(make_pair(s.first, s.second.get_rect()));
		}

		this->active_scene_id_list_tmp = this->active_scene_id_list;

		this->old_rects = this->rects_tmp;

		this->sa(this->rects_tmp, this->best_rects);

		void(SceneManager::*funcp)(unordered_map<int, ofRectangle> &old_rects, unordered_map<int, ofRectangle> &new_rects) = &SceneManager::transform;
		thread th(funcp, this, this->old_rects, this->best_rects);
		th.detach();
	}

	/* 新しく検出したカーソルがあればメインシーンのユーザidリストに追加する */
	for (const auto &t : this->hc->track_data) {
		if (this->cursor_log.find(t.first) == end(this->cursor_log)) {
			this->cursor_log.emplace(t.first);
			this->main_scene.user_id_list.emplace_back(t.first);
		}
	}

	/* 消滅したカーソルがあればメインシーンのユーザidリストから消す */
	for (auto id = begin(this->main_scene.user_id_list); id != end(this->main_scene.user_id_list);) {
		if (this->hc->track_data.find(*id) == end(this->hc->track_data)) {
			id = this->main_scene.user_id_list.erase(id);
		}
		else {
			++id;
		}
	}

	this->main_scene.update(); // メインシーンの更新

	for (auto &ss : this->sub_windows) {
		ss.second.update();
	}
}

void SceneManager::draw() {
	this->main_scene.draw();

	while (!this->erase_scene_id.empty()) {
		this->sub_windows.erase(this->erase_scene_id.front());
		this->erase_scene_id.pop();
	}

	for (auto &ss : this->sub_windows) {
		ss.second.draw();
	}

}

void SceneManager::pointed(pair<int, int> &id) {
	this->main_scene.point_icon(id.first, id.second);
}

void SceneManager::inactivate_sub_window(int &scene_id) {
	auto ite = find(begin(this->active_scene_id_list), end(this->active_scene_id_list), scene_id);
	if (ite != end(this->active_scene_id_list)) {
		this->active_scene_id_list.erase(ite);
	}
}

void SceneManager::make_sub_window(pair<int, int>& id) {
	if (this->sub_windows.empty()) {
		SubWindow sub_window;
		switch (id.first) {
		case static_cast<int>(CONTENT_ID::KYOTO_TOWER):
			sub_window.setup(new KyotoTowerScene(), this->hc, id.second, this->scene_id, ofRectangle(200, 200, W / 2, H / 2));
			break;
		case static_cast<int>(CONTENT_ID::HIGASHIHONGANJI) :
			sub_window.setup(new HigashihonganjiScene(), this->hc, id.second, this->scene_id, ofRectangle(200, 200, W / 2, H / 2));
			break;
		case static_cast<int>(CONTENT_ID::SYOSEIEN) :
			sub_window.setup(new SyoseienScene(), this->hc, id.second, this->scene_id, ofRectangle(200, 200, W / 2, H / 2));
			break;
		case static_cast<int>(CONTENT_ID::NISHIHONGANJI) :
			sub_window.setup(new NishihonganjiScene(), this->hc, id.second, this->scene_id, ofRectangle(200, 200, W / 2, H / 2));
			break;
		case static_cast<int>(CONTENT_ID::RYUKOKU_MUSEUM) :
			sub_window.setup(new RyukokuMuseumScene(), this->hc, id.second, this->scene_id, ofRectangle(200, 200, W / 2, H / 2));
			break;
		}
		
		ofAddListener(sub_window.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		ofAddListener(sub_window.cursor_disappear_event, this, &SceneManager::inactivate_sub_window);
		this->sub_windows.insert(make_pair(this->scene_id, sub_window));
		this->main_scene.user_id_list.erase(remove(begin(this->main_scene.user_id_list), end(this->main_scene.user_id_list), id.second), end(this->main_scene.user_id_list));

		this->active_scene_id_list.emplace_back(this->scene_id++);
	}
	else {
		RectangleOptimization ro(W, H);

		for (const auto &s : this->sub_windows) {
			ro.add_block(s.second.get_rect());
		}

		for (const auto &td : this->hc->track_data) {
			if (td.first == id.second) {
				continue;
			}
			ro.add_block(ofPoint(td.second.current_pointer.x, td.second.current_pointer.y));
		}

		ro.calculate();

		SubWindow sub_window;
		switch (id.first) {
		case static_cast<int>(CONTENT_ID::KYOTO_TOWER) :
			sub_window.setup(new KyotoTowerScene(), this->hc, id.second, this->scene_id, ro.get_optimize_rect());
			break;
		case static_cast<int>(CONTENT_ID::HIGASHIHONGANJI) :
			sub_window.setup(new HigashihonganjiScene(), this->hc, id.second, this->scene_id, ro.get_optimize_rect());
			break;
		case static_cast<int>(CONTENT_ID::SYOSEIEN) :
			sub_window.setup(new SyoseienScene(), this->hc, id.second, this->scene_id, ro.get_optimize_rect());
			break;
		case static_cast<int>(CONTENT_ID::NISHIHONGANJI) :
			sub_window.setup(new NishihonganjiScene(), this->hc, id.second, this->scene_id, ro.get_optimize_rect());
			break;
		case static_cast<int>(CONTENT_ID::RYUKOKU_MUSEUM) :
			sub_window.setup(new RyukokuMuseumScene(), this->hc, id.second, this->scene_id, ro.get_optimize_rect());
			break;
		}
		ofAddListener(sub_window.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		ofAddListener(sub_window.cursor_disappear_event, this, &SceneManager::inactivate_sub_window);
		this->sub_windows.insert(make_pair(this->scene_id, sub_window));
		this->main_scene.user_id_list.erase(remove(begin(this->main_scene.user_id_list), end(this->main_scene.user_id_list), id.second), end(this->main_scene.user_id_list));

		this->active_scene_id_list.emplace_back(this->scene_id++);

	}
}

void SceneManager::delete_sub_window(int &scene_id) {
	this->sub_windows[scene_id].exit();
	this->erase_scene_id.push(scene_id);
}

SceneManager::~SceneManager() {
	ofRemoveListener(this->main_scene.point_event, this, &SceneManager::pointed);
	ofRemoveListener(this->main_scene.make_sub_window_event, this, &SceneManager::make_sub_window);

	for (auto &s : this->sub_windows) {
		s.second.exit();
	}
}