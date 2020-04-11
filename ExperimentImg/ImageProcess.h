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
#include<afxwin.h>
#include<stdlib.h>
#include<algorithm>
#include <math.h>
using namespace cv;
using namespace std;

namespace ImageProcess {
	void MatToCImage(const Mat, CImage*);
	void CImageToMat(const CImage*, Mat&);

};