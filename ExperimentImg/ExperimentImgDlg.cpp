
// ExperimentImgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "ExperimentImgDlg.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <CL/cl.h>
#include <time.h>
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExperimentImgDlg �Ի���



CExperimentImgDlg::CExperimentImgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPERIMENTIMG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//���ضԻ����ʱ���ʼ��
	m_pImgSrc = NULL;
	m_pImgSrc2 = NULL;
	m_pImgResult = NULL;
	srand(time(0));
}

void CExperimentImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_EDIT_INFO, mEditInfo);
	DDX_Control(pDX, IDC_PICTURE, mPictureControl);
	DDX_Control(pDX, IDC_PICTURE2, mPictureControl2);
	DDX_Control(pDX, IDC_EDIT, output);
	DDX_Control(pDX, IDC_TAB, tab);
	DDX_Control(pDX, IDC_PICTURE3, mResult);
}

BEGIN_MESSAGE_MAP(CExperimentImgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CExperimentImgDlg::OnBnClickedButtonOpen)
//	ON_EN_CHANGE(IDC_EDIT1, &CExperimentImgDlg::OnEnChangeEdit1)
//	ON_EN_CHANGE(IDC_EDIT_INFO, &CExperimentImgDlg::OnEnChangeEditInfo)
ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CExperimentImgDlg::OnBnClickedButtonProcess)

/*ON_MESSAGE(, &CExperimentImgDlg::OnVerify_FERNSMsgReceived)
ON_MESSAGE(, &CExperimentImgDlg::OnSIFTmappingMsgReceived)
ON_MESSAGE(, &CExperimentImgDlg::OnSURFmappingMsgReceived)
ON_MESSAGE(, &CExperimentImgDlg::OnstitchingMsgReceived)
ON_MESSAGE(, &CExperimentImgDlg::OndetectingMsgReceived)*/


ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CExperimentImgDlg::OnTcnSelchangeTab)
ON_BN_CLICKED(IDC_BUTTON1, &CExperimentImgDlg::OnBnClickedButton1)
ON_STN_CLICKED(IDC_PICTURE2, &CExperimentImgDlg::OnStnClickedPicture2)
ON_STN_CLICKED(IDC_PICTURE, &CExperimentImgDlg::OnStnClickedPicture)
ON_STN_CLICKED(IDC_PICTURE3, &CExperimentImgDlg::OnStnClickedPicture3)
ON_BN_CLICKED(IDC_BUTTON2, &CExperimentImgDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CExperimentImgDlg ��Ϣ�������

BOOL CExperimentImgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ΪTab�ؼ������ǩ
	tab.InsertItem(0, _T("��֤ FERNS"));
	tab.InsertItem(1, _T("���ӳ��"));
	tab.InsertItem(2, _T("ͼ��ƴ��"));
	tab.InsertItem(3, _T("Ŀ����"));
	tab.InsertItem(4, _T("��Ƶ��ʵʱ�������"));
	// �����ӶԻ���
	mtab1.Create(IDD_DIALOG1, &tab);
	mtab2.Create(IDD_DIALOG2, &tab);
	mtab3.Create(IDD_DIALOG3, &tab);
	mtab4.Create(IDD_DIALOG4, &tab);
	mtab5.Create(IDD_DIALOG5, &tab);

	// �����ӶԻ����С��λ��
	CRect rc;
	tab.GetClientRect(&rc);
	CRect rcTabItem;
	tab.GetItemRect(0, rcTabItem);
	rc.top += rcTabItem.Height() + 4;
	rc.left += 4;
	rc.bottom -= 4;
	rc.right -= 4;
	mtab1.MoveWindow(&rc);
	mtab2.MoveWindow(&rc);
	mtab3.MoveWindow(&rc);
	mtab4.MoveWindow(&rc);
	mtab5.MoveWindow(&rc);

	// Ĭ�ϱ�ǩѡ��
	mtab1.ShowWindow(SW_SHOW);
	tab.SetCurFocus(0);

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
//	mEditInfo.SetWindowTextW(CString("File Path"));



	AfxBeginThread((AFX_THREADPROC)&CExperimentImgDlg::Update, this);

	ShowWindow(SW_MAXIMIZE);//�Ի���Ĭ����󻯵���
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExperimentImgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExperimentImgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (m_pImgSrc != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			height = m_pImgSrc->GetHeight();
			width = m_pImgSrc->GetWidth();

			mPictureControl.GetClientRect(&rect);
			CDC *pDC = mPictureControl.GetDC();

			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Height())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				
				m_pImgSrc->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex*0.98, (int)height*ScaleIndex*0.98));
				//��picture control������ͼ�����ź�Ĵ�С
//				CWnd *pWnd;
//				pWnd = GetDlgItem(IDC_PICTURE);
//				pWnd->MoveWindow(CRect((int)rect.top, (int)rect.left, (int)width*ScaleIndex, (int)height*ScaleIndex));
				m_pImgSrc->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
				//m_pImgSrc->StretchBlt(pDC2->m_hDC, rect1, SRCCOPY);
			}
			ReleaseDC(pDC);

		}
		if (m_pImgSrc2 != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			height = m_pImgSrc2->GetHeight();
			width = m_pImgSrc2->GetWidth();

			mPictureControl2.GetClientRect(&rect);
			CDC *pDC = mPictureControl2.GetDC();

			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Height())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));

				m_pImgSrc2->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex*0.98, (int)height*ScaleIndex*0.98));
				//��picture control������ͼ�����ź�Ĵ�С
//				CWnd *pWnd;
//				pWnd = GetDlgItem(IDC_PICTURE);
//				pWnd->MoveWindow(CRect((int)rect.top, (int)rect.left, (int)width*ScaleIndex, (int)height*ScaleIndex));
				m_pImgSrc2->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
				//m_pImgSrc->StretchBlt(pDC2->m_hDC, rect1, SRCCOPY);
			}
			ReleaseDC(pDC);

		}
		if (m_pImgResult != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			height = m_pImgResult->GetHeight();
			width = m_pImgResult->GetWidth();

			mResult.GetClientRect(&rect);
			CDC *pDC = mResult.GetDC();

			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Height())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));

				m_pImgResult->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex*0.98, (int)height*ScaleIndex*0.98));

				m_pImgResult->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			
			}
			ReleaseDC(pDC);

		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExperimentImgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CExperimentImgDlg::Update(void* p)
{
	while (1)
	{
		Sleep(200);
		CExperimentImgDlg* dlg = (CExperimentImgDlg*)p;
		if (dlg->m_pImgSrc != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			height = dlg->m_pImgSrc->GetHeight();
			width = dlg->m_pImgSrc->GetWidth();
		
			
			dlg->mPictureControl.GetClientRect(&rect);
			CDC *pDC = dlg->mPictureControl.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Height())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				dlg->m_pImgSrc->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex*0.98, (int)height*ScaleIndex*0.98));
				dlg->m_pImgSrc->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			dlg->ReleaseDC(pDC);
		}
		if (dlg->m_pImgSrc2 != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			height = dlg->m_pImgSrc2->GetHeight();
			width = dlg->m_pImgSrc2->GetWidth();


			dlg->mPictureControl2.GetClientRect(&rect);
			CDC *pDC = dlg->mPictureControl2.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Height())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				dlg->m_pImgSrc2->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex*0.98, (int)height*ScaleIndex*0.98));
				dlg->m_pImgSrc2->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			dlg->ReleaseDC(pDC);
		}

		if (dlg->m_pImgResult != NULL)
		{
			int height;
			int width;
			CRect rect;
			CRect rect1;
			height = dlg->m_pImgResult->GetHeight();
			width = dlg->m_pImgResult->GetWidth();


			dlg->mResult.GetClientRect(&rect);
			CDC *pDC = dlg->mResult.GetDC();
			SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

			if (width <= rect.Width() && height <= rect.Height())
			{
				rect1 = CRect(rect.TopLeft(), CSize(width, height));
				dlg->m_pImgResult->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			else
			{
				float xScale = (float)rect.Width() / (float)width;
				float yScale = (float)rect.Height() / (float)height;
				float ScaleIndex = (xScale <= yScale ? xScale : yScale);
				rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex*0.98, (int)height*ScaleIndex*0.98));
				dlg->m_pImgResult->StretchBlt(pDC->m_hDC, rect1, SRCCOPY);
			}
			dlg->ReleaseDC(pDC);
		}
	}
	return 0;
}

void CExperimentImgDlg::ThreadDraw(DrawPara *p)
{
	CRect rect;
	GetClientRect(&rect);
	CDC     memDC;             // ���ڻ����ͼ���ڴ滭��  
	CBitmap memBitmap;         // ���ڻ����ͼ���ڴ滭��
	memDC.CreateCompatibleDC(p->pDC);  // ������ԭ���ʼ��ݵĻ���
	memBitmap.CreateCompatibleBitmap(p->pDC, p->width, p->height);  // ������ԭλͼ���ݵ��ڴ滭��
	memDC.SelectObject(&memBitmap);      // ���������뻭���Ĺ���
	memDC.FillSolidRect(rect, p->pDC->GetBkColor());
	p->pDC->SetStretchBltMode(HALFTONE);
	// ��pImgSrc���������Ż����ڴ滭����
	p->pImgSrc->StretchBlt(memDC.m_hDC, 0, 0, p->width, p->height);

	// ���ѻ��õĻ������Ƶ������Ļ�������
	p->pDC->BitBlt(p->oriX, p->oriY, p->width, p->height, &memDC, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

void CExperimentImgDlg::ImageCopy(CImage* pImgSrc, CImage* pImgDrt)
{
	int MaxColors = pImgSrc->GetMaxColorTableEntries();
	RGBQUAD* ColorTab;
	ColorTab = new RGBQUAD[MaxColors];

	CDC *pDCsrc, *pDCdrc;
	if (!pImgDrt->IsNull())
	{
		pImgDrt->Destroy();
	}
	pImgDrt->Create(pImgSrc->GetWidth(), pImgSrc->GetHeight(), pImgSrc->GetBPP(), 0);

	if (pImgSrc->IsIndexed())
	{
		pImgSrc->GetColorTable(0, MaxColors, ColorTab);
		pImgDrt->SetColorTable(0, MaxColors, ColorTab);
	}

	pDCsrc = CDC::FromHandle(pImgSrc->GetDC());
	pDCdrc = CDC::FromHandle(pImgDrt->GetDC());
	pDCdrc->BitBlt(0, 0, pImgSrc->GetWidth(), pImgSrc->GetHeight(), pDCsrc, 0, 0, SRCCOPY);
	pImgSrc->ReleaseDC();
	pImgDrt->ReleaseDC();
	delete ColorTab;

}

void CExperimentImgDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("JPEG(*jpg)|*.jpg|*.bmp|*.png|TIFF(*.tif)|*.tif|All Files ��*.*��|*.*||");
	CString filePath("");
	
	CFileDialog fileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDialog.DoModal() == IDOK)
	{
		VERIFY(filePath = fileOpenDialog.GetPathName());
		CString strFilePath(filePath);
//		mEditInfo.SetWindowTextW(strFilePath);	//���ı�������ʾͼ��·��

		if (m_pImgSrc != NULL)
		{
			m_pImgSrc->Destroy();
			delete m_pImgSrc;
		}
		m_pImgSrc = new CImage;
		m_pImgSrc->Load(strFilePath);
		this->Invalidate();
		Output(_T("����ͼƬA��")+filePath);
	}
}


double pp;
double junzhi;
double fangcha;
void CExperimentImgDlg::OnBnClickedButtonProcess()
{
	CComboBox* cmb1 = ((CComboBox*)mtab1.GetDlgItem(IDC_COMBO_FUNCTION));
	int func1 = cmb1->GetCurSel();
	// CComboBox* cmb2 = ((CComboBox*)mtab2.GetDlgItem(IDC_COMBO1));
	// int func2 = cmb2->GetCurSel();
	CComboBox* cmb3 = ((CComboBox*)mtab3.GetDlgItem(IDC_COMBO1));
	int func3 = cmb3->GetCurSel();
	CComboBox* cmb4 = ((CComboBox*)mtab4.GetDlgItem(IDC_COMBO1));
	int func4 = cmb4->GetCurSel();


	CString str;
	CString str2;
	CEdit* edit;
	CEdit* edit2;
	CStatic* pic = ((CStatic*)GetDlgItem(IDC_PICTURE));
	
	CRect IRect;
	pic->GetClientRect(&IRect);
	pic->GetDC()->FillSolidRect(IRect.left+1, IRect.top+1, IRect.Width()-2, IRect.Height()-2, RGB(240, 240, 240));
	switch (tab.GetCurSel())
	{
	case 0:
		switch (func1)
		{
		case 0:  //��֤ FERNS
			verify_FERNS();
			break;
		
		default:
			break;
		}

		break;
	case 1:
		RemappingAndCorrection();
		break;
	case 2:
		switch (func3)
		{
		case 0:  //SURFͼ��ƴ��
			stitching();
			break;

		case 1: 
			
			break;
		default:
			break;
		}
		break;
	case 3:
		switch (func4)
		{
		case 0:  //XXX����Ŀ����
			detecting();
			break;

		default:
			break;
		}
		break;

	case 4:
		//��Ƶ���


		break;
	default:
		break;
	}
	
	
}


void CExperimentImgDlg::verify_FERNS()
{
	startTime = clock();
	
}


void CExperimentImgDlg::RemappingAndCorrection() {
	auto StartTime = GetTickCount64();

	CImage* temp = new CImage();
	mtab2.Run(m_pImgSrc, m_pImgSrc2, temp);

	auto EndTime = GetTickCount64();

	delete m_pImgResult;
	m_pImgResult = temp;

	CString msg=L"";
	msg += L"������ȡ����Ϊ ";
	msg += (mtab2.ComboSift.GetCurSel() == 0 ? L"SIFT" : L"SURF");
	msg += L" �� ";
	msg += (mtab2.ComboMethod.GetCurSel() == 0 ? L"���ӳ��" : L"���ν���");
	msg += L" ������� \r\n�� ";
	msg += (mtab2.ComboMatch.GetCurSel() == 0 ? L"BruteForce" : L"Flann");
	msg += L" ƥ�䷽ʽ \r\nͼ���СΪ ";
	
	CString str;
	mtab2.EditScale.GetWindowTextW(str);
	double scale = _wtof(str);

	CString tmp;
	tmp.Format(L"%d * %d \r\n��ʱ %d ms", int(scale * m_pImgSrc->GetWidth()),
		int(scale * m_pImgSrc->GetHeight()), int(EndTime - StartTime));

	Output(msg + tmp);
}


void CExperimentImgDlg::stitching()
{
	startTime = clock();
}


void CExperimentImgDlg::detecting()
{
	startTime = clock();
}


void CExperimentImgDlg::ORB()
{
	startTime = clock();
}






LRESULT CExperimentImgDlg::OnVerify_FERNSMsgReceived(WPARAM wParam, LPARAM lParam)
{
	static int tempThreadCount = 0;
	static int tempProcessCount = 0;
	if ((int)wParam == 1)
	{

		tempProcessCount = 0;
		clock_t endTime = clock();
		CString timeStr;
		timeStr.Format(_T("��֤FERNS��ʱ:%dms "), endTime - startTime);
		Output(timeStr);

	}
	return 0;
}

LRESULT CExperimentImgDlg::OnstitchingMsgReceived(WPARAM wParam, LPARAM lParam)
{
	static int tempThreadCount = 0;
	static int tempProcessCount = 0;
	if ((int)wParam == 1)
	{
		// �������̶߳�������ֵ1����ȫ������~��ʾʱ��

		tempProcessCount = 0;
		clock_t endTime = clock();
		CString timeStr;
		timeStr.Format(_T("ͼ��ƴ�Ӻ�ʱ:%dms "), endTime - startTime);
		Output(timeStr);


	}
	return 0;
}

LRESULT CExperimentImgDlg::OndetectingMsgReceived(WPARAM wParam, LPARAM lParam)
{
	static int tempThreadCount = 0;
	static int tempProcessCount = 0;
	if ((int)wParam == 1)
	{
		// �������̶߳�������ֵ1����ȫ������~��ʾʱ��

		tempProcessCount = 0;
		clock_t endTime = clock();
		CString timeStr;
		timeStr.Format(_T("��ֵ�˲�����, ��ʱ:%dms "), endTime - startTime);
		Output(timeStr);


	}
	return 0;
}









void CExperimentImgDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (tab.GetCurSel())
	{
	case 0:
		mtab1.ShowWindow(SW_SHOW);
		mtab2.ShowWindow(SW_HIDE);
		mtab3.ShowWindow(SW_HIDE);
		mtab4.ShowWindow(SW_HIDE);
		mtab5.ShowWindow(SW_HIDE);
		mtab1.SetFocus();
		break;
	case 1:
		mtab2.ShowWindow(SW_SHOW);
		mtab1.ShowWindow(SW_HIDE);
		mtab3.ShowWindow(SW_HIDE);
		mtab4.ShowWindow(SW_HIDE);
		mtab5.ShowWindow(SW_HIDE);
		mtab2.SetFocus();
		break;
	case 2:
		mtab3.ShowWindow(SW_SHOW);
		mtab1.ShowWindow(SW_HIDE);
		mtab2.ShowWindow(SW_HIDE);
		mtab4.ShowWindow(SW_HIDE);
		mtab5.ShowWindow(SW_HIDE);
		mtab3.SetFocus();
		break;
	case 3:
		mtab3.ShowWindow(SW_HIDE);
		mtab1.ShowWindow(SW_HIDE);
		mtab2.ShowWindow(SW_HIDE);
		mtab4.ShowWindow(SW_SHOW);
		mtab5.ShowWindow(SW_HIDE);
		mtab4.SetFocus();
		break;

	case 4:
		mtab3.ShowWindow(SW_HIDE);
		mtab1.ShowWindow(SW_HIDE);
		mtab2.ShowWindow(SW_HIDE);
		mtab4.ShowWindow(SW_HIDE);
		mtab5.ShowWindow(SW_SHOW);
		mtab5.SetFocus();
		break;
	default:
		break;
	}
	
	*pResult = 0;
}

void CExperimentImgDlg::Output(const CString &str)
{
	output.SetSel(output.GetWindowTextLengthW(), output.GetWindowTextLengthW(), FALSE);
	output.ReplaceSel(str + _T("\r\n"));
}

void CExperimentImgDlg::OnBnClickedButton1()
{
	output.SetWindowTextW(_T(""));
}

void CExperimentImgDlg::OnStnClickedPicture2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

char* CExperimentImgDlg::LoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength)
{
	FILE* pFileStream = NULL;
	size_t szSourceLength;

	// open the OpenCL source code file  
	pFileStream = fopen(cFilename, "rb");
	if (pFileStream == NULL)
	{
		return NULL;
	}

	size_t szPreambleLength = strlen(cPreamble);

	// get the length of the source code  
	fseek(pFileStream, 0, SEEK_END);
	szSourceLength = ftell(pFileStream);
	fseek(pFileStream, 0, SEEK_SET);

	// allocate a buffer for the source code string and read it in  
	char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1);
	memcpy(cSourceString, cPreamble, szPreambleLength);
	if (fread((cSourceString)+szPreambleLength, szSourceLength, 1, pFileStream) != 1)
	{
		fclose(pFileStream);
		free(cSourceString);
		return 0;
	}

	// close the file and return the total length of the combined (preamble + source) string  
	fclose(pFileStream);
	if (szFinalLength != 0)
	{
		*szFinalLength = szSourceLength + szPreambleLength;
	}
	cSourceString[szSourceLength + szPreambleLength] = '\0';

	return cSourceString;
}

void CExperimentImgDlg::init_buf(int *buf, int len)
{
	int i;
	for (int i = 0; i < len; i++)
	{
		buf[i] = i + 1;
	}
}

size_t CExperimentImgDlg::RoundUp(int groupSize, int globalSize)
{
	int r = globalSize % groupSize;
	if (r == 0)
	{
		return globalSize;
	}
	else
	{
		return globalSize + groupSize - r;
	}
}

void CExperimentImgDlg::OnStnClickedPicture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CExperimentImgDlg::OnStnClickedPicture3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CExperimentImgDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("JPEG(*jpg)|*.jpg|*.bmp|*.png|TIFF(*.tif)|*.tif|All Files ��*.*��|*.*||");
	CString filePath("");

	CFileDialog fileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDialog.DoModal() == IDOK)
	{
		VERIFY(filePath = fileOpenDialog.GetPathName());
		CString strFilePath(filePath);
		//		mEditInfo.SetWindowTextW(strFilePath);	//���ı�������ʾͼ��·��

		if (m_pImgSrc2 != NULL)
		{
			m_pImgSrc2->Destroy();
			delete m_pImgSrc2;
		}
		m_pImgSrc2 = new CImage;
		m_pImgSrc2->Load(strFilePath);
		this->Invalidate();
		Output(_T("����ͼƬB��") + filePath);
	}
}
