#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/calib3d.hpp>
#include <iostream>
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

typedef struct
{
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}CORNERS;

class ImageProcess
{
public:
	static void stitch(string,string, CImage*);


};