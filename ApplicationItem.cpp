#include <opencv2/opencv.hpp>

#include "ApplicationItem.h"

using namespace cv;

ApplicationItem::ApplicationItem(Rect &rect, Scalar color) :rect(rect), color(color) {}