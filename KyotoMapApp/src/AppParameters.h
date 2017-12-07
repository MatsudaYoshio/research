#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* �J�����Ŏ擾����RGB�摜�̉𑜓x */
	constexpr int CAMERA_W = 1920;
	constexpr int CAMERA_H = 1080;

	/* �f�B�X�v���C�\���̉𑜓x */
	constexpr int DISPLAY_W = 3840;
	constexpr int DISPLAY_H = 2160;

	constexpr int HALF_DISPLAY_W = DISPLAY_W >> 1;
	constexpr int HALF_DISPLAY_H = DISPLAY_H >> 1;

	/* �J�����ƃf�B�X�v���C�̉𑜓x�̔䗦 */
	constexpr double RESOLUTION_RATIO_W = DISPLAY_W / CAMERA_W;
	constexpr double RESOLUTION_RATIO_H = DISPLAY_H / CAMERA_H;

	constexpr int FRAME_SIZE = 20; // �T�u�E�B���h�E�̘g�̑傫��

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, RYUKOKU_MUSEUM
	};
}

#endif