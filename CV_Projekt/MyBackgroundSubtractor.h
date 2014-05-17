#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class MyBackgroundSubtractor :
	public cv::BackgroundSubtractor
{
public:
	MyBackgroundSubtractor(void);
	~MyBackgroundSubtractor(void);
};

