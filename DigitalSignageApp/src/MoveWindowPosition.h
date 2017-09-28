#pragma once
#ifndef ___Class_MoveWindowPosition
#define ___Class_MoveWindowPosition

#include "ofThread.h"
#include "ofxSecondWindow.h"

class MoveWindowPosition : public ofThread {
private:
	ofxSecondWindow* secondWindow;
public:
	void setup(ofxSecondWindow* w);
	void start();
	void threadedFunction();
};

#endif