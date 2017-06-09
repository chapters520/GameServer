#include "StdAfx.h"
#include "Resource.h"
#include ".\dlgmatchawards.h"


BEGIN_MESSAGE_MAP(CDlgMatchAwards, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


CDlgMatchAwards::CDlgMatchAwards(void):CDialog(IDD_DLG_MATCH_AWARDS)
{
	m_pMatchResult=NULL;

}

CDlgMatchAwards::~CDlgMatchAwards(void)
{

}


void CDlgMatchAwards::OnPaint()
{
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//变量定义
	CPngImage ImageBack;

	//加载图片
	ImageBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	ImageBack.DrawImage(&dc, 0,0);

	//创建字体
	CFont DrawFont;
	DrawFont.CreateFont(20,0,0,0,300,0,0,0,134,3,2,1,2,TEXT("黑体"));
	CFont * pOldFont=pBufferDC->SelectObject(&DrawFont);
	pBufferDC->SetTextColor(RGB(218,31,23));

	//变量定义
	CPngImage ImageAwards;

	//加载图片
	ImageAwards.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//计算位置
	INT nXPos=(rcClient.Width()-ImageAwards.GetWidth())/2;
	INT nYPos=(rcClient.Height()-ImageAwards.GetHeight())/2;

	//绘画奖励
	if(m_pMatchResult!=NULL)
	{
		ImageAwards.DrawImage(pBufferDC,nXPos,nYPos);
		pBufferDC->DrawText(m_pMatchResult->szDescribe, &CRect(nXPos+60,nYPos+115,nXPos+382,nYPos+195),DT_TOP|DT_WORDBREAK);

		pBufferDC->SelectObject(pOldFont);
		pBufferDC->SetTextColor(RGB(0,0,0));

		int nIndex=0;
		CString strText;

		//游戏币奖励
		if(m_pMatchResult->dwGold>0)
		{
			strText.Format(TEXT("◆ %d游戏币"), m_pMatchResult->dwGold);
			pBufferDC->DrawText(strText, &CRect(nXPos+60,nYPos+205,nXPos+360,nYPos+230),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//奖牌奖励
		if(m_pMatchResult->dwMedal>0)
		{
			strText.Format(TEXT("◆ %d奖牌"), m_pMatchResult->dwMedal);
			pBufferDC->DrawText(strText, &CRect(nXPos+60,nYPos+205+nIndex*30,nXPos+360,nYPos+230+nIndex*30),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//经验奖励
		if(m_pMatchResult->dwExperience>0)
		{
			strText.Format(TEXT("◆ %d经验"), m_pMatchResult->dwExperience);
			pBufferDC->DrawText(strText, &CRect(nXPos+60,nYPos+205+nIndex*30,nXPos+360,nYPos+230+nIndex*30),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//清理资源
		DrawFont.DeleteObject();
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);
}

void CDlgMatchAwards::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDOK, m_btCloseAwards);

	CWnd::DoDataExchange(pDX);
}

BOOL CDlgMatchAwards::OnInitDialog()
{
	CDialog::OnInitDialog();

	//变量定义
	CPngImage ImageBack;

	//加载图片
	ImageBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));
	m_btCloseAwards.SetButtonImage(IDB_BT_CLOSE_AWARDS,GetModuleHandle(GAME_FRAME_DLL_NAME),false,false);
	m_btCloseAwards.SetWindowPos(NULL,ImageBack.GetWidth()-55,30,27,22,SWP_NOZORDER);

    SetWindowPos(NULL, 0,0,ImageBack.GetWidth(),ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);
	CenterWindow();

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.top=20;
	rcUnLayered.left=20;
	rcUnLayered.right=rcWindow.Width()-20;
	rcUnLayered.bottom=rcWindow.Height()-20;

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,7,7);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(this,rcWindow);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(7,7),false);

	return TRUE; 
}

void CDlgMatchAwards::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//显示分层
	if (m_SkinLayered.m_hWnd!=NULL)
	{
		m_SkinLayered.ShowWindow((bShow==FALSE)?SW_HIDE:SW_SHOW);
	}

	return;
}

void CDlgMatchAwards::OnWindowPosChanged(WINDOWPOS* lpWndPos)
{
	CDialog::OnWindowPosChanged(lpWndPos);

	//移动分层
	if ((m_SkinLayered.m_hWnd!=NULL)&&(lpWndPos->cx>=0)&&(lpWndPos->cy>0))
	{
		m_SkinLayered.SetWindowPos(NULL,lpWndPos->x,lpWndPos->y,lpWndPos->cx,lpWndPos->cy,SWP_NOZORDER);
	}
}



//确定函数
VOID CDlgMatchAwards::OnOK()
{
	ShowWindow(SW_HIDE);
}