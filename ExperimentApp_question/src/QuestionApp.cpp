#include "QuestionApp.h"

using namespace param;

const string QuestionApp::output_data_directory = "./data/output_data/";

const array<array<string, QuestionApp::question_number>, MENU_ITEM_NUMBER> QuestionApp::questions = { {
	{ u8"wifiが使えて,世界遺産であるところは?", u8"料金が最も高いのは?", u8"国宝があるのは?", u8"車いすが使えて,7時から入れるのは?" },
	{ u8"フレンチが出るのは?", u8"禁煙で料金が5000円以下なのは?", u8"20時までに入れて,個室があるのは?", u8"料金が最も高いのは?" },
	{ u8"最も遅くまで営業しているのは?", u8"Wifiが使えて,レストランがあるのは?", u8"8時から入れるのは?", u8"試食とラッピングのサービスがあるのは?" },
	{ u8"Luxuryクラスで禁煙なのは?", u8"最も料金が安いのは?", u8"Wifiが使えて,料金が1万円以下なのは?", u8"Middleクラスなのは?" }
} };

const array<array<int, QuestionApp::question_number>, MENU_ITEM_NUMBER> QuestionApp::answers = { {
	{5, 6, 4, 2},
	{3, 1, 5, 4},
	{3, 4, 7, 5},
	{6, 3, 1, 7}
} };

void QuestionApp::setup() {
	/* 被験者の名前と操作するカテゴリー番号を取得 */
	cout << "Participant's name: ";
	cin >> this->participant_name;

	do {
		cout << "Category ID(1~4): ";
		cin >> this->menu_item_id;
	} while (this->menu_item_id <= 0 || this->menu_item_id > MENU_ITEM_NUMBER);
	--this->menu_item_id;

	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする
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