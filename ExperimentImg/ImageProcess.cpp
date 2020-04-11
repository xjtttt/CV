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
using namespace cv::xfeatures2d;

void MatToCImage(Mat& mat, CImage& cimage)
{
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//�ؽ�cimage
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage.GetBits();		//ָ����������ָ��
	int nStep = cimage.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	if (1 == nChannels)								//���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}

void CImageToMat(CImage& cimage, Mat& mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//�ؽ�mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cimage.GetBits();		//ָ����������ָ��
	int nStep = cimage.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}


void calculateCorners(const Mat& H, const Mat& src,CORNERS& corners)
{
	double v2[] = { 0, 0, 1 };// ���Ͻ�
	double v1[3];// �任�������ֵ
	Mat V2 = Mat(3, 1, CV_64FC1, v2);  // ������
	Mat V1 = Mat(3, 1, CV_64FC1, v1);  // ������

	V1 = H * V2;
	// ���Ͻ�(0,0,1)
	cout << "V2: " << V2 << endl;
	cout << "V1: " << V1 << endl;
	corners.left_top.x = v1[0] / v1[2];
	corners.left_top.y = v1[1] / v1[2];

	// ���½�(0,src.rows,1)
	v2[0] = 0;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  // ������
	V1 = Mat(3, 1, CV_64FC1, v1);  // ������
	V1 = H * V2;
	corners.left_bottom.x = v1[0] / v1[2];
	corners.left_bottom.y = v1[1] / v1[2];

	// ���Ͻ�(src.cols,0,1)
	v2[0] = src.cols;
	v2[1] = 0;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  // ������
	V1 = Mat(3, 1, CV_64FC1, v1);  // ������
	V1 = H * V2;
	corners.right_top.x = v1[0] / v1[2];
	corners.right_top.y = v1[1] / v1[2];

	// ���½�(src.cols,src.rows,1)
	v2[0] = src.cols;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  // ������
	V1 = Mat(3, 1, CV_64FC1, v1);  // ������
	V1 = H * V2;
	corners.right_bottom.x = v1[0] / v1[2];
	corners.right_bottom.y = v1[1] / v1[2];

}
void ImageProcess::stitch(string a,string b,CImage* r)
{
	// ��ȡ��Ҫƴ�ӵ�����ͼƬ
	CORNERS corners;
	
	Mat image_original_1 = imread(b, 1);// ��ͼ
	Mat image_original_2 = imread(a, 1);// ��ͼ
	
	/*Mat image_original_1 ;// ��ͼ
	Mat image_original_2 ;// ��ͼ
	CImageToMat(*a, image_original_2);
	CImageToMat(*b, image_original_1);*/

	// ת��Ϊ�Ҷ�ͼ
	Mat image_gray_1, image_gray_2;
	cvtColor(image_original_1, image_gray_1, COLOR_RGB2GRAY);
	cvtColor(image_original_2, image_gray_2, COLOR_RGB2GRAY);


	// ��ȡ������
	Ptr<Feature2D> surf = xfeatures2d::SURF::create(2000);
	vector<KeyPoint> key_point_1, key_point_2;
	surf->detect(image_gray_1, key_point_1);
	surf->detect(image_gray_2, key_point_2);

	// ������������Ϊ�±ߵ�������ƥ����׼��
	Mat image_desc_1, image_desc_2;
	surf->compute(image_gray_1, key_point_1, image_desc_1);
	surf->compute(image_gray_2, key_point_2, image_desc_2);

	// BFMatcher matcher;
	FlannBasedMatcher matcher;
	vector<vector<DMatch> > matche_points;
	vector<DMatch> good_matche_points;

	vector<Mat> train_desc(1, image_desc_1);
	matcher.add(train_desc);
	matcher.train();

	matcher.knnMatch(image_desc_2, matche_points, 3);

	// ʹ�� Lowe's algorithm, ��ȡ�õ�ƥ���
	for (int i = 0; i < matche_points.size(); i++)
	{
		if (matche_points[i][0].distance < 0.4 * matche_points[i][1].distance)
		{
			good_matche_points.push_back(matche_points[i][0]);
		}
	}

	Mat match_1;
	drawMatches(image_original_2, key_point_2, image_original_1,
		key_point_1, good_matche_points, match_1);
	imshow("match_1 ", match_1);

	vector<Point2f> image_points_1, image_points_2;

	for (int i = 0; i < good_matche_points.size(); i++)
	{
		image_points_2.push_back(key_point_2[good_matche_points[i].queryIdx].pt);
		image_points_1.push_back(key_point_1[good_matche_points[i].trainIdx].pt);
	}


	// ��ȡͼ��1��ͼ��2��ͶӰӳ����� �ߴ�Ϊ3*3   Ѱ�Ҵ��ұߵ���ߵľ���任
	Mat homo = findHomography(image_points_1, image_points_2, FM_RANSAC);

	// ������׼ͼ���ĸ���������
	calculateCorners(homo, image_original_1,corners);

	//ͼ����׼
	Mat image_transform_1, image_transform_2;
	//�ұ�ת�����  ����任
	warpPerspective(image_original_1, image_transform_1, homo, Size(MAX(corners.right_top.x,
		corners.right_bottom.x), image_original_2.rows));
	//imshow("image_transform_1", image_transform_1);


	// ����ƴ�Ӻ��ͼ,����ǰ����ͼ�Ĵ�С
	int dst_width = image_transform_1.cols;  // ȡ���ҵ�ĳ���Ϊƴ��ͼ�ĳ���
	int dst_height = image_original_2.rows;

	Mat dst(dst_height, dst_width, CV_8UC3);
	dst.setTo(0);

	image_transform_1.copyTo(dst(Rect(0, 0, image_transform_1.cols, image_transform_1.rows)));
	image_original_2.copyTo(dst(Rect(0, 0, image_original_2.cols, image_original_2.rows)));

	imshow("dst", dst);
	//MatToCImage(match_1, *r);
	
	
}