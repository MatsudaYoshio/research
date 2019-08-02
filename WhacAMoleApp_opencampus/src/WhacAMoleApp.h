#pragma once
#ifndef ___Class_WhacAMoleApp
#define ___Class_WhacAMoleApp

#include "SkeltonEstimator.h"
#include "KeyPointsRenderer.h"
#include "Mole.h"
#include "Timer.h"

class WhacAMoleApp : public ofBaseApp {
private:
	static constexpr int mole_num_row{ 4 }; // �e�s�ɂ����郂�O���̐�
	static constexpr int mole_num_col{ 4 }; // �e��ɂ����郂�O���̐�
	static constexpr int mole_num_all{ mole_num_row * mole_num_col }; // ���O���̑���
	static constexpr double mole_margin_row{ 250 }; // �e�s�ɂ����郂�O���Ԃ̗]������
	static constexpr double mole_margin_col{ 40 }; // �e��ɂ����郂�O���Ԃ̗]������
	static constexpr double information_view_size{ 200 }; // ���r���[�̑傫��(��ʏ㕔����̒���)
	static constexpr int bang_time{ 3 };
	static constexpr int result_time{ 13 };
	static const ofColor bg_color; // �w�i�̐F
	static const double mole_width; // �e���O���̕�
	static const double mole_height; // �e���O���̍���
	static const string top_score_path;

	/* ���� */
	static random_device rd;
	static mt19937 mt;
	static uniform_int_distribution<int> random_0or1; // 0��1��Ԃ�����
	static uniform_int_distribution<int> random_appearance_time; // ���O���̏o�����Ԃ����߂邽�߂̗���
	static uniform_int_distribution<int> random_hidden_time; // ���O���̉B�ꎞ�Ԃ����߂邽�߂̗���

	const enum class STATE { PLAY, GAMEOVER };

	SkeltonEstimator se;
	KeyPointsRenderer kpr;
	Timer overall_timer, gameover_timer;

	STATE state{ STATE::PLAY };

	ofTrueTypeFont small_font, big_font;
	ofPoint bang_position;
	array<Mole, mole_num_all> moles;
	long long int playing_score{ 0 }, result_score;
	long long int playing_star{ 0 }, result_star;
	long long int top_score;
	ofstream ofs;

	void setup();
	void update();
	void draw();
	void exit();

	void initialize_image() const;
	void initialize_moles();
	bool is_whacked(const Mole& mole, const ofPoint& key_point) const; // ���O����@���Ă��邩�ǂ����𔻒肷��֐�
	void gameover(pair<int, int>& position);
	void reset_game();
};

#endif