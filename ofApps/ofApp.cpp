#include <iostream>
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowPosition(50, 50);
	//ofSetFullscreen(true);
	ofSetFrameRate(30);
	ofSetBackgroundAuto(true);
	//ofBackground(255, 255, 255);
	this->gui.setup();
	this->gui.add(this->b1.setup("test button"));
	this->icon.setup(200, 200, 200, 200, "D:/machine_learning_data/hand/Background/background5404.jpg");
	ofAddListener(this->icon.my_event, this, &ofApp::callback);
}

//--------------------------------------------------------------
void ofApp::update(){
	this->icon.update_state(ofPoint(mouseX, mouseY));
}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.draw();
	this->icon.draw();
	//this->icon.fireEvent();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::callback(float &val) {
	this->icon.draw();
}
