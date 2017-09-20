#include "SubScene.h"
#include "MainScene.h"
#include "DetailScene.h"

#include <opencv2/opencv.hpp>

using namespace cv;

void SubScene::setup(BaseScene* scene, HandPointer* hp, int pointer_id) {
	this->pointer_id = pointer_id;
	this->hp = hp;

	this->sub_window.setup(this->window_name.c_str(), (this->hp->track_data[this->pointer_id].face.left() + this->hp->track_data[this->pointer_id].face.right()) / 2 - this->window_width / 4, (this->hp->track_data[this->pointer_id].face.top() + this->hp->track_data[this->pointer_id].face.bottom()) / 2 - this->window_height / 4, this->window_width / 2, this->window_height / 2, false);
	this->sub_window.show();
	this->scene = scene;
	this->scene->setup(this->hp);
	/*
	this->fbo.allocate(this->window_width, this->window_height, GL_RGBA);
	this->fbo.begin();
	ofClear(255, 255, 255, 0);
	this->fbo.end();

	this->scene = scene;
	this->scene.setup(this->hp);

	HWND a = FindWindowA(NULL, this->window_name.c_str());
	SetWindowLongPtr(a, GWL_STYLE, WS_THICKFRAME); //WS_THICKFRAME : ×˜g | WS_POPUP ˜g‚Ù‚Ú‚È‚µ
	SetWindowPos(a, NULL, 0, 0, this->window_width / 2, this->window_height / 2, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED); //‚±‚ê‚ª‚È‚¢‚ÆÄ•`‰æ‚³‚ê‚È‚¢
	*/
}

void SubScene::update() {
	this->scene->update();
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
	ofBackground(255);
	ofSetColor(ofColor::white);
	this->scene->draw(); // ƒV[ƒ“‚Ì•`‰æ
	this->sub_window.end();
}

SubScene::~SubScene() {

}