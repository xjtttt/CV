#include "stdafx.h"
#include "ImageProcess.h"

void ImageProcess::MatToCImage(const Mat mat, CImage* cimage) {
	if (0 == mat.total()) {
		return;
	}

	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels)) {
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//重建cimage
	cimage->Destroy();
	cimage->Create(nWidth, nHeight, 8 * nChannels);

	//拷贝数据

	const uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage->GetBits();		//指向数据区的指针
	int nStep = cimage->GetPitch();					//每行的字节数,注意这个返回值有正有负

	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage->GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++) {
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage->SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[] rgbquadColorTable;
	}

	for (int nRow = 0; nRow < nHeight; nRow++) {
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++) {
			if (1 == nChannels) {
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels) {
				for (int nCha = 0; nCha < 3; nCha++) {
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}

void ImageProcess::CImageToMat(const CImage* cimage, Mat& mat) {
	if (true == cimage->IsNull()) {
		return;
	}

	int nChannels = cimage->GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels)) {
		return;
	}
	int nWidth = cimage->GetWidth();
	int nHeight = cimage->GetHeight();

	//重建mat
	if (1 == nChannels) {
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels) {
		mat.create(nHeight, nWidth, CV_8UC3);
	}

	//拷贝数据
	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage->GetBits();		//指向数据区的指针
	int nStep = cimage->GetPitch();					//每行的字节数,注意这个返回值有正有负

	for (int nRow = 0; nRow < nHeight; nRow++) {
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++) {
			if (1 == nChannels) {
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels) {
				for (int nCha = 0; nCha < 3; nCha++) {
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

static void calculateCorners(const Mat& H, const Mat& src, CORNERS& corners) {
	double v2[] = { 0, 0, 1 };// 左上角
	double v1[3];// 变换后的坐标值
	Mat V2 = Mat(3, 1, CV_64FC1, v2);  // 列向量
	Mat V1 = Mat(3, 1, CV_64FC1, v1);  // 列向量

	V1 = H * V2;
	// 左上角(0,0,1)
	//cout << "V2: " << V2 << endl;
	//cout << "V1: " << V1 << endl;
	corners.left_top.x = v1[0] / v1[2];
	corners.left_top.y = v1[1] / v1[2];

	// 左下角(0,src.rows,1)
	v2[0] = 0;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  // 列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  // 列向量
	V1 = H * V2;
	corners.left_bottom.x = v1[0] / v1[2];
	corners.left_bottom.y = v1[1] / v1[2];

	// 右上角(src.cols,0,1)
	v2[0] = src.cols;
	v2[1] = 0;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  // 列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  // 列向量
	V1 = H * V2;
	corners.right_top.x = v1[0] / v1[2];
	corners.right_top.y = v1[1] / v1[2];

	// 右下角(src.cols,src.rows,1)
	v2[0] = src.cols;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  // 列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  // 列向量
	V1 = H * V2;
	corners.right_bottom.x = v1[0] / v1[2];
	corners.right_bottom.y = v1[1] / v1[2];

}
void ImageProcess::stitch(string a, string b, CImage* r) {
	// 读取需要拼接的两张图片
	CORNERS corners;

	Mat image_original_1 = imread(b, 1);// 右图
	Mat image_original_2 = imread(a, 1);// 左图
	

	// 转换为灰度图
	Mat image_gray_1, image_gray_2;
	cvtColor(image_original_1, image_gray_1, COLOR_RGB2GRAY);
	cvtColor(image_original_2, image_gray_2, COLOR_RGB2GRAY);


	// 提取特征点
	Ptr<Feature2D> surf = xfeatures2d::SURF::create(2000);
	static vector<KeyPoint> key_point_1, key_point_2;
	surf->detect(image_gray_1, key_point_1);
	surf->detect(image_gray_2, key_point_2);

	// 特征点描述，为下边的特征点匹配做准备
	Mat image_desc_1, image_desc_2;
	surf->compute(image_gray_1, key_point_1, image_desc_1);
	surf->compute(image_gray_2, key_point_2, image_desc_2);

	// BFMatcher matcher;
	FlannBasedMatcher matcher;
	static vector<vector<DMatch> > matche_points;
	static vector<DMatch> good_matche_points;

	static vector<Mat> train_desc(1, image_desc_1);
	matcher.add(train_desc);
	matcher.train();

	matcher.knnMatch(image_desc_2, matche_points, 3);

	// 使用 Lowe's algorithm, 获取好的匹配点
	for (int i = 0; i < matche_points.size(); i++) {
		if (matche_points[i][0].distance < 0.4 * matche_points[i][1].distance) {
			good_matche_points.push_back(matche_points[i][0]);
		}
	}

	Mat match_1;
	drawMatches(image_original_2, key_point_2, image_original_1,
		key_point_1, good_matche_points, match_1);
	//imshow("match_1 ", match_1);

	static vector<Point2f> image_points_1, image_points_2;

	for (int i = 0; i < good_matche_points.size(); i++) {
		image_points_2.push_back(key_point_2[good_matche_points[i].queryIdx].pt);
		image_points_1.push_back(key_point_1[good_matche_points[i].trainIdx].pt);
	}


	// 获取图像1到图像2的投影映射矩阵 尺寸为3*3   寻找从右边到左边的矩阵变换
	Mat homo = findHomography(image_points_1, image_points_2, FM_RANSAC);

	// 计算配准图的四个顶点坐标
	calculateCorners(homo, image_original_1, corners);

	//图像配准
	Mat image_transform_1, image_transform_2;
	//右边转到左边  矩阵变换
	warpPerspective(image_original_1, image_transform_1, homo, Size(MAX(corners.right_top.x,
		corners.right_bottom.x), image_original_2.rows));
	//imshow("image_transform_1", image_transform_1);


	// 创建拼接后的图,需提前计算图的大小
	int dst_width = image_transform_1.cols;  // 取最右点的长度为拼接图的长度
	int dst_height = image_original_2.rows;

	Mat dst(dst_height, dst_width, CV_8UC3);
	dst.setTo(0);

	image_transform_1.copyTo(dst(Rect(0, 0, image_transform_1.cols, image_transform_1.rows)));
	image_original_2.copyTo(dst(Rect(0, 0, image_original_2.cols, image_original_2.rows)));

	//imshow("dst", dst);
	MatToCImage(dst, r);
}

void ImageProcess::SIFTstitch(string a, string b, CImage* r) {
	// 读取需要拼接的两张图片
	CORNERS corners;

	Mat image_original_1 = imread(b, 1);// 右图
	Mat image_original_2 = imread(a, 1);// 左图


	// 转换为灰度图
	Mat image_gray_1, image_gray_2;
	cvtColor(image_original_1, image_gray_1, COLOR_RGB2GRAY);
	cvtColor(image_original_2, image_gray_2, COLOR_RGB2GRAY);


	// 提取特征点
	Ptr<Feature2D> sift = xfeatures2d::SIFT::create(2000);
	static vector<KeyPoint> key_point_1, key_point_2;
	sift->detect(image_gray_1, key_point_1);
	sift->detect(image_gray_2, key_point_2);

	// 特征点描述，为下边的特征点匹配做准备
	Mat image_desc_1, image_desc_2;
	sift->compute(image_gray_1, key_point_1, image_desc_1);
	sift->compute(image_gray_2, key_point_2, image_desc_2);

	// BFMatcher matcher;
	FlannBasedMatcher matcher;
	static vector<vector<DMatch> > matche_points;
	static vector<DMatch> good_matche_points;

	static vector<Mat> train_desc(1, image_desc_1);
	matcher.add(train_desc);
	matcher.train();

	matcher.knnMatch(image_desc_2, matche_points, 3);

	// 使用 Lowe's algorithm, 获取好的匹配点
	for (int i = 0; i < matche_points.size(); i++) {
		if (matche_points[i][0].distance < 0.4 * matche_points[i][1].distance) {
			good_matche_points.push_back(matche_points[i][0]);
		}
	}

	Mat match_1;
	drawMatches(image_original_2, key_point_2, image_original_1,
		key_point_1, good_matche_points, match_1);
	//imshow("match_1 ", match_1);

	static vector<Point2f> image_points_1, image_points_2;

	for (int i = 0; i < good_matche_points.size(); i++) {
		image_points_2.push_back(key_point_2[good_matche_points[i].queryIdx].pt);
		image_points_1.push_back(key_point_1[good_matche_points[i].trainIdx].pt);
	}


	// 获取图像1到图像2的投影映射矩阵 尺寸为3*3   寻找从右边到左边的矩阵变换
	Mat homo = findHomography(image_points_1, image_points_2, FM_RANSAC);

	// 计算配准图的四个顶点坐标
	calculateCorners(homo, image_original_1, corners);

	//图像配准
	Mat image_transform_1, image_transform_2;
	//右边转到左边  矩阵变换
	warpPerspective(image_original_1, image_transform_1, homo, Size(MAX(corners.right_top.x,
		corners.right_bottom.x), image_original_2.rows));
	//imshow("image_transform_1", image_transform_1);


	// 创建拼接后的图,需提前计算图的大小
	int dst_width = image_transform_1.cols;  // 取最右点的长度为拼接图的长度
	int dst_height = image_original_2.rows;

	Mat dst(dst_height, dst_width, CV_8UC3);
	dst.setTo(0);

	image_transform_1.copyTo(dst(Rect(0, 0, image_transform_1.cols, image_transform_1.rows)));
	image_original_2.copyTo(dst(Rect(0, 0, image_original_2.cols, image_original_2.rows)));

	//imshow("dst", dst);
	MatToCImage(dst, r);
}

void ImageProcess::image_detection(string cfgFile, string weight, string clsNames, string image_path)
{
	float confidenceThreshold = 0.25;//置信度
	string pro_dir = "./opencv-targetDetection/"; //yolov2-tiny目录

	// 加载网络模型
	dnn::Net net = readNetFromDarknet(cfgFile, weight);
	if (net.empty())
	{
		printf("Could not load net...\n");
		return;
	}
	// 加载分类信息
	vector<string> classNamesVec;
	ifstream classNamesFile(clsNames);
	if (classNamesFile.is_open())
	{
		string className = "";
		while (std::getline(classNamesFile, className))
			classNamesVec.push_back(className);
	}

	// 加载图像
	Mat frame = imread(image_path);
	Mat inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
	net.setInput(inputBlob, "data");

	// 进行目标检测
	Mat detectionMat = net.forward("detection_out");
	vector<double> layersTimings;
	double freq = getTickFrequency() / 1000;
	double time = net.getPerfProfile(layersTimings) / freq;
	ostringstream ss;
	ss << "detection time: " << time << " ms";
	putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));

	// 输出结果
	for (int i = 0; i < detectionMat.rows; i++)
	{
		const int probability_index = 5;
		const int probability_size = detectionMat.cols - probability_index;
		float* prob_array_ptr = &detectionMat.at<float>(i, probability_index);
		size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
		float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
		if (confidence > confidenceThreshold)
		{
			float x = detectionMat.at<float>(i, 0);
			float y = detectionMat.at<float>(i, 1);
			float width = detectionMat.at<float>(i, 2);
			float height = detectionMat.at<float>(i, 3);
			int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
			int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
			int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
			int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
			Rect object(xLeftBottom, yLeftBottom,
				xRightTop - xLeftBottom,
				yRightTop - yLeftBottom);
			rectangle(frame, object, Scalar(0, 0, 255), 2, 8);
			if (objectClass < classNamesVec.size())
			{
				ss.str("");
				ss << confidence;
				String conf(ss.str());
				String label = String(classNamesVec[objectClass]) + ": " + conf;
				int baseLine = 0;
				Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
				rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom),
					Size(labelSize.width, labelSize.height + baseLine)),
					Scalar(255, 255, 255), -1);
				putText(frame, label, Point(xLeftBottom, yLeftBottom + labelSize.height),
					FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
			}
		}
	}
	imshow("YOLO-Detections", frame);
	waitKey(0);
	return;
}