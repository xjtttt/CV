#pragma once


// tab4 对话框

class tab4 : public CDialogEx
{
	DECLARE_DYNAMIC(tab4)

public:
	tab4(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~tab4();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox com4;
};
