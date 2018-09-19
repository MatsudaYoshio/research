#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include "Mole.h"

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 6 }; // �e�s�ɂ����郂�O���̐�
	static constexpr int mole_num_col{ 8 }; // �e��ɂ����郂�O���̐�
	static constexpr int mole_num_all{ mole_num_row * mole_num_col };
	static constexpr int mole_margin_row{ 400 }; // �e�s�ɂ����郂�O���Ԃ̗]������
	static constexpr int mole_margin_col{ 40 }; // �e��ɂ����郂�O���Ԃ̗]������
	static constexpr int information_view_size{ 200 }; // ���r���[�̑傫��(��ʏ㕔����̒���)
	static const ofColor bg_color; // �w�i�̐F
	
	array<Mole, mole_num_all> moles;

	void initialize_image() const;
	void initialize_moles();
public:
	void setup();
	void update();
	void draw();
};

#endif