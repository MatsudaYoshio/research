#include "MapApp.h"

using namespace param;

void MapApp::setup() {
	this->sm.setup();
}

void MapApp::update() {
	this->sm.update();
}

void MapApp::draw() {
	this->sm.draw();
}