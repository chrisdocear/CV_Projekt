#include <time.h>
#include <math.h>
#include "MyBackgroundSubtractor.hpp"

using namespace cv;
MyBackgroundSubtractor::MyBackgroundSubtractor()
{
	numSamples = 20;
	radius = 20;
	numMinSamples = 2;
	randomSampling = 16;
	isInitialized = false;
	initCounter = 10;
}

MyBackgroundSubtractor::MyBackgroundSubtractor(int numSamples, int radius, int numMinSamples, int randomSampling)
{
	this->numSamples = numSamples;
	this->radius = radius;
	this->numMinSamples = numMinSamples;
	this->randomSampling = randomSampling;
	this->randomSpreadSampling = randomSampling / 2;
	isInitialized = false;
	initCounter = 10;
}

MyBackgroundSubtractor::MyBackgroundSubtractor(int numSamples, int radius, int numMinSamples, int randomSampling, int initCounter, int randomSpreadSampling)
{
	this->numSamples = numSamples;
	this->radius = radius;
	this->numMinSamples = numMinSamples;
	this->randomSampling = randomSampling;
	if (initCounter <= numSamples && initCounter >= 0)
		this->initCounter = initCounter;
	else
		this->initCounter = numSamples;
	this->randomSpreadSampling = randomSpreadSampling;
	isInitialized = false;
}

MyBackgroundSubtractor::~MyBackgroundSubtractor()
{
}

int MyBackgroundSubtractor::getNumSamples()
{
	return numSamples;
}

int MyBackgroundSubtractor::getRadius()
{
	return radius;
}

int MyBackgroundSubtractor::getNumMinSamples()
{
	return numMinSamples;
}

int MyBackgroundSubtractor::getRandomSampling()
{
	return randomSampling;
}

void MyBackgroundSubtractor::getBackgroundImage(Mat backgroundImage)
{//just takes one set of random samples of every pixel as background
	if(!isInitialized)
	{//no background model available
		backgroundImage.setTo(0);
	} else
	{//calculate some background
		backgroundModel[rand() % initCounter].copyTo(backgroundImage);
	}
}

void MyBackgroundSubtractor::operator() (Mat image, Mat fgMask)
{
	//main bgs implementation
	Size sz = image.size();
	int height = sz.height;
	int width = sz.width;
	srand(time(NULL));
	if(!isInitialized)
	{//initialize background model
		isInitialized = true;
		backgroundModel = vector<Mat>(numSamples);
		for (int i = 0; i <= initCounter; i++)
		{
			//backgroundModel[i] = Mat(height, width, CV_8UC3);
			backgroundModel[i] = Mat(height, width, CV_8U);
			for (int j = 0; j < height; j++)
			{
				for (int k = 0; k < width; k++)
				{
					int xCoord = k + rand() % 3 - 1;
					int yCoord = j + rand() % 3 - 1;
					if (xCoord < 0)
						xCoord = 0;
					if (xCoord >= width)
						xCoord = width - 1;
					if (yCoord < 0)
						yCoord = 0;
					if (yCoord >= height)
						yCoord = height - 1;
					//Vec3b sample = image.at<Vec3b>(yCoord, xCoord);
					//backgroundModel[i].at<Vec3b>(j, k) = sample;
					uchar sample = image.at<uchar>(yCoord, xCoord);
					backgroundModel[i].at<uchar>(j, k) = sample;
				}
			}
		}
		fgMask.setTo(255);
		initCounter++;
	} else
	{
		//calculate foreground mask and update background
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int count = 0;
				int distance = 0;
				int index = 0;
				while (count < numMinSamples && index < initCounter)
				{//calculate euclidean distance of pixel to sample
					//Vec3b sample = backgroundModel[index].at<Vec3b>(y, x);
					//Vec3b pixel = image.at<Vec3b>(y, x);
					//int distR = sample.val[0] - pixel.val[0];
					//int distG = sample.val[1] - pixel.val[1];
					//int distB = sample.val[2] - pixel.val[2];
					//distance = (int) sqrt(distR * distR + distG * distG + distB * distB);

					//distance = max(abs(distR), max(abs(distG), abs(distB)));
					int backSamp = backgroundModel[index].at<uchar>(y, x);
					int pixel = image.at<uchar>(y, x);
					distance = abs(backSamp - pixel);

					if (distance < radius)
						count++;
					index++;
				}
				if (count >= numMinSamples)
				{//pixel detected as background
					fgMask.at<uchar>(y, x) = 0;
					int random = rand() % randomSampling;
					if (random == 0)
					{
						random = rand() % initCounter;
						//backgroundModel[random].at<Vec3b>(y, x) = image.at<Vec3b>(y, x);
						backgroundModel[random].at<uchar>(y, x) = image.at<uchar>(y, x);
					}
					//random = rand() % randomSampling;
					random = rand() % (randomSpreadSampling);
					if (random == 0)
					{
						int xCoord = x + rand() % 3 - 1;
						int yCoord = y + rand() % 3 - 1;
						if (xCoord < 0)
							xCoord = 0;
						if (xCoord >= width)
							xCoord = width - 1;
						if (yCoord < 0)
							yCoord = 0;
						if (yCoord >= height)
							yCoord = height - 1;
						random = rand() % initCounter;
						//backgroundModel[random].at<Vec3b>(yCoord, xCoord) = image.at<Vec3b>(yCoord, xCoord);
						backgroundModel[random].at<uchar>(yCoord, xCoord) = image.at<uchar>(y, x);
					}
				}else
				{
					fgMask.at<uchar>(y, x) = 255;
				}
			}
		}if (initCounter < numSamples)
		{//more initialization
			//backgroundModel[initCounter] = Mat(height, width, CV_8UC3);
			backgroundModel[initCounter] = Mat(height, width, CV_8U);
			for (int j = 0; j < height; j++)
			{
				for (int k = 0; k < width; k++)
				{
					int xCoord = k + rand() % 3 - 1;
					int yCoord = j + rand() % 3 - 1;
					if (xCoord < 0)
						xCoord++;
					if (xCoord >= width)
						xCoord--;
					if (yCoord < 0)
						yCoord++;
					if (yCoord >= height)
						yCoord--;
					//Vec3b sample = image.at<Vec3b>(yCoord, xCoord);
					//backgroundModel[initCounter].at<Vec3b>(j, k) = sample;
					uchar sample = image.at<uchar>(yCoord, xCoord);
					backgroundModel[initCounter].at<uchar>(j, k) = sample;
				}
			}
			initCounter++;
		}
	}


}
