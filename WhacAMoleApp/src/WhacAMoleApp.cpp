#include "WhacAMoleApp.h"

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // ƒtƒŒ[ƒ€XVŽž‚É“h‚è‚Â‚Ô‚µ‚ð–³Œø‰»
	ofHideCursor(); // ƒJ[ƒ\ƒ‹”ñ•\Ž¦
}

void WhacAMoleApp::update() {
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // ”wŒi‚Ì•`‰æ

	this->hammer_image.draw(mouseX, mouseY);
}