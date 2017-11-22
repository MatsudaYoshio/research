#include "SubWindow.h"
#include "AppParameters.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace param;

void SubWindow::setup(BaseScene* scene, HandCursor* hc, int user_id, int scene_id, int x, int y, int w, int h) {
	this->scene_id = scene_id;
	this->user_id = user_id;
	this->hc = hc;

	this->window.setup(x, y, w, h, true); // �Ō�̈�����true�ɕς���Θg�Ȃ��̃E�B���h�E
	this->window.show();

	this->scene = scene;
	this->scene->setup();

	this->view_rect.set(0, 0, w, h);
	this->frame.set(x - 2, y - 2, w + 4, h + 4);

	this->cursor_state = "point";

	/* ���[�U�̃J�[�\�����T�u�E�B���h�E�̒��S�Ɉړ������� */
	ofPoint center = this->view_rect.getCenter();
	try {
		this->hc->track_data.at(this->user_id).current_pointer = this->hc->track_data.at(this->user_id).past_pointer = Point(center.x, center.y);
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
			ofNotifyEvent(this->cursor_disappear_event, id); // �J�[�\�����ŃC�x���g����
			this->tmp_width = this->window.getWidth();
			this->tmp_height = this->window.getHeight();
			this->cursor_state = "none";
		}

		/* �E�B���h�E�̃��C�t�����X�Ɍ��炵�ăT�C�Y�����������Ă��� */
		this->life -= 5;
		this->window.setWindowSize(this->tmp_width*this->life / this->max_life, this->tmp_height*this->life / this->max_life);
		this->view_rect.setWidth(this->window.getWidth());
		this->view_rect.setHeight(this->window.getHeight());

		/* �E�B���h�E�����̃T�C�Y�ȉ��ɂȂ�����E�B���h�E������ */
		if (this->window.getWidth() < 200 && this->window.getHeight() < 200) {
			int id = this->scene_id;
			ofNotifyEvent(this->delete_sub_window_event, id); // �T�u�E�B���h�E�폜�C�x���g����
			return;
		}
	}
	else {
		try {
			/* �E�B���h�E�̕\���̈�̒[�����ɋ߂Â��ƃE�B���h�E���ŃX���C�h */
			int width_threshold = this->view_rect.getWidth()*0.1;
			int height_threshold = this->view_rect.getHeight()*0.1;
			if (this->view_rect.getRight() - W + this->hc->track_data.at(this->user_id).current_pointer.x < width_threshold) {
				this->view_rect.setX(this->view_rect.getX() + 30);
			}
			else if (W - this->hc->track_data.at(this->user_id).current_pointer.x - this->view_rect.getLeft() < width_threshold) {
				this->view_rect.setX(this->view_rect.getX() - 30);
			}
			else if (this->view_rect.getBottom() - this->hc->track_data.at(this->user_id).current_pointer.y < height_threshold) {
				this->view_rect.setY(this->view_rect.getY() + 30);
			}
			else if (this->hc->track_data.at(this->user_id).current_pointer.y - this->view_rect.getTop() < height_threshold) {
				this->view_rect.setY(this->view_rect.getY() - 30);
			}

			/* �E�B���h�E�̕\���̈�𐧌� */
			this->view_rect.setX(min(max(static_cast<int>(this->view_rect.getX()), 0), static_cast<int>(W / 2 - this->view_rect.getWidth())));
			this->view_rect.setY(min(max(static_cast<int>(this->view_rect.getY()), 0), static_cast<int>(H / 2 - this->view_rect.getHeight())));

			/* �J�[�\�����E�B���h�E�O�ɏo�Ȃ��悤�ɐ��� */
			if (W - this->hc->track_data.at(this->user_id).current_pointer.x < 0) {
				this->hc->track_data.at(this->user_id).current_pointer.x = this->hc->track_data.at(this->user_id).past_pointer.x = 0;
			}
			else if (W - this->hc->track_data.at(this->user_id).current_pointer.x > W / 2) {
				this->hc->track_data.at(this->user_id).current_pointer.x = this->hc->track_data.at(this->user_id).past_pointer.x = W / 2;
			}
			this->hc->track_data.at(this->user_id).current_pointer.y = this->hc->track_data.at(this->user_id).past_pointer.y = max(min(this->hc->track_data.at(this->user_id).current_pointer.y, H / 2), 0);
		}
		catch (std::out_of_range&) {
			this->cursor_state = "none";
		}

	}
}

void SubWindow::draw() {

	try {
		/* �E�B���h�E�̘g��`�� */
		ofSetColor(this->hc->track_data.at(this->user_id).cursor_color);
		ofNoFill();
		ofSetLineWidth(4);
		ofDrawRectangle(this->frame);
		ofFill();

		this->window.begin();

		gluLookAt(this->view_rect.getX(), this->view_rect.getY(), 0, this->view_rect.getX(), this->view_rect.getY(), -1, 0, 1, 0); // ���_�ړ�

		this->scene->draw(); // �V�[���̕`��

		/* �J�[�\���̕`�� */
		if (this->cursor_state == "point") {
			ofSetColor(ofColor::white);
			int alpha = 255;
			double r = 1;
			for (int i = 0; i < 50; ++i) {
				r += 0.6;
				alpha -= 12;
				ofSetColor(this->hc->track_data.at(this->user_id).cursor_color, alpha);
				ofCircle(W - this->hc->track_data.at(this->user_id).current_pointer.x, this->hc->track_data.at(this->user_id).current_pointer.y, r);
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
	this->frame.set(this->window.getX() - 2, this->window.getY() - 2, this->window.getWidth() + 4, this->window.getHeight() + 4);
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