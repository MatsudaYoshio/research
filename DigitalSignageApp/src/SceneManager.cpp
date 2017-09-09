#include "SceneManager.h"
#include "MainScene.h"
#include "DetailScene.h"

void SceneManager::setup(HandPointer* hp) {
	this->scenes.insert(make_pair("main", new MainScene()));
	this->scenes.insert(make_pair("detail", new DetailScene()));
	
	this->scenes["main"]->setup(hp);
	this->scenes["detail"]->setup(hp);

	for (auto s : this->scenes) {
		ofAddListener(s.second->point_event, this, &SceneManager::pointed);
	}

	this->current_scene = "main";
}

void SceneManager::update() {
	this->scenes[this->current_scene]->update();
}

void SceneManager::draw() {
	this->scenes[this->current_scene]->draw();
}

void SceneManager::pointed(pair<string,int> &id) {
	if (this->current_scene == "main") {
		this->current_scene = "detail";
	}
	else if (this->current_scene == "detail") {
		if (id.first == "return") {
			this->current_scene = "main";
		}
	}
}

SceneManager::~SceneManager() {
	delete this->scenes["main"];
	delete this->scenes["detail"];
}