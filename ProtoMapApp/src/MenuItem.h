#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "AppParameters.h"

class MenuItem {
public:
	const enum class STATE {
		INACTIVE, POINT, SELECTED
	};

	ofEvent<pair<param::MENU_ITEM_ID, long long int>> select_event;

	void setup(const string& image_path, const ofRectangle& item_rect, const string& item_name, int font_x, int font_y, param::MENU_ITEM_ID item_id);
	void update();
	void update(long long int user_id);
	void draw() const;

	bool is_inside(int x, int y) const;
	bool is_selected() const;
	void set_state(const STATE& s);
private:
	STATE state{ STATE::INACTIVE };

	static constexpr double texture_size_ratio{ 0.6 }; // ���ڂɑ΂���e�N�X�`���̔䗦
	static constexpr int font_size{ 30 }; // ���ږ��̃t�H���g�T�C�Y
	static constexpr double progress_ratio{ 0.05 }; // �v���O���X�o�[�̐i�s���x�����߂�䗦
	static constexpr int progress_bar_size{ 20 }; // �v���O���X�o�[�̃T�C�Y(����)

	int progress_state{ 0 };
	string item_name;
	param::MENU_ITEM_ID item_id;
	long long int user_id;
	ofTexture texture;
	ofRectangle item_rect, texture_rect;
	ofTrueTypeFont font;
	int font_x, font_y; // ������`�����W
	float alpha{ ofColor::limit() };
};

#endif