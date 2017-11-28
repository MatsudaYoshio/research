#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	constexpr int W = 1920;
	constexpr int H = 1080;

	constexpr int GRID_W = 120;
	constexpr int GRID_H = 60;

	constexpr int FORM_W = W / GRID_W;
	constexpr int FORM_H = H / GRID_H;
	constexpr int BITS_SIZE = FORM_W*FORM_H;
	using genome_type = bitset<BITS_SIZE>;

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, RYUKOKU_MUSEUM
	};
}

#endif