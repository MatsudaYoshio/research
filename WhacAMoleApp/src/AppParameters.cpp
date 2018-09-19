#include "AppParameters.h"

/* �f�B�X�v���C�\���̉𑜓x */
const int param::DISPLAY_WIDTH{ 3840 };
const int param::DISPLAY_HEIGHT{ 2160 };

/* �f�B�X�v���C�\���̉𑜓x�̔��� */
const int param::HALF_DISPLAY_WIDTH{ param::DISPLAY_WIDTH >> 1 };
const int param::HALF_DISPLAY_HEIGHT{ param::DISPLAY_HEIGHT >> 1 };

/* �J�����ƃf�B�X�v���C�̉𑜓x�̔䗦 */
const double param::RESOLUTION_RATIO_WIDTH{ param::DISPLAY_WIDTH / param::CAMERA_WIDTH };
const double param::RESOLUTION_RATIO_H{ param::DISPLAY_HEIGHT / param::CAMERA_HEIGHT };

const long long int param::NOT_USER{ -1 }; // �����̃��[�UID

/* �A���t�@�l(���ߓx�����肷��) */
const float param::MAX_ALFHA{ ofColor::limit() };
const float param::HALF_MAX_ALFHA{ param::MAX_ALFHA / 2 };

/* �����Œl�̏��������s��Ȃ��ɁAWhacAMoleApp�ŏ��������s�� */
ofImage param::HAMMER_IMAGE{ ofImage{} }; // �n���}�[�̉摜
ofImage param::HOLE_IMAGE{ ofImage{} }; // ���̉摜
ofImage param::MOLE_IMAGE{ ofImage{} }; // ���O���̉摜