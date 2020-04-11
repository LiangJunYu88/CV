// Dlg2.cpp: 实现文件
//

#include "pch.h"
#include "LAB1.h"
#include "Dlg2.h"
#include "afxdialogex.h"
using namespace cv;
using namespace std;

// Dlg2 对话框

IMPLEMENT_DYNAMIC(Dlg2, CDialogEx)

Dlg2::Dlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

Dlg2::~Dlg2()
{
}

void Dlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOADIMG, &Dlg2::OnBnClickedButtonLoadimg)
	ON_BN_CLICKED(IDC_BUTTON_LOADIMG2, &Dlg2::OnBnClickedButtonLoadimg2)
	ON_BN_CLICKED(IDC_BUTTON_SIFT, &Dlg2::OnBnClickedButtonSift)
	ON_BN_CLICKED(IDC_BUTTON_SURF, &Dlg2::OnBnClickedButtonSurf)
	ON_BN_CLICKED(IDC_BUTTON_BF, &Dlg2::OnBnClickedButtonBf)
	ON_BN_CLICKED(IDC_BUTTON_FLANN, &Dlg2::OnBnClickedButtonFlann)
	ON_BN_CLICKED(IDC_BUTTON_RANSAC, &Dlg2::OnBnClickedButtonRansac)
	ON_BN_CLICKED(IDC_BUTTON1, &Dlg2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Dlg2::OnBnClickedButton2)
END_MESSAGE_MAP()


// Dlg2 消息处理程序


void Dlg2::OnBnClickedButtonLoadimg()
{
	// TODO: 在此添加控件通知处理程序代码
	//load file
	CString m_strFilePath = _T("");
	LPCTSTR szFilter = _T("JPG(*jpg)|*jpg|BMP(*.bmp)|*.bmp|ALLSUPORTFILE(*.*)|*.*||");

	CFileDialog dlgFileOpenImg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);

	if (dlgFileOpenImg.DoModal() == IDOK)
	{
		m_strFilePath = dlgFileOpenImg.GetPathName();
		String path = CT2A(m_strFilePath.GetString());
		srcMat = imread(path);
		imshow("t1", srcMat);

	}
	else
	{
		return;
	}
}


void Dlg2::OnBnClickedButtonLoadimg2()
{
	// TODO: 在此添加控件通知处理程序代码
	//load file
	CString m_strFilePath = _T("");
	LPCTSTR szFilter = _T("JPG(*jpg)|*jpg|BMP(*.bmp)|*.bmp|ALLSUPORTFILE(*.*)|*.*||");

	CFileDialog dlgFileOpenImg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);

	if (dlgFileOpenImg.DoModal() == IDOK)
	{
		m_strFilePath = dlgFileOpenImg.GetPathName();
		String path = CT2A(m_strFilePath.GetString());
		destMat = imread(path);
		imshow("t2", destMat);

	}
	else
	{
		return;
	}
}


void Dlg2::OnBnClickedButtonSift()
{
	// TODO: 在此添加控件通知处理程序代码
	Ptr<xfeatures2d::SIFT>feature = xfeatures2d::SIFT::create(500); //创建SIFT特征类
	feature->detect(srcMat, keyPoints1);
	feature->detect(destMat, keyPoints2);
	feature->compute(srcMat, keyPoints1, description1);
	feature->compute(destMat, keyPoints2, description2);


	Mat temp1, temp2;
	drawKeypoints(srcMat, keyPoints1, temp1);
	drawKeypoints(destMat, keyPoints2, temp2);

	imshow("t1", temp1);
	imshow("t2", temp2);
	isSIFT = true;
}


void Dlg2::OnBnClickedButtonSurf()
{
	// TODO: 在此添加控件通知处理程序代码
	Ptr<xfeatures2d::SURF>feature = xfeatures2d::SURF::create(500); //创建SIFT特征类

	feature->detect(srcMat, keyPoints1);
	feature->detect(destMat, keyPoints2);
	feature->compute(srcMat, keyPoints1, description1);
	feature->compute(destMat, keyPoints2, description2);

	Mat temp1, temp2;
	drawKeypoints(srcMat, keyPoints1, temp1);
	drawKeypoints(destMat, keyPoints2, temp2);
	imshow("t1", temp1);
	imshow("t2", temp2);
	isSIFT = false;
}


void Dlg2::OnBnClickedButtonBf()
{
	// TODO: 在此添加控件通知处理程序代码
		vector<DMatch> matches;
		BFMatcher matcher;
		matcher.match(description1, description2, matches);

		Mat image_match;
		drawMatches(srcMat, keyPoints1, destMat, keyPoints2, matches, image_match, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
		imshow("BF", image_match);
}


void Dlg2::OnBnClickedButtonFlann()
{
	// TODO: 在此添加控件通知处理程序代码
		//matching
		FlannBasedMatcher matcher;
		vector<DMatch> matches;
		matcher.match(description1, description2, matches);
		double minDist = INT_MAX;
		for (int i = 0; i < description1.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < minDist)
			{
				minDist = dist;
			}
		}

		vector<DMatch> goodMatches;
		for (int i = 0; i < description1.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < max(3 * minDist, 0.02))
			{
				goodMatches.push_back(matches[i]);
			}
		}

		Mat image_match;
		drawMatches(srcMat, keyPoints1, destMat, keyPoints2, goodMatches, image_match, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
		imshow("FLANN", image_match);
	
}


void Dlg2::OnBnClickedButtonRansac()
{
	// TODO: 在此添加控件通知处理程序代码

		//matching
		FlannBasedMatcher matcher;
		vector<DMatch> matches;
		matcher.match(description1, description2, matches);
		double minDist = INT_MAX;
		for (int i = 0; i < description1.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < minDist)
			{
				minDist = dist;
			}
		}

		vector<DMatch> goodMatches;
		for (int i = 0; i < description1.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < max(3 * minDist, 0.02))
			{
				goodMatches.push_back(matches[i]);
			}
		}
		int count = goodMatches.size();
		if (count < 15)
		{
			cout << "没有足够多的监测点" << endl;
			return;
		}
		vector<KeyPoint> RAN_KEY1, RAN_KEY2;
		for (size_t i = 0; i < goodMatches.size(); i++)
		{
			RAN_KEY1.push_back(keyPoints1[goodMatches[i].queryIdx]);
			RAN_KEY2.push_back(keyPoints2[goodMatches[i].trainIdx]);
		}
		vector<Point2f>p1, p2;
		for (size_t i = 0; i < goodMatches.size(); i++)
		{
			p1.push_back(RAN_KEY1[i].pt);
			p2.push_back(RAN_KEY2[i].pt);
		}
		vector<uchar> inliersMask(p1.size());
		Mat Fundamental = findFundamentalMat(p1, p2, inliersMask, FM_RANSAC);
		vector<DMatch>inliers;
		for (size_t i = 0; i < inliersMask.size(); i++)
		{
			if (inliersMask[i])
			{
				inliers.push_back(goodMatches[i]);
			}
		}
		goodMatches.swap(inliers);
		Mat image_match;
		drawMatches(srcMat, keyPoints1, destMat, keyPoints2, goodMatches, image_match);


		imshow("RANSAC", image_match);

}


BOOL Dlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	isSIFT = true;


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Dlg2::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(description1, description2, matches);
	double minDist = INT_MAX;
	for (int i = 0; i < description1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < minDist)
		{
			minDist = dist;
		}
	}

	vector<DMatch> goodMatches;
	for (int i = 0; i < description1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < max(3 * minDist, 0.02))
		{
			goodMatches.push_back(matches[i]);
		}
	}
	int count = goodMatches.size();
	if (count < 15)
	{
		cout << "没有足够多的监测点" << endl;
		return;
	}
	vector<KeyPoint> RAN_KEY1, RAN_KEY2;
	for (size_t i = 0; i < goodMatches.size(); i++)
	{
		RAN_KEY1.push_back(keyPoints1[goodMatches[i].queryIdx]);
		RAN_KEY2.push_back(keyPoints2[goodMatches[i].trainIdx]);
	}
	vector<Point2f>p1, p2;
	for (size_t i = 0; i < goodMatches.size(); i++)
	{
		p1.push_back(RAN_KEY1[i].pt);
		p2.push_back(RAN_KEY2[i].pt);
	}
	Mat homography = findHomography(p2, p1, FM_RANSAC);
	Size size = srcMat.size();
	
	Mat image_match;
	image_match = Mat::zeros(size, CV_8UC3);
	warpPerspective(destMat, image_match, homography, size);

	imshow("几何矫正", image_match);

}

typedef struct
{
	Point2f left_top;
	Point2f left_bottom;
	Point2f right_top;
	Point2f right_bottom;
}four_corners_t;

four_corners_t corners;

void Dlg2::OptimizeSeam(Mat& img1, Mat& trans, Mat& dst)
{
	int start = MIN(corners.left_top.x, corners.left_bottom.x);//开始位置，即重叠区域的左边界  
	double processWidth = img1.cols - start;//重叠区域的宽度  
	int rows = dst.rows;
	int cols = img1.cols;	//注意，是列数*通道数
	double alpha = 1;		//img1中像素的权重  
	for (int i = 0; i < rows; i++)
	{
		uchar* p = img1.ptr<uchar>(i);  //获取第i行的首地址
		uchar* t = trans.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = start; j < cols; j++)
		{
			//如果遇到图像trans中无像素的黑点，则完全拷贝img1中的数据
			if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
			{
				alpha = 1;
			}
			else
			{
				//img1中像素的权重，与当前处理点距重叠区域左边界的距离成正比，实验证明，这种方法确实好  
				alpha = (processWidth - (j - start)) / processWidth;
			}
			d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);
		}
	}
}

void Dlg2::CalcCorners(const Mat& H, const Mat& src) {
	double v2[] = { 0, 0, 1 };//左上角
	double v1[3];//变换后的坐标值
	Mat V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	Mat V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;

	//左上角(0,0,1)
	cout << "V2: " << V2 << endl;
	cout << "V1: " << V1 << endl;
	corners.left_top.x = v1[0] / v1[2];
	corners.left_top.y = v1[1] / v1[2];

	//左下角(0,src.rows,1)
	v2[0] = 0;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.left_bottom.x = v1[0] / v1[2];
	corners.left_bottom.y = v1[1] / v1[2];

	//右上角(src.cols,0,1)
	v2[0] = src.cols;
	v2[1] = 0;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.right_top.x = v1[0] / v1[2];
	corners.right_top.y = v1[1] / v1[2];

	//右下角(src.cols,src.rows,1)
	v2[0] = src.cols;
	v2[1] = src.rows;
	v2[2] = 1;
	V2 = Mat(3, 1, CV_64FC1, v2);  //列向量
	V1 = Mat(3, 1, CV_64FC1, v1);  //列向量
	V1 = H * V2;
	corners.right_bottom.x = v1[0] / v1[2];
	corners.right_bottom.y = v1[1] / v1[2];

}
void Dlg2::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	BFMatcher matcher;         //实例化一个暴力匹配器 a=destMat  b = srcMat c description2 d = description1
	vector<DMatch> matches;    //DMatch是用来描述匹配好的一对特征点的类，包含这两个点之间的相关信息
							   //比如左图有个特征m，它和右图的特征点n最匹配，这个DMatch就记录它俩最匹配，并且还记录m和n的
							   //特征向量的距离和其他信息，这个距离在后面用来做筛选


	matcher.match(description1, description2, matches);             //匹配，数据来源是特征向量，结果存放在DMatch类型里面  

											  //sort函数对数据进行升序排列
	sort(matches.begin(), matches.end());     //筛选匹配点，根据match里面特征对的距离从小到大排序
	vector<DMatch> good_matches;
	int ptsPairs = std::min(50, (int)(matches.size() * 0.15));
	cout << ptsPairs << endl;
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);//距离最小的50个压入新的DMatch
	}

	Mat outimg;                            //drawMatches这个函数直接画出实现特征点映射的图
	drawMatches(srcMat, keyPoints1, destMat, keyPoints2, good_matches, outimg, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);  //绘制匹配点  
	imshow("combine", outimg);

	//计算图像配准点
	vector<Point2f> imagePoints1, imagePoints2;

	for (int i = 0; i < good_matches.size(); i++)
	{
		imagePoints2.push_back(keyPoints1[good_matches[i].queryIdx].pt);
		imagePoints1.push_back(keyPoints2[good_matches[i].trainIdx].pt);
	}

	//获取图像1到图像2的投影映射矩阵 尺寸为3*3  
	Mat homo = findHomography(imagePoints1, imagePoints2, 8);//.c 文件中定义RANSAC方法的数字为8
	cout << "变换矩阵为：\n" << homo << endl << endl; //输出映射矩阵   

	//计算配准图的四个顶点坐标
	CalcCorners(homo, destMat);
	cout << "left_top:" << corners.left_top << endl;
	cout << "left_bottom:" << corners.left_bottom << endl;
	cout << "right_top:" << corners.right_top << endl;
	cout << "right_bottom:" << corners.right_bottom << endl;

	//图像配准  
	Mat imageTransform1, imageTransform2;
	warpPerspective(destMat, imageTransform1, homo, Size(MAX(corners.right_top.x, corners.right_bottom.x), srcMat.rows));
	//展示配准的重叠部分
	imshow("orb_trans", imageTransform1);

	//创建拼接后的图,需提前计算图的大小
	int dst_width = imageTransform1.cols;  //取最右点的长度为拼接图的长度
	int dst_height = srcMat.rows;
	Mat dst(dst_height, dst_width, CV_8UC3);
	dst.setTo(0);

	imageTransform1.copyTo(dst(Rect(0, 0, imageTransform1.cols, imageTransform1.rows)));
	srcMat.copyTo(dst(Rect(0, 0, srcMat.cols, srcMat.rows)));


	OptimizeSeam(srcMat, imageTransform1, dst);

	//展示优化后的拼接图
	imshow("opm_surf_result", dst);


}

