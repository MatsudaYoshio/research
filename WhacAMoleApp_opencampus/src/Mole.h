// ���O���N���X

#pragma once
#ifndef ___Class_Mole
#define ___Class_Mole

#include <random>

#include "ofMain.h"

#include "AppParameters.h"

class Mole {
private:
	static random_device rd;
	static mt19937 mt;
	static uniform_real_distribution<> random01; // 0~1��Ԃ�����
	static uniform_int_distribution<int> random_angry_time;

	static constexpr double star_probability{ 0.1 };
	static constexpr double angry_probability{ 0.1 };
	static constexpr int king_threshold{ 5 };
	static constexpr int normal_score{ 10 };
	static constexpr int star_score{ normal_score * 2 };
	static constexpr int king_score{ normal_score * 5 };
	const enum class STATE { HIDE, NORMAL, ANGRY, STAR, DAMEGE, KING };

	static long long int previous_star;

	int appearance_time{ 0 }; // �o������
	int hidden_time{ 0 }; // �B�ꎞ��
	int angry_time{ 0 }; // �{�莞��
	long long int score{ 0 }; // ���܂��Ă���X�R�A
	ofRectangle draw_rect; // ���O���摜��`�悷��ʒu�E�傫����ێ������`
	STATE state{ STATE::HIDE };
public:
	static long long int current_star; // ���܂��Ă���X�^�[

	ofEvent<pair<int, int>> explosion_event;

	void setup(const ofRectangle& draw_rect, int hidden_time); // ���O����`�悷��ʒu�E�傫���Ɋւ����`�ƍŏ��̉B�ꎞ�Ԃ������ɃZ�b�g�A�b�v
	void update();
	void draw() const ;

	ofRectangle get_rect() const;
	int get_score();

	bool is_idle() const; // ���O���̉B�ꎞ�ԂƏo�����Ԃ��ǂ������Ȃ���Ԃ��ǂ����𒲂ׂ�i�ɂ��ǂ����j
	bool is_appear() const; // ���O�����o�����Ă��邩�ǂ����𒲂ׂ�

	void appear(int appearance_time); // ���O���ɑ΂��Ďw�肵�����Ԃ����o�����Ԃ����Z����
	void hide(int hidden_time); // ���O���ɑ΂��Ďw�肵�����Ԃ����B�ꎞ�Ԃ����Z����
	void salvage();
};

#endif