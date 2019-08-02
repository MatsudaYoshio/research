#include "AppParameters.h"

/* ディスプレイ表示の解像度 */
const int param::DISPLAY_WIDTH{ 3840 };
const int param::DISPLAY_HEIGHT{ 2160 };

/* ディスプレイ表示の解像度の半分 */
const int param::HALF_DISPLAY_WIDTH{ param::DISPLAY_WIDTH >> 1 };
const int param::HALF_DISPLAY_HEIGHT{ param::DISPLAY_HEIGHT >> 1 };

/* カメラとディスプレイの解像度の比率 */
const double param::RESOLUTION_RATIO_WIDTH{ param::DISPLAY_WIDTH / param::CAMERA_WIDTH };
const double param::RESOLUTION_RATIO_HEIGHT{ param::DISPLAY_HEIGHT / param::CAMERA_HEIGHT };

const long long int param::NOT_USER{ -1 }; // 無効のユーザID

const int param::BODY25_KEY_POINTS_NUM{ 25 };

/* ここで値の初期化を行わなずに、WhacAMoleAppで初期化を行う */
ofImage param::MOLE_IMAGE[param::MOLE_STATE_NUM]{ ofImage{} };

ofImage param::HOSHI_IMAGE{ ofImage{} };
ofImage param::KAKERU_IMAGE{ ofImage{} };
ofImage param::BANG_IMAGE{ ofImage{} };