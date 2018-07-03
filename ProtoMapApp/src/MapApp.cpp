#include "MapApp.h"

using namespace param;

void MapApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���
	ofSetCircleResolution(256);
	
	this->initialize_image(); // �R���e���c�摜�̏�����

	this->sm.setup(&this->hc);
}

void MapApp::update() {
	this->hc.update();
	this->sm.update();
}

void MapApp::draw() {
	//cout << mouseX << " " << mouseY << endl;
	this->sm.draw();
}

void MapApp::exit() {
	this->hc.exit();
}

void MapApp::initialize_image() {
	for (int i = 0; i < param::CONTENT_NUM; ++i) {
		CONTENT_IMAGE[i].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/family_villa_keage.jpg");
	}

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SYOSEIEN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/shoseien_scene.jpg");
}