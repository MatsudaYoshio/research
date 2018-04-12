#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* カメラで取得するRGB画像の解像度 */
	constexpr int CAMERA_W{ 1920 };
	constexpr int CAMERA_H{ 1080 };

	/* ディスプレイ表示の解像度 */
	constexpr int DISPLAY_W{ 3840 };
	constexpr int DISPLAY_H{ 2160 };

	constexpr int HALF_DISPLAY_W{ DISPLAY_W >> 1 };
	constexpr int HALF_DISPLAY_H{ DISPLAY_H >> 1 };

	constexpr int MAX_SUB_WINDOW_W{ HALF_DISPLAY_W };
	constexpr int MAX_SUB_WINDOW_H{ HALF_DISPLAY_H };

	constexpr int HALF_MAX_SUB_WINDOW_W{ MAX_SUB_WINDOW_W >> 1 };
	constexpr int HALF_MAX_SUB_WINDOW_H{ MAX_SUB_WINDOW_H >> 1 };

	/* カメラとディスプレイの解像度の比率 */
	constexpr double RESOLUTION_RATIO_W{ DISPLAY_W / CAMERA_W };
	constexpr double RESOLUTION_RATIO_H{ DISPLAY_H / CAMERA_H };

	constexpr int FRAME_SIZE{ 20 }; // サブウィンドウの枠の大きさ

	constexpr int MENU_ITEM_NUM{ 4 };
	constexpr int CONTENT_NUM{ 11 };

	const enum class MENU_ITEM_ID {
		SIGHTSEEING, RESTAURANT, SHOPPING, HOTEL
	};

	const array<ofColor, MENU_ITEM_NUM> MENU_ITEM_COLOR{ ofColor::gold, ofColor::lawnGreen, ofColor::pink, ofColor::skyBlue };

	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, KIYOMIZUDERA, KYOTO_AQUARIUM, EIJUAN, SODOH, FUNATSURU, HANAROKU, RANTEI
	};

	const array<vector<CONTENT_ID>, MENU_ITEM_NUM> MENU_ITEM_CONTENTS{
		vector<CONTENT_ID>{ CONTENT_ID::KYOTO_TOWER, CONTENT_ID::HIGASHIHONGANJI, CONTENT_ID::SYOSEIEN, CONTENT_ID::NISHIHONGANJI, CONTENT_ID::KIYOMIZUDERA, CONTENT_ID::KYOTO_AQUARIUM},
		vector<CONTENT_ID>{CONTENT_ID::EIJUAN, CONTENT_ID::SODOH, CONTENT_ID::FUNATSURU, CONTENT_ID::HANAROKU, CONTENT_ID::RANTEI},
		vector<CONTENT_ID>{},
		vector<CONTENT_ID>{}
	};

	using content_data_type = struct {
		int x, y;
		int number;
		MENU_ITEM_ID menu_item_id;
		string image_path;
	};

	const array<content_data_type, CONTENT_NUM> CONTENT_DATA{
		content_data_type{ 1614, 1026, 1, MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_tower.jpg" },
		content_data_type{ 1570, 784, 2, MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/higashihonganji.jpg" },
		content_data_type{ 1814, 791, 3, MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/syoseien_img.jpg" },
		content_data_type{ 1197, 798, 4, MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/nishihonganji.jpg" },
		content_data_type{ 2937, 568, 5, MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kiyomizudera.jpg" },
		content_data_type{ 1012, 1025, 6, MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_aquarium.jpg" },
		content_data_type{ 1386, 1070, 1, MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/eijuan.jpg" },
		content_data_type{ 2666, 320, 2, MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/sodoh.png" },
		content_data_type{ 2101, 287, 3, MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hunatsuru.jpg" },
		content_data_type{ 1624, 634, 4, MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hanaroku.jpg" },
		content_data_type{ 1741, 1117, 5, MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/rantei.jpg" }
	};

	const enum class LANGUAGE_ID {
		JAPANENE, ENGLISH
	};

	constexpr int LANGUAGE{ static_cast<int>(LANGUAGE_ID::ENGLISH) };
}

#endif