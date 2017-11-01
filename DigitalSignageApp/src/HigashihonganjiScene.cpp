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

	ofSetColor(ofColor::black); // �����F���w��
	this->title_font.drawString(L"���{�莛�i�^�@�{�_�j", 50, H / 4 + 50);

	this->sentence_font.drawString(L"���{�莛�́A��y�^�@�u�^�@��J�h�v�̖{�R�Łu�^�@�{�_�v�Ƃ����A\n��e���ɂ͏@�c�E�e�a���l�̌�^�e���A����ɓ��ɂ͂��{���̈���ɔ@��\n�����u���Ă��܂��B\n�@�c�e�a���l�̖S����A���l��炤�����̐l�X�ɂ���Đ��l�̕���̒n��\n��^�e�����u����_�������Ă��܂����B", 50, H / 4 + 100);
}