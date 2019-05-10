#include "QuestionApp.h"

using namespace param;

int main() {
	ofSetupOpenGL(DISPLAY_WIDTH, DISPLAY_HEIGHT, OF_FULLSCREEN);

	ofRunApp(new QuestionApp());
}