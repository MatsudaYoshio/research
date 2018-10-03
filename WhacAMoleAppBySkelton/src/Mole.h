// ���O���N���X

#pragma once
#ifndef ___Class_Mole
#define ___Class_Mole

#include "ofMain.h"

#include "AppParameters.h"

class Mole {
private:
	int appearance_time{ 0 }; // �o������
	int hidden_time{ 0 }; // �B�ꎞ��
	ofRectangle draw_rect; // ���O���摜��`�悷��ʒu�E�傫����ێ������`
public:
	void setup(const ofRectangle& draw_rect, int hidden_time); // ���O����`�悷��ʒu�E�傫���Ɋւ����`�ƍŏ��̉B�ꎞ�Ԃ������ɃZ�b�g�A�b�v
	void draw();

	ofRectangle get_rect() const;
	bool is_idle() const; // ���O���̉B�ꎞ�ԂƏo�����Ԃ��ǂ������Ȃ���Ԃ��ǂ����𒲂ׂ�i�ɂ��ǂ����j
	bool is_appear() const; // ���O�����o�����Ă��邩�ǂ����𒲂ׂ�
	void appear(int appearance_time); // ���O���ɑ΂��Ďw�肵�����Ԃ����o�����Ԃ����Z����
	void hide(int hidden_time); // ���O���ɑ΂��Ďw�肵�����Ԃ����B�ꎞ�Ԃ����Z����
};

#endif