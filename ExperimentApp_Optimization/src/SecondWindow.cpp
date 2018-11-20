#include "SecondWindow.h"

SecondWindow::SecondWindow(int x, int y, int w, int h, bool undecorated, const char *title) {
	this->setup(x, y, w, h, undecorated, title);
}

SecondWindow::SecondWindow(ofRectangle rect, bool undecorated, const char *title) {
	this->setup(rect, undecorated, title);
}

void SecondWindow::setup(const int x, const int y, const int w, const int h, const bool undecorated, const char* title) {
	this->window_rect.set(x, y, w, h);
	glfwWindowHint(GLFW_DECORATED, !undecorated);
	this->main_window = glfwGetCurrentContext();
	this->aux_window = glfwCreateWindow(w, h, title, nullptr, this->main_window);
	glfwSetWindowPos(this->aux_window, x, y);

	glfwMakeContextCurrent(this->aux_window);
	glEnable(GL_BLEND);
#ifndef TARGET_OPENGLES
	glBlendEquation(GL_FUNC_ADD);
#endif
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwMakeContextCurrent(this->main_window);
}

void SecondWindow::setup(ofRectangle rect, bool undecorated, const char *title) {
	this->setup(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight(), undecorated, title);
}

void SecondWindow::set_window_position(const int x, const int y) {
	this->window_rect.setPosition(x, y);
	glfwSetWindowPos(this->aux_window, x, y);
}

void SecondWindow::set_window_size(const int w, const int h) {
	this->window_rect.setWidth(w);
	this->window_rect.setHeight(h);
	glfwSetWindowSize(this->aux_window, this->window_rect.width, this->window_rect.height);
}

int SecondWindow::get_x() const {
	return this->window_rect.getX();
}

int SecondWindow::get_y() const {
	return this->window_rect.getY();
}

int SecondWindow::get_width() const {
	return this->window_rect.getWidth();
}

int SecondWindow::get_height() const {
	return this->window_rect.getHeight();
}

bool SecondWindow::is_inside(const ofPoint &p) const {
	return this->window_rect.inside(p);
}

ofRectangle SecondWindow::get_rect() const {
	return this->window_rect;
}

void SecondWindow::begin() {
	glfwMakeContextCurrent(this->aux_window);
	int width, height;
	glfwGetFramebufferSize(this->aux_window, &width, &height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SecondWindow::end() {
	glfwSwapBuffers(this->aux_window);
	glfwPollEvents();
	glfwMakeContextCurrent(this->main_window);
}

void SecondWindow::show() {
	glfwShowWindow(this->aux_window);
	hidden_flag = false;
}

void SecondWindow::hide() {
	glfwHideWindow(this->aux_window);
	hidden_flag = true;
}

void SecondWindow::close() {
	glfwSetWindowShouldClose(this->aux_window, GL_FALSE);
}