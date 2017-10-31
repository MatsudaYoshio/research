#include "KyotoTowerScene.h"

using namespace param;

void KyotoTowerScene::setup() {
	this->title_font.loadFont("meiryob.ttc", 40);
	this->sentence_font.loadFont("meiryob.ttc", 20);

	this->img.load("C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/kyoto_tower.jpg");
}

void KyotoTowerScene::update() {

}

void KyotoTowerScene::draw() {
	ofBackground(255);
	ofSetColor(ofColor::white);

	this->img.draw(0, 0, W / 2, H / 4);

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"���s�̃����h�}�[�N�u���s�^���[�v", 50, H / 4 + 50);

	this->sentence_font.drawString(L"���s�̌��֌��ɘȂ݁A�Z�l�A�ό��q���������}���鋞�s�^���[�B", 50, H / 4 + 100);
}