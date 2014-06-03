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
	MyBackgroundSubtractor::MyBackgroundSubtractor(int numSamples, int radius, int numMinSamples, int randomSampling, int initCounter, int randomSpreadSampling);
	~MyBackgroundSubtractor();
	int getNumSamples();
	int getRadius();
	int getNumMinSamples();
	int getRandomSampling();
	//void setNumSamples();
	//void setRadius();
	//void setNumMinSamples();
	//void setRandomSampling();
	void getBackgroundImage(Mat backgroundImage);
	void operator() (Mat image, Mat fgMask);
private:
	//number of samples per pixel, increasing this increases probability that matching sample is found
	int numSamples;
	//radius of the sphere (for B/W just difference), increase to classify more as background
	int radius;
	//number of samples closer than radius for being part of the background
	int numMinSamples;
	//amount of random subsampling to replace background sample, lower values mean more replacements
	int randomSampling;
	//amounts of random subsampling to replace a neighboring sample (or same), lower values mean ghosts disintegrate faster
	int randomSpreadSampling;
	//model of the background (vector of images, each image contains one sample of every pixel)
	vector<Mat> backgroundModel;
	//whether at least one reference image is available
	bool isInitialized;
	//counter, so that background model is less dependant on first frame, can be set with constructor, high values make strong ghosts
	int initCounter;

};
