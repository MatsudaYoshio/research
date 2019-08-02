// スケルトン（骨格）を描画するクラス

#pragma once
#ifndef ___Class_KeyPointsRenderer
#define ___Class_KeyPointsRenderer

#define _USE_MATH_DEFINES

/* OpenPoseの骨格点参照のための定義(BODY25モデル) */
#define NOSE 0
#define NECK 1
#define RIGHT_SHOULDER 2
#define RIGHT_ELBOW 3
#define RIGHT_WRIST 4
#define LEFT_SHOULDER 5
#define LEFT_ELBOW 6
#define LEFT_WRIST 7
#define MIDDLE_HIP 8
#define RIGHT_HIP 9
#define RIGHT_KNEE 10
#define RIGHT_ANKLE 11
#define LEFT_HIP 12
#define LEFT_KNEE 13
#define LEFT_ANKLE 14
#define RIGHT_EYE 15
#define LEFT_EYE 16
#define RIGHT_EAR 17
#define LEFT_EAR 18
#define LEFT_BIG_TOE 19
#define LEFT_SMALL_TOE 20
#define LEFT_HEEL 21
#define RIGHT_BIG_TOE 22
#define RIGHT_SMALL_TOE 23
#define RIGHT_HEEL 24

#include <openpose/core/headers.hpp>

#include "ofMain.h"

#include "AppParameters.h"

class KeyPointsRenderer {
private:
	static constexpr int BODY25_key_points_num{ 25 }; // openPoseのBODY25モデルの骨格点の数
	static constexpr int BODY25_key_points_pairs_num{ 24 }; // openPoseのBODY25モデルの（描画する骨格の線分のための）骨格点のペアの数
	static constexpr int point_size{ 20 }; // 描画する骨格点の大きさ（円の半径）
	static constexpr int line_size = point_size*0.8; // 描画する骨格の線分の太さ

	static const array<pair<bool, ofColor>, BODY25_key_points_num> points_state; // 骨格点の描画設定
	static const array<pair<bool, ofColor>, BODY25_key_points_pairs_num> lines_state; // 骨格の線分の描画設定
	static const array<pair<int, int>, BODY25_key_points_pairs_num> key_points_pairs; // 描画する骨格の線分のための）骨格点のペア

	int people_num; // 検出された人数

	op::Array<float>* raw_key_points; // 検出されたそのままの（カメラ座標の）骨格点
	map<pair<int, int>, ofPoint> transformed_key_points; // 変換後の骨格点（ディスプレイ座標）

	void transform_points(); // 検出された骨格点をカメラ座標からディスプレイ座標へ変換する関数
public:
	void setup(op::Array<float>* const key_points);
	void draw(const map<pair<int, int>, ofPoint>& transformed_key_points) const;
};

#endif