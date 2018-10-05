#pragma once
#ifndef ___Class_HistoryPlotter
#define ___Class_HistoryPlotter

#include "ofMain.h"

class HistoryPlotter {
private:
	const enum class RANGE_MODE { MANUAL, LOWER_FIXED, AUTOMATIC };

	RANGE_MODE range_mode{ RANGE_MODE::AUTOMATIC };

	string plot_name;
	
	double max_history;
	int history_count{ 1 };
	
	int precision{ 2 };

	double lowest{ -1 };
	double highest{ 1 };
	
	double line_width{ 1 };
	ofColor line_color{ ofColor::deepPink };
	
	int draw_skip_num{ 1 };

	ofColor bg_color{ ofColor::black };

	bool plot_needs_refresh{ true };
	
	ofRectangle draw_area;
	
	double grid_unit;
	ofColor grid_color{ ofColor::gray, 32 };

	double manual_lowest;
	double manual_highest;

	double plot_low, plot_high;
	double respect_height;
	double respect_border_size{ 12 };
	double y_scale;

	deque<double> values;

	ofVboMesh grid_mesh;
	ofVboMesh plot_mesh;

	void refill_grid_mesh();
	void refill_plot_mesh();
public:
	HistoryPlotter(const string& name, const ofRectangle& draw_area, double max_history);

	void update(double new_value);
	void draw();

	void set_range(double low, double high);
	void set_lower_range(double low);
	void set_auto_range();
	void set_precision(int precision);
	void set_line_color(const ofColor& line_color);
	void set_background_color(const ofColor& bg_color);
	void set_grid_color(const ofColor& grid_color);
	void set_line_width(double line_width);
	void set_skip_num(int skip_num);
	void set_grid_unit(double grid_unit);
};

#endif