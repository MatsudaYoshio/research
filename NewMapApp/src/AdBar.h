#pragma once
#ifndef ___Class_AdBar
#define ___Class_AdBar

#include <random>
#include "AppParameters.h"
#include "DrawFrame.h"
#include "AdItem.h"

class AdBar {
private:
	static constexpr int ad_item_num{ 4 };
	static const array<ofRectangle, ad_item_num> ad_position;
	
	const enum class STATE {
		INACTIVE, ACTIVE
	};

	/* óêêî */
	static std::random_device rd;
	static std::mt19937 mt;

	DrawFrame df;
	STATE state;
	array<bool, param::MENU_ITEM_NUM>* menu_item_flag;
	vector<param::CONTENT_ID> content_id_list;
	array<AdItem, ad_item_num> ads;
	array<param::CONTENT_ID, ad_item_num> content_id_tmp;
public:
	void setup(array<bool, param::MENU_ITEM_NUM>* const menu_item_flag);
	void update();
	void draw();
};

#endif