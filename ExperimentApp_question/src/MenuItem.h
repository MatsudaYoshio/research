#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "AppParameters.h"

class MenuItem {
private:
	static constexpr double texture_size_ratio{ 0.6 }; // ���ڂɑ΂���e�N�X�`���̔䗦
	static constexpr int font_size{ 25 }; // ���ږ��̃t�H���g�T�C�Y
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