#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"
#include "ProgressBar.h"

class Icon {
private:
	static constexpr double big_ratio = 1.5;

	int content_id;
	ofRectangle default_rect, big_rect, rect; // �A�C�R���̋�`
	int default_w, default_h;
	ofTexture texture; // �A�C�R���ɕ\������e�N�X�`��
	int alpha;
	int selected_user_id; // �A�C�R����I���������[�U��id
	ProgressBar pb; // �v���O���X�o�[
	double angle;
public:
	ofEvent<pair<int,int>> select_event;
	int state; // �A�C�R���̏��
	const enum class STATE {
		INACTIVE, POINT
	};

	void setup(const int &x, const int &y, const int &width, const int &height, const string &img_path, const int &content_id);
	void update();
	void draw();

	bool is_inside(const ofPoint &p) const;
	void change_state(const STATE& state);
	void set_user_id(const int &user_id);
	int get_content_id() const;
};

#endif