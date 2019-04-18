#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "AppParameters.h"

class MenuItem {
public:
	const enum class STATE {
		ACTIVE, INACTIVE
	};

	void setup(const string& image_path, const ofRectangle& item_rect, const string& item_name, int font_x, int font_y, param::MENU_ITEM_ID item_id);
	void update();
	void update(long long int user_id);
	void draw() const;

	void set_state(const STATE& s);
	bool is_activated() const;
private:
	STATE state{ STATE::INACTIVE };

	static constexpr double texture_size_ratio{ 0.6 }; // 項目に対するテクスチャの比率
	static constexpr int font_size{ 30 }; // 項目名のフォントサイズ
	static constexpr double progress_ratio{ 0.05 }; // プログレスバーの進行速度を決める比率
	static constexpr int progress_bar_size{ 20 }; // プログレスバーのサイズ(高さ)

	string item_name;
	param::MENU_ITEM_ID item_id;
	long long int user_id;
	ofTexture texture;
	ofRectangle item_rect, texture_rect;
	ofTrueTypeFont font;
	int font_x, font_y; // 文字を描く座標
	float alpha{ ofColor::limit() };
};

#endif