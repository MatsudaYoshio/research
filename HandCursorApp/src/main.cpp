#include "ofMain.h"
#include "HandCursorApp.h"

int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);

	ofRunApp(new HandCursorApp());
}
