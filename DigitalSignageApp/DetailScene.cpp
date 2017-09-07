#include "DetailScene.h"

void DetailScene::setup() {

}

void DetailScene::update() {

}

void DetailScene::draw() {
	ofBackgroundHex(0xFF0000);
	ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString("Detailed Scene", 20, 20);
}