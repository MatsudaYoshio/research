#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include <random>

#include "Mole.h"

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 6 }; // �e�s�ɂ����郂�O���̐�
	static constexpr int mole_num_col{ 8 }; // �e��ɂ����郂�O���̐�
	static constexpr int mole_num_all{ mole_num_row * mole_num_col };
	static constexpr double mole_margin_row{ 400 }; // �e�s�ɂ����郂�O���Ԃ̗]������
	static constexpr double mole_margin_col{ 40 }; // �e��ɂ����郂�O���Ԃ̗]������
	static constexpr double information_view_size{ 200 }; // ���r���[�̑傫��(��ʏ㕔����̒���)
	static constexpr double hammer_width{ 200 }; // �n���}�[(�摜)�̕�
	static constexpr double hammer_height{ 200 }; // �n���}�[(�摜)�̍���
	static const ofColor bg_color; // �w�i�̐F

	/* ���� */
	static random_device rd;
	static mt19937 mt;
	static uniform_int_distribution<int> random_0or1; // 0��1��Ԃ�����
	static uniform_int_distribution<int> random_appearance_time; // ���O���̏o�����Ԃ����߂邽�߂̗���
	static uniform_int_distribution<int> random_hidden_time; // ���O���̉B�ꎞ�Ԃ����߂邽�߂̗���

	array<Mole, mole_num_all> moles;

	void initialize_image() const;
	void initialize_moles();
	bool is_whacked(const Mole& mole, const ofRectangle& hammer_rect) const;
public:
	void setup();
	void update();
	void draw();
};

#endif