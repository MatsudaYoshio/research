// �X�P���g���i���i�j��`�悷��N���X

#pragma once
#ifndef ___Class_KeyPointsRenderer
#define ___Class_KeyPointsRenderer

#define _USE_MATH_DEFINES

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

#include <openpose/core/headers.hpp>

#include "ofMain.h"

#include "AppParameters.h"

class KeyPointsRenderer {
private:
	static constexpr int BODY25_key_points_num{ 25 }; // openPose��BODY25���f���̍��i�_�̐�
	static constexpr int BODY25_key_points_pairs_num{ 24 }; // openPose��BODY25���f���́i�`�悷�鍜�i�̐����̂��߂́j���i�_�̃y�A�̐�
	static constexpr int point_size{ 20 }; // �`�悷�鍜�i�_�̑傫���i�~�̔��a�j
	static constexpr int line_size = point_size*0.8; // �`�悷�鍜�i�̐����̑���

	static const array<pair<bool, ofColor>, BODY25_key_points_num> points_state; // ���i�_�̕`��ݒ�
	static const array<pair<bool, ofColor>, BODY25_key_points_pairs_num> lines_state; // ���i�̐����̕`��ݒ�
	static const array<pair<int, int>, BODY25_key_points_pairs_num> key_points_pairs; // �`�悷�鍜�i�̐����̂��߂́j���i�_�̃y�A

	int people_num; // ���o���ꂽ�l��

	op::Array<float>* raw_key_points; // ���o���ꂽ���̂܂܂́i�J�������W�́j���i�_
	map<pair<int, int>, ofPoint> transformed_key_points; // �ϊ���̍��i�_�i�f�B�X�v���C���W�j

	void transform_points(); // ���o���ꂽ���i�_���J�������W����f�B�X�v���C���W�֕ϊ�����֐�
public:
	void setup(op::Array<float>* const key_points);
	void draw(const map<pair<int, int>, ofPoint>& transformed_key_points) const;
};

#endif