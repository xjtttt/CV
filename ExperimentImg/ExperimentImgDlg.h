
// ExperimentImgDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ImageProcess.h"
#include "tab1.h"
#include "tab2.h"
#include "tab3.h"
#include "tab4.h"
#include "tab5.h"

#define MAX_THREAD 8
#define MAX_SPAN 15
struct DrawPara
{
	CImage* pImgSrc;
	CDC* pDC;
	int oriX;
	int oriY;
	int width;
	int height;
};

// CExperimentImgDlg 对话框
class CExperimentImgDlg : public CDialogEx
{
// 构造
public:
	CExperimentImgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPERIMENTIMG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CImage* getImage() { return m_pImgSrc; }
	
	void ThreadDraw(DrawPara *p);
	static UINT Update(void* p);
	void ImageCopy(CImage* pImgSrc, CImage* pImgDrt);
	void init_buf(int *buf, int len);
	size_t RoundUp(int groupSize, int globalSize);
	char * LoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength);

	afx_msg LRESULT OnVerify_FERNSMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSIFTmappingMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSURFmappingMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnstitchingMsgReceived(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OndetectingMsgReceived(WPARAM wParam, LPARAM lParam);



// 实现
protected:
	HICON m_hIcon;
	CImage * m_pImgSrc;
	CImage * m_pImgSrc2;
	CImage * m_pImgResult;
	clock_t startTime;
//	ThreadParam * m_pThreadParam;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	CEdit mEditInfo;
	CStatic mPictureControl;
	afx_msg void OnBnClickedButtonProcess();
	CButton m_CheckCirculation;
	CStatic mPictureControl2;
	CStatic mResult;

	CEdit output;

	CTabCtrl tab;
	tab1 mtab1;
	tab2 mtab2;
	tab3 mtab3;
	tab4 mtab4;
	tab5 mtab5;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	
	
	void verify_FERNS();
	void SIFTmapping();
	void SURFmapping();
	void stitching();
	void detecting();
	void ORB();
	afx_msg void OnStnClickedPicture2();


	void Output(const CString &str);
	afx_msg void OnStnClickedPicture();
	afx_msg void OnStnClickedPicture3();
	afx_msg void OnBnClickedButton2();
	
};
