// �X�P���g���i���i�j��`�悷��N���X

#pragma once
#ifndef ___Class_KeyPointsRenderer
#define ___Class_KeyPointsRenderer

#define _USE_MATH_DEFINES

#include <openpose/core/headers.hpp>

#include "ofMain.h"

#include "AppParameters.h"
#include "HandCursor.h"

class KeyPointsRenderer {
private:
	HandCursor* hc;

	static constexpr int BODY25_key_points_num{ 25 }; // openPose��BODY25���f���̍��i�_�̐�
	static constexpr int BODY25_key_points_pairs_num{ 24 }; // openPose��BODY25���f���́i�`�悷�鍜�i�̐����̂��߂́j���i�_�̃y�A�̐�
	static constexpr int point_size{ 20 }; // �`�悷�鍜�i�_�̑傫���i�~�̔��a�j
	static constexpr int line_size = point_size*0.8; // �`�悷�鍜�i�̐����̑���
	static constexpr int skeleton_fade_speed{ 5 };

	static const array<pair<bool, ofColor>, BODY25_key_points_num> points_state; // ���i�_�̕`��ݒ�
	static const array<pair<bool, ofColor>, BODY25_key_points_pairs_num> lines_state; // ���i�̐����̕`��ݒ�
	static const array<pair<int, int>, BODY25_key_points_pairs_num> key_points_pairs; // �`�悷�鍜�i�̐����̂��߂́j���i�_�̃y�A

	int people_num; // ���o���ꂽ�l��
	op::Array<float>* raw_key_points; // ���o���ꂽ���̂܂܂́i�J�������W�́j���i�_
	map<pair<int, int>, ofPoint> transformed_key_points; // �ϊ���̍��i�_�i�f�B�X�v���C���W�j

	void transform_points(); // ���o���ꂽ���i�_���J�������W����f�B�X�v���C���W�֕ϊ�����֐�
public:
	void setup(HandCursor* hc);
	void draw();
};

#endif