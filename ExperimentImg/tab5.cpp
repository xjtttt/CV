// tab5.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab5.h"
#include "afxdialogex.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

// tab5 对话框

IMPLEMENT_DYNAMIC(tab5, CDialogEx)

tab5::tab5(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

tab5::~tab5()
{
}

void tab5::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDOK, button);
}

BOOL tab5::OnInitDialog()
{
	mvideo.Create(IDD_VIDEO, this);
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(tab5, CDialogEx)
	ON_BN_CLICKED(IDOK, &tab5::OnBnClickedOk)
END_MESSAGE_MAP()


// tab5 消息处理程序



void tab5::OnBnClickedOk()
{
	// 打开视频  需要更换地址
	cv::VideoCapture capture("../图片视频素材/cat.mp4");
	// 检查视频是否成功打开
	if (!capture.isOpened())
	{
		AfxMessageBox(_T("视频打开失败"));
	}
	else {
		cv::Mat frame;  //视频当前帧
		while (1)
		{
			capture >> frame;//等价于cap.read(frame);
			if (frame.empty())//如果某帧为空则退出循环
				break;

			//存储关键点
			vector<KeyPoint> keypoints;

			//ORB特征点检测
			//创建一个Ptr<ORB>类型的orb，用于接收ORB类中create()函数的返回值
			Ptr<ORB> detector = ORB::create();

			//找出关键点
			//detect参数【需要检测的图像，关键点数组，第三个参数为默认值】
			detector->detect(frame, keypoints, Mat());

			//定义输出检测特征点的图片
			Mat keypoint_img;
			//参数【原图，原图关键点，带有关键点的输出图像，后面两个为默认值】
			drawKeypoints(frame, keypoints, keypoint_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
			imshow("src", frame);
			imshow("video", keypoint_img);
			waitKey(1);//延时
		}

		capture.release();
	}
	//mvideo.ShowWindow(SW_SHOW);


	//mvideo.SetFocus();
	// TODO: 在此添加控件通知处理程序代码
	
}
