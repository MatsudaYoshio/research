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
	this->number_font.loadFont("meiryob.ttc", this->font_size);
	this->name_font.loadFont("Verdana.ttf", this->font_size);
	this->np[static_cast<int>(CONTENT_NAME_POSITION::TOP)] = ofPoint(this->tp.p3.x - this->font_size * 3.1, this->tp.p3.y - this->font_size * 3.5);
	this->np[static_cast<int>(CONTENT_NAME_POSITION::BOTTOM)] = ofPoint(this->tp.p3.x - this->font_size * 3.1, this->tp.p3.y + this->font_size);
	this->np[static_cast<int>(CONTENT_NAME_POSITION::LEFT)] = ofPoint(this->tp.p1.x - this->font_size*7.5, this->tp.p1.y + this->font_size*0.5);
	this->np[static_cast<int>(CONTENT_NAME_POSITION::RIGHT)] = ofPoint(this->tp.p2.x + this->font_size*0.75, this->tp.p2.y + this->font_size*0.5);

	d = this->size_ratio*this->big_radius;
	auto r{ 2 * (this->big_radius - this->radius) };
	this->big_cp = this->cp;
	this->big_cp.y -= r;
	this->big_tp = triangle_points{ ofPoint(this->cp.x - d, this->cp.y + d - r), ofPoint(this->cp.x + d, this->cp.y + d - r), ofPoint(this->cp.x, this->cp.y + this->big_radius * 2 - r) };
	this->big_fp = ofPoint(this->cp.x - this->big_radius / 2.5, this->cp.y + this->big_radius / 2.5 - r);
	this->big_number_font.loadFont("meiryob.ttc", this->big_font_size);
	this->big_name_font.loadFont("Verdana.ttf", this->big_font_size);
	this->big_np[static_cast<int>(CONTENT_NAME_POSITION::TOP)] = ofPoint(this->big_tp.p3.x - this->big_font_size * 3.1, this->big_tp.p3.y - this->big_font_size * 3.5);
	this->big_np[static_cast<int>(CONTENT_NAME_POSITION::BOTTOM)] = ofPoint(this->big_tp.p3.x - this->big_font_size * 3.1, this->big_tp.p3.y + this->big_font_size);
	this->big_np[static_cast<int>(CONTENT_NAME_POSITION::LEFT)] = ofPoint(this->big_tp.p1.x - this->big_font_size*7.5, this->big_tp.p1.y + this->big_font_size*0.5);
	this->big_np[static_cast<int>(CONTENT_NAME_POSITION::RIGHT)] = ofPoint(this->big_tp.p2.x + this->big_font_size*0.75, this->big_tp.p2.y + this->big_font_size*0.5);
}

void Pin::update() {
	if (this->progress > this->progress_threshold) {
		pair<CONTENT_ID, long long int> id(this->content_id, this->user_id);
		ofNotifyEvent(this->make_sub_window_event, id);
		this->state = STATE::NORMAL;
	}

	switch (this->state) {
	case STATE::NORMAL:
		this->progress = 0;
		this->alpha = MAX_ALFHA;
		break;
	case STATE::POINT:
		++this->progress;
		this->alpha = HALF_MAX_ALFHA;
		break;
	}
}

void Pin::draw() const {
	switch (this->state) {
	case STATE::NORMAL:
		this->draw_pin(this->cp, this->radius, this->tp, this->number_font, this->fp);
		break;
	case STATE::POINT:
		this->draw_pin(this->big_cp, this->big_radius, this->big_tp, this->big_number_font, this->big_fp);
		break;
	}

	this->draw_name();
}

void Pin::draw_pin(const ofPoint& cp, const int r, const triangle_points& tp, const ofTrueTypeFont& f, const ofPoint& fp) const {
	ofFill();
	ofSetColor(this->color, this->alpha);
	ofDrawCircle(cp, r);
	ofDrawTriangle(tp.p1, tp.p2, tp.p3);

	ofSetColor(ofColor::black, this->alpha);
	f.drawString(to_string(this->pin_number), fp.x, fp.y);
}

void Pin::draw_name() const {
	switch (this->state) {
	case STATE::NORMAL:
		if (CONTENT_DATA[static_cast<int>(this->content_id)].name.size() > 9) {
			this->name_font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name.substr(0, 7) + "...", this->np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].x, this->np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].y);
		}
		else {
			this->name_font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name, this->np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].x, this->np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].y);
		}
		break;
	case STATE::POINT:
		if (CONTENT_DATA[static_cast<int>(this->content_id)].name.size() > 9) {
			this->big_name_font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name.substr(0, 7) + "...", this->big_np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].x, this->big_np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].y);
		}
		else {
			this->big_name_font.drawString(CONTENT_DATA[static_cast<int>(this->content_id)].name, this->big_np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].x, this->big_np[static_cast<int>(CONTENT_DATA[static_cast<int>(this->content_id)].position)].y);
		}
	}
}

bool Pin::is_inside(const int x, const int y) {
	return ofDist(x, y, this->cp.x, this->cp.y) <= this->radius + 100;
}

void Pin::point(const long long int user_id) {
	this->user_id = user_id;
	this->state = STATE::POINT;
}

void Pin::reset_state() {
	this->state = STATE::NORMAL;
}