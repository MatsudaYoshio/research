#include "AppParameters.h"

const int param::DISPLAY_W{ 3840 };
const int param::DISPLAY_H{ 2160 };

const int param::HALF_DISPLAY_W{ param::DISPLAY_W >> 1 };
const int param::HALF_DISPLAY_H{ param::DISPLAY_H >> 1 };

const int param::MAX_SUB_WINDOW_W{ param::HALF_DISPLAY_W };
const int param::MAX_SUB_WINDOW_H{ param::HALF_DISPLAY_H };

const int param::HALF_MAX_SUB_WINDOW_W{ param::MAX_SUB_WINDOW_W >> 1 };
const int param::HALF_MAX_SUB_WINDOW_H{ param::MAX_SUB_WINDOW_H >> 1 };

const double param::RESOLUTION_RATIO_W{ param::DISPLAY_W / param::CAMERA_W };
const double param::RESOLUTION_RATIO_H{ param::DISPLAY_H / param::CAMERA_H };

const int param::FRAME_SIZE{ 20 };

const ofRectangle param::USER_CERTAIN_WINDOW{ 250, 250, 500, 500 };

const ofColor param::MENU_ITEM_COLOR[]{ ofColor::gold, ofColor::lawnGreen, ofColor::pink, ofColor::skyBlue };

const vector<param::CONTENT_ID> param::MENU_ITEM_CONTENTS[]{
	vector<param::CONTENT_ID>{ param::CONTENT_ID::KYOTO_TOWER, param::CONTENT_ID::HIGASHIHONGANJI, param::CONTENT_ID::SYOSEIEN, param::CONTENT_ID::NISHIHONGANJI, param::CONTENT_ID::KIYOMIZUDERA, param::CONTENT_ID::KYOTO_AQUARIUM},
	vector<param::CONTENT_ID>{param::CONTENT_ID::EIJUAN, param::CONTENT_ID::SODOH, param::CONTENT_ID::FUNATSURU, param::CONTENT_ID::HANAROKU, param::CONTENT_ID::RANTEI},
	vector<param::CONTENT_ID>{},
	vector<param::CONTENT_ID>{}
};

const param::content_data_type param::CONTENT_DATA[]{
	param::content_data_type{ 1614, 1026, 1, param::MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_tower.jpg", "Kyoto Tower" },
	param::content_data_type{ 1570, 784, 2, param::MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/higashihonganji.jpg", "Higashi-Hongan-ji" },
	param::content_data_type{ 1814, 791, 3, param::MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/syoseien_img.jpg", "Syoseien" },
	param::content_data_type{ 1197, 798, 4, param::MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/nishihonganji.jpg", "Nishi-Hongan-ji" },
	param::content_data_type{ 2937, 568, 5, param::MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kiyomizudera.jpg", "Kiyomizudera" },
	param::content_data_type{ 1012, 1025, 6, param::MENU_ITEM_ID::SIGHTSEEING, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyoto_aquarium.jpg", "Kyoto Aquarium" },
	param::content_data_type{ 1386, 1070, 1, param::MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/eijuan.jpg", "Eijuan" },
	param::content_data_type{ 2666, 320, 2, param::MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/sodoh.png" , "Sodoh" },
	param::content_data_type{ 2101, 287, 3, param::MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hunatsuru.jpg", "Hunatsuru" },
	param::content_data_type{ 1624, 634, 4, param::MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hanaroku.jpg", "Hanaroku" },
	param::content_data_type{ 1741, 1117, 5, param::MENU_ITEM_ID::RESTAURANT, "C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/rantei.jpg", "Rantei" }
};