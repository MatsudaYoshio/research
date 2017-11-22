#include "HigashihonganjiScene.h"

using namespace param;

void HigashihonganjiScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/higashihonganji.jpg");
}

void HigashihonganjiScene::update() {

}

void HigashihonganjiScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // 文字色を指定
	this->title_font.drawString(L"東本願寺（真宗本廟）", 50, H / 4 + 50);

	this->sentence_font.drawString(L"東本願寺は、浄土真宗「真宗大谷派」の本山で「真宗本廟」といい、\n御影堂には宗祖・親鸞聖人の御真影を、阿弥陀堂にはご本尊の阿弥陀如来\nを安置しています。\n宗祖親鸞聖人の亡き後、聖人を慕う多くの人々によって聖人の墳墓の地に\n御真影を安置する廟堂が建てられました。", 50, H / 4 + 100);
}