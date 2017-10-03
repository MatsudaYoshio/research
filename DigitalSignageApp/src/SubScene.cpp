#include "SubScene.h"
#include "MainScene.h"
#include "DetailScene.h"

#include <opencv2/opencv.hpp>

using namespace cv;

void SubScene::setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, int x, int y, int w, int h) {
	this->scene_id = scene_id;
	this->user_id = user_id;
	this->hc = hc;

	this->sub_window.setup(this->window_name.c_str(), x, y, w, h, false); // 最後の引数をtrueに変えれば枠なしのウィンドウ
	this->sub_window.show();
	
	this->scene = scene;
	this->scene->setup(this->hc);
	
	this->view_rect.set(0, 0, w, h);
	
	ofPoint center = this->view_rect.getCenter();
	this->hc->track_data[this->user_id].current_pointer = this->hc->track_data[this->user_id].past_pointer = Point(center.x, center.y);
	
	this->add_cursor_texture("arrow_down", "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_down.png");
	this->add_cursor_texture("arrow_left", "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_left.png");
	this->add_cursor_texture("arrow_right", "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_right.png");
	this->add_cursor_texture("arrow_up", "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_up.png");
	this->cursor_state = "point";

	this->life = this->max_life;
}

void SubScene::setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, ofRectangle rect) {
	this->setup(scene, hc, user_id, scene_id, rect.x, rect.y, rect.width, rect.height);
}

void SubScene::update() {
	this->scene->update();

	if (this->hc->track_data.find(this->user_id) == end(this->hc->track_data) || this->cursor_state == "none") {
		if (this->cursor_state != "none") {
			this->tmp_width = this->sub_window.getWidth();
			this->tmp_height = this->sub_window.getHeight();
			this->cursor_state = "none";
		}
		
		if (this->sub_window.getWidth() < 200 && this->sub_window.getHeight() < 200) {
			int id = this->scene_id;
			ofNotifyEvent(this->delete_sub_window_event, id);
			return;
		}

		this->life -= 0.5;
		this->sub_window.setWindowSize(this->tmp_width*this->life/this->max_life, this->tmp_height*this->life/this->max_life);
		this->view_rect.setWidth(this->sub_window.getWidth());
		this->view_rect.setHeight(this->sub_window.getHeight());
	}
	else {
		this->life = this->max_life;

		if (!this->view_rect.inside(this->hc->track_data[this->user_id].current_pointer.x, this->hc->track_data[this->user_id].current_pointer.y)) {
			alpha -= 10;
		}
		else {
			alpha = 255;
		}
		if ((this->hc->track_data[this->user_id].current_pointer.x <= 50 && this->hc->track_data[this->user_id].current_pointer.y <= 50) || (abs(this->hc->track_data[this->user_id].current_pointer.x - this->main_window_width) <= 50 && this->hc->track_data[this->user_id].current_pointer.y <= 50) || (this->hc->track_data[this->user_id].current_pointer.x <= 50 && abs(this->hc->track_data[this->user_id].current_pointer.y - this->main_window_height) <= 50) || (abs(this->hc->track_data[this->user_id].current_pointer.x - this->main_window_width) <= 50 && abs(this->hc->track_data[this->user_id].current_pointer.y - this->main_window_height) <= 50)) {
			this->tmp_width = this->sub_window.getWidth();
			this->tmp_height = this->sub_window.getHeight();
			this->cursor_state = "none";
			pair<int, int> id(this->scene_id, this->user_id);
			ofNotifyEvent(this->user_leave_event, id);
			return;
		}

		int width_threshold = this->view_rect.getWidth()*0.1;
		int height_threshold = this->view_rect.getHeight()*0.1;
		if (this->view_rect.getRight() - this->hc->track_data[this->user_id].current_pointer.x < width_threshold) {
			this->view_rect.setX(this->view_rect.getX() + 30);
			this->cursor_state = "arrow_right";
		}
		else if (this->hc->track_data[this->user_id].current_pointer.x - this->view_rect.getLeft() < width_threshold) {
			this->view_rect.setX(this->view_rect.getX() - 30);
			this->cursor_state = "arrow_left";
		}
		else if (this->view_rect.getBottom() - this->hc->track_data[this->user_id].current_pointer.y < height_threshold) {
			this->view_rect.setY(this->view_rect.getY() + 30);
			this->cursor_state = "arrow_down";
		}
		else if (this->hc->track_data[this->user_id].current_pointer.y - this->view_rect.getTop() < height_threshold) {
			this->view_rect.setY(this->view_rect.getY() - 30);
			this->cursor_state = "arrow_up";
		}
		else {
			this->cursor_state = "point";
		}

		this->hc->track_data[this->user_id].current_pointer.x = this->hc->track_data[this->user_id].past_pointer.x = max(min(this->hc->track_data[this->user_id].current_pointer.x, (int)this->view_rect.getRight()), (int)this->view_rect.getLeft());
		this->hc->track_data[this->user_id].current_pointer.y = this->hc->track_data[this->user_id].past_pointer.y = max(min(this->hc->track_data[this->user_id].current_pointer.y, (int)this->view_rect.getBottom()), (int)this->view_rect.getTop());
	}

}

void SubScene::draw() {

	this->sub_window.begin();

	gluLookAt(this->view_rect.getX(), this->view_rect.getY(), 0, this->view_rect.getX(), this->view_rect.getY(), -1, 0, 1, 0);

	ofBackground(255);
	ofSetColor(ofColor::white, this->alpha);
	this->scene->draw(); // シーンの描画

	ofSetColor(ofColor::white, this->alpha);

	if (this->cursor_state == "point") {
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 50; ++i) {
			r += 0.6;
			alpha -= 12;
			ofSetColor(this->hc->track_data[this->user_id].cursor_color, alpha);
			ofCircle(this->hc->track_data[this->user_id].current_pointer.x, this->hc->track_data[this->user_id].current_pointer.y, r);
		}
	}
	else if (this->cursor_state == "none") {

	}
	else {
		this->cursor_texture[this->cursor_state].draw(this->hc->track_data[this->user_id].current_pointer.x, this->hc->track_data[this->user_id].current_pointer.y, 50, 50);
	}

	ofSetColor(ofColor::red);
	ofCircle(this->main_window_width, 0, 10);

	this->sub_window.end();
}

ofRectangle SubScene::get_rect() const {
	return this->sub_window.get_rect();
}

void SubScene::set_rect(const ofRectangle &rect) {
	this->sub_window.setWindowPosition(rect.x, rect.y);
	this->sub_window.setWindowSize(rect.width, rect.height);
}

int SubScene::get_user_id() const {
	return this->user_id;
}

bool SubScene::is_inside(const ofPoint &p) const {
	return this->sub_window.is_inside(p);
}

void SubScene::add_cursor_texture(const string cursor_type, const string texture_path) {
	ofTexture texture;
	ofLoadImage(texture, texture_path);
	this->cursor_texture.insert(make_pair(cursor_type, texture));
}

void SubScene::exit() {
	delete this->scene;
	this->sub_window.hide();
	this->sub_window.close();
}

SubScene::~SubScene() {

}