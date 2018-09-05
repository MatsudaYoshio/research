#include "WhacAMoleApp.h"

using namespace param;

int main(){
	ofSetupOpenGL(DISPLAY_WIDTH, DISPLAY_HEIGHT, OF_FULLSCREEN); // フルスクリーンでウィンドウ表示

	/* マルチディスプレイなどで、フルスクリーンがうまくいかない場合は以下のようにやってみる */
	///* ウィンドウの準備 */
	//ofGLFWWindowSettings settings;
	//settings.setGLVersion(2, 1);
	//settings.width = DISPLAY_WIDTH;
	//settings.height = DISPLAY_HEIGHT;
	//settings.resizable = false;
	//ofCreateWindow(settings);

	///* ウィンドウの枠を消す(フルスクリーンにする) */
	//auto hWnd{ WindowFromDC(wglGetCurrentDC()) };
	//LONG lStyle;
	//lStyle = GetWindowLong(hWnd, GWL_STYLE);
	//lStyle &= ~WS_CAPTION;
	//lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);

	ofRunApp(new WhacAMoleApp());
}