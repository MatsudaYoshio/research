#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* ディスプレイ表示の幅と高さ */
	extern const int DISPLAY_WIDTH;
	extern const int DISPLAY_HEIGHT;

	constexpr int MENU_ITEM_NUMBER{ 4 }; // メニュー項目の数
	extern const ofColor MENU_ITEM_COLOR[]; // メニュー項目の色
}

#endif