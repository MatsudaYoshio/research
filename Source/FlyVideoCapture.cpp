#include <FlyCapture2.h>
#include <FlyCapture2GUI.h>

#include <opencv2/opencv.hpp>

#include "FlyVideoCapture.h"

using namespace std;
using namespace FlyCapture2;
using namespace cv;

FlyVideoCapture::FlyVideoCapture() {
	/* Connect the camera */
	this->flycam_error = this->flycam.Connect(0);
	if (this->flycam_error != PGRERROR_OK)
	{
		cout << "Failed to connect to camera" << endl;
		return;
	}

	/* Get the camera info and print it out */
	this->flycam_error = this->flycam.GetCameraInfo(&flycam_info);
	if (this->flycam_error != PGRERROR_OK)
	{
		cout << "Failed to get camera info from camera" << endl;
		return;
	}

	/* Setting */
	/*
	this->flycam_error = this->flycam.SetVideoModeAndFrameRate(VIDEOMODE_1280x960Y8, FRAMERATE_60);
	if (this->flycam_error != PGRERROR_OK)
	{
		cout << "Failed to set video mode and frame rate" << endl;
		return;
	}
	*/

	this->flycam_error = this->flycam.StartCapture();
	if (this->flycam_error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED)
	{
		cout << "Bandwidth exceeded" << endl;
		return;
	}
	else if (this->flycam_error != PGRERROR_OK)
	{
		cout << "Failed to start image capture" << endl;
		return;
	}
}

FlyVideoCapture::~FlyVideoCapture() {
	this->flycam_error = this->flycam.StopCapture();
	if (this->flycam_error != PGRERROR_OK)
	{
		// This may fail when the camera was removed, so don't show 
		// an error message
	}
	this->flycam.Disconnect();
}

Mat FlyVideoCapture::get_image() {
	/* Get the image */
	this->flycam_error = this->flycam.RetrieveBuffer(&this->fly_image);
	if (this->flycam_error != PGRERROR_OK)
	{
		cout << "capture error" << endl;
		return this->cv_image;
	}

	/* convert to bgr */
	this->fly_image.Convert(PIXEL_FORMAT_BGR, &this->bgr_image);
	
	/* convert to OpenCV Mat */
	unsigned int rowBytes = (unsigned int)((double)this->bgr_image.GetReceivedDataSize() / (double)this->bgr_image.GetRows());
	this->cv_image = Mat(this->bgr_image.GetRows(), this->bgr_image.GetCols(), CV_8UC3, this->bgr_image.GetData(), rowBytes);

	return this->cv_image;
}