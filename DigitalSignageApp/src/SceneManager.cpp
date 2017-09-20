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

	
	this->fbo.allocate(this->window_width, this->window_height, GL_RGBA);
	fbo.begin();
	ofClear(255, 255, 255, 0);
	fbo.end();
	
}

void SceneManager::update() {
	this->scenes[this->current_scene]->update();
	for (auto &ss : this->sub_scenes) {
		ss.update();
	}
}

void SceneManager::draw() {
	this->scenes[this->current_scene]->draw();

	for (auto &ss : this->sub_scenes) {
		ss.draw();
	}

	ofSetColor(ofColor::white);

	/* Žèƒ|ƒCƒ“ƒ^‚Ì•`‰æ */
	for (auto &t : this->hp->track_data) {
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 50; ++i) {
			r += 0.6;
			alpha -= 12;
			ofSetColor(t.second.pointer_color, alpha);
			ofCircle(t.second.current_pointer.x, t.second.current_pointer.y, r);
		}
		//ofCircle(t.second.current_pointer.x, t.second.current_pointer.y, 15);
	}
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
	SubScene sub_scene;
	sub_scene.setup(new DetailScene(), this->hp, pointer_id);
	sub_scene.track_id.emplace_back();
	this->sub_scenes.emplace_back(sub_scene);
}

SceneManager::~SceneManager() {
	delete this->scenes["main"];
	delete this->scenes["detail"];
	//destroyAllWindows();
}