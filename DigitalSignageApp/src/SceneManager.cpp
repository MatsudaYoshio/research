#include "SceneManager.h"
#include "MainScene.h"
#include "DetailScene.h"
#include "windows.h"

#include <opencv2/opencv.hpp>

using namespace cv;


void SceneManager::setup(HandPointer* hp) {
	this->scenes.insert(make_pair("main", new MainScene()));
	this->scenes.insert(make_pair("detail", new DetailScene()));
	
	this->hp = hp;
	this->scenes["main"]->setup(hp);
	this->scenes["detail"]->setup(hp);

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
}

void SceneManager::update() {
	for (auto &t : this->hp->track_data) {
		if (this->pointer_log.find(t.first) == end(this->pointer_log)) {
			this->pointer_log.insert(make_pair(t.first, true));
			this->scenes["main"]->pointer_id.emplace_back(t.first);
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
	for (auto &t : this->hp->track_data) {
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

void SceneManager::pointed(pair<string,int> &id) {
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

void SceneManager::make_sub_window(int &pointer_id) {
	if (0 && this->sub_scenes.empty()) {
		SubScene sub_scene;
		sub_scene.setup(new DetailScene(), this->hp, pointer_id, this->scene_id, 200, 200, this->window_width/2, this->window_height/2);
		ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		sub_scene.track_id.emplace_back();
		this->sub_scenes.insert(make_pair(this->scene_id++, sub_scene));
		this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));
	}
	else {
		vector<ofRectangle> rects;
		std::vector<std::vector<int>> hist(this->window_height, std::vector<int>(this->window_width, 1));
		for (int x = 0; x < this->window_width; ++x) {
			for (int y = 0; y < this->window_height; ++y) {
				if (x >= 100 && x <= 500 && y >= 200 && y <= 700) {
					hist[y][x] = 0;
				}
				if (x >= 900 && x <= 1100 && y >= 500 && y <= 900) {
					hist[y][x] = 0;
				}
				for (auto &s : this->sub_scenes) {
					if (s.second.is_inside(ofPoint(x, y))) {
						hist[y][x] = 0;
					}
				}
			}
		}
		for (int y = 1; y < this->window_height; ++y) {
			for (int x = 0; x < this->window_width; ++x) {
				if (hist[y][x] != 0) {
					hist[y][x] = hist[y - 1][x] + 1;
				}
			}
		}
		for (int y = 0; y < this->window_height; ++y) {
			stack<pair<int, int>> s;
			for (int x = 0; x < this->window_width; ++x) {
				if (s.empty()) {
					s.push(make_pair(hist[y][x], x));
				}
				else if (s.top().first < hist[y][x]) {
					s.push(make_pair(hist[y][x], x));
				}
				else if (s.top().first > hist[y][x]) {
					while (!s.empty() && s.top().first >= hist[y][x]) {
						//rects.emplace_back(ofRectangle(ofPoint(s.top().second, y - s.top().first), x - s.top().second, s.top().first));
						rects.emplace_back(ofRectangle(max(s.top().second, 0), max(y - s.top().first, 0), max(x - s.top().second,1), max(s.top().first, 1)));
						s.pop();
					}
					s.push(make_pair(hist[y][x], x));
				}
			}
			while (!s.empty()) {
				rects.emplace_back(ofRectangle(max(s.top().second,0), max(y - s.top().first,0), max(this->window_width - s.top().second,1), max(s.top().first,1)));
				s.pop();
			}
		}
		ofRectangle r = rects[ofRandom(0, rects.size())];

		SubScene sub_scene;
		sub_scene.setup(new DetailScene(), this->hp, pointer_id, this->scene_id, r.x, r.y, r.width, r.height);
		ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
		sub_scene.track_id.emplace_back();
		this->sub_scenes.insert(make_pair(this->scene_id++, sub_scene));
		this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));
	}

	/*
	vector<ofRectangle> rects;
	std::vector<std::vector<int>> hist(this->window_width, std::vector<int>(this->window_height, 1));
	for (int x = 0; x < this->window_width; ++x) {
		for (int y = 0; y < this->window_height; ++y) {
			for (auto &s : this->sub_scenes) {
				if (s.second.is_inside(ofPoint(x, y))) {
					hist[x][y] = 0;
				}
			}
		}
	}
	for (int y = 0; y < this->window_height; ++y) {
		for (int x = 1; x < this->window_width; ++x) {
			if (hist[x][y] != 0) {
				hist[x][y] = hist[x - 1][y] + 1;
			}
		}
	}
	for (int x = 0; x < this->window_width; ++x) {
		stack<pair<int, int>> s;
		for (int y = 0; y < this->window_height; ++y) {
			if (s.empty()) {
				s.push(make_pair(hist[x][y], y));
			}
			else if (s.top().first < hist[x][y]) {
				s.push(make_pair(hist[x][y], y));
			}
			else if (s.top().first > hist[x][y]) {
				while (!s.empty() && s.top().first > hist[x][y]) {
					rects.emplace_back(ofRectangle(ofPoint(s.top().second, x - s.top().first), y - s.top().second, s.top().first));
					s.pop();
				}
				s.push(make_pair(hist[x][y], y));
			}
		}
		while (!s.empty()) {
			rects.emplace_back(ofRectangle(ofPoint(s.top().second, x - s.top().first), this->window_width - s.top().second, s.top().first));
			s.pop();
		}
	}

	ofRectangle r = rects[ofRandom(0, rects.size())];

	SubScene sub_scene;
	sub_scene.setup(new DetailScene(), this->hp, pointer_id, this->scene_id, r.x, r.y, r.width, r.height);
	ofAddListener(sub_scene.delete_sub_window_event, this, &SceneManager::delete_sub_window);
	sub_scene.track_id.emplace_back();
	this->sub_scenes.insert(make_pair(this->scene_id++, sub_scene));
	this->scenes["main"]->pointer_id.erase(remove(begin(this->scenes["main"]->pointer_id), end(this->scenes["main"]->pointer_id), pointer_id), end(this->scenes["main"]->pointer_id));
	*/
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