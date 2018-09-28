#include "WhacAMoleApp.h"

using namespace param;

int main() {
	/* こっちで問題なければ、本番ではこっちを使う */
	/* ----------------------------------------------------------------------------- */
	//ofSetupOpenGL(DISPLAY_WIDTH, DISPLAY_HEIGHT, OF_FULLSCREEN); // フルスクリーンでウィンドウ表示
	/* ----------------------------------------------------------------------------- */



	/* マルチディスプレイなどで、フルスクリーンがうまくいかない場合、または、動作テスト(FPSや検出のチェックなど)をするときは以下のようにやってみる */
	/* ----------------------------------------------------------------------------- */
	/* ウィンドウの準備 */
	ofGLFWWindowSettings settings;
	settings.setGLVersion(2, 1);
	settings.width = DISPLAY_WIDTH;
	settings.height = DISPLAY_HEIGHT;
	settings.resizable = false;
	ofCreateWindow(settings);

	/* ウィンドウの枠を消す(フルスクリーンにする) */
	auto hWnd{ WindowFromDC(wglGetCurrentDC()) };
	auto lStyle{ GetWindowLong(hWnd, GWL_STYLE) };
	lStyle &= ~WS_CAPTION;
	lStyle = SetWindowLong(hWnd, GWL_STYLE, lStyle);
	/* ----------------------------------------------------------------------------- */

	ofRunApp(new WhacAMoleApp());
}