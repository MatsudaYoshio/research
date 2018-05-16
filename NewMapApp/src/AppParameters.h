#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* �J�����Ŏ擾����RGB�摜�̉𑜓x */
	constexpr int CAMERA_W{ 1920 };
	constexpr int CAMERA_H{ 1080 };

	/* �f�B�X�v���C�\���̉𑜓x */
	extern const int DISPLAY_W;
	extern const int DISPLAY_H;

	extern const int HALF_DISPLAY_W;
	extern const int HALF_DISPLAY_H;

	extern const int MAX_SUB_WINDOW_W;
	extern const int MAX_SUB_WINDOW_H;

	extern const int HALF_MAX_SUB_WINDOW_W;
	extern const int HALF_MAX_SUB_WINDOW_H;

	/* �J�����ƃf�B�X�v���C�̉𑜓x�̔䗦 */
	extern const double RESOLUTION_RATIO_W;
	extern const double RESOLUTION_RATIO_H;

	extern const int FRAME_SIZE; // �T�u�E�B���h�E�̘g�̑傫��

	extern const ofRectangle USER_CERTAIN_WINDOW;

	extern const long long int NOT_USER;

	constexpr int MENU_ITEM_NUM{ 4 };
	constexpr int CONTENT_NUM{ 11 };

	const enum class MENU_ITEM_ID { SIGHTSEEING, RESTAURANT, SHOPPING, HOTEL };

	extern const ofColor MENU_ITEM_COLOR[];

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, KIYOMIZUDERA, KYOTO_AQUARIUM, EIJUAN, SODOH, FUNATSURU, HANAROKU, RANTEI
	};

	extern const vector<CONTENT_ID> MENU_ITEM_CONTENTS[];

	using content_data_type = struct {
		int x, y;
		int number;
		MENU_ITEM_ID menu_item_id;
		string image_path;
		string name;
	};

	extern const content_data_type CONTENT_DATA[];
}

#endif