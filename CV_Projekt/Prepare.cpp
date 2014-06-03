
#include "Prepare.h"


Prepare::Prepare()
{
	std::cout << "PrepareBackground()" << std::endl;
}

Prepare::~Prepare()
{
	std::cout << "~PrepareBackground()" << std::endl;
}


void Prepare::process(const cv::Mat &img_input, cv::Mat &img_output, cv::Mat &img_bgmodel,bool useViBe)
{
	if(img_input.empty())
		return;

	cv::Mat frame,tmp;
	prepareFrame(img_input,frame);
	Sobel(img_input,tmp,CV_16S,1,0,3,1,0,BORDER_DEFAULT);
	convertScaleAbs(tmp,tmp);
	
	Size sz = img_input.size();
	int height = sz.height;
	int width = sz.width;


	cv::Mat img_background;
	if(useViBe || false)
	{
		//mbgs(20, 20, 2, 16);
		cvtColor(frame,frame,CV_RGB2GRAY);
		if (img_foreground.dims != 2)
			img_foreground = Mat(height, width, CV_8U);
		mbgs(frame, img_foreground);
		if (img_background.dims != 2)
			img_background = Mat(height, width, CV_8U);
		mbgs.getBackgroundImage(img_background);
	}
	else
	{
		initBGS(5,15,true);
		bgs(frame, img_foreground, 0.05);
		bgs.getBackgroundImage(img_background);
	}

	prepareForeground();

	//cv::imshow("Sobel",tmp);
	if(sfore) cv::imshow("Foreground", img_foreground);
	if(sback) cv::imshow("Background", img_background);
	if(smimg)
	{
		if(scont)
		{
		    std::vector<std::vector<cv::Point> > contours;
	        findContours(img_foreground,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
			drawContours(frame,contours,-1,Scalar(0,0,255),2);
		}
		cv::imshow("PreparedImage", frame);
	}

	img_foreground.copyTo(img_output);
	img_background.copyTo(img_bgmodel);
}

void Prepare::show(bool showForeground,bool showBackground,bool showManipulatedImg,bool showContours)
{
	sfore = showForeground;
	sback = showBackground;
	smimg = showManipulatedImg;
	scont = showContours;
}

void Prepare::initBGS(int history, int thereshold, bool shadowDetection)
{
	//bgs.set("nShadowDetection",true);
	bgs.set("varThresholdGen",0.5);
	bgs.set("fVarInit",8);
	bgs.set("fTau",0.2);
	bgs.set("backgroundRatio",0.85);
}



void Prepare::prepareForeground()
{

	//cv::blur(img_foreground,img_foreground,Size(7,7),Point(-1,-1),BORDER_DEFAULT);
	//cv::threshold(img_foreground, img_foreground, 5, 255, cv::THRESH_BINARY);

    erode(img_foreground,img_foreground,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(img_foreground,img_foreground,getStructuringElement(MORPH_ELLIPSE,Size(3,3)));
}
void Prepare::prepareFrame(const cv::Mat &img_input, cv::Mat &img_output)
{
	//img_input.copyTo(img_output);
	 cv::GaussianBlur(img_input ,img_output,Size(15,45),2,5,BORDER_DEFAULT);
	//cv::medianBlur(img_output,img_output,7);
	// cv::blur(img_output,img_output,Size(7,5),Point(-1,-1),BORDER_DEFAULT);
}



