#include "SubScene.h"
#include "MainScene.h"
#include "DetailScene.h"

#include <opencv2/opencv.hpp>

using namespace cv;

void SubScene::setup(BaseScene* scene, HandPointer* hp, int pointer_id, int scene_id, int x, int y, int w, int h) {
	this->scene_id = scene_id;
	this->pointer_id = pointer_id;
	this->hp = hp;
	//int window_width = this->main_window_width / 2;
	//int window_height = this->main_window_height / 2;
	int window_width = w;
	int window_height = h;
	//this->sub_window.setup(this->window_name.c_str(), (this->hp->track_data[this->pointer_id].face.left() + this->hp->track_data[this->pointer_id].face.right()) / 2 - this->main_window_width / 4, (this->hp->track_data[this->pointer_id].face.top() + this->hp->track_data[this->pointer_id].face.bottom()) / 2 - this->main_window_height / 4, window_width, window_height, false);
	// 最後の引数をtrueに変えれば枠なしのウィンドウ
	this->sub_window.setup(this->window_name.c_str(), x, y, w, h, false);
	this->sub_window.show();
	this->scene = scene;
	this->scene->setup(this->hp);
	//this->window_rect.set((this->hp->track_data[this->pointer_id].face.left() + this->hp->track_data[this->pointer_id].face.right()) / 2 - this->main_window_width / 4, (this->hp->track_data[this->pointer_id].face.top() + this->hp->track_data[this->pointer_id].face.bottom()) / 2 - this->main_window_height / 4, window_width, window_height);
	//this->view_rect.set(0, 0, window_width, window_height);
	this->view_rect.set(0, 0, w, h);
	ofPoint center = this->view_rect.getCenter();
	this->hp->track_data[this->pointer_id].current_pointer = this->hp->track_data[this->pointer_id].past_pointer = Point(center.x, center.y);
	ofTexture texture;
	ofLoadImage(texture, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_down.png");
	this->cursor_texture.insert(make_pair("arrow_down", texture));
	ofLoadImage(texture, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_left.png");
	this->cursor_texture.insert(make_pair("arrow_left", texture));
	ofLoadImage(texture, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_right.png");
	this->cursor_texture.insert(make_pair("arrow_right", texture));
	ofLoadImage(texture, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/mark_arrow_up.png");
	this->cursor_texture.insert(make_pair("arrow_up", texture));
	this->cursor_state = "point";
	/*
	this->fbo.allocate(this->window_width, this->window_height, GL_RGBA);
	this->fbo.begin();
	ofClear(255, 255, 255, 0);
	this->fbo.end();

	this->scene = scene;
	this->scene.setup(this->hp);

	HWND a = FindWindowA(NULL, this->window_name.c_str());
	SetWindowLongPtr(a, GWL_STYLE, WS_THICKFRAME); //WS_THICKFRAME : 細枠 | WS_POPUP 枠ほぼなし
	SetWindowPos(a, NULL, 0, 0, this->window_width / 2, this->window_height / 2, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED); //これがないと再描画されない
	*/
}

void SubScene::update() {
	this->scene->update();

	if (this->hp->track_data.find(this->pointer_id) == end(this->hp->track_data) || this->cursor_state == "none") {
		this->cursor_state = "none";
		if (--this->life == 0 || (this->sub_window.getHeight() < 100 && this->sub_window.getWidth() < 100)) {
			int id = this->scene_id;
			ofNotifyEvent(this->delete_sub_window_event, id);
			return;
		}

		this->sub_window.setWindowSize(this->sub_window.getWidth()*0.9, this->sub_window.getHeight()*0.9);
		this->view_rect.setWidth(this->sub_window.getWidth());
		this->view_rect.setHeight(this->sub_window.getHeight());
	}
	else {
		this->life = 100;

		if (!this->view_rect.inside(this->hp->track_data[this->pointer_id].current_pointer.x, this->hp->track_data[this->pointer_id].current_pointer.y)) {
			alpha -= 10;
		}
		else {
			alpha = 255;
		}
		int width_threshold = this->view_rect.getWidth()*0.1;
		int height_threshold = this->view_rect.getHeight()*0.1;
		if (this->view_rect.getRight() - this->hp->track_data[this->pointer_id].current_pointer.x < width_threshold) {
			this->view_rect.setX(this->view_rect.getX() + 30);
			this->cursor_state = "arrow_right";
		}
		else if (this->hp->track_data[this->pointer_id].current_pointer.x - this->view_rect.getLeft() < width_threshold) {
			this->view_rect.setX(this->view_rect.getX() - 30);
			this->cursor_state = "arrow_left";
		}
		else if (this->view_rect.getBottom() - this->hp->track_data[this->pointer_id].current_pointer.y < height_threshold) {
			this->view_rect.setY(this->view_rect.getY() + 30);
			this->cursor_state = "arrow_down";
		}
		else if (this->hp->track_data[this->pointer_id].current_pointer.y - this->view_rect.getTop() < height_threshold) {
			this->view_rect.setY(this->view_rect.getY() - 30);
			this->cursor_state = "arrow_up";
		}
		else {
			this->cursor_state = "point";
		}

		this->hp->track_data[this->pointer_id].current_pointer.x = this->hp->track_data[this->pointer_id].past_pointer.x = max(min(this->hp->track_data[this->pointer_id].current_pointer.x, (int)this->view_rect.getRight()), (int)this->view_rect.getLeft());
		this->hp->track_data[this->pointer_id].current_pointer.y = this->hp->track_data[this->pointer_id].past_pointer.y = max(min(this->hp->track_data[this->pointer_id].current_pointer.y, (int)this->view_rect.getBottom()), (int)this->view_rect.getTop());
	}
	
}

void SubScene::draw() {
	/*
	this->fbo.begin();
	ofClear(255, 255, 255, 0);
	ofSetColor(ofColor::white);
	this->scene.draw();
	this->fbo.end();
	this->fbo.readToPixels(this->pixels);
	this->img.setFromPixels(this->pixels);
	this->img.crop(0, 0, this->window_width / 2, this->window_height / 2);
	Mat cv_img(this->window_height / 2, this->window_width / 2, CV_8UC4, this->img.getPixels());
	cvtColor(cv_img, cv_img, CV_RGBA2BGRA);
	imshow(this->window_name, cv_img);
	*/

	this->sub_window.begin();
	
	//glViewport(200, 200, this->window_width / 2, this->window_height / 2);
	//gluLookAt(500, 500, 0, 500, 500, -1, 0, 1, 0);

	gluLookAt(this->view_rect.getX(), this->view_rect.getY(), 0, this->view_rect.getX(), this->view_rect.getY(), -1, 0, 1, 0);

	ofBackground(255);
	ofSetColor(ofColor::white, this->alpha);
	this->scene->draw(); // シーンの描画

	ofSetColor(ofColor::white, this->alpha);

	if (this->cursor_state == "point") {
		/* 手ポインタの描画 */
		int alpha = 255;
		double r = 1;
		for (int i = 0; i < 50; ++i) {
			r += 0.6;
			alpha -= 12;
			ofSetColor(this->hp->track_data[this->pointer_id].pointer_color, alpha);
			ofCircle(this->hp->track_data[this->pointer_id].current_pointer.x, this->hp->track_data[this->pointer_id].current_pointer.y, r);
		}
	}
	else if (this->cursor_state == "none") {
	
	}else{
		this->cursor_texture[this->cursor_state].draw(this->hp->track_data[this->pointer_id].current_pointer.x, this->hp->track_data[this->pointer_id].current_pointer.y, 50, 50);
	}

	this->sub_window.end();
}

bool SubScene::is_inside(const ofPoint &p) const {
	return this->window_rect.inside(p);
}

void SubScene::exit() {
	delete this->scene;
	this->sub_window.hide();
	this->sub_window.close();
}

SubScene::~SubScene() {

}