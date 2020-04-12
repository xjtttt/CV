// tab4.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab4.h"
#include "afxdialogex.h"


// tab4 对话框

IMPLEMENT_DYNAMIC(tab4, CDialogEx)

tab4::tab4(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

tab4::~tab4()
{
}

void tab4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, com4);
}

BOOL tab4::OnInitDialog()
{
	CComboBox * cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	cmb_function->InsertString(0, _T("yolo方法"));
	cmb_function->SetCurSel(0);
	return TRUE;
}

BEGIN_MESSAGE_MAP(tab4, CDialogEx)
END_MESSAGE_MAP()


// tab4 消息处理程序
