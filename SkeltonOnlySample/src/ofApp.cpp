#include "ofApp.h"

using namespace param;

void ofApp::setup() {
	ofHideCursor(); // カーソル非表示
	ofSetCircleResolution(256); // 円の精密度を設定
}

void ofApp::update() {
	this->se.update();
}

void ofApp::draw() {
	/* 骨格の描画 */
	this->kpr.draw(this->se.transformed_key_points);

	/* 全部の骨格点をコマンドラインに出力 */
	//this->show_key_points();
}

void ofApp::exit() {
	this->se.exit(); // アプリ終了時に手カーソルクラスのexit関数を呼び出す
}

void ofApp::show_key_points() const {
	for (const auto& p : this->se.transformed_key_points) {
		printf("user%d's key point [%d] -> (%f,%f)\n", p.first.first, p.first.second, p.second.x, p.second.y);
	}
}