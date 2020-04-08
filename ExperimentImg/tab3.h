#pragma once


// tab3 对话框

class tab3 : public CDialogEx
{
	DECLARE_DYNAMIC(tab3)

public:
	tab3(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~tab3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox com;
};
