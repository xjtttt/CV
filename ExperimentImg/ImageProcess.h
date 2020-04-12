#pragma once

#include "stdafx.h"
#include "ExperimentImgDlg.h"
#include "ImageProcess.h"
#include<vector>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/calib3d.hpp>
#include <opencv2/dnn.hpp>
#include<afxwin.h>
#include<stdlib.h>
#include<algorithm>
#include <math.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace cv::dnn;
using namespace cv;
using namespace std;

typedef struct {
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}CORNERS;

namespace ImageProcess {
	void MatToCImage(const Mat, CImage*);
	void CImageToMat(const CImage*, Mat&);
	void stitch(string, string, CImage*);
	void SIFTstitch(string, string, CImage*);
	void image_detection(string cfgFile, string weight, string clsNames, string image_path);
};