#include "AdBar.h"

using namespace param;

const array<ofRectangle, AdBar::ad_item_num> AdBar::ad_position{ ofRectangle(140, 50, 300, 300), ofRectangle(580, 50, 300, 300), ofRectangle(1020, 50, 300, 300), ofRectangle(1460, 50, 300, 300) };

random_device AdBar::rd;
mt19937 AdBar::mt(AdBar::rd());

void AdBar::setup(array<bool, param::MENU_ITEM_NUM>* const menu_item_flag) {
	this->state = STATE::INACTIVE;
	this->menu_item_flag = menu_item_flag;
}

void AdBar::update() {
	switch (this->state) {
	case STATE::INACTIVE:
		if (none_of(begin(*this->menu_item_flag), end(*this->menu_item_flag), [](const auto& x) {return x; })) {
			break;
		}

		for (int i = 0; i < MENU_ITEM_NUM; ++i) {
			if ((*this->menu_item_flag)[i]) {
				for (const auto& c : MENU_ITEM_CONTENTS[i]) {
					this->content_id_list.emplace_back(c);
				}
			}
		}

		{
			uniform_int_distribution<int> random_ad(0, this->content_id_list.size() - 1);
			auto x{ this->content_id_list[random_ad(this->mt)] };
			this->content_id_tmp[0] = x;
			this->ads[0].setup(this->ad_position[0], x);

			for (int i = 1; i < MENU_ITEM_NUM; ++i) {
				x = this->content_id_list[random_ad(this->mt)];
				auto last{ begin(this->content_id_tmp) + i };
				while (find(begin(this->content_id_tmp), last, x) != last) {
					x = this->content_id_list[random_ad(this->mt)];
				}
				this->content_id_tmp[i] = x;
				this->ads[i].setup(this->ad_position[i], x);
			}
		}

		this->state = STATE::ACTIVE;
		break;
	case STATE::ACTIVE:
		break;
	}
}

void AdBar::draw() {
	switch (this->state) {
	case STATE::INACTIVE:
		break;
	case STATE::ACTIVE:
		for (int i = 0; i < this->ad_item_num; ++i) {
			this->ads[i].draw();
		}
		break;
	}
}