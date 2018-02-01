#include "ofMain.h"
#include "HandCursorApp.h"
#include "AppParameters.h"

using namespace param;

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(2, 1);
	settings.width = DISPLAY_W;
	settings.height = DISPLAY_H;
	settings.resizable = false;
	ofCreateWindow(settings);

	/* ウィンドウの枠を消す(フルスクリーンにする) */
	HWND hWnd = WindowFromDC(wglGetCurrentDC());
	LONG lStyle;
	lStyle = GetWindowLong(hWnd, GWL_STYLE);
	lStyle &= ~WS_CAPTION;
	lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);

	ofRunApp(new HandCursorApp());

	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	//// this kicks off the running of my app
	//// can be OF_WINDOW or OF_FULLSCREEN
	//// pass in width and height too:
	//ofRunApp(new HandCursorApp());
}
