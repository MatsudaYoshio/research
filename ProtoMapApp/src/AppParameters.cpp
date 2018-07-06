#include "AppParameters.h"

/* ディスプレイ表示の解像度 */
const int param::DISPLAY_W{ 3840 };
const int param::DISPLAY_H{ 2160 };

/* ディスプレイ表示の解像度の半分 */
const int param::HALF_DISPLAY_W{ param::DISPLAY_W >> 1 };
const int param::HALF_DISPLAY_H{ param::DISPLAY_H >> 1 };

/* サブウィンドウの最大サイズ */
const int param::MAX_SUB_WINDOW_W{ 3*param::HALF_DISPLAY_W/4 };
const int param::MAX_SUB_WINDOW_H{ 3*param::HALF_DISPLAY_H/4 };

/* サブウィンドウの最大サイズの半分 */
const int param::HALF_MAX_SUB_WINDOW_W{ param::MAX_SUB_WINDOW_W >> 1 };
const int param::HALF_MAX_SUB_WINDOW_H{ param::MAX_SUB_WINDOW_H >> 1 };

/* カメラとディスプレイの解像度の比率 */
const double param::RESOLUTION_RATIO_W{ param::DISPLAY_W / param::CAMERA_W };
const double param::RESOLUTION_RATIO_H{ param::DISPLAY_H / param::CAMERA_H };

const ofRectangle param::USER_CERTAIN_WINDOW{ 250, 250, 500, 500 }; // ユーザのポインタの重複仮想領域の範囲

const long long int param::NOT_USER{ -1 }; // 無効のユーザID

const ofColor param::MENU_ITEM_COLOR[]{ ofColor::gold, ofColor::lawnGreen, ofColor::pink, ofColor::skyBlue }; // メニュー項目の色

/* メニュー項目に含まれるコンテンツの内訳 */
const vector<param::CONTENT_ID> param::MENU_ITEM_CONTENTS[]{
	vector<param::CONTENT_ID>{param::CONTENT_ID::KYOTO_TOWER, param::CONTENT_ID::HIGASHIHONGANJI, param::CONTENT_ID::SYOSEIEN, param::CONTENT_ID::NISHIHONGANJI, param::CONTENT_ID::KIYOMIZUDERA, param::CONTENT_ID::KYOTO_AQUARIUM, param::CONTENT_ID::KANNONJI, param::CONTENT_ID::TOJI},
	vector<param::CONTENT_ID>{param::CONTENT_ID::EIJUAN, param::CONTENT_ID::SODOH, param::CONTENT_ID::FUNATSURU, param::CONTENT_ID::HANAROKU, param::CONTENT_ID::RANTEI},
	vector<param::CONTENT_ID>{param::CONTENT_ID::AEON_MALL, param::CONTENT_ID::ISETAN, param::CONTENT_ID::AVANTI, param::CONTENT_ID::TAKASHIMAYA, param::CONTENT_ID::YAMASHITA},
	vector<param::CONTENT_ID>{param::CONTENT_ID::ALMONT, param::CONTENT_ID::SAKURA_HONGANJI, param::CONTENT_ID::KYOSTAY_IROHA_TOJI, param::CONTENT_ID::HYATT_REGENCY, param::CONTENT_ID::FAMILY_VILLA_KEAGE}
};

/* コンテンツの情報 */
const param::content_data_type param::CONTENT_DATA[]{
	// y座標を大体90引きすることでピンの位置になる
	param::content_data_type{ 1614, 1026, 1, param::MENU_ITEM_ID::SIGHTSEEING, "Kyoto Tower", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1570, 784, 2, param::MENU_ITEM_ID::SIGHTSEEING, "Higashi-Hongan-ji", param::CONTENT_NAME_POSITION::BOTTOM },
	param::content_data_type{ 1814, 791, 3, param::MENU_ITEM_ID::SIGHTSEEING, "Syoseien", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1197, 798, 4, param::MENU_ITEM_ID::SIGHTSEEING, "Nishi-Hongan-ji", param::CONTENT_NAME_POSITION::LEFT },
	param::content_data_type{ 2937, 568, 5, param::MENU_ITEM_ID::SIGHTSEEING, "Kiyomizudera", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1012, 1025, 6, param::MENU_ITEM_ID::SIGHTSEEING, "Kyoto Aquarium", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 2727, 1513, 7, param::MENU_ITEM_ID::SIGHTSEEING, "Kannon-ji", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 955, 1499, 8, param::MENU_ITEM_ID::SIGHTSEEING, "To-ji", param::CONTENT_NAME_POSITION::TOP },
	param::content_data_type{ 1386, 1070, 1, param::MENU_ITEM_ID::RESTAURANT, "Eijuan", param::CONTENT_NAME_POSITION::BOTTOM },
	param::content_data_type{ 2666, 320, 2, param::MENU_ITEM_ID::RESTAURANT, "Sodoh", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 2101, 287, 3, param::MENU_ITEM_ID::RESTAURANT, "Hunatsuru", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1624, 634, 4, param::MENU_ITEM_ID::RESTAURANT, "Hanaroku", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1741, 1117, 5, param::MENU_ITEM_ID::RESTAURANT, "Rantei", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1368, 1323, 1, param::MENU_ITEM_ID::SHOPPING, "Aeon Mall", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1547, 1165, 2, param::MENU_ITEM_ID::SHOPPING, "Isetan", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1674, 1279, 3, param::MENU_ITEM_ID::SHOPPING, "Avanti", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 2068, 49, 4, param::MENU_ITEM_ID::SHOPPING, "Takashimaya", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 2737, 455, 5, param::MENU_ITEM_ID::SHOPPING, "Kyo-Yamashita", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1852, 1294, 1, param::MENU_ITEM_ID::HOTEL, "Almont", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 1338, 734, 2, param::MENU_ITEM_ID::HOTEL, "Sakura-Honganji", param::CONTENT_NAME_POSITION::TOP },
	param::content_data_type{ 1108, 1484, 3, param::MENU_ITEM_ID::HOTEL, "Kyostay-Iroha-Toji", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 2335, 958, 4, param::MENU_ITEM_ID::HOTEL, "Hyatt Regency", param::CONTENT_NAME_POSITION::RIGHT },
	param::content_data_type{ 3314, 96, 5, param::MENU_ITEM_ID::HOTEL, "Family Villa Keage", param::CONTENT_NAME_POSITION::RIGHT }
};

ofImage param::CONTENT_IMAGE[]{
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{}, 
	ofImage{}, 
	ofImage{}, 
	ofImage{},
	ofImage{}, 
	ofImage{}, 
	ofImage{}, 
	ofImage{}, 
	ofImage{}, 
	ofImage{}, 
	ofImage{}, 
	ofImage{},
	ofImage{},
	ofImage{},
	ofImage{}
};