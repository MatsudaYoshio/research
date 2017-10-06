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

	const double change_rate = 0.05;
	const int change_times = 20;

	for (const auto &id : this->active_scene_id_list) {
		int x_sign = (new_rects[id].x > old_rects[id].x) ? +1 : -1;
		int y_sign = (new_rects[id].y > old_rects[id].y) ? +1 : -1;
		double x_change_val = change_rate*abs(new_rects[id].x - old_rects[id].x)*x_sign;
		double y_change_val = change_rate*abs(new_rects[id].y - old_rects[id].y)*y_sign;

		for (int i = 0; i < change_times; ++i) {
			this->sub_scenes[id].set_rect(old_rects[id]);
			old_rects[id].setX(old_rects[id].x + x_change_val);
			old_rects[id].setY(old_rects[id].y + y_change_val);
		}

		int w_sign = (new_rects[id].width > old_rects[id].width) ? +1 : -1;
		int h_sign = (new_rects[id].height > old_rects[id].height) ? +1 : -1;
		double w_change_val = change_rate*abs(new_rects[id].width - old_rects[id].width)*w_sign;
		double h_change_val = change_rate*abs(new_rects[id].height - old_rects[id].height)*h_sign;

		for (int i = 0; i < change_times; ++i) {
			if (find(begin(this->active_scene_id_list), end(this->active_scene_id_list), id) == end(this->active_scene_id_list)) {
				break;
			}
			this->sub_scenes[id].set_rect(old_rects[id]);
			old_rects[id].setWidth(old_rects[id].width + w_change_val);
			old_rects[id].setHeight(old_rects[id].height + h_change_val);
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

	if (!this->flag && !this->sub_scenes.empty() && !this->active_scene_id_list.empty()) {
		this->best_cost = this->past_cost = DBL_MAX;
		int current_component_num = this->active_scene_id_list.size() + this->hc->track_data.size();

		if (this->past_component_num != current_component_num) {
			this->past_component_num = current_component_num;
			this->best_cost = this->past_cost = DBL_MAX;
		}

		this->rects_tmp.clear();
		this->best_rects.clear();
		this->old_rects.clear();

		for (const auto &s : this->sub_scenes) {
			this->rects_tmp.insert(make_pair(s.first, s.second.get_rect()));
		}

		this->best_rects = this->old_rects = this->rects_tmp;

		for (int i = 0; i < 2000; ++i) {
			int modify_window_num = this->active_scene_id_list[ofRandom(0, this->active_scene_id_list.size())];

			int p = ofRandom(0, 4);
			if (p == 0) {
				this->rects_tmp[modify_window_num].setX(this->rects_tmp[modify_window_num].x + ofRandom(-500, 500));
			}
			else if (p == 1) {
				this->rects_tmp[modify_window_num].setY(this->rects_tmp[modify_window_num].y + ofRandom(-500, 500));
			}
			else if (p == 2) {
				this->rects_tmp[modify_window_num].setWidth(ofRandom(30, this->window_width / 2));
			}
			else {
				this->rects_tmp[modify_window_num].setHeight(ofRandom(30, this->window_height / 2));
			}

			this->current_cost = this->calculate_cost();

			if (this->current_cost > this->past_cost) {
				double diff = this->past_cost - this->current_cost;
				double t = (double)i / 2000.0;
				if (ofRandomuf() > exp(diff / t)) {
					this->rects_tmp[modify_window_num] = this->sub_scenes[modify_window_num].get_rect();
				}
			}
			else {
				this->past_cost = this->current_cost;

				if (this->current_cost < this->best_cost) {
					this->best_cost = this->current_cost;
					this->best_rects = this->rects_tmp;
				}
			}

			/*
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
			*/
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
		double aspect_ratio = r.second.width / r.second.height;
		if (aspect_ratio < 0.625) {
			cost += 100 * (0.625 - aspect_ratio);
		}
		else if (aspect_ratio >= 1.6) {
			cost += 100 * (1.6 - aspect_ratio);
		}
		cost += 10*euclid_distance(this->window_width / 2, this->window_height / 2, r.second.getCenter().x, r.second.getCenter().y);
		cost -= 11*r.second.getArea();
		cost -= 10*r.second.getIntersection(main_rect).getArea();
		for (const auto &td : this->hc->track_data) {
			if (this->sub_scenes[r.first].get_user_id() == td.first) {
				cost += 10*euclid_distance(r.second.x, r.second.y, td.second.face.left() + td.second.face.width() / 2, td.second.face.top() + td.second.face.height() / 2);
				continue;
			}
			cost -= 2000*euclid_distance(r.second.x, r.second.y, td.second.current_pointer.x, td.second.current_pointer.y);
		}
		if (r.second.getLeft() < 0 || r.second.getRight() > this->window_width || r.second.getTop() < 0 || r.second.getBottom() > this->window_height) {
			cost += 100000;
		}
		
		for (const auto &r2 : this->rects_tmp) {
			cost += 10*r.second.getIntersection(r2.second).getArea();
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
	this->inactivate_sub_window(id.first);
}

void SceneManager::inactivate_sub_window(int &scene_id) {
	auto ite = find(begin(this->active_scene_id_list), end(this->active_scene_id_list), scene_id);
	if (ite != end(this->active_scene_id_list)) {
		this->active_scene_id_list.erase(ite);
	}
}

void SceneManager::make_sub_window(int &pointer_id) {
	if (this->sub_scenes.empty()) {
		SubScene sub_scene;
		sub_scene.setup(new DetailScene(), this->hc, pointer_id, this->scene_id, ofRectangle(200, 200, this->window_width / 2, this->window_height / 2));
		ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		ofAddListener(sub_scene.user_leave_event, this, &SceneManager::change_cursor_to_main_window);
		ofAddListener(sub_scene.cursor_disappear_event, this, &SceneManager::inactivate_sub_window);
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
		ofAddListener(sub_scene.cursor_disappear_event, this, &SceneManager::inactivate_sub_window);
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
	for (auto &s : this->sub_scenes) {
		s.second.exit();
	}
	delete this->scenes["main"];
	delete this->scenes["detail"];
	//destroyAllWindows();
}