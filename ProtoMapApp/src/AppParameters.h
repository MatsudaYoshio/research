#pragma once
#ifndef ___AppParameters
#define ___AppParameters

#include "ofMain.h"

namespace param {
	/* �J�����Ŏ擾����RGB�摜�̉𑜓x */
	constexpr int CAMERA_W{ 1920 };
	constexpr int CAMERA_H{ 1080 };

	/* �f�B�X�v���C�\���̉𑜓x */
	extern const int DISPLAY_W;
	extern const int DISPLAY_H;

	/* �f�B�X�v���C�\���̉𑜓x�̔��� */
	extern const int HALF_DISPLAY_W;
	extern const int HALF_DISPLAY_H;

	/* �T�u�E�B���h�E�̍ő�T�C�Y */
	extern const int MAX_SUB_WINDOW_W;
	extern const int MAX_SUB_WINDOW_H;

	/* �T�u�E�B���h�E�̍ő�T�C�Y�̔��� */
	extern const int HALF_MAX_SUB_WINDOW_W;
	extern const int HALF_MAX_SUB_WINDOW_H;

	/* �J�����ƃf�B�X�v���C�̉𑜓x�̔䗦 */
	extern const double RESOLUTION_RATIO_W;
	extern const double RESOLUTION_RATIO_H;

	extern const ofRectangle USER_CERTAIN_WINDOW; // ���[�U�̃|�C���^�̏d�����z�̈�͈̔�

	extern const long long int NOT_USER; // �����̃��[�UID

	constexpr int MENU_ITEM_NUM{ 4 }; // ���j���[���ڂ̐�

	constexpr int CONTENT_NUM{ 22 }; // �R���e���c�̐�

	const enum class MENU_ITEM_ID { SIGHTSEEING, RESTAURANT, SHOPPING, HOTEL }; // ���j���[����ID

	extern const ofColor MENU_ITEM_COLOR[]; // ���j���[���ڂ̐F

	/* �R���e���cID */
	const enum class CONTENT_ID {
		KYOTO_TOWER, HIGASHIHONGANJI, SYOSEIEN, NISHIHONGANJI, KIYOMIZUDERA, KYOTO_AQUARIUM, KANNONJI, TOJI, EIJUAN, SODOH, FUNATSURU, HANAROKU, RANTEI, AEON_MALL, ISETAN, AVANTI, TAKASHIMAYA, YAMASHITA, ALMONT, SAKURA_HONGANJI, KYOSTAY_IROHA_TOJI, HYATT_REGENCY, FAMILY_VILLA_KEAGE
	};

	extern const vector<CONTENT_ID> MENU_ITEM_CONTENTS[]; // ���j���[���ڂɊ܂܂��R���e���c�̓���

	const enum class CONTENT_NAME_POSITION { TOP, BOTTOM, LEFT, RIGHT }; // �R���e���c�̖��O�̔z�u(�s�����猩�ď㉺���E�̂ǂꂩ)

	/* �R���e���c�f�[�^�̌^ */
	using content_data_type = struct {
		int x, y; // �s���̎w���������W
		int number; // �e���j���[���ڂɂ�����ԍ�
		MENU_ITEM_ID menu_item_id; // �������Ă��郁�j���[���ڂ�ID
		string name; // �\�����閼�O
		CONTENT_NAME_POSITION position;
	};

	extern const content_data_type CONTENT_DATA[]; // �R���e���c�̏��

	/* �Ȃ����摜�f�[�^�������ŏ��������邱�Ƃ��ł��Ȃ�����A�A�v���N�����ɏ���������
	   �����ł͕ϐ���p�ӂ��邾�� */
	extern ofImage CONTENT_IMAGE[];
}

#endif