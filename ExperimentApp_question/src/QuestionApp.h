#pragma once
#ifndef ___Class_QuestionApp
#define ___Class_QuestionApp

#include "MenuItem.h"
#include "FormSet.h"
#include "Timer.h"

class QuestionApp : public ofBaseApp {
private:
#define LEFT_CLICKED 0
	static constexpr int limit_time{ 3 * 60 }; // 180•b(3•ª)
	static constexpr int question_number{ 4 };
	static const string output_data_directory;
	static const array<array<string, question_number>, param::MENU_ITEM_NUMBER> questions;
	static const array<array<int, question_number>, param::MENU_ITEM_NUMBER> answers;

	const enum class STATE {
		START, RUNNING, TIMEOVER
	};

	STATE state{ STATE::START };
	array<FormSet, param::MENU_ITEM_NUMBER> form_sets;
	Button start_button, submit_button;
	MenuItem menu_item;
	Timer timer;

	string participant_name;
	int menu_item_id;

	void output_data() const;
public:
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
};

#endif