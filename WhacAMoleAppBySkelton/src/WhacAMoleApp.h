#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include <random>

#include "SkeltonEstimator.h"
#include "KeyPointsRenderer.h"
#include "Mole.h"

/* OpenPose�̍��i�_�Q�Ƃ̂��߂̒�`(BODY25���f��) */
#define NOSE 0
#define NECK 1
#define RIGHT_SHOULDER 2
#define RIGHT_ELBOW 3
#define RIGHT_WRIST 4
#define LEFT_SHOULDER 5
#define LEFT_ELBOW 6
#define LEFT_WRIST 7
#define MIDDLE_HIP 8
#define RIGHT_HIP 9
#define RIGHT_KNEE 10
#define RIGHT_ANKLE 11
#define LEFT_HIP 12
#define LEFT_KNEE 13
#define LEFT_ANKLE 14
#define RIGHT_EYE 15
#define LEFT_EYE 16
#define RIGHT_EAR 17
#define LEFT_EAR 18
#define LEFT_BIG_TOE 19
#define LEFT_SMALL_TOE 20
#define LEFT_HEEL 21
#define RIGHT_BIG_TOE 22
#define RIGHT_SMALL_TOE 23
#define RIGHT_HEEL 24

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 6 }; // �e�s�ɂ����郂�O���̐�
	static constexpr int mole_num_col{ 8 }; // �e��ɂ����郂�O���̐�
	static constexpr int mole_num_all{ mole_num_row * mole_num_col };
	static constexpr double mole_margin_row{ 400 }; // �e�s�ɂ����郂�O���Ԃ̗]������
	static constexpr double mole_margin_col{ 40 }; // �e��ɂ����郂�O���Ԃ̗]������
	static constexpr double information_view_size{ 200 }; // ���r���[�̑傫��(��ʏ㕔����̒���)
	static const ofColor bg_color; // �w�i�̐F
	static const ofPoint font_draw_point; // �t�H���g��`�悷����W(�)

	/* ���� */
	static random_device rd;
	static mt19937 mt;
	static uniform_int_distribution<int> random_0or1; // 0��1��Ԃ�����
	static uniform_int_distribution<int> random_appearance_time; // ���O���̏o�����Ԃ����߂邽�߂̗���
	static uniform_int_distribution<int> random_hidden_time; // ���O���̉B�ꎞ�Ԃ����߂邽�߂̗���

	SkeltonEstimator se;
	KeyPointsRenderer kpr;

	long long int score{ 0 };
	ofTrueTypeFont font;
	array<Mole, mole_num_all> moles;

	void initialize_image() const;
	void initialize_moles();
	bool is_whacked(const Mole& mole, const ofPoint& key_point) const; // ���O����@���Ă��邩�ǂ����𔻒肷��֐�
public:
	void setup();
	void update();
	void draw();
	void exit();
};

#endif