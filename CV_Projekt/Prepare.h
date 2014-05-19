#ifndef PREPARE_H_
#define PREPARE_H_

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class Prepare
{
private:
  cv::BackgroundSubtractorMOG2 bgs;
  cv::Mat img_foreground;
  double alpha;
  bool enableThreshold;
  int threshold;
  bool sfore;
  bool sback;
  bool smimg;
  bool scont;



public:
  Prepare();
  ~Prepare();

  void process(const cv::Mat &img_input, cv::Mat &img_output, cv::Mat &img_bgmodel);
  void show(bool showForeground,bool showBackground,bool showManipulatedImg,bool showContours);
  void initBGS(int history, int thereshold, bool shadowDetection);


private:

  void prepareFrame(const cv::Mat &img_input, cv::Mat &img_output);
  void prepareForeground();

};




#endif /* PREPARE_H_ */
