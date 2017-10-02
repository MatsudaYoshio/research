#include "DetailScene.h"

void DetailScene::setup(HandCursor *hc) {
	this->icons.insert(make_pair("return", Icon(this->window_width - 200, 100, 100, 100, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/return.jpeg")));

	this->hc = hc;

	font.loadFont("meiryob.ttc", 30);
}

void DetailScene::update() {
	bool flag;
	for (auto &i : this->icons) {
		flag = false;
		for (auto &t : this->hc->track_data) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // �A�C�R�����Ǝ�|�C���^id�̏��
				ofNotifyEvent(this->point_event, id);
				flag = true;
				break;
			}
		}
		if (!flag) {
			i.second.change_state("None");
		}
		i.second.update();
	}
	/* ��|�C���^���A�C�R���ɐG�ꂽ��C�x���g�𔭉� */
	/*
	for (auto t : this->hc->track_data) {
		for (auto i : this->icons) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // �A�C�R�����Ǝ�|�C���^id�̏��
				ofNotifyEvent(this->point_event, id);
			}
		}
	}
	*/
}

void DetailScene::draw() {
	for (auto &i : this->icons) {
		i.second.draw();
	}

	ofSetColor(30, 200, 200); // �����F���w��
	font.drawString(L"\n\n\n\n���f���i�����������j�́A���f���q�̃K�X��n�����������ł���A�����A���L�A���F�ł���B\n���f�͐��ɂ����킸���ɗn����B�H�Ɨp�̐��f���͔����̂�t���̐��ɗp������B\n�_�Ƃł͍앨������H�i�ۑ��ł̌������s���Ă���B\n�܂����p�̃A���J�����d�𐅂̐����ɔ������f�������������\n�i���f���ł͂Ȃ��A���J���C�I�����������u�ɂ́u�ݒ��Ǐ�̉��P�v�̌��\�\�����F�߂��Ă���j�B\n�ق��Ɏs�̂̈��������̔�����Ă��邪�A���f���܂܂�Ȃ����i����������A\n�܂����\�\���Ɋւ��āA���������Z���^�[�ɂ��\���ł��Ȃ����Ƃ̒��ӌx���ƁA\n���ʂ��Ȃ��ƌ������Ă���Ƃ����Ǝ҂̈ӌ��ƂŐH���Ⴂ�������Ă���B\n�������ʂ̋��ʐ����琅�f�Ɛ��f���̍R�_����p�ɂ͋^���]�n�͂Ȃ����A\n��p�@���͂܂����m�ɂ͓��肳��Ă��Ȃ��B\n����2007�N�ȍ~�ɐ��f�Ɋւ����w�I�Ȍ��������{�A�A�����J�A�����ɂđ������A\n2015�N6���܂łɐ��f���̗Տ�������14�����񍐂���L�ӂȌ��ʂ��݂��Ă���B\n�d�𐅑f���ł́A�p�[�L���\���a���҂ł̓�d�ӌ��@�����A\n�l�H���͂̍ۂ̓��͉t�Ƃ��Ă̗��p����������B\n2016�N5���ɂ́A�������N�E�h�{�������́A6�̃����_������r���������ɁA\n�قƂ�ǂ��a�C�̊��҂ł̗\���I�����ł��邽�߁A\n���N�Ȑl�ւ̗L�����ɂ��āu�M���ł���\���ȃf�[�^����������Ȃ��v�Ƃ��Ă���B", 100, 100);
}