#include "MainScene.h"

void MainScene::setup(HandPointer* hp) {
	ofSetBackgroundAuto(true);

	/* アイコンを生成 */
	this->icons.insert(make_pair("test_icon1", Icon(200, 200, 200, 200, "C:/Users/matsuda/Desktop/car.jpg")));
	this->icons.insert(make_pair("test_icon2", Icon(1000, 600, 200, 200, "C:/Users/matsuda/Desktop/car.jpg")));
	
	this->hp = hp;
}

void MainScene::update() {
	/* 手ポインタがアイコンに触れたらイベントを発火 */
	for (auto t : this->hp->track_data) {
		for (auto i : this->icons) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // アイコン名と手ポインタidの情報
				ofNotifyEvent(this->point_event, id);
			}
		}
	}
}

void MainScene::draw() {
	/* アイコンを描画 */
	for (auto i : this->icons) {
		i.second.draw();
	}
}