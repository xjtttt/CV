// tab3.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab3.h"
#include "afxdialogex.h"


// tab3 对话框

IMPLEMENT_DYNAMIC(tab3, CDialogEx)

tab3::tab3(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

tab3::~tab3()
{
}

void tab3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, com);
}

BOOL tab3::OnInitDialog()
{
	CComboBox * cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	cmb_function->InsertString(0, _T("SURF"));
	cmb_function->InsertString(1, _T("SIFT"));
	cmb_function->SetCurSel(0);
	return TRUE;
}
BEGIN_MESSAGE_MAP(tab3, CDialogEx)
END_MESSAGE_MAP()


// tab3 消息处理程序
