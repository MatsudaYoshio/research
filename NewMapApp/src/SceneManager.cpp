#include "SceneManager.h"

using namespace param;

void SceneManager::setup() {
	this->mb.setup();
}

void SceneManager::update() {
	this->mb.update();
}

void SceneManager::draw() {
	ofSetColor(ofColor::white);
	this->map_image.draw(0, 0, DISPLAY_W, DISPLAY_H);
	this->mb.draw();
}