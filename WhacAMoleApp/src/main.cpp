#include "WhacAMoleApp.h"

using namespace param;

int main() {
	/* �������Ŗ��Ȃ���΁A�{�Ԃł͂��������g�� */
	/* ----------------------------------------------------------------------------- */
	//ofSetupOpenGL(DISPLAY_WIDTH, DISPLAY_HEIGHT, OF_FULLSCREEN); // �t���X�N���[���ŃE�B���h�E�\��
	/* ----------------------------------------------------------------------------- */



	/* �}���`�f�B�X�v���C�ȂǂŁA�t���X�N���[�������܂������Ȃ��ꍇ�A�܂��́A����e�X�g(FPS�⌟�o�̃`�F�b�N�Ȃ�)������Ƃ��͈ȉ��̂悤�ɂ���Ă݂� */
	/* ----------------------------------------------------------------------------- */
	/* �E�B���h�E�̏��� */
	ofGLFWWindowSettings settings;
	settings.setGLVersion(2, 1);
	settings.width = DISPLAY_WIDTH;
	settings.height = DISPLAY_HEIGHT;
	settings.resizable = false;
	ofCreateWindow(settings);

	/* �E�B���h�E�̘g������(�t���X�N���[���ɂ���) */
	auto hWnd{ WindowFromDC(wglGetCurrentDC()) };
	auto lStyle{ GetWindowLong(hWnd, GWL_STYLE) };
	lStyle &= ~WS_CAPTION;
	lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);
	/* ----------------------------------------------------------------------------- */

	ofRunApp(new WhacAMoleApp());
}