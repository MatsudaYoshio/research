#include "ofMain.h"
#include "DigitalSignageApp.h"

//========================================================================
int main(){
	/* メインウィンドウの起動 */
	//ofSetupOpenGL(1920, 1080, OF_WINDOW);

	ofGLFWWindowSettings settings;
	settings.setGLVersion(2, 1);
	settings.width = 1920;
	settings.height = 1080;
	settings.resizable = false;
	ofCreateWindow(settings);
	
	/* ウィンドウの枠を消す(フルスクリーンにする) */
	HWND hWnd = WindowFromDC(wglGetCurrentDC());
	LONG lStyle;
	lStyle = GetWindowLong(hWnd, GWL_STYLE);
	lStyle &= ~WS_CAPTION;
	lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);

	ofRunApp(new DigitalSignageApp());
}