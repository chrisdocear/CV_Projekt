#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv/cv.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>
#include "Helpers.hpp"
#include "MyBackgroundSubtractor.hpp"

using namespace cv;
using namespace std;

 
int main( int argc, char** argv )
{	
	//find first file number
	int trainingFolder = 4;
	stringstream sstream;
	sstream << "../training/" << trainingFolder << "/";
	string dir = sstream.str();
	string searchTerm = "0*";
	TCHAR* searchString = new TCHAR[dir.size()+3];
	searchString[dir.size()] = '0';
	searchString[dir.size() + 1] = '*';
	searchString[dir.size() + 2] = 0;
	copy(dir.begin(), dir.end(), searchString);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(searchString, &FindFileData);

	int firstFile;
	wstring fileString(FindFileData.cFileName);
	wstringstream convert(fileString);
	convert >> firstFile;

	char buffer [10];
	Mat img;
	/*/////////////// backgroundsubtraction, exclude
	BackgroundSubtractorMOG2 bgs = BackgroundSubtractorMOG2(20, 50);
	cout << "nmixtures " << bgs.getInt("nmixtures") << endl;
	cout << "backgroundRatio " << bgs.getDouble("backgroundRatio") << endl;
	cout << "fCT " << bgs.getDouble("fCT") << endl;
	cout << "fTau " << bgs.getDouble("fTau") << endl;
	cout << "fVarInit " << bgs.getDouble("fVarInit") << endl;
	cout << "fVarMin " << bgs.getDouble("fVarMin") << endl;
	cout << "fVarMax " << bgs.getDouble("fVarMax") << endl;
	cout << "nShadowDetection " << bgs.getInt("nShadowDetection") << endl;
	cout << "varThreshold " << bgs.getDouble("varThreshold") << endl;
	cout << "varThresholdGen " << bgs.getDouble("varThresholdGen") << endl;
	cout << "history: " << bgs.getInt("history") << endl;
	
	*//////////////////
	Mat fore(512, 1392, CV_8U);
	Mat back;
	//BackgroundSubtractorMOG2 bgsMog = BackgroundSubtractorMOG2(20, 50);
	MyBackgroundSubtractor bgs = MyBackgroundSubtractor(20, 20, 2, 16);
	for (int i = firstFile; ; i++)
	{
		try
		{
			sprintf(buffer, "%010d", i);
			ostringstream path;
			path << dir << buffer << ".png";

			Mat img = imread(path.str().c_str()); 
			if (!img.data) 
			{
				cerr << " Could not load image file : " << path.str() << endl ;
				exit ( EXIT_FAILURE ) ;
			}

			//do preprocessing
			//Mat processed = preprocess(img);
			//Mat im_gray;
			//cvtColor(img,im_gray,CV_RGB2GRAY);
			//imshow("gray", im_gray);
			//do background subtraction
			//---bgs(img, fore);
			//bgs(img, fore);
			cvtColor(img,img,CV_RGB2GRAY);
			bgs(img, fore);
			//back = bgs.getBackgroundImage();
			//imshow("background", back);
			imshow("foreground", fore);
			imshow("original", img);
			//bgs(img, back);
			//do postprocessing

			//do object tracking

			//do object classification


			/*////////////////////////include frame number, taken from opencv tutorials
			string frameNumberString = buffer;
			istringstream iss(frameNumberString);
			rectangle(fore, cv::Point(10, 2), cv::Point(120,20), cv::Scalar(255,255,255), -1);
			putText(fore, frameNumberString.c_str(), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
			*/////////////////////////

			//imshow("fore ground mask", fore);	
			//imshow("original", img);
		}
		catch( cv::Exception& e ){
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
		}		


		if(cvWaitKey (0) == 27){			
			break;
		}
	}
	cv::destroyAllWindows();
	return EXIT_SUCCESS ;
}


