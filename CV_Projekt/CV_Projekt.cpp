#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <vector>
#include <opencv/cv.h>
#include <iostream>
#include "Prepare.h"
#include <Windows.h>

using namespace cv;
using namespace std;

bool first = true;
int main( int argc, char** argv )
{
    //find first file number
	int trainingFolder = 5;
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
	//end find first file number
	
	/*
	namedWindow("original",WINDOW_NORMAL);
	string dir = "/home/wuang/Develop/workspace/training/1/";  	//  Directory
	*/
	char buffer [10];

    Mat origin, back, fore;
    Prepare pre = Prepare();

	for (int i = firstFile; i < 400; i++)
	//for (int i = 0; i < 400; i++)
	{
		try
		{
			sprintf(buffer, "%010d", i);
			ostringstream path;
			path << dir << buffer << ".png";
			origin = imread(path.str().c_str());
			if (!origin.data) {
				cerr << " Could not load image file : " << path.str() << endl ;
				exit ( EXIT_FAILURE ) ;
			}


			//show(foreground, background, manipulated image, contours on image)
			pre.show(true,true,true,true);
			//origin wird nicht angeruehrt, fore und back werden geupdated
			pre.process(origin,fore,back,true);



			//TODO Tracking



			if(false)
			{
				ostringstream p;
				p << dir << "fore" << buffer << ".png";
				imwrite(p.str(),fore);
				imshow("original", origin);
			}

		}
		catch( cv::Exception& e )
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
		}
		if(cvWaitKey (0) == 27)
			break;
	}
	destroyAllWindows();
	return EXIT_SUCCESS ;
}
