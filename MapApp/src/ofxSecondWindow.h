#pragma once
#include "ofMain.h"
#include <GLFW/glfw3.h>

class ofxSecondWindow {
    
public:
    void setup(int xpos, int ypos, int width, int height, bool undecorated, const char *name = "sub window");
	void setup(ofRectangle rect, bool undecorated, const char *name = "sub window");
    void begin();
    void end();
    void show();
    void hide();
	void close();
    void toggleHidden();
    
    void setWindowPosition(int x, int y);
    void setWindowSize(int width, int height);
    
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;

	bool is_inside(const ofPoint &p) const;
	ofRectangle get_rect() const;

private:
    GLFWwindow *mainWindow, *auxWindow;
    bool hidden;
	ofRectangle rect;
};
