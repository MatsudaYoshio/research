// �A�v���S�̂Ŏg���ϐ����Ǘ�����

#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

//#define LENGTH 6

namespace param {
	/* �J�����Ŏ擾����RGB�摜�̉𑜓x */
	constexpr int CAMERA_WIDTH{ 1920 };
	constexpr int CAMERA_HEIGHT{ 1080 };

	/* �f�B�X�v���C�\���̉𑜓x */
	extern const int DISPLAY_WIDTH;
	extern const int DISPLAY_HEIGHT;

	/* �f�B�X�v���C�\���̉𑜓x�̔��� */
	extern const int HALF_DISPLAY_WIDTH;
	extern const int HALF_DISPLAY_HEIGHT;

	/* �J�����ƃf�B�X�v���C�̉𑜓x�̔䗦 */
	extern const double RESOLUTION_RATIO_WIDTH;
	extern const double RESOLUTION_RATIO_HEIGHT;

	extern const long long int NOT_USER; // �����̃��[�UID

	extern const int BODY25_KEY_POINTS_NUM; // COCO���f���̍��i�_�̐�

	constexpr int MOLE_STATE_NUM{ 6 }; // ���O���̏�Ԑ�
	extern ofImage MOLE_IMAGE[MOLE_STATE_NUM]; // ���O���̉摜
	
	extern ofImage HOSHI_IMAGE;
	extern ofImage KAKERU_IMAGE;
	extern ofImage BANG_IMAGE;
}

#endif