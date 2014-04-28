#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv/cv.h>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{	
    namedWindow("original", WINDOW_AUTOSIZE );
	namedWindow("canny");
	string dir = "..\\training\\1\\";
	char buffer [10];	
	Mat img;
	Mat binaryImage;
	Mat normalized;
	Mat gauss;
	Mat cannyimg;
	for (int i = 0; i <= 119; i++)
	{
		try{
		sprintf(buffer, "%010d", i);
		ostringstream path;
		path << dir << buffer << ".png";
		
		Mat img = imread(path.str().c_str()); 
		if (!img.data) {
			cerr << " Could not load image file : " << path.str() << endl ;
			exit ( EXIT_FAILURE ) ;
		}
		int c = img.channels();
	
		Mat binaryImage = Mat(img.rows, img.cols, CV_8UC1);
		cvtColor(img, binaryImage, CV_RGB2GRAY);		
    
		Mat normalized =  Mat(img.size(), img.depth()); 
		equalizeHist(binaryImage, normalized );
	
		Mat gauss =  Mat(img.size(), img.depth()); 
		GaussianBlur(normalized, gauss, Size(13, 13), 0, 0);		

		Mat cannyimg = Mat(img.size(), img.depth());  
		Canny(gauss, cannyimg, 40, 130);		

		imshow("original", img);	
		imshow("canny", cannyimg);	
		}
		catch( cv::Exception& e ){
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
		}		
		
		
		if(cvWaitKey (0) == 27){			
			break;
		}
	}
	destroyAllWindows();
	return EXIT_SUCCESS ;
}

