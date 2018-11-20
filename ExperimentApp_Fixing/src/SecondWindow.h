#pragma once
#include "ofMain.h"
#include <GLFW/glfw3.h>

class SecondWindow {
private:
	GLFWwindow *main_window, *aux_window;

	bool hidden_flag{ true };
	ofRectangle window_rect;

public:
	SecondWindow() {};
	SecondWindow(int x, int y, int w, int h, bool undecorated, const char *title = "sub window");
	SecondWindow(ofRectangle rect, bool undecorated, const char *title = "sub window");

	void setup(int x, int y, int w, int h, bool undecorated, const char *title = "sub window");
	void setup(ofRectangle rect, bool undecorated, const char *title = "sub window");
	void begin();
	void end();
	void show();
	void hide();
	void close();
	void set_window_position(int x, int y);
	void set_window_size(int w, int h);

	int get_x() const;
	int get_y() const;
	int get_width() const;
	int get_height() const;

	bool is_inside(const ofPoint &p) const;

	ofRectangle get_rect() const;
};