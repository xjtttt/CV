// tab2.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab2.h"
#include "afxdialogex.h"


// tab2 对话框

IMPLEMENT_DYNAMIC(tab2, CDialogEx)

tab2::tab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent) {}

tab2::~tab2() {}

void tab2::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SIFT, ComboSift);
	DDX_Control(pDX, IDC_COMBO_METHOD, ComboMethod);
	DDX_Control(pDX, IDC_COMBO_MATCH, ComboMatch);
	DDX_Control(pDX, IDC_EDIT_SCALE, EditScale);
}

BOOL tab2::OnInitDialog() {
	CDialogEx::OnInitDialog();

	ComboSift.InsertString(0, _T("SIFT"));
	ComboSift.InsertString(1, _T("SURF"));
	ComboSift.SetCurSel(0);


	ComboMethod.InsertString(0, L"点对映射");
	ComboMethod.InsertString(1, L"几何校正");
	ComboMethod.SetCurSel(0);

	ComboMatch.InsertString(0, L"BruteForce");
	ComboMatch.InsertString(1, L"FLANN");
	ComboMatch.SetCurSel(0);

	EditScale.SetWindowTextW(L"1.0");

	return TRUE;
}

BEGIN_MESSAGE_MAP(tab2, CDialogEx)
END_MESSAGE_MAP()


// tab2 消息处理程序

// 使用模板，代码复用
// 因为对于SIFT跟SURF这两种方法，大部分代码都是相同的
template<typename T>
static void Solve(T feature, CImage* src, CImage* img, CImage* res, int run_method, int match_method, double Scale) {

	// 将读入的 CImage 改为 Mat 格式
	Mat source, change;
	ImageProcess::CImageToMat(src, source);
	ImageProcess::CImageToMat(img, change);

	// 更改图像大小，等比缩放
	Mat sou_1, cha_1;
	resize(source, sou_1, Size(), Scale, Scale);
	resize(change, cha_1, Size(), Scale, Scale);
	source = sou_1, change = cha_1;

	//检测特征点，检测信息保存在keypoint中
	static vector<KeyPoint>keypoint_source;
	keypoint_source.clear();
	feature->detect(source, keypoint_source);
	Mat output_source;
	drawKeypoints(source, keypoint_source, output_source);

	static vector<KeyPoint>keypoint_change = vector<KeyPoint>();
	keypoint_change.clear();
	feature->detect(change, keypoint_change);
	Mat output_change;
	drawKeypoints(change, keypoint_change, output_change);

	//计算描述矩阵，保存在description中
	Mat description_source;
	feature->compute(source, keypoint_source, description_source);

	Mat description_change;
	feature->compute(change, keypoint_change, description_change);

	static vector<DMatch>matches = vector<DMatch>(); //匹配矩阵
	matches.clear();
	if (run_method == 0) {
		BFMatcher matcher;// BruteForce匹配
		matcher.match(description_source, description_change, matches);
	}
	else {
		FlannBasedMatcher matcher;// Flann匹配
		matcher.match(description_source, description_change, matches);
	}

	static vector<Point2f>obj = vector<Point2f>();
	obj.clear();
	static vector<Point2f>scene = vector<Point2f>();
	scene.clear();
	//保存之前匹配的点的坐标
	for (auto i : matches) {
		obj.push_back(keypoint_source[i.queryIdx].pt);
		scene.push_back(keypoint_change[i.trainIdx].pt);
	}

	// 采用findHomography函数进行RANSAC筛选
	static vector<uchar>inliersMask(obj.size());
	inliersMask.clear();

	// 转移矩阵H
	Mat H = findHomography(scene, obj, FM_RANSAC, 3.0, inliersMask, 2000);

	// 点对映射
	if (run_method == 0) {
		// 去掉外点
		static vector<DMatch>inliers = vector<DMatch>();
		inliers.clear();

		for (int i = 0; i < inliersMask.size(); i++) {
			if (inliersMask[i])
				inliers.push_back(matches[i]);
		}

		// 交换，保留新的筛选结果
		matches.swap(inliers);

		Mat image_match2;
		drawMatches(source, keypoint_source, change, keypoint_change, matches, image_match2);

		// 将答案矩阵转换为Cimage
		ImageProcess::MatToCImage(image_match2, res);
		return;
	}

	// 投影变换
	Mat after;
	warpPerspective(change, after, H, source.size());

	// 图像简单拼接，左边是原图，右边是通过几何矫正之后的图
	Mat ans;
	ans.create(max(source.size().height, after.size().height), source.size().width + after.size().width, source.type());
	source.copyTo(ans(Rect(0, 0, source.size().width, source.size().height)));
	after.copyTo(ans(Rect(source.size().width, 0, after.size().width, after.size().height)));

	// 将答案矩阵转换为Cimage
	ImageProcess::MatToCImage(ans, res);
	return;
}

// src为原图像，img为待处理图像，res为处理后图像
void tab2::Run(CImage* src, CImage* img, CImage* res) {
	int is_SIFT = (ComboSift.GetCurSel() == 0 ? 1 : 0);
	int run_method = ComboMethod.GetCurSel();
	int match_method = ComboMatch.GetCurSel();
	CString str;
	EditScale.GetWindowTextW(str);
	double scale = _wtof(str);

	if (is_SIFT) {
		auto feature = xfeatures2d::SIFT::create(2000);
		Solve(feature, src, img, res, run_method, match_method, scale);
	}
	else {
		auto feature = xfeatures2d::SURF::create(2000);
		Solve(feature, src, img, res, run_method, match_method, scale);
	}
}
