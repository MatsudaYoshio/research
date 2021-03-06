// スケルトン（骨格）を描画するクラス

#pragma once
#ifndef ___Class_KeyPointsRenderer
#define ___Class_KeyPointsRenderer

#include <openpose/core/headers.hpp>

#include "ofMain.h"

#include "AppParameters.h"

class KeyPointsRenderer {
private:
	static constexpr int COCO_key_points_num{ 18 }; // openPoseのCOCOモデルの骨格点の数
	static constexpr int COCO_key_points_pairs_num{ 17 }; // openPoseのCOCOモデルの（描画する骨格の線分のための）骨格点のペアの数
	static constexpr int point_size{ 20 }; // 描画する骨格点の大きさ（円の半径）
	static constexpr int line_size{ 20 }; // 描画する骨格の線分の太さ

	static const array<pair<bool, ofColor>, COCO_key_points_num> points_state; // 骨格点の描画設定
	static const array<pair<bool, ofColor>, COCO_key_points_pairs_num> lines_state; // 骨格の線分の描画設定
	static const array<pair<int, int>, COCO_key_points_pairs_num> key_points_pairs; // 描画する骨格の線分のための）骨格点のペア
	
	int people_num; // 検出された人数

	op::Array<float>* raw_key_points; // 検出されたそのままの（カメラ座標の）骨格点
	map<pair<int, int>, ofPoint> transformed_key_points; // 変換後の骨格点（ディスプレイ座標）

	void transform_points(); // 検出された骨格点をカメラ座標からディスプレイ座標へ変換する関数
public:
	void setup(op::Array<float>* const key_points);
	void draw();
};

#endif