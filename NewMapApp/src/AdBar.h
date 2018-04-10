#pragma once
#ifndef ___Class_AdBar
#define ___Class_AdBar

#include <random>
#include <unordered_set>
#include "AppParameters.h"
#include "DrawFrame.h"
#include "AdItem.h"

class AdBar {
private:
	static constexpr int ad_item_num{ 4 };
	static const array<pair<int, int>, ad_item_num> ad_position;
	
	const enum class STATE {
		INACTIVE, ACTIVE
	};

	/* óêêî */
	static std::random_device rd;
	static std::mt19937 mt;

	DrawFrame df;
	STATE state;
	array<bool, param::MENU_ITEM_NUM>* menu_item_flag;
	unordered_set<param::CONTENT_ID> ads_tmp;
	array<AdItem, ad_item_num> ads;
public:
	void setup(array<bool, param::MENU_ITEM_NUM>* const menu_item_flag);
	void update();
	void draw();
};

#endif