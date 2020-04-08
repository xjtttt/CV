#pragma once


// tab1 对话框

class tab1 : public CDialogEx
{
	DECLARE_DYNAMIC(tab1)

public:
	tab1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~tab1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
