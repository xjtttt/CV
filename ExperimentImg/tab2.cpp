// tab2.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab2.h"
#include "afxdialogex.h"


// tab2 对话框

IMPLEMENT_DYNAMIC(tab2, CDialogEx)

tab2::tab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

tab2::~tab2()
{
}

void tab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, tab2combo);
}

BOOL tab2::OnInitDialog()
{
	CComboBox * cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO1));;
	cmb_function->InsertString(0, _T("SIFT"));
	cmb_function->InsertString(1, _T("SURF"));
	cmb_function->SetCurSel(0);
	return TRUE;
}

BEGIN_MESSAGE_MAP(tab2, CDialogEx)
END_MESSAGE_MAP()


// tab2 消息处理程序
