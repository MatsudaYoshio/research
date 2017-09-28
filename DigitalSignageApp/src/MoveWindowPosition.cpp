#include "MoveWindowPosition.h"

void MoveWindowPosition::setup(ofxSecondWindow* w) {
	this->lock();
	this->secondWindow = w;
	this->unlock();
}

void MoveWindowPosition::start() {
	this->startThread();
}

void MoveWindowPosition::threadedFunction() {
	this->lock();
	this->secondWindow->setWindowPosition(100, 100);
	this->unlock();
	this->lock();
	this->secondWindow->setWindowPosition(500, 500);
	this->unlock();
	this->lock();
	this->secondWindow->setWindowPosition(300, 300);
	this->unlock();
}