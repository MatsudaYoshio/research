#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* カメラで取得するRGB画像の解像度 */
	constexpr int CAMERA_WIDTH{ 1920 };
	constexpr int CAMERA_HEIGHT{ 1080 };

	/* ディスプレイ表示の解像度 */
	extern const int DISPLAY_WIDTH;
	extern const int DISPLAY_HEIGHT;

	/* ディスプレイ表示の解像度の半分 */
	extern const int HALF_DISPLAY_WIDTH;
	extern const int HALF_DISPLAY_HEIGHT;

	/* カメラとディスプレイの解像度の比率 */
	extern const double RESOLUTION_RATIO_WIDTH;
	extern const double RESOLUTION_RATIO_HEIGHT;

	extern const long long int NOT_USER; // 無効のユーザID

	extern ofImage HAMMER_IMAGE; // ハンマーの画像
	extern ofImage HOLE_IMAGE; // 穴の画像
	extern ofImage MOLE_IMAGE; // モグラの画像
}

#endif