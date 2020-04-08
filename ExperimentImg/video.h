#pragma once


// video 对话框
struct DrawPara2
{
	CImage* pImgSrc;
	CDC* pDC;
	int oriX;
	int oriY;
	int width;
	int height;
};

class video : public CDialogEx
{
	DECLARE_DYNAMIC(video)

public:
	video(CWnd* pParent = nullptr);   // 标准构造函数

	virtual ~video();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEO };
#endif

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
	CImage* getImage() { return m_pImgSrc; }
	afx_msg void OnPaint();
	void ThreadDraw(DrawPara2 *p);
	static UINT Update(void* p);
	void ImageCopy(CImage* pImgSrc, CImage* pImgDrt);
	DECLARE_MESSAGE_MAP()
public:
	HICON m_hIcon;
	CStatic pic;
	CImage* m_pImgSrc;
	afx_msg void OnBnClickedButton1();
};
