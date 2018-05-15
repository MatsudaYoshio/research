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
	static constexpr int change_time_threshold{ 500 };
	static const array<ofRectangle, ad_item_num> ad_position;

	const enum class STATE {
		INACTIVE, ACTIVE
	};

	/* óêêî */
	static std::random_device rd;
	static std::mt19937 mt;

	DrawFrame df;
	STATE state;
	array<long long int, param::MENU_ITEM_NUM>* menu_item_user_id;
	vector<param::CONTENT_ID> content_id_list;
	array<AdItem, ad_item_num> ads;
	array<param::CONTENT_ID, ad_item_num> content_id_tmp;
	array<int, ad_item_num> change_weight;
	int time_count{ 0 };
public:
	void setup(array<long long int, param::MENU_ITEM_NUM>* const menu_item_user_id);
	void update();
	void draw();
};

#endif