#include "AppParameters.h"

/* ディスプレイ表示の解像度 */
const int param::DISPLAY_WIDTH{ 3840 };
const int param::DISPLAY_HEIGHT{ 2160 };

/* ディスプレイ表示の解像度の半分 */
const int param::HALF_DISPLAY_WIDTH{ param::DISPLAY_WIDTH >> 1 };
const int param::HALF_DISPLAY_HEIGHT{ param::DISPLAY_HEIGHT >> 1 };

/* カメラとディスプレイの解像度の比率 */
const double param::RESOLUTION_RATIO_WIDTH{ param::DISPLAY_WIDTH / param::CAMERA_WIDTH };
const double param::RESOLUTION_RATIO_H{ param::DISPLAY_HEIGHT / param::CAMERA_HEIGHT };

const long long int param::NOT_USER{ -1 }; // 無効のユーザID

/* アルファ値(透過度を決定する) */
const float param::MAX_ALFHA{ ofColor::limit() };
const float param::HALF_MAX_ALFHA{ param::MAX_ALFHA / 2 };

/* ここで値の初期化を行わなずに、WhacAMoleAppで初期化を行う */
ofImage param::HAMMER_IMAGE{ ofImage{} }; // ハンマーの画像
ofImage param::HOLE_IMAGE{ ofImage{} }; // 穴の画像
ofImage param::MOLE_IMAGE{ ofImage{} }; // モグラの画像