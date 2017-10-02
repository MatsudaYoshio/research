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

void SceneManager::test(int old_x, int old_y, int old_w, int old_h, int new_x, int new_y, int new_w, int new_h) {
	this->flag = true;
	if (old_x < new_x) {
		while (old_x++ < new_x) {
			this->secondWindow.setWindowPosition(old_x, old_y);
		}
	}
	else {
		while (old_x-- > new_x) {
			this->secondWindow.setWindowPosition(old_x, old_y);
		}
	}
	if (old_y < new_y) {
		while (old_y++ < new_y) {
			this->secondWindow.setWindowPosition(old_x, old_y);
		}
	}
	else {
		while (old_y-- > new_y) {
			this->secondWindow.setWindowPosition(old_x, old_y);
		}
	}
	if (old_w < new_w) {
		while (old_w++ < new_w) {
			this->secondWindow.setWindowSize(old_w, old_h);
		}
	}
	else {
		while (old_w-- > new_w) {
			this->secondWindow.setWindowSize(old_w, old_h);
		}
	}
	if (old_h < new_h) {
		while (old_h++ < new_h) {
			this->secondWindow.setWindowSize(old_w, old_h);
		}
	}
	else {
		while (old_h-- > new_h) {
			this->secondWindow.setWindowSize(old_w, old_h);
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

	/*
	this->fbo.allocate(this->window_width, this->window_height, GL_RGBA);
	fbo.begin();
	ofClear(255, 255, 255, 0);
	fbo.end();
	*/

	//this->secondWindow.setup("second window", this->x, this->y, this->w, this->h, false);
	//ofPoint center_point = ofPoint(this->x + this->w / 2, this->y + this->h / 2);
	////this->past_cost = euclid_distance(this->window_width / 2, this->window_height / 2, center_point.x, center_point.y);
	//this->past_cost = this->current_cost = 10000000;
	//for (auto &t : this->hc->track_data) {
	//	if (euclid_distance(t.second.current_pointer.x, t.second.current_pointer.y, center_point.x, center_point.y) != 0) {
	//		this->past_cost += 1 / euclid_distance(t.second.current_pointer.x, t.second.current_pointer.y, center_point.x, center_point.y);
	//	}
	//}
	//if (this->x < 0 || this->x > this->window_width || this->y < 0 || this->y > this->window_height || this->w > this->window_width/2 || this->h > this->window_height/2) {
	//	this->past_cost = this->past_cost*this->past_cost;
	//}
}

void SceneManager::update() {

	//if (!this->flag) {
	//	void(SceneManager::*funcp)() = &SceneManager::test;
	//	thread th(funcp, this);
	//	th.detach();
	//}

	//if (!this->flag) {

	//	if (this->f == 100) {
	//		this->past_cost = 9999999999;
	//		this->t = 2;
	//		this->f = 0;
	//	}
	//	++this->f;
	//	int p = ofRandom(0, 4);
	//	int x_tmp = this->x, y_tmp = this->y, w_tmp = this->w, h_tmp = this->h;
	//	if (p == 0) {
	//		this->x += ofRandom(-this->window_width / 2, this->window_width / 2);
	//	}
	//	else if (p == 1) {
	//		this->y += ofRandom(-this->window_height / 2, this->window_height / 2);
	//	}
	//	else if (p == 2) {
	//		this->w = ofRandom(30, this->window_width / 2);
	//	}
	//	else {
	//		this->h = ofRandom(30, this->window_height / 2);
	//	}
	//	ofPoint center_point = ofPoint(this->x + this->w / 2, this->y + this->h / 2);
	//	this->current_cost = log(euclid_distance(this->window_width / 2, this->window_height / 2, center_point.x, center_point.y));
	//	//this->current_cost = 0;
	//	for (auto &t : this->hc->track_data) {
	//		/*
	//		if (euclid_distance(t.second.current_pointer.x, t.second.current_pointer.y, center_point.x, center_point.y) != 0) {
	//			this->current_cost = 0.5*log(1 / euclid_distance(t.second.current_pointer.x, t.second.current_pointer.y, center_point.x, center_point.y));
	//		}
	//		*/
	//		this->current_cost -= euclid_distance(t.second.current_pointer.x, t.second.current_pointer.y, center_point.x, center_point.y);
	//	}
	//	this->current_cost += 100000 / (this->w*this->h);
	//	if (this->x < 0 || this->x > this->window_width || this->y < 0 || this->y > this->window_height || this->w > this->window_width / 2 || this->h > this->window_height / 2 || this->x + this->w > this->window_width || this->x + this->w < 0 || this->y + this->h > this->window_height || this->y + this->h < 0) {
	//		this->current_cost = this->current_cost*this->current_cost;
	//	}
	//	//cout << this->current_cost << endl;
	//	if (this->current_cost < this->past_cost) {
	//		this->past_cost = this->current_cost;
	//	}
	//	else {
	//		if (ofRandom(0, t) == 0) {
	//			this->past_cost = this->current_cost;
	//		}
	//		else {
	//			this->x = x_tmp;
	//			this->y = y_tmp;
	//			this->w = w_tmp;
	//			this->h = h_tmp;
	//		}
	//	}
	//	t++;

	//	void(SceneManager::*funcp)(int, int, int, int, int, int, int, int) = &SceneManager::test;
	//	thread th(funcp, this, x_tmp, y_tmp, w_tmp, h_tmp, this->x, this->y, this->w, this->h);
	//	th.detach();

	//	//this->secondWindow.setWindowPosition(this->x, this->y);
	//	//this->secondWindow.setWindowSize(this->w, this->h);
	//}

	//this->secondWindow.show();

	
	for (auto &t : this->hc->track_data) {
		if (this->pointer_log.find(t.first) == end(this->pointer_log)) {
			this->pointer_log.insert(make_pair(t.first, true));
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

void SceneManager::change_cursor_to_main_window(int &id) {
	this->scenes["main"]->pointer_id.emplace_back(id);
}

void SceneManager::make_sub_window(int &pointer_id) {
	if (this->sub_scenes.empty()) {
		SubScene sub_scene;
		sub_scene.setup(new DetailScene(), this->hc, pointer_id, this->scene_id, ofRectangle(200, 200, this->window_width / 2, this->window_height / 2));
		ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		ofAddListener(sub_scene.user_leave_event, this, &SceneManager::change_cursor_to_main_window);
		sub_scene.track_id.emplace_back();
		this->sub_scenes.insert(make_pair(this->scene_id++, sub_scene));
		this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));
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
		this->sub_scenes.insert(make_pair(this->scene_id++, sub_scene));
		this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));
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