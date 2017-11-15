#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	constexpr int W = 1920; // ウィンドウの横幅
	constexpr int H = 1080; // ウィンドウの縦幅
	
	constexpr int GRID_W = 120;
	constexpr int GRID_H = 60;
	
	constexpr int FORM_W = W / GRID_W;
	constexpr int FORM_H = H / GRID_H;
	constexpr int BITS_SIZE = FORM_W*FORM_H;
	constexpr int BLOCK_SIZE = FORM_W*FORM_H;
	using genome_type = vector<bool>;
	using window_type = genome_type;

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI
	};
}

#endif