#ifndef ___Class_ofApp
#define ___Class_ofApp

#include "ofMain.h"
#include "ofxGui.h"
#include "Icon.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void callback(float &val);

	ofxPanel gui;
	ofxButton b1;
	Icon icon;
};

#endif