#include "HistoryPlotter.h"

HistoryPlotter::HistoryPlotter(const char* name, const ofRectangle& draw_area, const double max_history) :plot_name(name), draw_area(draw_area), max_history(max_history) {
	this->grid_mesh.setMode(OF_PRIMITIVE_LINES);
	this->plot_mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	this->plot_mesh.getVertices().reserve(max_history * 2);
	this->grid_unit = draw_area.getWidth() / max_history;
	this->respect_height = this->draw_area.getHeight() - this->respect_border_size;
}

void HistoryPlotter::update(const double new_value) {
	++this->history_count;

	if (this->values.empty()) {
		this->lowest = -1;
		this->highest = 1;
	}
	else {
		const auto minmax_result{ minmax_element(cbegin(this->values), cend(this->values)) };
		this->lowest = min(DBL_MAX, *minmax_result.first);
		this->highest = max(-DBL_MIN, *minmax_result.second);
	}

	this->lowest = min(this->lowest, new_value);
	this->highest = max(this->highest, new_value);

	switch (this->range_mode) {
	case RANGE_MODE::MANUAL:
		this->plot_low = this->manual_lowest;
		this->plot_high = this->manual_highest;
		break;
	case RANGE_MODE::LOWER_FIXED:
		this->plot_low = this->manual_lowest;
		this->plot_high = this->highest;
		break;
	case RANGE_MODE::AUTOMATIC:
		this->plot_low = this->lowest;
		this->plot_high = this->highest;
		break;
	}
	this->y_scale = (this->respect_height - 1) / (this->plot_high - this->plot_low);

	this->values.emplace_back(new_value);

	while (this->values.size() > this->max_history) {
		this->values.erase(cbegin(this->values));
	}

	this->plot_needs_refresh = true;
}

void HistoryPlotter::draw() {
	ofPushStyle();

	/* 背景の描画 */
	ofSetColor(this->bg_color);
	ofDrawRectangle(this->draw_area);

	/* グリッドの描画 */
	if (this->grid_mesh.getVertices().size() == 0) {
		this->refill_grid_mesh();
	}
	ofSetColor(this->grid_color);
	ofSetLineWidth(1);
	ofDisableAntiAliasing();
	this->grid_mesh.draw();
	ofEnableAntiAliasing();

	ofSetColor(this->line_color);

	/* タイトルの描画 */
	const auto text = this->plot_name + string(" " + ofToString(*this->values.rbegin(), this->precision));
	ofDrawBitmapString(text, this->draw_area.getX() + this->draw_area.getWidth() - (text.length()) * 8, this->draw_area.getY() + 10);

	/* 数値情報（最大値と最小値）の表示 */
	ofSetColor(ofColor::white);
	ofDrawBitmapString(ofToString(this->plot_high, this->precision), 1 + this->draw_area.getX(), this->draw_area.getY() + 10);
	ofDrawBitmapString(ofToString(this->plot_low, this->precision), 1 + this->draw_area.getX(), this->draw_area.getY() + this->draw_area.getHeight() - 1);

	ofNoFill();
	ofSetLineWidth(this->line_width);

	if (this->plot_needs_refresh) {
		this->refill_plot_mesh();
		this->plot_needs_refresh = false;
	}

	ofSetColor(this->line_color);

	ofPushMatrix();
	ofTranslate(this->draw_area.getX(), this->draw_area.getY() + this->respect_height + this->respect_border_size - 1);
	ofScale(this->draw_area.getWidth() / this->max_history, -this->y_scale);
	ofTranslate(0, -this->plot_low);
	this->plot_mesh.draw();
	ofPopMatrix();

	ofFill();
	ofPopStyle();
}

void HistoryPlotter::refill_grid_mesh() {
	this->grid_mesh.clear();
	this->grid_mesh.setMode(OF_PRIMITIVE_LINES);

	const auto num_lines_h{ this->draw_area.getHeight() / this->grid_unit };
	for (int i = 0; i < num_lines_h; ++i) {
		this->grid_mesh.addVertex(ofVec3f(this->draw_area.getX(), this->draw_area.getY() + this->grid_unit*i));
		this->grid_mesh.addVertex(ofVec3f(this->draw_area.getX() + this->draw_area.getWidth(), this->draw_area.getY() + this->grid_unit*i));
	}

	const auto num_lines_w{ this->draw_area.getWidth() / this->grid_unit };
	for (int i = 0; i < num_lines_w; ++i) {
		this->grid_mesh.addVertex(ofVec3f(floor(this->grid_unit*0.5) + this->draw_area.getX() + this->grid_unit*i, this->draw_area.getY()));
		this->grid_mesh.addVertex(ofVec3f(floor(this->grid_unit*0.5) + this->draw_area.getX() + this->grid_unit*i, this->draw_area.getY() + this->draw_area.getHeight()));
	}
}

void HistoryPlotter::refill_plot_mesh() {
	this->plot_mesh.clear();

	int start = (this->history_count >= this->max_history) ? this->draw_skip_num - (this->history_count) % (this->draw_skip_num) : 0;

	for (int i = start; i < this->values.size(); i += this->draw_skip_num) {
		this->plot_mesh.addVertex(ofVec3f(i, this->values[i]));
	}
}

void HistoryPlotter::set_range(const double low, const double high) {
	this->range_mode = RANGE_MODE::MANUAL;
	this->manual_lowest = low;
	this->manual_highest = high;
}

void HistoryPlotter::set_lower_range(const double low) {
	this->range_mode = RANGE_MODE::LOWER_FIXED;
	this->manual_lowest = low;
}

void HistoryPlotter::set_auto_range() {
	this->range_mode = RANGE_MODE::AUTOMATIC;
}

void HistoryPlotter::set_precision(const int precision) {
	this->precision = ofClamp(precision, 0, 15);
}

void HistoryPlotter::set_line_color(const ofColor& line_color) {
	this->line_color = line_color;
}

void HistoryPlotter::set_background_color(const ofColor& bg_color) {
	this->bg_color = bg_color;
}

void HistoryPlotter::set_grid_color(const ofColor& grid_color) {
	this->grid_color = grid_color;
}

void HistoryPlotter::set_line_width(const double line_width) {
	this->line_width = line_width;
}

void HistoryPlotter::set_skip_num(const int skip_num) {
	this->draw_skip_num = skip_num;
}

void HistoryPlotter::set_grid_unit(const double grid_unit) {
	this->grid_unit = grid_unit;
}