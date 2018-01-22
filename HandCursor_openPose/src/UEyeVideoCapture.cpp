#include <iostream>

#include <uEye.h>
#include <uEye_tools.h>
#include <ueye_deprecated.h>

#include <opencv2/opencv.hpp>

#include "UEyeVideoCapture.h"

using namespace std;
using namespace cv;

UEyeVideoCapture::UEyeVideoCapture() :hCam(0) {
	if (is_InitCamera(&this->hCam, NULL) != IS_SUCCESS) {
		cerr << "Fail to initialize the camera" << endl;
		return;
	}

	if (is_AllocImageMem(this->hCam, this->img_width, this->img_height, this->img_bpp, &this->imgMem, &this->memId) == IS_NO_SUCCESS) {
		cerr << "Fail to allocate memory" << endl;
		return;
	}

	if (is_SetImageMem(this->hCam, this->imgMem, this->memId) == IS_NO_SUCCESS) {
		cerr << "Fail to set image memory" << endl;
		return;
	}

	if (is_SetDisplayMode(this->hCam, IS_SET_DM_DIB) == IS_NO_SUCCESS) {
		cerr << "Fail to set display mode" << endl;
		return;
	}

	if (is_SetColorMode(this->hCam, IS_SET_CM_RGB24) != IS_SUCCESS) {
		cerr << "Fail to set color mode" << endl;
		return;
	}

	is_SetPixelClock(this->hCam, this->pixel_clock);

	double dummy;

	if (is_SetFrameRate(this->hCam, this->fps, &dummy) == IS_NO_SUCCESS) {
		cerr << "Fail to set frame rate" << endl;
		return;
	}

	if (is_SetExposureTime(this->hCam, this->exp, &dummy) == IS_NO_SUCCESS) {
		cerr << "Fail to set exposure time" << endl;
		return;
	}

	if (is_SetImageSize(this->hCam, this->img_width, this->img_height) != IS_SUCCESS) {
		cerr << "Fail to set image size" << endl;
		return;
	}

	double enable = 1;
	double disable = 0;

	if (is_SetAutoParameter(this->hCam, IS_SET_ENABLE_AUTO_GAIN, &enable, 0) == IS_NO_SUCCESS) {
		cerr << "Fail to set auto parameter : gain" << endl;
		return;
	}

	if (is_SetAutoParameter(this->hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0) == IS_NO_SUCCESS) {
		cerr << "Fail to set auto parameter : whitebalance" << endl;
		return;
	}

	if (is_SetAutoParameter(this->hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0) == IS_NO_SUCCESS) {
		cerr << "Fail to set auto parameter : framerate" << endl;
		return;
	}
	
	if (is_SetAutoParameter(this->hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0) == IS_NO_SUCCESS) {
		cerr << "Fail to set auto parameter : shutter" << endl;
		return;
	}
}

Mat UEyeVideoCapture::get_image() {
	if (is_FreezeVideo(this->hCam, IS_WAIT) == IS_SUCCESS) {
		is_GetImageMem(this->hCam, &this->pMemVoid);
		return Mat(this->img_height, this->img_width, CV_8UC3, this->pMemVoid);
	}
}

UEyeVideoCapture::~UEyeVideoCapture() {
	is_ExitCamera(this->hCam);
}