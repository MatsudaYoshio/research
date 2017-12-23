#ifndef ___Class_FlyVideoCapture
#define ___Class_FlyVideoCapture

#include <FlyCapture2.h>
#include <FlyCapture2GUI.h>

#include <opencv2/opencv.hpp>

class FlyVideoCapture {
private:
	FlyCapture2::Camera flycam;
	FlyCapture2::CameraInfo flycam_info;
	FlyCapture2::Error flycam_error;
	FlyCapture2::Image fly_image, bgr_image;
	cv::Mat cv_image;
public:
	FlyVideoCapture();
	~FlyVideoCapture();
	cv::Mat get_image();
};

#endif