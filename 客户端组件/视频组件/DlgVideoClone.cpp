#include "stdafx.h"
#include "DlgVideoClone.h"
#include "resource.h"
#include "VideoServiceModule.h"

BEGIN_MESSAGE_MAP(CDlgVideoClone, CDialog)
END_MESSAGE_MAP()
IMPLEMENT_DYNAMIC(CDlgVideoClone, CDialog)


//构造函数
CDlgVideoClone::CDlgVideoClone(CWnd* pParent)
	: CDialog(IDD_VIDEO_CLONE, pParent)
{
	AfxSetResourceHandle(GetModuleHandle(VIDEO_SERVICE_DLL_NAME));
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GAME_VIDEO);
	AfxSetResourceHandle(GetModuleHandle(NULL));
}

//析构函数
CDlgVideoClone::~CDlgVideoClone()
{
}

//数据绑定
void CDlgVideoClone::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//绘制视频
void CDlgVideoClone::DrawUserVideo(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, BITMAPINFOHEADER bmiHeader)
{
	//构建位图
	BITMAPINFOHEADER  *lpbmi = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER)+dwLen);  
	ZeroMemory(lpbmi,sizeof(BITMAPINFOHEADER)+dwLen);
    memcpy((LPBYTE)lpbmi,&bmiHeader,sizeof(bmiHeader));
	memcpy(((LPBYTE)lpbmi+sizeof(BITMAPINFOHEADER)),lpBuf,dwLen);  

	HDC hdc=::GetDC(m_hWnd);
	HDC dcMem = ::CreateCompatibleDC(hdc);  
	HBITMAP hBitmap = CreateDIBitmap(hdc,lpbmi,CBM_INIT,(LPBYTE)lpbmi+sizeof(BITMAPINFOHEADER),(LPBITMAPINFO)lpbmi,DIB_RGB_COLORS);  

	//设置位置
	CRect rcVideo;
	GetClientRect(rcVideo);

	//绘图位图 
	HBITMAP holdbm = (HBITMAP)SelectObject(dcMem,hBitmap);  
	::SetStretchBltMode(hdc,COLORONCOLOR);
	::StretchBlt(hdc,rcVideo.left,rcVideo.top,rcVideo.Width(),rcVideo.Height(),dcMem,0,0,bmiHeader.biWidth,bmiHeader.biHeight,SRCCOPY);

	//释放资源
	SelectObject(dcMem,holdbm);  
	DeleteObject(dcMem);
	DeleteObject(hBitmap);
	DeleteObject(hdc);
	free(lpbmi);
	lpbmi = NULL;
}


BOOL CDlgVideoClone::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置图标
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return TRUE;
}
