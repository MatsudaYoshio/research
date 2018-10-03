#include "ofApp.h"

using namespace param;

void ofApp::setup() {
	ofHideCursor(); // �J�[�\����\��
	ofSetCircleResolution(256); // �~�̐����x��ݒ�

	this->kpr.setup(&this->hc.pose_key_points); // ���i�`��N���X�̏���
}

void ofApp::update() {
	this->hc.update(); // ��J�[�\���̍X�V
}

void ofApp::draw() {
	// ���ꂼ��̃R�����g���O���Ċm�F���Ă�������

	/* �E��̑���̂��߂̃|�C���^�̕`�� */
	this->draw_pointer();

	/* ���i�̕`�� */
	//this->kpr.draw();

	/* ���[�U�f�[�^�̕\�� */
	//this->show_user_data();

	/* ���́i�J�������W��́j���i�_����\�� */
	//this->show_key_points(this->hc.pose_key_points);

	///* �f�B�X�v���C���W��̍��i�_����\�� */
	//op::Array<float> new_key_points = this->hc.pose_key_points; // �V���ȍ��i�_������ϐ���p��
	//this->transform_key_points(new_key_points); // �ϊ�����B��J�[�\���N���X��pose_key_points�i�J�������W��̍��i�_�j���f�B�X�v���C���W�ɕϊ��������i�_�𓾂�inew_key_points�ɑ���j
	//this->show_key_points(new_key_points); // �\��
}

void ofApp::exit() {
	this->hc.exit(); // �A�v���I�����Ɏ�J�[�\���N���X��exit�֐����Ăяo��
}

void ofApp::draw_pointer() const {
	for (const auto& ud : this->hc.user_data) {
		if (ud.second.state == HandCursor::STATE::INACTIVE) { // ��Ԃ�INACTIVE�̂Ƃ��A�`�悵�Ȃ��悤�ɂ���i���[�U�����������Ə�Ԃ�INACTIVE�ɂȂ邩��j�B�����A��������Ă���Ƃ����`�悵�����ꍇ�͂���if��������
			continue;
		}

		ofNoFill();
		ofSetLineWidth(60);
		ofSetColor(ofColor::white);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 50);
		ofFill();
		ofSetColor(ud.second.cursor_color);
		ofDrawCircle(ud.second.cursor_point.x, ud.second.cursor_point.y, 45);
	}
}

void ofApp::show_key_points(const op::Array<float>& key_points) const {
	const auto people_num{ key_points.getSize(0) }; // ���o���ꂽ�l��

	for (int i = 0; i < people_num; ++i) {
		printf("user%d's key points:\n", i);

		///* �S���̍��i�_��\�� */
		//for (int j = 0; j < 18; ++j) {
		//	if (key_points[{i, j, 0}] != 0.0 && key_points[{i, j, 1}] != 0.0) {
		//		printf("key point [%d] -> (%f,%f)\n", j, key_points[{i, j, 0}], key_points[{i, j, 1}]);
		//	}
		//}

		///* ����̍��i�_�����\���i�Ⴆ�ΉE���j */
		//if (key_points[RIGHT_WRIST_X(i)] != 0.0 && key_points[RIGHT_WRIST_Y(i)] != 0.0) {
		//	printf("key point [right wrist] -> (%f,%f)\n", key_points[RIGHT_WRIST_X(i)], key_points[RIGHT_WRIST_Y(i)]);
		//}

		/* ����̍��i�_�����\���i�Ⴆ�΍��ځj*/
		if (key_points[LEFT_EYE_X(i)] != 0.0 && key_points[LEFT_EYE_Y(i)] != 0.0) {
			printf("key point [left eye] -> (%f,%f)\n", key_points[LEFT_EYE_X(i)], key_points[LEFT_EYE_Y(i)]);
		}

		puts("");
	}
}

void ofApp::show_user_data() const {
	puts("Show user data!!");
	for (const auto& ud : this->hc.user_data) {
		puts("------------------------------------");
		printf("user id : %lld\n", ud.first); // ���[�UID
		printf("color id : %d\n", ud.second.cursor_color_id); // ���[�U�Ɋ��蓖�Ă�ꂽ�F��ID
		printf("image id : %d\n", ud.second.cursor_image_id); // ���[�U�Ɋ��蓖�Ă�ꂽ�摜��ID
		printf("cursor point : (%d, %d)\n", ud.second.cursor_point.x, ud.second.cursor_point.y); // �J�[�\���̍��W�i�E���̍��W�����Ƀf�B�X�v���C���W��ɕϊ����ꂽ���́j
		printf("face point : (%d, %d)\n", ud.second.face_point.x, ud.second.face_point.y); // ��̒��S�̍��W�i�J�������W�ŁA�f�B�X�v���C���W��ɕϊ�����Ă��Ȃ����́j
		printf("transformed face point : (%d, %d)\n", ud.second.transformed_face_point.x, ud.second.transformed_face_point.y); // ��̒��S�̍��W�i�f�B�X�v���C���W��ɕϊ����ꂽ���́j
		printf("face size : %lf\n", ud.second.face_size); // ���肵����̑傫��
		puts("------------------------------------");
		puts("");
	}
}

void ofApp::transform_key_points(op::Array<float>& key_points) const {
	const auto people_num{ this->hc.pose_key_points.getSize(0) }; // ���o���ꂽ�l��

	/* ���ׂĂ̍��i�_���f�B�X�v���C���W�ւƕϊ����� */
	for (int i = 0; i < people_num; ++i) {
		for (int j = 0; j < 18; ++j) {
			if (this->hc.pose_key_points[{ i, j, 0 }] != 0.0 && this->hc.pose_key_points[{ i, j, 1 }] != 0.0) {
				key_points[{i, j, 0}] = DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*key_points[{ i, j, 0 }]; // x���W�͂�������ĕϊ�����ix' = DISPLAY_WIDTH - RESOLUTION_RATIO_WIDTH*x�j
				key_points[{i, j, 1}] = RESOLUTION_RATIO_HEIGHT*key_points[{ i, j, 1 }]; // y���W�͂�������ĕϊ�����iy' = RESOLUTION_RATIO_HEIGHT*y�j
			}
		}
	}
}