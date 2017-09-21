#include "SubScene.h"
#include "MainScene.h"
#include "DetailScene.h"

#include <opencv2/opencv.hpp>

using namespace cv;

void SubScene::setup(BaseScene* scene, HandPointer* hp, int pointer_id) {
	this->pointer_id = pointer_id;
	this->hp = hp;
	this->hp->track_data[pointer_id].current_pointer = this->hp->track_data[pointer_id].past_pointer = Point(50, 50);
	this->sub_window.setup(this->window_name.c_str(), (this->hp->track_data[this->pointer_id].face.left() + this->hp->track_data[this->pointer_id].face.right()) / 2 - this->window_width / 4, (this->hp->track_data[this->pointer_id].face.top() + this->hp->track_data[this->pointer_id].face.bottom()) / 2 - this->window_height / 4, this->window_width / 2, this->window_height / 2, false);
	// �Ō�̈�����true�ɕς���Θg�Ȃ��̃E�B���h�E
	this->sub_window.show();
	this->scene = scene;
	this->scene->setup(this->hp);
	this->rect.set((this->hp->track_data[this->pointer_id].face.left() + this->hp->track_data[this->pointer_id].face.right()) / 2 - this->window_width / 4, (this->hp->track_data[this->pointer_id].face.top() + this->hp->track_data[this->pointer_id].face.bottom()) / 2 - this->window_height / 4, this->window_width / 2, this->window_height / 2);
	/*
	this->fbo.allocate(this->window_width, this->window_height, GL_RGBA);
	this->fbo.begin();
	ofClear(255, 255, 255, 0);
	this->fbo.end();

	this->scene = scene;
	this->scene.setup(this->hp);

	HWND a = FindWindowA(NULL, this->window_name.c_str());
	SetWindowLongPtr(a, GWL_STYLE, WS_THICKFRAME); //WS_THICKFRAME : �טg | WS_POPUP �g�قڂȂ�
	SetWindowPos(a, NULL, 0, 0, this->window_width / 2, this->window_height / 2, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED); //���ꂪ�Ȃ��ƍĕ`�悳��Ȃ�
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
	
	//glViewport(200, 200, this->window_width / 2, this->window_height / 2);
	gluLookAt(500, 500, 0, 500, 500, -1, 0, 1, 0);

	ofBackground(255);
	ofSetColor(ofColor::white);
	this->scene->draw(); // �V�[���̕`��

	ofSetColor(ofColor::white);

	/* ��|�C���^�̕`�� */
	int alpha = 255;
	double r = 1;
	for (int i = 0; i < 50; ++i) {
		r += 0.6;
		alpha -= 12;
		ofSetColor(this->hp->track_data[this->pointer_id].pointer_color, alpha);
		ofCircle(this->hp->track_data[this->pointer_id].current_pointer.x, this->hp->track_data[this->pointer_id].current_pointer.y, r);
	}

	this->sub_window.end();
}

bool SubScene::is_inside(const ofPoint &p) const {
	return this->rect.inside(p);
}

SubScene::~SubScene() {

}