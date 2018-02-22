#pragma once
#ifndef ___Class_MainScene
#define ___Class_MainScene

#include "ofMain.h"
#include "HandCursor.h"
#include "Icon.h"
#include "../../../addons/ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h"

class MainScene {
private:
	HandCursor* hc;

	vector<Icon> icons;

	/* �Ȑ���`�����߂̕ϐ� */
	static constexpr int curve_vertices_num{ 7 };
	vector<ofPoint> curve_vertices;
	
	ofxTrueTypeFontUC title_font;
	ofxTrueTypeFontUC kamogawa_font;

	ofTexture hotel_texture;
	ofTexture face_texture;

	/* ��J�[�\����`�����߂̕ϐ� */
	int alpha;
	double r;
public:
	set<long long int> user_id_list;

	ofEvent<pair<int, long long int>> point_event;
	ofEvent<pair<int, long long int>> make_sub_window_event;

	void setup(HandCursor *hc);
	void update();
	void draw();

	void select_icon(pair<int, long long int>& id);
	void point_icon(int content_id, long long int user_id);

	~MainScene();
};

#endif