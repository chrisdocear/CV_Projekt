#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class MyBackgroundSubtractor : 
	public BackgroundSubtractor
{
public:
	MyBackgroundSubtractor();
	MyBackgroundSubtractor(int numSamples, int radius, int numMinSamples, int randomSampling);
	~MyBackgroundSubtractor();
	int getNumSamples();
	int getRadius();
	int getNumMinSamples();
	int getRandomSampling();
	//void setNumSamples();
	//void setRadius();
	//void setNumMinSamples();
	//void setRandomSampling();
	Mat getBackgroundImage();
	Mat operator() (Mat image);
private:
	//number of samples per pixel
	int numSamples;
	//radius of the sphere
	int radius;
	//number of close samples for being part of the background
	int numMinSamples;
	//amount of random subsampling
	int randomSampling;
	//model of the background (3d matrix, because multiple samples for each pixel)
	vector<Mat> backgroundModel;
	//whether at least one reference image is available
	bool isInitialized;

};

