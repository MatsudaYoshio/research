#include "QuestionApp.h"

using namespace param;

const string QuestionApp::output_data_directory = "./data/output_data/";

const array<array<string, QuestionApp::question_number>, MENU_ITEM_NUMBER> QuestionApp::questions = { {
	{ u8"wifi���g����,���E��Y�ł���Ƃ����?", u8"�������ł������̂�?", u8"���󂪂���̂�?", u8"�Ԃ������g����,7����������̂�?" },
	{ u8"�t�����`���o��̂�?", u8"�։��ŗ�����5000�~�ȉ��Ȃ̂�?", u8"20���܂łɓ����,��������̂�?", u8"�������ł������̂�?" },
	{ u8"�ł��x���܂ŉc�Ƃ��Ă���̂�?", u8"Wifi���g����,���X�g����������̂�?", u8"8����������̂�?", u8"���H�ƃ��b�s���O�̃T�[�r�X������̂�?" },
	{ u8"Luxury�N���X�ŋ։��Ȃ̂�?", u8"�ł������������̂�?", u8"Wifi���g����,������1���~�ȉ��Ȃ̂�?", u8"Middle�N���X�Ȃ̂�?" }
} };

const array<array<int, QuestionApp::question_number>, MENU_ITEM_NUMBER> QuestionApp::answers = { {
	{5, 6, 4, 2},
	{3, 1, 5, 4},
	{3, 4, 7, 5},
	{6, 3, 1, 7}
} };

void QuestionApp::setup() {
	/* �팱�҂̖��O�Ƒ��삷��J�e�S���[�ԍ����擾 */
	cout << "Participant's name: ";
	cin >> this->participant_name;

	do {
		cout << "Category ID(1~4): ";
		cin >> this->menu_item_id;
	} while (this->menu_item_id <= 0 || this->menu_item_id > MENU_ITEM_NUMBER);
	--this->menu_item_id;

	ofHideCursor(); // �J�[�\����\��
	ofEnableAlphaBlending(); // �A���t�@�`�����l�����g�p�\�ɂ���
	ofSetBackgroundColor(ofColor::white);

	for (int i = 0; i < MENU_ITEM_NUMBER; ++i) {
		this->form_sets[i].setup(ofPoint(120, DISPLAY_HEIGHT*(i*0.5 + 1) / 3) + 50, MENU_ITEM_COLOR[this->menu_item_id], this->questions[this->menu_item_id][i]);
	}

	this->start_button.setup(ofRectangle(DISPLAY_WIDTH / 2 - DISPLAY_WIDTH / 6, DISPLAY_HEIGHT / 2 - DISPLAY_HEIGHT / 10, DISPLAY_WIDTH / 3, DISPLAY_HEIGHT / 5), "START", ofPoint(20, 275), 200);
	this->submit_button.setup(ofRectangle(DISPLAY_WIDTH * 4 / 5 + 75, DISPLAY_HEIGHT / 12 + 50, 180 * 1.6, 170), "SUBMIT", ofPoint(15, 110), 50);

	this->menu_item.setup(this->menu_item_id);

	this->timer.setup(ofPoint(DISPLAY_WIDTH / 2.2, DISPLAY_HEIGHT / 5.5), 100);
}

void QuestionApp::update() {
	switch (this->state) {
	case STATE::RUNNING:
		if (this->timer.get_seconds() >= this->limit_time) {
			this->state = STATE::TIMEOVER;
		}
	case STATE::TIMEOVER:
		for (int i = 0; i < MENU_ITEM_NUMBER; ++i) {
			this->form_sets[i].update();
		}

		this->timer.update();
		break;
	}
}

void QuestionApp::draw() {
	this->menu_item.draw();
	switch (this->state) {
	case STATE::START:
		this->start_button.draw();
		break;
	case STATE::TIMEOVER:
		ofSetBackgroundColor(ofColor::red);
	case STATE::RUNNING:
		this->submit_button.draw();

		for (int i = 0; i < MENU_ITEM_NUMBER; ++i) {
			this->form_sets[i].draw();
		}

		this->timer.draw();

		break;
	}
}

void QuestionApp::output_data() const {
	const auto category_id = this->menu_item_id + 1;
	ofstream ofs(this->output_data_directory + this->participant_name + "_" + to_string(category_id) + ".dat");
	ofs << "name: " << this->participant_name << ", category: " << category_id << endl;
	ofs << "time: " << to_string(this->timer.get_milliseconds()) << "[ms]" << endl;
	for (int i = 0; i < this->question_number; ++i) {
		const auto answer = this->form_sets[i].selected_number + 1;
		ofs << "answer[" << to_string(i + 1) << "]: " << to_string(answer) << " " << ((answer == answers[this->menu_item_id][i]) ? "O" : "X") << endl;
	}
	ofs.close();
}

void QuestionApp::mousePressed(int x, int y, int button) {
	if (button == LEFT_CLICKED) {
		switch (this->state) {
		case STATE::START:
			if (this->start_button.rect.inside(ofPoint(x, y))) {
				this->state = STATE::RUNNING;
				this->timer.start();
			}
			break;
		case STATE::RUNNING:
		case STATE::TIMEOVER:
			if (this->submit_button.rect.inside(ofPoint(x, y))) {
				this->output_data();
				std::exit(0);
			}

			for (int i = 0; i < MENU_ITEM_NUMBER; ++i) {
				this->form_sets[i].update_button_state(ofPoint(x, y));
			}

			break;
		}
	}
}