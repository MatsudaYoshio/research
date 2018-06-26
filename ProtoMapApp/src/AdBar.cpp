#include "AdBar.h"

using namespace param;

const array<ofRectangle, AdBar::ad_item_num> AdBar::ad_position{ ofRectangle(30, 30, 420, 300), ofRectangle(510, 30, 420, 300), ofRectangle(990, 30, 420, 300), ofRectangle(1470, 30, 420, 300) };

random_device AdBar::rd;
mt19937 AdBar::mt(AdBar::rd());

void AdBar::setup(array<long long int, param::MENU_ITEM_NUM>* const menu_item_user_id) {
	this->state = STATE::INACTIVE;
	this->menu_item_user_id = menu_item_user_id;
}

void AdBar::update() {
	if (all_of(begin(*this->menu_item_user_id), end(*this->menu_item_user_id), [this](const auto& x) {return x == NOT_USER; })) {
		this->state = STATE::INACTIVE;
		return;
	}

	switch (this->state) {
	case STATE::INACTIVE:
		{
			int n;
			for (int i = 0; i < MENU_ITEM_NUM; ++i) {
				if ((*this->menu_item_user_id)[i] != NOT_USER) {
					this->content_id_list = MENU_ITEM_CONTENTS[i];
					n = i + 1;
					break;
				}
			}

			for (int i = n; i < MENU_ITEM_NUM; ++i) {
				if ((*this->menu_item_user_id)[i] != NOT_USER) {
					copy(begin(MENU_ITEM_CONTENTS[i]), end(MENU_ITEM_CONTENTS[i]), back_inserter(this->content_id_list));
				}
			}

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
		int x;
		for (int i = 0; i < MENU_ITEM_NUM; ++i) {
			if ((*this->menu_item_user_id)[i] != NOT_USER) {
				this->content_id_list = MENU_ITEM_CONTENTS[i];
				x = i + 1;
				break;
			}
		}

		for (int i = x; i < MENU_ITEM_NUM; ++i) {
			if ((*this->menu_item_user_id)[i] != NOT_USER) {
				copy(begin(MENU_ITEM_CONTENTS[i]), end(MENU_ITEM_CONTENTS[i]), back_inserter(this->content_id_list));
			}
		}

		for (int i = 0; i < this->ad_item_num; ++i) {
			++this->change_weight[i];
		}

		if (this->time_count++ > this->change_time_threshold) {
			discrete_distribution<int> random_change_ad(begin(this->change_weight), end(this->change_weight));
			auto n{ random_change_ad(this->mt) };

			uniform_int_distribution<int> random_ad(0, this->content_id_list.size() - 1);
			auto id{ this->content_id_list[random_ad(this->mt)] };
			while (find(begin(this->content_id_tmp), end(this->content_id_tmp), id) != end(this->content_id_tmp)) {
				id = this->content_id_list[random_ad(this->mt)];
			}
			this->content_id_tmp[n] = id;
			this->ads[n].change(id);

			this->change_weight[n] = 0;
			this->time_count = 0;
		}

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