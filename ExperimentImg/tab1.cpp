// tab1.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab1.h"
#include "afxdialogex.h"
#include "ExperimentImgDlg.h"

// tab1 对话框

IMPLEMENT_DYNAMIC(tab1, CDialogEx)

tab1::tab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

tab1::~tab1()
{
}

void tab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
// CTabExamplesDlg 消息处理程序
BOOL tab1::OnInitDialog()
{
	CComboBox * cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION));
	cmb_function->InsertString(0, _T("基于分类器的随机阙特征检测"));
	cmb_function->SetCurSel(0);
	return TRUE;
}


BEGIN_MESSAGE_MAP(tab1, CDialogEx)
END_MESSAGE_MAP()


// tab1 消息处理程序
