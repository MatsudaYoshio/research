#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* カメラで取得するRGB画像の解像度 */
	constexpr int CAMERA_W = 1920; // 横
	constexpr int CAMERA_H = 1080; // 縦

	/* ディスプレイ表示の解像度 */
	constexpr int DISPLAY_W = 3840; // 横
	constexpr int DISPLAY_H = 2160; // 縦
	
	constexpr int GRID_W = 120;
	constexpr int GRID_H = 60;
	
	constexpr int FORM_W = DISPLAY_W / GRID_W;
	constexpr int FORM_H = DISPLAY_H / GRID_H;
	constexpr int BITS_SIZE = FORM_W*FORM_H;
	constexpr int BLOCK_SIZE = FORM_W*FORM_H;
	using genome_type = vector<bool>;
	using window_type = genome_type;

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, RYUKOKU_MUSEUM
	};
}

#endif