#pragma once
#ifndef ___Class_MenuItem
#define ___Class_MenuItem

#include "ofMain.h"
#include "AppParameters.h"

class MenuItem {
private:
	static constexpr double texture_size_ratio{ 0.6 }; // ���ڂɑ΂���e�N�X�`���̔䗦
	static constexpr int font_size{ 30 }; // ���ږ��̃t�H���g�T�C�Y
	static constexpr double progress_ratio{ 0.025 }; // �v���O���X�o�[�̐i�s���x�����߂�䗦
	static constexpr int progress_bar_size{ 20 }; // �v���O���X�o�[�̃T�C�Y(����)
	static constexpr float pointed_alpha{ 100 }; // ���ڂ��I�����ꂽ�Ƃ��̓����x(�A���t�@�l)

	int progress_state{ 0 };
	string item_name;
	param::MENU_ITEM_ID item_id;
	long long int user_id;
	ofTexture texture;
	ofRectangle item_rect, texture_rect;
	ofTrueTypeFont font;
	int font_x, font_y; // ������`�����W
	float alpha{ ofColor::limit() };
public:
	const enum class STATE {
		INACTIVE, POINT
	};
private:
	STATE state{ STATE::INACTIVE };
public:
	ofEvent<pair<param::MENU_ITEM_ID, long long int>> select_event;

	void setup(const string& image_path, const ofRectangle& item_rect, const string& item_name, int font_x, int font_y, param::MENU_ITEM_ID item_id);
	void update();
	void update(long long int user_id);
	void draw() const;
	
	bool is_inside(int x, int y) const;
	void set_state(const STATE& s);
};

#endif