#include "FormSet.h"

using namespace param;

void FormSet::setup(const ofPoint& position, const ofColor& color, const string& question_text) {
	this->position = position;
	this->question_text = question_text;
	this->setting_font();

	for (int i = 0; i < this->option_size; ++i) {
		this->number_buttons[i].setup(ofRectangle(DISPLAY_WIDTH / 2 + position.x + i * 130, position.y - 75, 100, 100), to_string(i + 1), ofPoint(35, 70), 40, color);
	}
}

void FormSet::update() {
	for (int i = 0; i < this->option_size; ++i) {
		this->number_buttons[i].update();
	}
}

void FormSet::draw() const {
	ofSetColor(ofColor::black);
	this->font.drawString(this->question_text, this->position.x, this->position.y);

	for (int i = 0; i < this->option_size; ++i) {
		this->number_buttons[i].draw();
	}
}

void FormSet::update_button_state(const ofPoint& pressed_point) {
	for (int i = 0; i < this->option_size; ++i) {
		if (this->number_buttons[i].rect.inside(pressed_point)) {
			if (i != this->selected_number) {
				this->number_buttons[this->selected_number].switch_state();
				this->number_buttons[i].switch_state();
				this->selected_number = i;
			}
			break;
		}
	}
}

void FormSet::setting_font() {
	ofTrueTypeFontSettings settings("./font/meiryob.ttc", 45);
	settings.addRanges(ofAlphabet::Japanese);
	settings.addRange(ofUnicode::IdeographicSpace);
	settings.addRange(ofUnicode::Latin);
	settings.addRange(ofUnicode::Latin1Supplement);
	settings.addRange(ofUnicode::Hiragana);
	settings.addRange(ofUnicode::Katakana);

	this->font.load(settings);
	this->font.setLetterSpacing(1.2);
}