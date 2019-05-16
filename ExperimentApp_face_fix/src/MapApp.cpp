#include "MapApp.h"

using namespace param;

void MapApp::setup() {
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする

	this->initialize_image(); // コンテンツ画像の初期化
	this->initialize_output_data_file();

	this->sm.setup(&this->hc);
}

void MapApp::update() {
	this->hc.update();
	this->sm.update();
}

void MapApp::draw() {
	this->sm.draw();
}

void MapApp::keyPressed(int key) {
	const auto n = static_cast<int>(key - '1');
	if (0 <= n && n < MENU_ITEM_NUM) {
		this->sm.switch_menu_item(n);
	}
}

void MapApp::exit() {
	this->hc.exit();

	this->ofs << "SIGHTSEEING, RESTAURANT, SHOPPING, HOTEL" << endl;
	for (int i = 0; i < MENU_ITEM_NUM; ++i) {
		this->ofs << to_string(this->sm.pin_selected_count[i]) << " ";
	}
	this->ofs << endl << endl;
	
	this->ofs << "Cursor total distance" << endl;

	auto is_valid_point = [](const cv::Point& p) { return p.x >= 0 && p.x <= DISPLAY_W && p.y >= 0 && p.y <= DISPLAY_H; };
	double cursor_total_distance = 0;

	for (int i = 1; i < this->hc.left_user_cursor_log.size(); ++i) {
		if (is_valid_point(this->hc.left_user_cursor_log[i]) && is_valid_point(this->hc.left_user_cursor_log[i-1])) {
			cursor_total_distance += ofDist(this->hc.left_user_cursor_log[i].x, this->hc.left_user_cursor_log[i].y, this->hc.left_user_cursor_log[i - 1].x, this->hc.left_user_cursor_log[i - 1].y);
		}
	}

	this->ofs << cursor_total_distance;

	cursor_total_distance = 0;

	for (int i = 1; i < this->hc.right_user_cursor_log.size(); ++i) {
		if (is_valid_point(this->hc.right_user_cursor_log[i]) && is_valid_point(this->hc.right_user_cursor_log[i - 1])) {
			cursor_total_distance += ofDist(this->hc.right_user_cursor_log[i].x, this->hc.right_user_cursor_log[i].y, this->hc.right_user_cursor_log[i - 1].x, this->hc.right_user_cursor_log[i - 1].y);
		}
	}

	this->ofs << " " << cursor_total_distance;

	this->ofs.close();
}

void MapApp::initialize_image() {
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].load("./fig/kyoto_tower_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].load(".//fig/higashihonganji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::NISHIHONGANJI)].load("./fig/nishihonganji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SYOSEIEN)].load("./fig/shoseien_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KIYOMIZUDERA)].load("./fig/kiyomizudera_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_AQUARIUM)].load("./fig/kyoto_aquarium_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KANNONJI)].load("./fig/kannonji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TOJI)].load("./fig/toji_scene.jpg");

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::EIJUAN)].load("./fig/eijuan_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SODOH)].load("./fig/sodoh_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::FUNATSURU)].load("./fig/funatsuru_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HANAROKU)].load("./fig/hanaroku_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::RANTEI)].load("./fig/rantei_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::MARUYASU)].load("./fig/maruyasu_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::IMASA)].load("./fig/imasa_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SAWASHO)].load("./fig/sawasho_scene.jpg");

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::AEON_MALL)].load("./fig/aeon_mall_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::ISETAN)].load("./fig/isetan_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::AVANTI)].load("./fig/avanti_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TAKASHIMAYA)].load("./fig/takashimaya_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::YAMASHITA)].load("./fig/yamashita_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HOBBY_SQUARE_KYOTO)].load("./fig/hobby_square_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::GION_NARUMIYA)].load("./fig/gion_narumiya_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::UNIQLO)].load("./fig/uniqlo_scene.jpg");

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::ALMONT)].load("./fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SAKURA_HONGANJI)].load("./fig/sakura_honganji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOSTAY_IROHA_TOJI)].load("./fig/kyostay_iroha_toji_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HYATT_REGENCY)].load("./fig/hyatt_regency_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::FAMILY_VILLA_KEAGE)].load("./fig/keage_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::MUROMACHI_YUTONE)].load("./fig/muromachi_yutone_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KURAYA)].load("./fig/kuraya_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TOKIWAAN)].load("./fig/tokiwaan_scene.jpg");
}

void MapApp::initialize_output_data_file() {
	const string directory_path = "C:/of_v0.9.8_vs_release/apps/myApps/ExperimentApp_face_fix/bin/data/experiment_data/";

	cout << "Participant's name on the left: ";
	cin >> left_user_name;
	cout << "Participant's name on the right: ";
	cin >> right_user_name;

	this->ofs.open(directory_path + left_user_name + "_" + right_user_name + ".dat");
	this->ofs << left_user_name << " and " << right_user_name << endl << endl;
}