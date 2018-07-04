#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* カメラで取得するRGB画像の解像度 */
	constexpr int CAMERA_W{ 1920 };
	constexpr int CAMERA_H{ 1080 };

	/* ディスプレイ表示の解像度 */
	extern const int DISPLAY_W;
	extern const int DISPLAY_H;

	/* ディスプレイ表示の解像度の半分 */
	extern const int HALF_DISPLAY_W;
	extern const int HALF_DISPLAY_H;

	/* サブウィンドウの最大サイズ */
	extern const int MAX_SUB_WINDOW_W;
	extern const int MAX_SUB_WINDOW_H;

	/* サブウィンドウの最大サイズの半分 */
	extern const int HALF_MAX_SUB_WINDOW_W;
	extern const int HALF_MAX_SUB_WINDOW_H;

	/* カメラとディスプレイの解像度の比率 */
	extern const double RESOLUTION_RATIO_W;
	extern const double RESOLUTION_RATIO_H;

	extern const ofRectangle USER_CERTAIN_WINDOW; // ユーザのポインタの重複仮想領域の範囲

	extern const long long int NOT_USER; // 無効のユーザID

	constexpr int MENU_ITEM_NUM{ 4 }; // メニュー項目の数

	constexpr int CONTENT_NUM{ 22 }; // コンテンツの数

	const enum class MENU_ITEM_ID { SIGHTSEEING, RESTAURANT, SHOPPING, HOTEL }; // メニュー項目ID

	extern const ofColor MENU_ITEM_COLOR[]; // メニュー項目の色

	/* コンテンツID */
	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, KIYOMIZUDERA, KYOTO_AQUARIUM, KANNONJI, TOJI, EIJUAN, SODOH, FUNATSURU, HANAROKU, RANTEI, AEON_MALL, ISETAN, AVANTI, TAKASHIMAYA, YAMASHITA, ALMONT, SAKURA_HONGANJI, KYOSTAY_IROHA_TOJI, HYATT_REGENCY, FAMILY_VILLA_KEAGE
	};

	extern const vector<CONTENT_ID> MENU_ITEM_CONTENTS[]; // メニュー項目に含まれるコンテンツの内訳

	const enum class CONTENT_NAME_POSITION { TOP, BOTTOM, LEFT, RIGHT }; // コンテンツの名前の配置(ピンから見て上下左右のどれか)

	/* コンテンツデータの型 */
	using content_data_type = struct {
		int x, y; // ピンの指し示す座標
		int number; // 各メニュー項目における番号
		MENU_ITEM_ID menu_item_id; // 所属しているメニュー項目のID
		string name; // 表示する名前
		CONTENT_NAME_POSITION position;
	};

	extern const content_data_type CONTENT_DATA[]; // コンテンツの情報

	/* なぜか画像データをここで初期化することができないから、アプリ起動時に初期化する
	   ここでは変数を用意するだけ */
	extern ofImage CONTENT_IMAGE[];
}

#endif