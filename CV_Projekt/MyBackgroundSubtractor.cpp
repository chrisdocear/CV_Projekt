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
}

MyBackgroundSubtractor::MyBackgroundSubtractor(int numSamples, int radius, int numMinSamples, int randomSampling)
{
	this->numSamples = numSamples;
	this->radius = radius;
	this->numMinSamples = numMinSamples;
	this->randomSampling = randomSampling;
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

Mat MyBackgroundSubtractor::getBackgroundImage()
{//just takes first sample of every pixel as background
	if(!isInitialized)
	{//no background model available
		Mat backgroundImage;
		backgroundImage.create(100, 100, CV_8U);
		backgroundImage.setTo(0);
		return backgroundImage;
	} else
	{//calculate some background
		return backgroundModel[0];
	}
}

void MyBackgroundSubtractor::operator() (Mat image, Mat fgMask)
{
	//main bgs implementation
	Size sz = image.size();
	int height = sz.height;
	int width = sz.width;
	srand(time(NULL));
	if (!isInitialized)
	{//initialize background model
		backgroundModel = vector<Mat>(numSamples);
		for (int i = 0; i < numSamples; i++)
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
						xCoord++;
					if (xCoord >= width)
						xCoord--;
					if (yCoord < 0)
						yCoord++;
					if (yCoord >= height)
						yCoord--;
					//Vec3b sample = image.at<Vec3b>(yCoord, xCoord);
					//backgroundModel[i].at<Vec3b>(j, k) = sample; 
					uchar sample = image.at<uchar>(yCoord, xCoord);
					backgroundModel[i].at<uchar>(j, k) = sample; 
				}
			}
		}
		fgMask.setTo(255);
		isInitialized = true;
	} else
	{//calculate foreground mask and update background
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int count = 0;
				int distance = 0;
				int index = 0;
				while (count < numMinSamples && index < numSamples)
				{//calculate euclidean distance of pixel to sample
					//Vec3b sample = backgroundModel[index].at<Vec3b>(y, x);
					//Vec3b pixel = image.at<Vec3b>(y, x);
					//Vec3b distVector = sample - pixel;
					//distance = (int) sqrt(	distVector.val[0] * distVector.val[0] + 
					//	distVector.val[1] * distVector.val[1] + 
					//	distVector.val[2] * distVector.val[2]);
					distance = abs(backgroundModel[index].at<uchar>(y, x) - image.at<uchar>(y, x));
					if (distance < radius)
						count++;
					index++;
				}
				if (count >= numMinSamples)
				{
					fgMask.at<uchar>(y, x) = 0;
					int random = rand() % randomSampling;
					if (random == 0)
					{
						random = rand() % numSamples;
						//backgroundModel[random].at<Vec3b>(y, x) = image.at<Vec3b>(y, x);
						backgroundModel[random].at<uchar>(y, x) = image.at<uchar>(y, x);
					}
					random = rand() % randomSampling;
					if (random == 0)
					{
						int xCoord = x + rand() % 3 - 1;
						int yCoord = y + rand() % 3 - 1;
						if (xCoord < 0)
							xCoord++;
						if (xCoord >= width)
							xCoord--;
						if (yCoord < 0)
							yCoord++;
						if (yCoord >= height)
							yCoord--;
						random = rand() % numSamples;
						//backgroundModel[random].at<Vec3b>(yCoord, xCoord) = image.at<Vec3b>(y, x);
						backgroundModel[random].at<uchar>(yCoord, xCoord) = image.at<uchar>(y, x);
					}
				}else
				{
					fgMask.at<uchar>(y, x) = 255;
				}
			}
		}
	}
}
