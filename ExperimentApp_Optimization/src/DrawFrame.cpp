#include "DrawFrame.h"

void DrawFrame::setup(const ofColor& color, const int size, const ofRectangle& frame_rect) {
	this->setup(color, size);
	this->frame_rect.set(frame_rect);
	this->make_path();
}

void DrawFrame::setup(const ofColor& color, const int size) {
	this->path.setFillColor(color);
	this->size = size;
}

void DrawFrame::update(ofRectangle& frame_rect) {
	this->frame_rect.set(frame_rect);
	this->make_path();
}

void DrawFrame::draw() {
	this->path.draw();
}

void DrawFrame::change_color(const ofColor& color) {
	this->path.setFillColor(color);
	this->make_path();
}

void DrawFrame::make_path() {
	this->path.clear();

	this->path.rectangle(this->frame_rect);
	this->path.rectangle(this->frame_rect.getX() + size, this->frame_rect.getY() + size, this->frame_rect.getWidth() - (size << 1), this->frame_rect.getHeight() - (size << 1));
}

DrawFrame::~DrawFrame() {
	this->path.close();
}