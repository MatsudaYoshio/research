#include "AppParameters.h"

/* �f�B�X�v���C�\���̉𑜓x */
const int param::DISPLAY_WIDTH{ 3840 };
const int param::DISPLAY_HEIGHT{ 2160 };

/* �f�B�X�v���C�\���̉𑜓x�̔��� */
const int param::HALF_DISPLAY_WIDTH{ param::DISPLAY_WIDTH >> 1 };
const int param::HALF_DISPLAY_HEIGHT{ param::DISPLAY_HEIGHT >> 1 };

/* �J�����ƃf�B�X�v���C�̉𑜓x�̔䗦 */
const double param::RESOLUTION_RATIO_WIDTH{ param::DISPLAY_WIDTH / param::CAMERA_WIDTH };
const double param::RESOLUTION_RATIO_HEIGHT{ param::DISPLAY_HEIGHT / param::CAMERA_HEIGHT };

const long long int param::NOT_USER{ -1 }; // �����̃��[�UID

const int param::COCO_KEY_POINTS_NUM{ 18 };

/* �����Œl�̏��������s��Ȃ��ɁAWhacAMoleApp�ŏ��������s�� */
ofImage param::HOLE_IMAGE{ ofImage{} }; // ���̉摜
ofImage param::MOLE_IMAGE{ ofImage{} }; // ���O���̉摜