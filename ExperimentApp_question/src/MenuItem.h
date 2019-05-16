#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "AppParameters.h"

class MenuItem {
private:
	static constexpr double texture_size_ratio{ 0.6 }; // 項目に対するテクスチャの比率
	static constexpr int font_size{ 25 }; // 項目名のフォントサイズ
	static const array<string, param::MENU_ITEM_NUMBER> menu_item_name;
	static const array<string, param::MENU_ITEM_NUMBER> menu_item_image_path;
	static const ofPoint font_position;
	static const ofRectangle item_rect, texture_rect;

    int item_id;
	ofTexture texture;
	ofTrueTypeFont font;
public:
	void setup(int menu_item_id);
	void draw() const;
};

#endif