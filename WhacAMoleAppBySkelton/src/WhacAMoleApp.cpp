#include "WhacAMoleApp.h"

using namespace param;

const ofColor WhacAMoleApp::bg_color{ 33, 215, 102 };
const ofPoint WhacAMoleApp::font_draw_point{ 400, 120 };

random_device WhacAMoleApp::rd;
mt19937 WhacAMoleApp::mt(WhacAMoleApp::rd());
uniform_int_distribution<int> WhacAMoleApp::random_0or1(0, 1);
uniform_int_distribution<int> WhacAMoleApp::random_appearance_time(8, 90);
uniform_int_distribution<int> WhacAMoleApp::random_hidden_time(8, 90);

void WhacAMoleApp::setup() {
	ofSetBackgroundAuto(false); // �t���[���X�V���ɓh��Ԃ��𖳌���
	ofHideCursor(); // �J�[�\����\��

	this->font.load("./font/SFNSText.ttf", 60); // �t�H���g�̃��[�h

	this->initialize_image(); // �摜�̏�����

	this->initialize_moles(); // ���O���I�u�W�F�N�g�̏�����
}

void WhacAMoleApp::update() {
	this->se.update();

	for (const auto& up : this->se.transformed_key_points) { // ���ׂẴ��[�U���܂߂����i�_�̂����[�v�ł܂킷
		if (up.first.second == RIGHT_WRIST || up.first.second == LEFT_WRIST) { // �E��񂩍����̍��i�_��������
			for (int i = 0; i < this->mole_num_all; ++i) {
				if (this->moles[i].is_appear() && this->is_whacked(this->moles[i], up.second)) { // �������O����@������
					++score; // �X�R�A�����Z
					this->moles[i].hide(this->random_hidden_time(this->mt)); // ���O���ɉB�ꎞ�Ԃ�^���A�B��Ă��炤
				}
			}
		}
	}

	if (this->se.transformed_key_points.empty()) { // �������[�U�����Ȃ�������
		for (int i = 0; i < this->mole_num_all; ++i) {
			if (this->moles[i].is_idle()) { // ���O�����ɂ�������
				/* �����_���ɏo�����Ԃ܂��͉B�ꎞ�Ԃ�^���� */
				if (this->random_0or1(this->mt)) {
					this->moles[i].appear(this->random_appearance_time(this->mt));
				}
				else {
					this->moles[i].hide(this->random_hidden_time(this->mt));
				}
			}
		}
	}
	else { // �������[�U�������炻�̂ɂ̓��O�����o�������Ȃ�
		for (const auto& up : this->se.transformed_key_points) { // ���ׂẴ��[�U���܂߂����i�_�̂����[�v�ł܂킷
			if (up.first.second == RIGHT_WRIST || up.first.second == LEFT_WRIST) { // �E��񂩍����̍��i�_��������
				for (int i = 0; i < this->mole_num_all; ++i) {
					if (!this->is_whacked(this->moles[i], up.second) && this->moles[i].is_idle()) { // ���O���̈ʒu�ɍ��i�_���Ȃ��āA���A���O�����ɂ��Ă�����
						/* �����_���ɏo�����Ԃ܂��͉B�ꎞ�Ԃ�^���� */
						if (this->random_0or1(this->mt)) {
							this->moles[i].appear(this->random_appearance_time(this->mt));
						}
						else {
							this->moles[i].hide(this->random_hidden_time(this->mt));
						}
					}
				}
			}
		}
	}
}

void WhacAMoleApp::draw() {
	ofBackground(this->bg_color); // �w�i�̕`��

	/* ���r���[�i�X�R�A��\������Ƃ���j�̕`�� */
	ofSetColor(ofColor::black);
	ofDrawRectangle(0, 0, DISPLAY_WIDTH, this->information_view_size);

	/* �X�R�A��`�� */
	ofSetColor(ofColor::white);
	this->font.drawString("Score :", this->font_draw_point.x, this->font_draw_point.y);
	this->font.drawString(to_string(this->score), this->font_draw_point.x + 300, this->font_draw_point.y);

	/* ���O����`�� */
	ofSetColor(ofColor::white);
	for (int i = 0; i < this->mole_num_all; ++i) {
		this->moles[i].draw();
	}

	/* ���i�̕`�� */
	this->kpr.draw(this->se.transformed_key_points);
}

void WhacAMoleApp::exit() {
	this->se.exit();
}

void WhacAMoleApp::initialize_image() const {
	HOLE_IMAGE.load("fig/mole0.png");
	MOLE_IMAGE.load("fig/mole1.png");
}

void WhacAMoleApp::initialize_moles() {
	const auto mole_width{ (DISPLAY_WIDTH - this->mole_margin_row) / this->mole_num_row - this->mole_margin_row }; // �e���O���̕�
	const auto mole_height{ (DISPLAY_HEIGHT - this->mole_margin_col - this->information_view_size) / this->mole_num_col - this->mole_margin_col }; // �e���O���̍���

	for (int i = 0, x = this->mole_margin_row; x < DISPLAY_WIDTH; x += mole_width + this->mole_margin_row) {
		for (int y = this->information_view_size + this->mole_margin_col; y < DISPLAY_HEIGHT && i < this->mole_num_all; y += mole_height + this->mole_margin_col) {
			this->moles[i++].setup(ofRectangle(x, y, mole_width, mole_height), this->random_hidden_time(this->mt));
		}
	}
}

bool WhacAMoleApp::is_whacked(const Mole& mole, const ofPoint& key_point) const {
	return mole.get_rect().inside(key_point);
}