#include "SubWindow.h"
#include "AppParameters.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace param;

void SubWindow::setup(unique_ptr<BaseScene>&& scene, HandCursor* hc, long long int user_id, long long int scene_id, int x, int y, int w, int h) {
	this->scene_id = scene_id;
	this->user_id = user_id;
	this->hc = hc;

	this->window.setup(x, y, w, h, true); // 最後の引数をtrueに変えれば枠なしのウィンドウ
	this->window.show();

	this->scene = move(scene);
	this->scene->setup();

	this->view_rect.set(0, 0, w, h);

	this->cursor_state = static_cast<int>(CURSOR_STATE::POINT);

	try {
		/* ユーザのカーソルをサブウィンドウの中心に移動させる */
		this->hc->user_data.at(this->user_id).transformed_cursor_point.x() = w >> 1;
		this->hc->user_data.at(this->user_id).transformed_cursor_point.y() = h >> 1;
		this->hc->modulate_cursor(this->user_id);

		this->df.setup(this->hc->user_data.at(this->user_id).cursor_color, FRAME_SIZE);
	}
	catch (std::out_of_range&) {
		this->cursor_state = static_cast<int>(CURSOR_STATE::INACTIVE);
	}

	//this->life = this->max_life;

	this->track_index = this->TRACK_READY;
}

void SubWindow::setup(unique_ptr<BaseScene>&& scene, HandCursor* hc, long long int user_id, long long int scene_id, ofRectangle rect) {
	this->setup(move(scene), hc, user_id, scene_id, rect.x, rect.y, rect.width, rect.height);
}

void SubWindow::update() {
	if (this->cursor_state == static_cast<int>(CURSOR_STATE::POINT)) {
		if (this->track_index == this->track_rects_num) {
			this->track_index = this->TRACK_READY;
		}
		else if (this->track_index != this->TRACK_READY) {
			this->set_rect(this->track_rects[this->track_index++]);
		}
	}

	this->view_rect.setWidth(this->window.getWidth());
	this->view_rect.setHeight(this->window.getHeight());

	if (this->hc->user_data.find(this->user_id) == end(this->hc->user_data) || this->cursor_state == static_cast<int>(CURSOR_STATE::INACTIVE)) {
		this->cursor_state = static_cast<int>(CURSOR_STATE::INACTIVE);
		this->tmp_id = this->scene_id;
		ofNotifyEvent(this->delete_sub_window_event, this->tmp_id); // サブウィンドウ削除イベント発火
		return;
	}
	else {
		try {
			/* ウィンドウの表示領域の端っこに近づくとウィンドウ内でスライド */
			const double width_threshold = this->view_rect.getWidth()*0.1;
			const double height_threshold = this->view_rect.getHeight()*0.1;
			if (this->view_rect.getRight() - this->hc->user_data.at(this->user_id).transformed_cursor_point.x() < width_threshold) {
				this->view_rect.setX(this->view_rect.getX() + width_threshold - this->view_rect.getRight() + this->hc->user_data.at(this->user_id).transformed_cursor_point.x());
			}
			else if (this->hc->user_data.at(this->user_id).transformed_cursor_point.x() - this->view_rect.getLeft() < width_threshold) {
				this->view_rect.setX(this->view_rect.getX() - width_threshold - this->view_rect.getLeft() + this->hc->user_data.at(this->user_id).transformed_cursor_point.x());
			}
			else if (this->view_rect.getBottom() - this->hc->user_data.at(this->user_id).transformed_cursor_point.y() < height_threshold) {
				this->view_rect.setY(this->view_rect.getY() + height_threshold - this->view_rect.getBottom() + this->hc->user_data.at(this->user_id).transformed_cursor_point.y());
			}
			else if (this->hc->user_data.at(this->user_id).transformed_cursor_point.y() - this->view_rect.getTop() < height_threshold) {
				this->view_rect.setY(this->view_rect.getY() - height_threshold - this->view_rect.getTop() + this->hc->user_data.at(this->user_id).transformed_cursor_point.y());
			}

			/* ウィンドウの表示領域を制限 */
			this->view_rect.setX(ofClamp(this->view_rect.getX(), 0, HALF_DISPLAY_W - this->view_rect.getWidth()));
			this->view_rect.setY(ofClamp(this->view_rect.getY(), 0, HALF_DISPLAY_H - this->view_rect.getHeight()));

			/* カーソルがウィンドウ外に出ないように制限 */
			this->hc->user_data.at(this->user_id).transformed_cursor_point.x() = ofClamp(this->hc->user_data.at(this->user_id).transformed_cursor_point.x(), 0, HALF_DISPLAY_W);
			this->hc->user_data.at(this->user_id).transformed_cursor_point.y() = ofClamp(this->hc->user_data.at(this->user_id).transformed_cursor_point.y(), 0, HALF_DISPLAY_H);
		}
		catch (std::out_of_range&) {
			this->cursor_state = static_cast<int>(CURSOR_STATE::INACTIVE);
		}
	}

	this->df.update(this->view_rect);
}

void SubWindow::draw() {
	try {
		this->window.begin(); // サブウィンドウの描画開始

		gluLookAt(this->view_rect.getX(), this->view_rect.getY(), 0, this->view_rect.getX(), this->view_rect.getY(), -1, 0, 1, 0); // 視点移動

		ofBackground(ofColor::white);
		//ofBackgroundGradient(ofColor::white, this->hc->user_data.at(this->user_id).cursor_color, OF_GRADIENT_LINEAR);

		this->scene->draw(); // シーンの描画

		this->df.draw(); // 枠の描画

		/* カーソルの描画 */
		if (this->cursor_state == static_cast<int>(CURSOR_STATE::POINT)) {
			ofNoFill();
			ofSetLineWidth(60);
			ofSetColor(ofColor::white);
			ofDrawCircle(this->hc->user_data.at(this->user_id).transformed_cursor_point.x(), this->hc->user_data.at(this->user_id).transformed_cursor_point.y(), 60);
			ofFill();
			ofSetColor(this->hc->user_data.at(this->user_id).cursor_color);
			ofDrawCircle(this->hc->user_data.at(this->user_id).transformed_cursor_point.x(), this->hc->user_data.at(this->user_id).transformed_cursor_point.y(), 55);
		}

		this->window.end(); // サブウィンドウのの描画終了
	}
	catch (std::out_of_range&) {
		this->cursor_state = static_cast<int>(CURSOR_STATE::INACTIVE);
	}
}

ofRectangle SubWindow::get_rect() const {
	return this->window.get_rect();
}

void SubWindow::set_rect(const ofRectangle& rect) {
	if (this->cursor_state == static_cast<int>(CURSOR_STATE::POINT)) {
		this->window.setWindowPosition(rect.x, rect.y);
		this->window.setWindowSize(rect.width, rect.height);
	}
}

int SubWindow::get_user_id() const {
	return this->user_id;
}

bool SubWindow::is_inside(const ofPoint& p) const {
	return this->window.is_inside(p);
}

void SubWindow::exit() {
	this->window.hide();
	this->window.close();
}