#include "ofMain.h"
#include "DigitalSignageApp.h"
#include "AppParameters.h"

using namespace param;

//========================================================================
int main(){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(2, 1);
	settings.width = DISPLAY_W;
	settings.height = DISPLAY_H;
	settings.resizable = false;
	ofCreateWindow(settings);
	
	/* �E�B���h�E�̘g������(�t���X�N���[���ɂ���) */
	HWND hWnd = WindowFromDC(wglGetCurrentDC());
	LONG lStyle;
	lStyle = GetWindowLong(hWnd, GWL_STYLE);
	lStyle &= ~WS_CAPTION;
	lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);

	ofRunApp(new DigitalSignageApp());
}