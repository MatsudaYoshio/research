#include "MapApp.h"

using namespace param;

void MapApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする
	ofSetCircleResolution(256);
	
	this->initialize_image(); // コンテンツ画像の初期化

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
	for (int i = 0; i < CONTENT_NUM; ++i) {
		CONTENT_IMAGE[i].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/family_villa_keage.jpg");
	}

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kyoto_tower_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/higashihonganji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::NISHIHONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/nishihonganji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SYOSEIEN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/shoseien_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KIYOMIZUDERA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kiyomizudera_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_AQUARIUM)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kyoto_aquarium_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KANNONJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kannonji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TOJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/toji_scene.jpg");
}