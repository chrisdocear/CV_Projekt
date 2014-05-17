#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include "Helpers.hpp"

using namespace cv;

Mat preprocess(Mat img)
{
	Mat rtn;
	//medianBlur(img, rtn, 3);
	return rtn;
}

Mat postprocess(Mat img)
{
	Mat rtn;
	//TODO: draw bounding boxes
	return rtn;
}
