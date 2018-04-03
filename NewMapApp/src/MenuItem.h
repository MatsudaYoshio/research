#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "ofMain.h"
#include "AppParameters.h"

class MenuItem {
private:
	static constexpr double texture_size_ratio{ 0.6 };
	static constexpr int font_size{ 30 };

	string item_name;
	param::MENU_ITEM_ID item_id;
	ofColor color;
	ofTexture texture;
	ofRectangle item_rect, texture_rect;
	ofTrueTypeFont font;
	int font_x, font_y;
	float alpha{ ofColor::limit() };
public:
	const enum class STATE {
		INACTIVE, POINT
	};
private:
	STATE state{ STATE::INACTIVE };
public:
	void setup(const ofColor& color, const string& image_path, const ofRectangle& item_rect, const string& item_name, int font_x, int font_y, param::MENU_ITEM_ID item_id);
	void update();
	void draw() const;
	bool is_inside(int x, int y) const;
	void set_state(const STATE& s);
};

#endif