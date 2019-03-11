#pragma once
#ifndef ___Class_HistoryPlotterSecondWindow
#define ___Class_HistoryPlotterSecondWindow

#include "HistoryPlotter.h"
#include "SecondWindow.h"

class HistoryPlotterSecondWindow : public HistoryPlotter {
private:
	SecondWindow window;
public:
	HistoryPlotterSecondWindow(const char* name, const ofRectangle& window_rect, double max_history);

	void draw();
	void set_window_position(int x, int y);

	~HistoryPlotterSecondWindow();
};

#endif