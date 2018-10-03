// �X�P���g���i���i�j��`�悷��N���X

#pragma once
#ifndef ___Class_KeyPointsRenderer
#define ___Class_KeyPointsRenderer

#include <openpose/core/headers.hpp>

#include "ofMain.h"

#include "AppParameters.h"

class KeyPointsRenderer {
private:
	static constexpr int COCO_key_points_num{ 18 }; // openPose��COCO���f���̍��i�_�̐�
	static constexpr int COCO_key_points_pairs_num{ 17 }; // openPose��COCO���f���́i�`�悷�鍜�i�̐����̂��߂́j���i�_�̃y�A�̐�
	static constexpr int point_size{ 20 }; // �`�悷�鍜�i�_�̑傫���i�~�̔��a�j
	static constexpr int line_size{ 20 }; // �`�悷�鍜�i�̐����̑���

	static const array<pair<bool, ofColor>, COCO_key_points_num> points_state; // ���i�_�̕`��ݒ�
	static const array<pair<bool, ofColor>, COCO_key_points_pairs_num> lines_state; // ���i�̐����̕`��ݒ�
	static const array<pair<int, int>, COCO_key_points_pairs_num> key_points_pairs; // �`�悷�鍜�i�̐����̂��߂́j���i�_�̃y�A
	
	int people_num; // ���o���ꂽ�l��

	op::Array<float>* raw_key_points; // ���o���ꂽ���̂܂܂́i�J�������W�́j���i�_
	map<pair<int, int>, ofPoint> transformed_key_points; // �ϊ���̍��i�_�i�f�B�X�v���C���W�j

	void transform_points(); // ���o���ꂽ���i�_���J�������W����f�B�X�v���C���W�֕ϊ�����֐�
public:
	void setup(op::Array<float>* const key_points);
	void draw();
	void draw(const map<pair<int, int>, ofPoint>& transformed_key_points) const;
};

#endif