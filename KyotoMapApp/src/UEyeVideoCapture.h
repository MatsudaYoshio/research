#ifndef ___Class_UEyeVideoCapture
#define ___Class_UEyeVideoCapture

#include <uEye.h>

#include <opencv2/opencv.hpp>

#include "AppParameters.h"

class UEyeVideoCapture {
private:
	static constexpr INT img_width = param::CAMERA_W;
	static constexpr INT img_height = param::CAMERA_H;
	static constexpr INT img_bpp = 24;
	static constexpr INT pixel_clock = 140;
	static constexpr double fps = 50;
	static constexpr double exp = 18;
	HIDS hCam;
	char* imgMem;
	int memId;
	void* pMemVoid;
public:
	UEyeVideoCapture();
	~UEyeVideoCapture();
	cv::Mat get_image();
};

#endif