#pragma once
#ifndef ___Class_FormSet
#define ___Class_FormSet

#include "Button.h"
#include "AppParameters.h"

class FormSet {
private:
	static constexpr int option_size{ 8 };

	ofTrueTypeFont font;
	ofPoint position;
	array<Button, option_size + 1> number_buttons;

	string question_text;

	void setting_font();
public:
	int selected_number{ option_size };

	void setup(const ofPoint& position, const ofColor& color, const string& question_text);
	void update();
	void draw() const;

	void update_button_state(const ofPoint& pressed_point);
};

#endif