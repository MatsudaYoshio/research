#include "SceneManager.h"
#include "MainScene.h"
#include "DetailScene.h"
#include "windows.h"
#include "MoveWindowPosition.h"
#include "RectangleOptimization.h"

#include <opencv2/opencv.hpp>

using namespace cv;

double euclid_distance(const double x1, const double y1, const double x2, const double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void SceneManager::transform(unordered_map<int, ofRectangle> &old_rects, unordered_map<int, ofRectangle> &new_rects) {
	this->flag = true;

	for (auto &s : this->sub_scenes) {
		if (old_rects[s.first].x < new_rects[s.first].x) {
			while (old_rects[s.first].x < new_rects[s.first].x) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setX(old_rects[s.first].x + 10);
			}
		}
		else {
			while (old_rects[s.first].x > new_rects[s.first].x) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setX(old_rects[s.first].x - 10);
			}
		}

		if (old_rects[s.first].y < new_rects[s.first].y) {
			while (old_rects[s.first].y < new_rects[s.first].y) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setY(old_rects[s.first].y + 10);
			}
		}
		else {
			while (old_rects[s.first].y > new_rects[s.first].y) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setY(old_rects[s.first].y - 10);
			}
		}

		if (old_rects[s.first].width < new_rects[s.first].width) {
			while (old_rects[s.first].width < new_rects[s.first].width) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setWidth(old_rects[s.first].width + 10);
			}
		}
		else {
			while (old_rects[s.first].width > new_rects[s.first].width) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setWidth(old_rects[s.first].width - 10);
			}
		}

		if (old_rects[s.first].height < new_rects[s.first].height) {
			while (old_rects[s.first].height < new_rects[s.first].height) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setHeight(old_rects[s.first].height + 10);
			}
		}
		else {
			while (old_rects[s.first].height > new_rects[s.first].height) {
				s.second.set_rect(old_rects[s.first]);
				old_rects[s.first].setHeight(old_rects[s.first].height - 10);
			}
		}
	}

	this->flag = false;
}

void SceneManager::setup(HandCursor* hc) {
	this->scenes.insert(make_pair("main", new MainScene()));
	this->scenes.insert(make_pair("detail", new DetailScene()));

	this->hc = hc;
	this->scenes["main"]->setup(hc);
	this->scenes["detail"]->setup(hc);

	for (auto s : this->scenes) {
		ofAddListener(s.second->point_event, this, &SceneManager::pointed);
		ofAddListener(s.second->transition_event, this, &SceneManager::transition);
		ofAddListener(s.second->make_sub_window_event, this, &SceneManager::make_sub_window);
	}

	this->current_scene = "main";

}

void SceneManager::update() {

	if (!this->flag && !this->sub_scenes.empty()) {
		//this->best_cost = this->past_cost = DBL_MAX;
		this->rects_tmp.clear();
		this->best_rects.clear();
		this->old_rects.clear();

		for (const auto &s : this->sub_scenes) {
			this->rects_tmp.insert(make_pair(s.first, s.second.get_rect()));
		}

		this->best_rects = this->old_rects = this->rects_tmp;

		for (int i = 0; i < 1000; ++i) {
			int modify_window_num = this->active_scene_id_list[ofRandom(0, this->active_scene_id_list.size())];

			int p = ofRandom(0, 4);
			if (p == 0) {
				this->rects_tmp[modify_window_num].setX(this->rects_tmp[modify_window_num].x + ofRandom(-100, 100));
			}
			else if (p == 1) {
				this->rects_tmp[modify_window_num].setY(this->rects_tmp[modify_window_num].y + ofRandom(-100, 100));
			}
			else if (p == 2) {
				this->rects_tmp[modify_window_num].setWidth(ofRandom(30, this->window_width / 2));
			}
			else {
				this->rects_tmp[modify_window_num].setHeight(ofRandom(30, this->window_height / 2));
			}

			this->current_cost = this->calculate_cost();

			if (this->current_cost > this->past_cost && ofRandom(0, i + 2) == 0) {
				this->rects_tmp[modify_window_num] = this->sub_scenes[modify_window_num].get_rect();
			}
			else {
				this->past_cost = this->current_cost;

				if (this->current_cost < this->best_cost) {
					this->best_cost = this->current_cost;
					this->best_rects = this->rects_tmp;
				}
			}
		}

		void(SceneManager::*funcp)(unordered_map<int, ofRectangle> &old_rects, unordered_map<int, ofRectangle> &new_rects) = &SceneManager::transform;
		thread th(funcp, this, old_rects, this->best_rects);
		th.detach();
	}

	for (auto &t : this->hc->track_data) {
		if (this->cursor_log.find(t.first) == end(this->cursor_log)) {
			this->cursor_log.insert(make_pair(t.first, true));
			this->scenes["main"]->pointer_id.emplace_back(t.first);
		}
	}

	for (auto id = begin(this->scenes["main"]->pointer_id); id != end(this->scenes["main"]->pointer_id);) {
		if (this->hc->track_data.find(*id) == end(this->hc->track_data)) {
			id = this->scenes["main"]->pointer_id.erase(id);
		}
		else {
			++id;
		}
	}

	this->scenes[this->current_scene]->update();
	while (!this->erase_scene_id.empty()) {
		this->sub_scenes.erase(this->erase_scene_id.front());
		this->active_scene_id_list.erase(find(begin(this->active_scene_id_list), end(this->active_scene_id_list), this->erase_scene_id.front()));
		this->erase_scene_id.pop();
	}
	for (auto &ss : this->sub_scenes) {
		ss.second.update();
	}
}

void SceneManager::draw() {
	this->scenes[this->current_scene]->draw();

	while (!this->erase_scene_id.empty()) {
		this->sub_scenes.erase(this->erase_scene_id.front());
		this->erase_scene_id.pop();
	}
	for (auto &ss : this->sub_scenes) {
		ss.second.draw();
	}

	//ofSetColor(ofColor::white);

	/* Žèƒ|ƒCƒ“ƒ^‚Ì•`‰æ */
	/*
	for (auto &t : this->hc->track_data) {
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 50; ++i) {
			r += 0.6;
			alpha -= 12;
			ofSetColor(t.second.pointer_color, alpha);
			ofCircle(t.second.current_pointer.x, t.second.current_pointer.y, r);
		}
	}
	*/
}

double SceneManager::calculate_cost() {
	double cost = 0.0;
	ofRectangle main_rect(0, 0, this->window_width, this->window_height);
	for (const auto &r : this->rects_tmp) {
		cost += euclid_distance(this->window_width / 2, this->window_height / 2, r.second.getCenter().x, r.second.getCenter().y);
		cost -= r.second.getArea();
		cost -= r.second.getIntersection(main_rect).getArea();
		for (const auto &td : this->hc->track_data) {
			//cost -= euclid_distance(r.x, r.y, td.second.current_pointer.x, td.second.current_pointer.y);
		}
		if (r.second.getLeft() < 0 || r.second.getRight() > this->window_width || r.second.getTop() < 0 || r.second.getBottom() > this->window_height) {
			cost += 100000;
		}
	}

	return cost;
}

void SceneManager::pointed(pair<string, int> &id) {
	if (this->current_scene == "main") {
		this->scenes[this->current_scene]->change_icon_state(id.first, "point");
		this->scenes[this->current_scene]->set_icon_pointer_id(id.first, id.second);
	}
	else if (this->current_scene == "detail") {
		if (id.first == "return") {
			this->current_scene = "main";
		}
	}
}

void SceneManager::transition(int &pointer_id) {
	this->current_scene = "detail";
}

void SceneManager::change_cursor_to_main_window(pair<int, int> &id) {
	this->scenes["main"]->pointer_id.emplace_back(id.second);
	this->active_scene_id_list.erase(find(begin(this->active_scene_id_list), end(this->active_scene_id_list), id.first));
}

void SceneManager::make_sub_window(int &pointer_id) {
	if (this->sub_scenes.empty()) {
		SubScene sub_scene;
		sub_scene.setup(new DetailScene(), this->hc, pointer_id, this->scene_id, ofRectangle(200, 200, this->window_width / 2, this->window_height / 2));
		ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		ofAddListener(sub_scene.user_leave_event, this, &SceneManager::change_cursor_to_main_window);
		sub_scene.track_id.emplace_back();
		this->sub_scenes.insert(make_pair(this->scene_id, sub_scene));
		this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));

		this->active_scene_id_list.emplace_back(this->scene_id++);
	}
	else {
		RectangleOptimization ro(this->window_width, this->window_height);

		for (auto &s : this->sub_scenes) {
			ro.add_block(s.second.get_rect());
		}

		for (auto &td : this->hc->track_data) {
			if (td.first == pointer_id) {
				continue;
			}
			ro.add_block(ofPoint(td.second.current_pointer.x, td.second.current_pointer.y));
		}

		ro.calculate();

		SubScene sub_scene;
		sub_scene.setup(new DetailScene(), this->hc, pointer_id, this->scene_id, ro.get_max_area_rect());
		ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		ofAddListener(sub_scene.user_leave_event, this, &SceneManager::change_cursor_to_main_window);
		sub_scene.track_id.emplace_back();
		this->sub_scenes.insert(make_pair(this->scene_id, sub_scene));
		this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));

		this->active_scene_id_list.emplace_back(this->scene_id++);

	}
}

void SceneManager::delete_sub_window(int &scene_id) {
	this->sub_scenes[scene_id].exit();
	this->erase_scene_id.push(scene_id);
}

SceneManager::~SceneManager() {
	delete this->scenes["main"];
	delete this->scenes["detail"];
	//destroyAllWindows();
}