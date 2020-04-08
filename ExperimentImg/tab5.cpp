// tab5.cpp: 实现文件
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "tab5.h"
#include "afxdialogex.h"


// tab5 对话框

IMPLEMENT_DYNAMIC(tab5, CDialogEx)

tab5::tab5(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

tab5::~tab5()
{
}

void tab5::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDOK, button);
}

BOOL tab5::OnInitDialog()
{
	mvideo.Create(IDD_VIDEO, this);
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(tab5, CDialogEx)
	ON_BN_CLICKED(IDOK, &tab5::OnBnClickedOk)
END_MESSAGE_MAP()


// tab5 消息处理程序



void tab5::OnBnClickedOk()
{
	
	mvideo.ShowWindow(SW_SHOW);


	//mvideo.SetFocus();
	// TODO: 在此添加控件通知处理程序代码
	
}
