#include "Pin.h"

using namespace param;

void Pin::setup(param::CONTENT_ID content_id) {
	this->content_id = content_id;
	this->cp = ofPoint(CONTENT_DATA[static_cast<int>(content_id)].x, CONTENT_DATA[static_cast<int>(content_id)].y);
	this->pin_number = CONTENT_DATA[static_cast<int>(content_id)].number;
	this->color = MENU_ITEM_COLOR[static_cast<int>(CONTENT_DATA[static_cast<int>(content_id)].menu_item_id)];

	auto d{ this->size_ratio*this->radius };
	this->cp = ofPoint(CONTENT_DATA[static_cast<int>(content_id)].x, CONTENT_DATA[static_cast<int>(content_id)].y);
	this->tp = triangle_points{ ofPoint(this->cp.x - d, this->cp.y + d), ofPoint(this->cp.x + d, this->cp.y + d), ofPoint(this->cp.x, this->cp.y + this->radius * 2) };
	this->fp = ofPoint(this->cp.x - this->radius / 2.5, this->cp.y + this->radius / 2.5);
	this->font.loadFont("meiryob.ttc", this->font_size);

	d = this->size_ratio*this->big_radius;
	auto r{ 2 * (this->big_radius - this->radius) };
	this->big_cp = this->cp;
	this->big_cp.y -= r;
	this->big_tp = triangle_points{ ofPoint(this->cp.x - d, this->cp.y + d - r), ofPoint(this->cp.x + d, this->cp.y + d - r), ofPoint(this->cp.x, this->cp.y + this->big_radius * 2 - r) };
	this->big_fp = ofPoint(this->cp.x - this->big_radius / 2.5, this->cp.y + this->big_radius / 2.5 - r);
	this->big_font.loadFont("meiryob.ttc", this->big_font_size);
}

void Pin::update() {
	if (this->progress > this->progress_threshold) {
		cout << "Selected!!" << endl;
		this->state = STATE::NORMAL;
	}

	switch (this->state) {
	case STATE::NORMAL:
		this->progress = 0;
		this->alpha = ofColor::limit();
		break;
	case STATE::POINT:
		++this->progress;
		this->alpha = 150;
		break;
	}
}

void Pin::draw() const {
	switch (this->state) {
	case STATE::NORMAL:
		this->draw(this->cp, this->radius, this->tp, this->font, this->fp);
		break;
	case STATE::POINT:
		this->draw(this->big_cp, this->big_radius, this->big_tp, this->big_font, this->big_fp);
		break;
	}
}

void Pin::draw(const ofPoint& cp, const int r, const triangle_points& tp, const ofTrueTypeFont& f, const ofPoint& fp) const {
	ofFill();
	ofSetColor(this->color, this->alpha);
	ofDrawCircle(cp, r);
	ofDrawTriangle(tp.p1, tp.p2, tp.p3);

	ofSetColor(ofColor::black, this->alpha);
	f.drawString(to_string(this->pin_number), fp.x, fp.y);
}

bool Pin::is_inside(const int x, const int y) {
	return ofDist(x, y, this->cp.x, this->cp.y) <= this->radius + 80;
}

void Pin::point() {
	this->state = STATE::POINT;
}

void Pin::reset_state() {
	this->state = STATE::NORMAL;
}