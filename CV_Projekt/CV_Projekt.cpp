#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv/cv.h>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{	
    cvNamedWindow ("original", CV_WINDOW_AUTOSIZE ) ;
	cvNamedWindow ("canny", CV_WINDOW_AUTOSIZE ) ;
	string dir = "..\\training\\1\\";
	char buffer [10];	
	IplImage* img;
	IplImage* binaryImage;
	IplImage* normalized;
	IplImage* gauss;
	IplImage* cannyimg;
	for (int i = 0; i <= 119; i++)
	{		
		sprintf(buffer, "%010d", i);
		ostringstream path;
		path << dir << buffer << ".png";

		img = cvLoadImage (path.str().c_str()); 
		if (! img ) {
			cerr << " Could not load image file : " << path.str() << endl ;
			exit ( EXIT_FAILURE ) ;
		}
	
		binaryImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		cvSplit (img, binaryImage, NULL , NULL , NULL );
    
		normalized = cvCreateImage (cvGetSize (img), img ->depth , 1); 
		cvEqualizeHist ( binaryImage , normalized );
	
		gauss = cvCreateImage ( cvGetSize (img), img->depth , 1); 
		cvSmooth ( normalized , gauss , CV_GAUSSIAN , 13, 13);

		cannyimg = cvCreateImage ( cvGetSize (gauss), gauss ->depth , 1); 
		cvCanny(gauss, cannyimg, 40, 130); 
   
		/*int dstWidth=img->width;
		int dstHeight=img->height+img->height;

		IplImage* dst=cvCreateImage(cvSize(dstWidth,dstHeight),cannyimg->depth,3); 

		try{
			cvSetImageROI(dst, cvRect(0, 0,img->width,img->height));
			cvCopy(cannyimg,dst,NULL);
			cvResetImageROI(dst);

			cvSetImageROI(dst, cvRect(0, img->height,cannyimg->width,cannyimg->height) );
			cvCopy(img,dst,NULL);
			cvResetImageROI(dst);
		}
		catch( cv::Exception& e ){
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
		}*/
	

	
		cvShowImage ("original", img ) ;	
		cvShowImage ("canny", cannyimg ) ;

		
		cvReleaseImage (&img) ;
		cvReleaseImage (&normalized) ;
		cvReleaseImage (&gauss) ;
		cvReleaseImage (&cannyimg) ;
		cvReleaseImage (&binaryImage) ;
		if(cvWaitKey (0) == 27){			
			break;
		}
	}
	
    cvDestroyWindow ("original") ;
	cvDestroyWindow ("canny") ;
	
	//cvReleaseImage (&dst) ;
	return EXIT_SUCCESS ;
}

