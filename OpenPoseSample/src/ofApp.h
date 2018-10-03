// 手カーソルの検出、情報更新のクラス

#pragma once
#ifndef ___Class_ofApp
#define ___Class_ofApp

#include "ofMain.h"

#include "HandCursor.h"
#include "KeyPointsRenderer.h"

/* OpenPoseの骨格点参照のための定義(COCOモデル) */
#define NOSE_X(i) {i,0,0}
#define NOSE_Y(i) {i,0,1}
#define NECK_X(i) {i,1,0}
#define NECK_Y(i) {i,1,1}
#define RIGHT_SHOULDER_X(i) {i,2,0}
#define RIGHT_SHOULDER_Y(i) {i,2,1}
#define RIGHT_ELBOW_X(i) {i,3,0}
#define RIGHT_ELBOW_Y(i) {i,3,1}
#define RIGHT_WRIST_X(i) {i,4,0}
#define RIGHT_WRIST_Y(i) {i,4,1}
#define LEFT_SHOULDER_X(i) {i,5,0}
#define LEFT_SHOULDER_Y(i) {i,5,1}
#define LEFT_ELBOW_X(i) {i,6,0}
#define LEFT_ELBOW_Y(i) {i,6,1}
#define LEFT_WRIST_X(i) {i,7,0}
#define LEFT_WRIST_Y(i) {i,7,1}
#define RIGHT_HIP_X(i) {i,8,0}
#define RIGHT_HIP_Y(i) {i,8,1}
#define RIGHT_KNEE_X(i) {i,9,0}
#define RIGHT_KNEE_Y(i) {i,9,1}
#define RIGHT_ANKLE_X(i) {i,10,0}
#define RIGHT_ANKLE_Y(i) {i,10,1}
#define LEFT_HIP_X(i) {i,11,0}
#define LEFT_HIP_Y(i) {i,11,1}
#define LEFT_KNEE_X(i) {i,12,0}
#define LEFT_KNEE_Y(i) {i,12,1}
#define LEFT_ANKLE_X(i) {i,13,0}
#define LEFT_ANKLE_Y(i) {i,13,1}
#define RIGHT_EYE_X(i) {i,14,0}
#define RIGHT_EYE_Y(i) {i,14,1}
#define LEFT_EYE_X(i) {i,15,0}
#define LEFT_EYE_Y(i) {i,15,1}
#define RIGHT_EAR_X(i) {i,16,0}
#define RIGHT_EAR_Y(i) {i,16,1}
#define LEFT_EAR_X(i) {i,17,0}
#define LEFT_EAR_Y(i) {i,17,1}
#define BACKGROUND_X(i) {i,18,0}
#define BACKGROUND_Y(i) {i,18,1}

class ofApp : public ofBaseApp {
private:
	HandCursor hc;
	KeyPointsRenderer kpr;

	void draw_pointer() const;
	void show_key_points(const op::Array<float>& key_points) const;
	void show_user_data() const;
	void transform_key_points(op::Array<float>& key_points) const;
public:
	void setup();
	void update();
	void draw();
	void exit();
};

#endif