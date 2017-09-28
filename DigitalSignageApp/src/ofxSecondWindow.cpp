#include "ofxSecondWindow.h"

void ofxSecondWindow::setup(const char *name, int xpos, int ypos, int width, int height, bool undecorated) {
    this->width = width;
    this->height = height;
    glfwWindowHint(GLFW_DECORATED, !undecorated);
    mainWindow = glfwGetCurrentContext();
    auxWindow = glfwCreateWindow(width, height, name, NULL, mainWindow);
    glfwSetWindowPos(auxWindow, xpos, ypos);
    
    /* enable alpha blending by default */
    glfwMakeContextCurrent(auxWindow);
    glEnable(GL_BLEND);
#ifndef TARGET_OPENGLES
    glBlendEquation(GL_FUNC_ADD);
#endif
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwMakeContextCurrent(mainWindow);
    
    hidden = false;
}

void ofxSecondWindow::setup(const char *name, ofRectangle rect, bool undecorated) {
	this->setup(name, rect.x, rect.y, rect.width, rect.height, undecorated);
}

void ofxSecondWindow::setWindowPosition(int x, int y) {
    glfwSetWindowPos(auxWindow, x, y);
}

void ofxSecondWindow::setWindowSize(int width, int height) {
    this->width = width;
    this->height = height;
    glfwSetWindowSize(auxWindow, width, height);
}

void ofxSecondWindow::begin(){
    glfwMakeContextCurrent(auxWindow);
    int width, height;
    glfwGetFramebufferSize(auxWindow, &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ofxSecondWindow::end(){
    glfwSwapBuffers(auxWindow);
    glfwPollEvents();
    glfwMakeContextCurrent(mainWindow);
}

void ofxSecondWindow::show(){
    glfwShowWindow(auxWindow);
    hidden = false;
}

void ofxSecondWindow::hide(){
    glfwHideWindow(auxWindow);
    hidden = true;
}

void ofxSecondWindow::close() {
	glfwSetWindowShouldClose(auxWindow, GL_FALSE);
}

void ofxSecondWindow::toggleHidden() {
    if (hidden) {
        show();
    } else {
        hide();
    }
}