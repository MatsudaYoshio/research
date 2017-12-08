#pragma once
#ifndef ___Class_MainScene
#define ___Class_MainScene

#include "ofMain.h"
#include "HandCursor.h"
#include "DrawBackground.h"
#include "Icon.h"
#include "../../../addons/ofxTrueTypeFontUC/src/ofxTrueTypeFontUC.h"

class MainScene {
private:
	HandCursor* hc;
	DrawBackground db;

	vector<Icon> icons;

	/* ‹Èü‚ğ•`‚­‚½‚ß‚Ì•Ï” */
	static constexpr int curve_vertices_num = 7;
	vector<ofPoint> curve_vertices;
	
	ofxTrueTypeFontUC title_font;
	ofxTrueTypeFontUC kamogawa_font;

	ofTexture hotel_texture;
	ofTexture face_texture;

	/* èƒJ[ƒ\ƒ‹‚ğ•`‚­‚½‚ß‚Ì•Ï” */
	int alpha;
	double r;
public:
	vector<int> user_id_list;

	ofEvent<pair<int, int>> point_event;
	ofEvent<pair<int, int>> make_sub_window_event;

	void setup(HandCursor *hc);
	void update();
	void draw();

	void select_icon(pair<int, int>& id);
	void point_icon(const int &content_id, const int &user_id);

	~MainScene();
};

#endif