// アプリ全体で使う変数を管理する

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

	extern const int COCO_KEY_POINTS_NUM; // COCOモデルの骨格点の数
}

#endif