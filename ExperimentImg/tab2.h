#pragma once


// tab2 对话框

class tab2 : public CDialogEx
{
	DECLARE_DYNAMIC(tab2)

public:
	tab2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~tab2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox tab2combo;
};
