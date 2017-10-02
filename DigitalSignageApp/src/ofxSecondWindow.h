#pragma once

#include "ofMain.h"
#include <GLFW/glfw3.h>

class ofxSecondWindow {
    
public:
    void setup(const char *name, int xpos, int ypos, int width, int height, bool undecorated);
	void setup(const char *name, ofRectangle rect, bool undecorated);
    void begin();
    void end();
    void show();
    void hide();
	void close();
    void toggleHidden();
    
    void setWindowPosition(int x, int y);
    void setWindowSize(int width, int height);
    
	int getWidth() const;
	int getHeight() const;

	bool is_inside(const ofPoint &p) const;
	ofRectangle get_rect() const;

private:
    GLFWwindow *mainWindow, *auxWindow;
    bool hidden;
	ofRectangle rect;
};
