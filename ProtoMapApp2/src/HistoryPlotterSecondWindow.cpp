#include "HistoryPlotterSecondWindow.h"

HistoryPlotterSecondWindow::HistoryPlotterSecondWindow(const char* name, const ofRectangle& window_rect, double max_history) : HistoryPlotter(name, window_rect, max_history) {
	this->window.setup(window_rect, true); // false�Řg�t���Atrue�Řg���Ȃ��E�B���h�E
	this->window.show();
}

void HistoryPlotterSecondWindow::draw() {
	this->window.begin();
	HistoryPlotter::draw();
	this->window.end();
}

void HistoryPlotterSecondWindow::set_window_position(const int x, const int y) {
	this->window.set_window_position(x, y);
}

HistoryPlotterSecondWindow::~HistoryPlotterSecondWindow() {
	this->window.hide();
	this->window.end();
}