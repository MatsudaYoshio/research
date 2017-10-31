#include "ofxSecondWindow.h"

void ofxSecondWindow::setup(int xpos, int ypos, int width, int height, bool undecorated, const char* name) {
	this->rect.set(xpos, ypos, width, height);
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

void ofxSecondWindow::setup(ofRectangle rect, bool undecorated, const char *name) {
	this->setup(rect.x, rect.y, rect.width, rect.height, undecorated, name);
}

void ofxSecondWindow::setWindowPosition(int x, int y) {
	this->rect.setPosition(x, y);
	glfwSetWindowPos(auxWindow, x, y);
}

void ofxSecondWindow::setWindowSize(int width, int height) {
	this->rect.setWidth(width);
	this->rect.setHeight(height);
	glfwSetWindowSize(auxWindow, this->rect.width, this->rect.height);
}

int ofxSecondWindow::getX() const {
	return this->rect.x;
}

int ofxSecondWindow::getY() const {
	return this->rect.y;
}

int ofxSecondWindow::getWidth() const {
	return this->rect.width;
}

int ofxSecondWindow::getHeight() const {
	return this->rect.height;
}

bool ofxSecondWindow::is_inside(const ofPoint &p) const {
	return this->rect.inside(p);
}

ofRectangle ofxSecondWindow::get_rect() const {
	return this->rect;
}

void ofxSecondWindow::begin() {
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

void ofxSecondWindow::end() {
	glfwSwapBuffers(auxWindow);
	glfwPollEvents();
	glfwMakeContextCurrent(mainWindow);
}

void ofxSecondWindow::show() {
	glfwShowWindow(auxWindow);
	hidden = false;
}

void ofxSecondWindow::hide() {
	glfwHideWindow(auxWindow);
	hidden = true;
}

void ofxSecondWindow::close() {
	glfwSetWindowShouldClose(auxWindow, GL_FALSE);
}

void ofxSecondWindow::toggleHidden() {
	if (hidden) {
		show();
	}
	else {
		hide();
	}
}