#include "DigitalSignageApp.h"

//--------------------------------------------------------------
void DigitalSignageApp::setup(){
	ofSetFrameRate(30);
	ofSetBackgroundAuto(true);
}

//--------------------------------------------------------------
void DigitalSignageApp::update(){
	this->hp.loop();
}

//--------------------------------------------------------------
void DigitalSignageApp::draw(){
	for (auto t : this->hp.track_data) {
		ofSetColor(t.second.pointer_color);
		ofCircle(t.second.current_pointer.x, t.second.current_pointer.y, 10);
	}
}

//--------------------------------------------------------------
void DigitalSignageApp::keyPressed(int key){

}

//--------------------------------------------------------------
void DigitalSignageApp::keyReleased(int key){

}

//--------------------------------------------------------------
void DigitalSignageApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void DigitalSignageApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void DigitalSignageApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void DigitalSignageApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void DigitalSignageApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void DigitalSignageApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void DigitalSignageApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void DigitalSignageApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void DigitalSignageApp::dragEvent(ofDragInfo dragInfo){

}
