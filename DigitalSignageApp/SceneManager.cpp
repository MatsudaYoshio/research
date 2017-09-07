#include "SceneManager.h"
#include "MainScene.h"
#include "DetailScene.h"

void SceneManager::setup(HandPointer* hp) {
	this->scenes.insert(make_pair("main", new MainScene()));
	this->scenes.insert(make_pair("detail", new DetailScene()));
	
	this->scenes["main"]->setup(hp);
	ofAddListener(this->scenes["main"]->point_event, this, &SceneManager::pointed);
	
	this->scenes["detail"]->setup();

	this->current_scene = "main";
}

void SceneManager::update() {
	this->scenes[this->current_scene]->update();
}

void SceneManager::draw() {
	this->scenes[this->current_scene]->draw();
}

void SceneManager::pointed(pair<string,int> &id) {
	this->current_scene = "detail";
}

SceneManager::~SceneManager() {
	delete this->scenes["main"];
	delete this->scenes["detail"];
}