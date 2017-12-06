#include "SubWindow.h"
#include "AppParameters.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace param;

void SubWindow::setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, int x, int y, int w, int h) {
	this->scene_id = scene_id;
	this->user_id = user_id;
	this->hc = hc;

	this->window.setup(x, y, w, h, true); // 最後の引数をtrueに変えれば枠なしのウィンドウ
	this->window.show();

	this->scene = scene;
	this->scene->setup();

	this->view_rect.set(0, 0, w, h);
	this->frame.set(x - 20, y - 20, w + 40, h + 40);

	this->cursor_state = "point";

	/* ユーザのカーソルをサブウィンドウの中心に移動させる */
	try {
		this->hc->track_data.at(this->user_id).transformed_cursor_point.x() = w >> 1;
		this->hc->track_data.at(this->user_id).transformed_cursor_point.y() = h >> 1;
		this->hc->modulate_cursor(this->user_id);
	}
	catch (std::out_of_range&) {
		this->cursor_state = "none";
	}


	this->life = this->max_life;
}

void SubWindow::setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, ofRectangle rect) {
	this->setup(scene, hc, user_id, scene_id, rect.x, rect.y, rect.width, rect.height);
}

void SubWindow::update() {
	this->scene->update();

	this->view_rect.setWidth(this->window.getWidth());
	this->view_rect.setHeight(this->window.getHeight());
	this->set_frame();

	if (this->hc->track_data.find(this->user_id) == end(this->hc->track_data) || this->cursor_state == "none") {
		if (this->life == this->max_life) {
			int id = this->scene_id;
			ofNotifyEvent(this->cursor_disappear_event, id); // カーソル消滅イベント発火
			this->tmp_width = this->window.getWidth();
			this->tmp_height = this->window.getHeight();
			this->cursor_state = "none";
		}

		/* ウィンドウのライフを徐々に減らしてサイズを小さくしていく */
		this->life -= 5;
		this->window.setWindowSize(this->tmp_width*this->life / this->max_life, this->tmp_height*this->life / this->max_life);
		this->view_rect.setWidth(this->window.getWidth());
		this->view_rect.setHeight(this->window.getHeight());

		/* ウィンドウが一定のサイズ以下になったらウィンドウを消す */
		if (this->window.getWidth() < 200 && this->window.getHeight() < 200) {
			int id = this->scene_id;
			ofNotifyEvent(this->delete_sub_window_event, id); // サブウィンドウ削除イベント発火
			return;
		}
	}
	else {
		try {
			/* ウィンドウの表示領域の端っこに近づくとウィンドウ内でスライド */
			int width_threshold = this->view_rect.getWidth()*0.1;
			int height_threshold = this->view_rect.getHeight()*0.1;
			if (this->view_rect.getRight() - this->hc->track_data.at(this->user_id).transformed_cursor_point.x() < width_threshold) {
				this->view_rect.setX(this->view_rect.getX() + 30);
			}
			else if (this->hc->track_data.at(this->user_id).transformed_cursor_point.x() - this->view_rect.getLeft() < width_threshold) {
				this->view_rect.setX(this->view_rect.getX() - 30);
			}
			else if (this->view_rect.getBottom() - this->hc->track_data.at(this->user_id).transformed_cursor_point.y() < height_threshold) {
				this->view_rect.setY(this->view_rect.getY() + 30);
			}
			else if (this->hc->track_data.at(this->user_id).transformed_cursor_point.y() - this->view_rect.getTop() < height_threshold) {
				this->view_rect.setY(this->view_rect.getY() - 30);
			}

			/* ウィンドウの表示領域を制限 */
			this->view_rect.setX(min(max(static_cast<int>(this->view_rect.getX()), 0), static_cast<int>(DISPLAY_W / 2 - this->view_rect.getWidth())));
			this->view_rect.setY(min(max(static_cast<int>(this->view_rect.getY()), 0), static_cast<int>(DISPLAY_H / 2 - this->view_rect.getHeight())));

			/* カーソルがウィンドウ外に出ないように制限 */
			if (this->hc->track_data.at(this->user_id).transformed_cursor_point.x() < 0) {
				this->hc->track_data.at(this->user_id).transformed_cursor_point.x() = 0;
			}
			else if (this->hc->track_data.at(this->user_id).transformed_cursor_point.x() > DISPLAY_W / 2) {
				this->hc->track_data.at(this->user_id).transformed_cursor_point.x() = DISPLAY_W / 2;
			}
			this->hc->track_data.at(this->user_id).transformed_cursor_point.y() = max(min(this->hc->track_data.at(this->user_id).transformed_cursor_point.y(), static_cast<long>(DISPLAY_H / 2)), 0L);
		}
		catch (std::out_of_range&) {
			this->cursor_state = "none";
		}

	}
}

void SubWindow::draw() {

	try {
		/* ウィンドウの枠を描く */
		ofSetColor(this->hc->track_data.at(this->user_id).cursor_color);
		ofNoFill();
		ofSetLineWidth(20);
		ofDrawRectangle(this->frame);
		ofFill();

		this->window.begin();

		gluLookAt(this->view_rect.getX(), this->view_rect.getY(), 0, this->view_rect.getX(), this->view_rect.getY(), -1, 0, 1, 0); // 視点移動

		this->scene->draw(); // シーンの描画

		/* カーソルの描画 */
		if (this->cursor_state == "point") {
			ofSetColor(ofColor::white);
			int alpha = 255;
			double r = 1;
			for (int i = 0; i < 50; ++i) {
				r += 3;
				alpha -= 12;
				ofSetColor(this->hc->track_data.at(this->user_id).cursor_color, alpha);
				ofCircle(this->hc->track_data.at(this->user_id).transformed_cursor_point.x(), this->hc->track_data.at(this->user_id).transformed_cursor_point.y(), r);
			}
		}

		this->window.end();
	}
	catch (std::out_of_range&) {
		this->cursor_state = "none";
	}
}

ofRectangle SubWindow::get_rect() const {
	return this->window.get_rect();
}

void SubWindow::set_rect(const ofRectangle &rect) {
	this->window.setWindowPosition(rect.x, rect.y);
	this->window.setWindowSize(rect.width, rect.height);
}

void SubWindow::set_frame() {
	this->frame.set(this->window.getX() - 20, this->window.getY() - 20, this->window.getWidth() + 40, this->window.getHeight() + 40);
}

int SubWindow::get_user_id() const {
	return this->user_id;
}

bool SubWindow::is_inside(const ofPoint &p) const {
	return this->window.is_inside(p);
}

void SubWindow::exit() {
	delete this->scene;
	this->window.hide();
	this->window.close();
}