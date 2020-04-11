#pragma once


// Dlg2 对话框

class Dlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg2)

public:
	Dlg2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Dlg2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	cv::Mat srcMat;
	cv::Mat destMat;
	cv::Mat description1;
	cv::Mat description2;
	bool isSIFT;
	void CalcCorners(const cv::Mat& H, const cv::Mat& src);
	void OptimizeSeam(cv::Mat &img1,cv::Mat&trans, cv::Mat&dst);
	std::vector<cv::KeyPoint> keyPoints1, keyPoints2;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadimg();
	afx_msg void OnBnClickedButtonLoadimg2();
	afx_msg void OnBnClickedButtonSift();
	afx_msg void OnBnClickedButtonSurf();
	afx_msg void OnBnClickedButtonBf();
	afx_msg void OnBnClickedButtonFlann();
	afx_msg void OnBnClickedButtonRansac();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
