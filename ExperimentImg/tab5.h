#pragma once

#include"video.h"

// tab5 对话框

class tab5 : public CDialogEx
{
	DECLARE_DYNAMIC(tab5)

public:
	tab5(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~tab5();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	video mvideo;
	afx_msg void OnBnClickedOk();
	CButton button;
};
