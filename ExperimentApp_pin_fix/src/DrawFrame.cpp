#include "DrawFrame.h"

void DrawFrame::setup(const ofColor& color, const ofRectangle& frame_rect) {
	this->path.setFillColor(color);
	this->frame_rect.set(frame_rect);
	
	this->path.rectangle(this->frame_rect);
	this->path.rectangle(this->frame_rect.getX() + this->frame_size, this->frame_rect.getY() + this->frame_size, this->frame_rect.getWidth() - (this->frame_size << 1), this->frame_rect.getHeight() - (this->frame_size << 1));
}

void DrawFrame::draw() {
	this->path.draw();
}

DrawFrame::~DrawFrame() {
	this->path.close();
}