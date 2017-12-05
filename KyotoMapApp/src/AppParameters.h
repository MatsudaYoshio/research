#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* カメラで取得するRGB画像の解像度 */
	constexpr int CAMERA_W = 1920;
	constexpr int CAMERA_H = 1080;

	/* ディスプレイ表示の解像度 */
	constexpr int DISPLAY_W = 3840;
	constexpr int DISPLAY_H = 2160;

	/* カメラとディスプレイの解像度の比率 */
	constexpr double RESOLUTION_RATIO_W = DISPLAY_W / CAMERA_W;
	constexpr double RESOLUTION_RATIO_H = DISPLAY_H / CAMERA_H;

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, RYUKOKU_MUSEUM
	};
}

#endif