#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	static constexpr int W = 1920;
	static constexpr int H = 1080;
	
	static constexpr int GRID_W = 10;
	static constexpr int GRID_H = 10;
	
	static constexpr int FORM_W = W / GRID_W;
	static constexpr int FORM_H = H / GRID_H;
	static constexpr int BITS_SIZE = FORM_W*FORM_H;
	
	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI
	};
}

#endif
