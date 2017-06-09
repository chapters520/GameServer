#include "StdAfx.h"
#include "Resource.h"
#include "GameOptionItem.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameOptionItem, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameOptionItem::CGameOptionItem()
{
	//设置变量
	m_ControlSize.SetSize(0,0);

	//加载资源
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageOptionLogo.LoadFromResource(hInstance,IDB_OPTION_LOGO);

	//获取大小
	m_LogoSize.SetSize(m_ImageOptionLogo.GetWidth(),m_ImageOptionLogo.GetHeight());

	return;
}

//析构函数
CGameOptionItem::~CGameOptionItem()
{
}

//接口查询
VOID * CGameOptionItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameOptionItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameOptionItem,Guid,dwQueryVer);
	return NULL;
}

//调整控件
VOID __cdecl CGameOptionItem::RectifyControl(INT nWidth, INT nHeight)
{
	//调整控件
	if ((m_ControlSize.cx!=nWidth)||(m_ControlSize.cy!=nHeight))
	{
		//变量定义
		CWnd * pWndChild=NULL;

		//查找窗口
		do
		{
			//查找窗口
			pWndChild=FindWindowEx(m_hWnd,pWndChild->GetSafeHwnd(),NULL,NULL);

			//移动窗口
			if (pWndChild!=NULL)
			{
				//获取位置
				CRect rcControl;
				pWndChild->GetWindowRect(&rcControl);

				//移动窗口
				ScreenToClient(&rcControl);
				pWndChild->SetWindowPos(NULL,rcControl.left+(nWidth-m_ControlSize.cx)/2,rcControl.top+(nHeight-m_ControlSize.cy)/2,0,0,SWP_NOSIZE|SWP_NOZORDER);
			}
		} while (pWndChild!=NULL);
	}

	return;
}

//绘画控件
VOID __cdecl CGameOptionItem::DrawControlFace(CDC * pDC, INT nWidth, INT nHeight)
{
	//绘画背景
	pDC->FillSolidRect(0,0,nWidth,nHeight,CSkinDialog::m_SkinAttribute.m_crBackGround);
	m_ImageOptionLogo.BitBlt(pDC->m_hDC,nWidth-m_ImageOptionLogo.GetWidth(),nHeight-m_ImageOptionLogo.GetHeight());

	return;
}

//确定消息
VOID CGameOptionItem::OnOK()
{
	//发送消息
	CWnd * pParentWnd=GetParent();
	pParentWnd->SendMessage(WM_COMMAND,IDOK,(LPARAM)(pParentWnd->GetSafeHwnd()));

	return;
}

//取消消息
VOID CGameOptionItem::OnCancel()
{
	//发送消息
	CWnd * pParentWnd=GetParent();
	pParentWnd->SendMessage(WM_COMMAND,IDCANCEL,(LPARAM)(pParentWnd->GetSafeHwnd()));

	return;
}

//绘画消息
VOID CGameOptionItem::OnPaint()
{
	//变量定义
	CPaintDC dc(this);

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画界面
	DrawControlFace(&dc,rcClient.Width(),rcClient.Height());

	return;
}

//绘画背景
BOOL CGameOptionItem::OnEraseBkgnd(CDC * pDC)
{
	//更新窗口
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//位置消息
VOID CGameOptionItem::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//调整位置
	RectifyControl(cx,cy);

	return;
}

//创建消息
BOOL CGameOptionItem::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);

	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//设置变量
	m_ControlSize.SetSize(rcWindow.Width()+m_LogoSize.cx,rcWindow.Height());

	return TRUE;
}

//控件颜色
HBRUSH CGameOptionItem::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(CSkinDialog::m_SkinAttribute.m_crBackGround);
			pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crControlText);
			return CSkinDialog::m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//////////////////////////////////////////////////////////////////////////
