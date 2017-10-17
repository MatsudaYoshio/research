#pragma once
#ifndef ___Class_Icon
#define ___Class_Icon

#include "ofMain.h"

class Icon {
private:
	ofRectangle rect; // �A�C�R���̋�`
	ofPoint tl, tr, bl, br; // ����A�E��A�����A�E����4�_
	ofTexture texture; // �A�C�R���ɕ\������e�N�X�`��
	static constexpr double frame_ratio = 0.2; // �A�C�R���̋�`�ɑ΂���g�̔䗦
	int frame_r, frame_g, frame_b;
	int alpha;
	string state; // �A�C�R���̏��
	int selected_user_id; // �A�C�R����I���������[�U��id
public:
	ofEvent<int> transition_event;

	Icon(const int &x, const int &y, const int &width, const int &height, const string &img_path);
	void setup(const int &x, const int &y, const int &width, const int &height, const string &img_path);
	void update();
	void draw();

	bool is_inside(const ofPoint &p) const;
	void change_state(const string &state);
	void set_user_id(const int &user_id);
};

#endif