// �A�v���S�̂Ŏg���ϐ����Ǘ�����

#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

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

	extern const int COCO_KEY_POINTS_NUM; // COCO���f���̍��i�_�̐�
}

#endif