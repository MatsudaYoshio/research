#include "MapApp.h"

using namespace param;

void MapApp::setup() {
	ofSetBackgroundAuto(false); // フレーム更新時に塗りつぶしを無効化
	ofHideCursor(); // カーソル非表示
	ofEnableAlphaBlending(); // アルファチャンネルを使用可能にする
	ofSetCircleResolution(256); // 円の精密度を設定
	
	this->initialize_image(); // コンテンツ画像の初期化

	this->sm.setup(&this->hc);
}

void MapApp::update() {
	this->hc.update();
	this->sm.update();
}

void MapApp::draw() {
	//cout << mouseX << " " << mouseY << endl;
	this->sm.draw();
}

void MapApp::exit() {
	this->hc.exit();
}

void MapApp::initialize_image() {
	// 練習用
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::NISHIHONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SYOSEIEN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KIYOMIZUDERA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_AQUARIUM)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KANNONJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TOJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::EIJUAN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SODOH)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::FUNATSURU)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HANAROKU)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::RANTEI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::MARUYASU)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::IMASA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SAWASHO)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::AEON_MALL)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::ISETAN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::AVANTI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TAKASHIMAYA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::YAMASHITA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");

	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::ALMONT)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SAKURA_HONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOSTAY_IROHA_TOJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HYATT_REGENCY)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	CONTENT_IMAGE[static_cast<int>(CONTENT_ID::FAMILY_VILLA_KEAGE)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");

	// 本番用
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_TOWER)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kyoto_tower_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HIGASHIHONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/higashihonganji_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::NISHIHONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/nishihonganji_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SYOSEIEN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/shoseien_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KIYOMIZUDERA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kiyomizudera_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOTO_AQUARIUM)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kyoto_aquarium_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KANNONJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/kannonji_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TOJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/toji_scene.jpg");

	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::EIJUAN)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/eijuan_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SODOH)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/sodoh_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::FUNATSURU)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/funatsuru_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HANAROKU)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/hanaroku_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::RANTEI)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/rantei_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::MARUYASU)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/maruyasu_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::IMASA)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/imasa_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SAWASHO)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/sawasho_scene.jpg");

	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::AEON_MALL)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/aeon_mall.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::ISETAN)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/isetan.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::AVANTI)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/avanti.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::TAKASHIMAYA)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/takashimaya.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::YAMASHITA)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/yamashita.jpg");

	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::ALMONT)].load("C:/of_v0.9.8_vs_release/apps/myApps/ProtoMapApp/fig/almont_scene.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::SAKURA_HONGANJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/sakura_honganji.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::KYOSTAY_IROHA_TOJI)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/kyostay_iroha_toji.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::HYATT_REGENCY)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/hyatt_regency.jpg");
	//CONTENT_IMAGE[static_cast<int>(CONTENT_ID::FAMILY_VILLA_KEAGE)].load("C:/of_v0.9.8_vs_release/apps/myApps/NewMapApp/fig/family_villa_keage.jpg");
}