#include "StdAfx.h"
#include "Resource.h"
#include "PlatformPublicize.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPlatformPublicize::CPlatformPublicize()
{
	//设置变量
	m_crBoradColor=CLR_INVALID;
	ZeroMemory(m_szSpareURL,MAX_PATH);

	return;
}

//析构函数
CPlatformPublicize::~CPlatformPublicize()
{
}

//位置函数
VOID CPlatformPublicize::GetWebBrowserRect(CRect & rcWebBrowser)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置位置
	if (m_crBoradColor!=CLR_INVALID)
	{
		rcWebBrowser.top=1;
		rcWebBrowser.left=1;
		rcWebBrowser.right=rcClient.Width()-1;
		rcWebBrowser.bottom=rcClient.Height()-1;
	}
	else
	{
		rcWebBrowser.top=0;
		rcWebBrowser.left=0;
		rcWebBrowser.right=rcClient.Width();
		rcWebBrowser.bottom=rcClient.Height();
	}

	return;
}

//绘画界面
VOID CPlatformPublicize::DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus)
{
	//绘画背景
	pDC->FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));

	//加载资源
	CPngImage ImagePublicize;
	ImagePublicize.LoadImage(AfxGetInstanceHandle(),TEXT("LOAD_STATUS"));

	//绘画背景
	CSize SizePublicize;
	SizePublicize.SetSize(ImagePublicize.GetWidth(),ImagePublicize.GetHeight());
	ImagePublicize.DrawImage(pDC,(nWidth-SizePublicize.cx)/2,(nHeight-SizePublicize.cy)/2,SizePublicize.cx,SizePublicize.cy,0,0);

	//广告边框
	if (m_crBoradColor!=CLR_INVALID) pDC->Draw3dRect(0,0,nWidth,nHeight,m_crBoradColor,m_crBoradColor);

	return;
}

//设置边框
VOID CPlatformPublicize::SetBoradColor(COLORREF crBoradColor)
{
	//设置变量
	m_crBoradColor=crBoradColor;

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//状态通知
VOID CPlatformPublicize::OnBrowserStatus(BYTE cbStatus)
{
	if(cbStatus==BROWSER_STATUS_ERROR && m_szSpareURL[0]!='\0')
	{
		Navigate(m_szSpareURL);
		return ;
	}
	__super::OnBrowserStatus(cbStatus);
}

//备用网址
VOID CPlatformPublicize::SetSpareURL(LPCTSTR pszURL)
{
	_tcsncpy(m_szSpareURL,pszURL,MAX_PATH);
}
//////////////////////////////////////////////////////////////////////////////////
