#include "StdAfx.h"
#include "GameFrameAvatar.h"
#include "GameFrameViewGDI.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////
//时间定义
#define IDI_ROLL_TEXT				30									//滚动文字

#define IDC_BT_CLOSE_AWARDS			1000									//关闭按钮

BEGIN_MESSAGE_MAP(CGameFrameViewGDI, CGameFrameView)
	ON_WM_PAINT()
	ON_WM_TIMER()

	//按钮消息
	ON_BN_CLICKED(IDC_BT_CLOSE_AWARDS,OnBnClickedCloseAwards)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameViewGDI::CGameFrameViewGDI()
{
	//缓冲变量
	m_SizeBackSurface.SetSize(0,0);
	m_RectDirtySurface.SetRect(0,0,0,0);

	return;
}

//析构函数
CGameFrameViewGDI::~CGameFrameViewGDI()
{
	//销毁缓冲
	if (m_ImageBackSurface.IsNull()==false) m_ImageBackSurface.Destroy();
}

//重置界面
VOID CGameFrameViewGDI::ResetGameView()
{
	return;
}

//调整控件
VOID CGameFrameViewGDI::RectifyControl(INT nWidth, INT nHeight)
{
	return;
}

//界面更新
VOID CGameFrameViewGDI::InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//构造位置
	CRect rcInvalid;
	rcInvalid.SetRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);

	//位置调整
	if (rcInvalid.IsRectNull()==TRUE) GetClientRect(&rcInvalid);

	//设置矩形
	if (m_RectDirtySurface.IsRectEmpty()==FALSE)
	{
		//设置变量
		m_RectDirtySurface.top=__min(m_RectDirtySurface.top,rcInvalid.top);
		m_RectDirtySurface.left=__min(m_RectDirtySurface.left,rcInvalid.left);
		m_RectDirtySurface.right=__max(m_RectDirtySurface.right,rcInvalid.right);
		m_RectDirtySurface.bottom=__max(m_RectDirtySurface.bottom,rcInvalid.bottom);
	}
	else
	{
		//设置变量
		m_RectDirtySurface=rcInvalid;
	}

	//刷新窗口
	InvalidateRect(&rcInvalid,FALSE);

	return;
}

//绘画准备
VOID CGameFrameViewGDI::DrawUserReady(CDC * pDC, INT nXPos, INT nYPos)
{
	//WIN 7 执行
	if (RealizeWIN7() && CD2DEngine::GetD2DEngine())
	{
		//加载资源
		if(m_ImageUserReadyEx.IsNull())
		{
			if(!m_ImageUserReadyEx.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_READY"))) return;
		}

		//获取大小
		CSize SizeUserReady;
		SizeUserReady.SetSize(m_ImageUserReadyEx.GetWidth(),m_ImageUserReadyEx.GetHeight());

		m_ImageUserReadyEx.DrawImage(pDC,nXPos-SizeUserReady.cx/2,nYPos-SizeUserReady.cy/2);
	}
	else
	{
		//加载资源
		CPngImage ImageUserReady;
		ImageUserReady.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_READY"));

		//获取大小
		CSize SizeUserReady;
		SizeUserReady.SetSize(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());

		//绘画准备
		ImageUserReady.DrawImage(pDC,nXPos-SizeUserReady.cx/2,nYPos-SizeUserReady.cy/2);
	}

	return;
}

//绘画时间
VOID CGameFrameViewGDI::DrawUserClock(CDC * pDC, INT nXPos, INT nYPos, WORD wUserClock)
{
	//绘画时间
	if ((wUserClock>0)&&(wUserClock<100))
	{
		//WIN 7 执行
		if (RealizeWIN7() && CD2DEngine::GetD2DEngine())
		{
			//加载资源
			if(m_ImageClockItemEx.IsNull())
			{
				if(!m_ImageClockItemEx.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_ITEM")))return;
			}
			if(m_ImageClockBackEx.IsNull())
			{
				if(!m_ImageClockBackEx.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_BACK")))return;
			}

			//获取大小
			CSize SizeClockItem;
			CSize SizeClockBack;
			SizeClockBack.SetSize(m_ImageClockBackEx.GetWidth(),m_ImageClockBackEx.GetHeight());
			SizeClockItem.SetSize(m_ImageClockItemEx.GetWidth()/10,m_ImageClockItemEx.GetHeight());

			//绘画背景
			INT nXDrawPos=nXPos-SizeClockBack.cx/2;
			INT nYDrawPos=nYPos-SizeClockBack.cy/2;
			m_ImageClockBackEx.DrawImage(pDC,nXDrawPos,nYDrawPos);

			//绘画时间
			WORD nClockItem1=wUserClock/10;
			WORD nClockItem2=wUserClock%10;
			m_ImageClockItemEx.DrawImage(pDC,nXDrawPos+13,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem1*SizeClockItem.cx,0);
			m_ImageClockItemEx.DrawImage(pDC,nXDrawPos+28,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem2*SizeClockItem.cx,0);
		}
		else
		{
			//加载资源
			CPngImage ImageClockItem;
			CPngImage ImageClockBack;
			ImageClockItem.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_ITEM"));
			ImageClockBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("USER_CLOCK_BACK"));

			//获取大小
			CSize SizeClockItem;
			CSize SizeClockBack;
			SizeClockBack.SetSize(ImageClockBack.GetWidth(),ImageClockBack.GetHeight());
			SizeClockItem.SetSize(ImageClockItem.GetWidth()/10,ImageClockItem.GetHeight());

			//绘画背景
			INT nXDrawPos=nXPos-SizeClockBack.cx/2;
			INT nYDrawPos=nYPos-SizeClockBack.cy/2;
			ImageClockBack.DrawImage(pDC,nXDrawPos,nYDrawPos);

			//绘画时间
			WORD nClockItem1=wUserClock/10;
			WORD nClockItem2=wUserClock%10;
			ImageClockItem.DrawImage(pDC,nXDrawPos+13,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem1*SizeClockItem.cx,0);
			ImageClockItem.DrawImage(pDC,nXDrawPos+28,nYDrawPos+27,SizeClockItem.cx,SizeClockItem.cy,nClockItem2*SizeClockItem.cx,0);
		}
	}

	return;
}

//绘画头像
VOID CGameFrameViewGDI::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	DrawUserAvatar(pDC, nXPos, nYPos, FACE_CX, FACE_CY, pIClientUserItem);

	return;
}

//绘画头像
VOID CGameFrameViewGDI::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, IClientUserItem * pIClientUserItem)
{
	//WIN 7 执行
	if (  RealizeWIN7() && CD2DEngine::GetD2DEngine() )
	{
		//效验参数
		ASSERT(pIClientUserItem!=NULL);
		if (pIClientUserItem==NULL) return;

		//变量定义
		tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
		tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

		WORD wFaceID=pIClientUserItem->GetFaceID();
		bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

		//设置变量
		WORD wLineCount=m_ImageUserFace.GetWidth()/FACE_CX;
		WORD wFullCount=wLineCount*m_ImageUserFace.GetHeight()/(FACE_CY*2);

		//绘画头像
		if ((pUserInfo->dwCustomID!=0)&&(pCustomFaceInfo->dwDataSize!=0))
		{
			//绘画头像
			if (bOffLine==false)
			{
				//定义变量
				DWORD dwImage[FACE_CY*FACE_CX];
				ZeroMemory(dwImage, sizeof(dwImage));

				//创建区域
				for (INT i=0;i<FACE_CY*FACE_CX;i++)
				{
					dwImage[i] = RGB(((BYTE)((pCustomFaceInfo->dwCustomFace[i])>>16)),  ((BYTE)(((WORD)(pCustomFaceInfo->dwCustomFace[i])) >> 8)),  ((BYTE)(pCustomFaceInfo->dwCustomFace[i])));
				}
				CBitImageEx::D2DDrawCustomImage(this,nXPos,nYPos,nWidth,nHeight,dwImage,FACE_CX,FACE_CY);
			}
			else
			{
				//定义变量
				DWORD dwImage[FACE_CY*FACE_CX];
				ZeroMemory(dwImage, sizeof(dwImage));

				//创建区域
				for (INT i=0;i<FACE_CY*FACE_CX;i++)
				{
					//提取颜色
					BYTE cbColorR=((BYTE)((pCustomFaceInfo->dwCustomFace[i])>>16));
					BYTE cbColorG=((BYTE)(((WORD)(pCustomFaceInfo->dwCustomFace[i])) >> 8));
					BYTE cbColorB=((BYTE)(pCustomFaceInfo->dwCustomFace[i]));
					BYTE cbColorGray=(BYTE)(cbColorR*0.30+cbColorG*0.59+cbColorB*0.11);

					dwImage[i] = RGB(cbColorGray,cbColorGray,cbColorGray);
				}
				CBitImageEx::D2DDrawCustomImage(this,nXPos,nYPos,nWidth,nHeight,dwImage,FACE_CX,FACE_CY);
			}
		}
		else
		{
			//变量定义
			WORD wFaceID=pUserInfo->wFaceID;

			//绘画头像
			if (bOffLine==false)
			{
				//调整参数
				if (wFaceID>=wFullCount) wFaceID=0;

				//绘画头像
				INT nXImagePos=wFaceID%wLineCount;
				INT nYImagePos=(wFaceID/wLineCount)*2L;
				m_ImageUserFace.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY,FACE_CX,FACE_CY);
			}
			else
			{
				//调整参数
				if (wFaceID>=wFullCount) wFaceID=0;

				//绘画头像
				INT nXImagePos=wFaceID%wLineCount;
				INT nYImagePos=(wFaceID/wLineCount)*2L;
				m_ImageUserFace.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY+FACE_CY,FACE_CX,FACE_CY);
			}
		}
	}
	else
	{
		//绘画头像
		CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
		if (pGameFrameAvatar!=NULL) pGameFrameAvatar->DrawUserAvatar(pDC,nXPos,nYPos,nWidth,nHeight,pIClientUserItem);
	}

	return;
}

//绘画背景
VOID CGameFrameViewGDI::DrawViewImage(CDC * pDC, CBitImage & BitImage, BYTE cbDrawMode)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//无效区域
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//绘画位图
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//平铺模式
		{
			//加载资源
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//绘画位图
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
				{
					bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
					if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//居中模式
		{
			//变量定义
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//位置计算
			INT nXPos=(rcClient.Width()-BitImage.GetWidth())/2;
			INT nYPos=(rcClient.Height()-BitImage.GetHeight())/2;

			//绘画位图
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
			if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//拉伸模式
		{
			//变量定义
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//绘画位图
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,0,0,rcClient.Width(),rcClient.Height());
			if (bIntersect==true) BitImage.StretchBlt(pDC->m_hDC,0,0,rcClient.Width(),rcClient.Height(),0,0,nImageWidth,nImageHeight);

			return;
		}
	}

	return;
}

//绘画字符
VOID CGameFrameViewGDI::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect,CDFontEx *pFontEx)
{
	//变量定义
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;

	//保存格式
	COLORREF rcTextColor=pDC->SetTextColor(crFrame);
	UINT nTextAlign=pDC->SetTextAlign(TA_TOP|TA_LEFT);

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//计算位置
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//绘画字符
		if (RealizeWIN7() && CD2DEngine::GetD2DEngine())
		{
			LOGFONT logfont;
			pDC->GetCurrentFont()->GetLogFont(&logfont);
			if(logfont.lfWidth==0)logfont.lfWidth=12;
			if(logfont.lfWeight==0)logfont.lfWeight=500;
			if(pFontEx != NULL)
				pFontEx->DrawText(pDC,pszString,&rcFrame,crFrame,nDrawFormat);
			else
				CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,pszString,&rcFrame,crFrame,nDrawFormat);
		}
		else
			pDC->DrawText(pszString,nStringLength,&rcFrame,nDrawFormat);
	}

	//绘画字符
	pDC->SetTextColor(crText);
	if (RealizeWIN7() && CD2DEngine::GetD2DEngine())
	{
		LOGFONT logfont;
		pDC->GetCurrentFont()->GetLogFont(&logfont);
		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,pszString,&rcRect,crText,nDrawFormat);
		else
			CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,pszString,&rcRect,crText,nDrawFormat);
	}
	else
		pDC->DrawText(pszString,nStringLength,&rcRect,nDrawFormat);

	//还原设置
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return;
}

//艺术字体
VOID CGameFrameViewGDI::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos,CDFontEx *pFontEx)
{
	//变量定义
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;

	//保存设置
	UINT nTextAlign=pDC->GetTextAlign();
	COLORREF rcTextColor=pDC->GetTextColor();

	//绘画边框
	pDC->SetTextColor(crFrame);
	if (RealizeWIN7() && CD2DEngine::GetD2DEngine())
	{
		LOGFONT logfont;
		pDC->GetCurrentFont()->GetLogFont(&logfont);
		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		for (INT i=0;i<CountArray(nXExcursion);i++)
		{
			if(pFontEx != NULL)
				pFontEx->DrawText(pDC,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i],crFrame,nDrawFormat);
			else
				CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i],crFrame,nDrawFormat);
		}
	}
	else
	{
		for (INT i=0;i<CountArray(nXExcursion);i++)
		{
			pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
		}
	}

	//绘画字体
	pDC->SetTextColor(crText);
	if (RealizeWIN7() && CD2DEngine::GetD2DEngine())
	{
		LOGFONT logfont;
		pDC->GetCurrentFont()->GetLogFont(&logfont);
		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,pszString,nXPos,nYPos,crFrame,nDrawFormat);
		else
			CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,pszString,nXPos,nYPos,crFrame,nDrawFormat);
	}
	else
		pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	//还原设置
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return;
}

//绘画函数
VOID CGameFrameViewGDI::OnPaint()
{
	CPaintDC dc(this);

	//剪切区域
	CRect rcDirty;
	dc.GetClipBox(&rcDirty);

	//窗口尺寸
	CRect rcClient;
	GetClientRect(&rcClient);

	//调整尺寸
	if ((rcClient.Width()>m_SizeBackSurface.cx)||(rcClient.Height()>m_SizeBackSurface.cy))
	{
		//设置大小
		m_SizeBackSurface.cx=__max(m_SizeBackSurface.cx,rcClient.Width());
		m_SizeBackSurface.cy=__max(m_SizeBackSurface.cy,rcClient.Height());

		//更新区域
		m_RectDirtySurface.SetRect(0,0,rcClient.Width(),rcClient.Height());

		//销毁缓冲
		if (m_ImageBackSurface.IsNull()==false) m_ImageBackSurface.Destroy();

		//创建缓冲
		m_ImageBackSurface.Create(m_SizeBackSurface.cx,m_SizeBackSurface.cy,32);
	}

	// WIN7 执行
	if(RealizeWIN7() && CD2DEngine::BeginDraw(this))
	{
		//窗口尺寸
		CRect rcClient;
		GetClientRect(&rcClient);

		//获取 DC
		CImageDC SurfaceDC(m_ImageBackSurface);
		CDC * pSurfaceDC=CDC::FromHandle(SurfaceDC);

		//绘画
		DrawGameView(pSurfaceDC,rcClient.Width(),rcClient.Height());

		//绘画比赛
		DrawMatchInfo(pSurfaceDC);

		CD2DEngine::EndDraw(this);
	}
	else
	{
		//更新判断
		if (m_RectDirtySurface.IsRectEmpty()==FALSE)
		{
			//获取 DC
			CImageDC SurfaceDC(m_ImageBackSurface);
			CDC * pSurfaceDC=CDC::FromHandle(SurfaceDC);

			//区域调整
			m_RectDirtySurface.top=__max(m_RectDirtySurface.top,rcClient.top);
			m_RectDirtySurface.left=__max(m_RectDirtySurface.left,rcClient.left);
			m_RectDirtySurface.right=__min(m_RectDirtySurface.right,rcClient.right);
			m_RectDirtySurface.bottom=__min(m_RectDirtySurface.bottom,rcClient.bottom);

			//设置 DC
			pSurfaceDC->SetBkMode(TRANSPARENT);
			pSurfaceDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

			//绘画界面
			m_RectDirtySurface.SetRect(0,0,0,0);

			DrawGameView(pSurfaceDC,rcClient.Width(),rcClient.Height());

			//绘画比赛
			DrawMatchInfo(pSurfaceDC);

			//更新界面
			m_ImageBackSurface.BitBlt(dc,rcDirty.left,rcDirty.top,rcDirty.Width(),rcDirty.Height(),rcDirty.left,rcDirty.top);
		}
		else
		{
			//更新界面
			m_ImageBackSurface.BitBlt(dc,rcDirty.left,rcDirty.top,rcDirty.Width(),rcDirty.Height(),rcDirty.left,rcDirty.top,SRCCOPY);
		}
	}


	return;
}

void CGameFrameViewGDI::OnTimer(UINT nIDEvent)
{
	if(IDI_ROLL_TEXT==nIDEvent)
	{	
		m_wRollTextCount--;

		LONG lLeft=m_rcText.left;
		if(m_wRollTextCount<50)
		{
			m_rcText=CRect(m_wTextLeft,319,m_wTextLeft+500,319+m_wRollTextCount*2);
			m_RectDirtySurface=m_rcText;
			if(lLeft<m_rcText.left)
				InvalidateRect(&CRect(m_wTextLeft-500,310,m_wTextLeft+500,325+m_wRollTextCount*2));
			else if(lLeft>m_rcText.left)
				InvalidateRect(&CRect(m_wTextLeft,310,m_wTextLeft+1000,325+m_wRollTextCount*2));
			else
				InvalidateRect(&CRect(m_wTextLeft,310,m_wTextLeft+500,325+m_wRollTextCount*2));
		}
		else if(m_wRollTextCount>=110)
		{
			m_rcText=CRect(m_wTextLeft,430-(160-m_wRollTextCount),m_wTextLeft+500,430);
			m_RectDirtySurface=m_rcText;
			if(lLeft<m_rcText.left)
				InvalidateRect(&CRect(m_rcText.left-500,m_rcText.top,m_rcText.right,m_rcText.bottom));
			else if(lLeft>m_rcText.left)
				InvalidateRect(&CRect(m_rcText.left,m_rcText.top,m_rcText.right+500,m_rcText.bottom));
			else
				InvalidateRect(&CRect(m_rcText.left,m_rcText.top,m_rcText.right,m_rcText.bottom));

		}
		else
		{
			m_rcText=CRect(m_wTextLeft,m_wRollTextCount+270,m_wTextLeft+500,m_wRollTextCount+370);		
			m_RectDirtySurface=m_rcText;
			if(lLeft<m_rcText.left)
				InvalidateRect(&CRect(m_rcText.left-500,m_rcText.top,m_rcText.right,m_rcText.bottom));
			else if(lLeft>m_rcText.left)
				InvalidateRect(&CRect(m_rcText.left-500,m_rcText.top,m_rcText.right+500,m_rcText.bottom));
			else
				InvalidateRect(&CRect(m_rcText.left,m_rcText.top,m_rcText.right,m_rcText.bottom));
		}

		if(m_wRollTextCount==0)
		{
			if(m_vecText.size()<=1)
			{
				InvalidateRect(NULL);
				GetClientRect(&m_RectDirtySurface);
				m_vecText.clear();
				KillTimer(IDI_ROLL_TEXT);
				m_strRollText="";
			}
			else
			{
				InvalidateRect(NULL);
				GetClientRect(&m_RectDirtySurface);
				vector <CString>::iterator Iter;
				Iter=m_vecText.begin();
				m_vecText.erase(Iter);
				m_strRollText=m_vecText.at(0);
				m_wRollTextCount=160;
			}

		}
	}
	__super::OnTimer(nIDEvent);
}

//桌面消息
void CGameFrameViewGDI::AddGameTableMessage(LPCTSTR pszMessage,WORD wLen,WORD wType)
{
	if(wType&SMT_TABLE_ROLL)
	{
		m_strRollText=CString(pszMessage,wLen);
		if(m_vecText.empty())
		{
			m_wRollTextCount=160;
			SetTimer(IDI_ROLL_TEXT,20,NULL);
		}
		m_vecText.push_back(m_strRollText);
	}
	// 	else if(wType&SMT_TABLE_FIX)
	// 	{
	// 		m_strFixText="";
	// 		m_strFixText=CString(pszMessage,wLen);
	// 		UpdateGameView(&CRect(m_wTextLeft,400,m_wTextLeft+500,600));
	// 	}

}

//绘画数字
VOID CGameFrameViewGDI::DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign)
{
	//加载资源
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/10;

	//计算数目
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos;
	if(cbAlign==TA_RIGHT)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//关闭奖状
VOID CGameFrameViewGDI::OnBnClickedCloseAwards()
{
	//投递关闭
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

//比赛信息
VOID CGameFrameViewGDI::DrawMatchInfo(CDC * pDC)
{
	//窗口尺寸
	CRect rcClient;
	GetClientRect(&rcClient);

	//比赛信息
	if(m_pMatchInfo!=NULL)
	{
		CPngImage ImageMatchInfo;
		ImageMatchInfo.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("MATCH_INFO"));
		ImageMatchInfo.AlphaDrawImage(pDC,0,0,210);
		pDC->SetTextColor(RGB(255,255,255));
		CString strNum;
		strNum.Format(TEXT("%d"),m_pMatchInfo->wGameCount);
		pDC->TextOut(12,40,strNum); 
		pDC->SetTextColor(RGB(196,221,239));
		pDC->TextOut(38,10,m_pMatchInfo->szTitle[0]);
		pDC->TextOut(38,30,m_pMatchInfo->szTitle[1]);
		pDC->TextOut(38,50,m_pMatchInfo->szTitle[2]);
		pDC->SetTextColor(RGB(255,221,35));
		pDC->TextOut(38,70,m_pMatchInfo->szTitle[3]);
	}

	//等待提示
	if(m_pMatchWaitTip!=NULL)
	{
		//定义变量
		CPngImage ImageWaitTip;
		CPngImage ImageNumberOrange;
		CPngImage ImageNumberGreen;
		CPngImage ImageLine;
		HMODULE hModule=GetModuleHandle(GAME_FRAME_DLL_NAME);

		//加载图片
		ImageWaitTip.LoadImage(hModule,TEXT("MATCH_WAIT_TIP"));				
		ImageNumberOrange.LoadImage(hModule,TEXT("NUMBER_ORANGE"));
		ImageNumberGreen.LoadImage(hModule, TEXT("NUMBER_GREEN"));
		ImageLine.LoadImage(hModule, TEXT("MATCH_LINE"));

		//计算位置
		INT nXPos=(rcClient.Width()-ImageWaitTip.GetWidth())/2;
		INT nYPos=(rcClient.Height()-ImageWaitTip.GetHeight())-40;

		//创建字体
		CFont DrawFont;
		DrawFont.CreateFont(22,0,0,0,300,0,0,0,134,3,2,1,2,TEXT("宋体"));
		CFont * pOldFont=pDC->SelectObject(&DrawFont);
		pDC->SetTextColor(RGB(255,255,0));

		//绘画信息
		ImageWaitTip.AlphaDrawImage(pDC,nXPos,nYPos-20,220);
		pDC->DrawText(m_pMatchWaitTip->szMatchName,&CRect(nXPos+20,nYPos+10, nXPos+ImageWaitTip.GetWidth()-40,nYPos+40),DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		DrawFont.DeleteObject();

		//创建字体
		DrawFont.CreateFont(16,0,0,0,300,0,0,0,134,3,2,1,2,TEXT("宋体"));
		pDC->SelectObject(&DrawFont);
		pDC->SetTextColor(RGB(200,246,244));

		//输出文字
		CString strDrawText;
		strDrawText.Format(TEXT("您目前积分：         排名："));
		pDC->DrawText(strDrawText, &CRect(nXPos+18,nYPos+64, nXPos+ImageWaitTip.GetWidth()-36, nYPos+90),DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);
		DrawNumberString(pDC, ImageNumberOrange, (LONG)m_pMatchWaitTip->lScore, nXPos+158, nYPos+70);
		DrawNumberString(pDC, ImageNumberOrange, m_pMatchWaitTip->wRank, nXPos+250,nYPos+70); 
		ImageLine.DrawImage(pDC, nXPos+265, nYPos+60);
		DrawNumberString(pDC, ImageNumberOrange, m_pMatchWaitTip->wUserCount, nXPos+280,nYPos+70,TA_RIGHT);
		strDrawText.Format(TEXT("请耐心等待，还有      桌未完成比赛"));
		pDC->DrawText(strDrawText, &CRect(nXPos+18,nYPos+104, nXPos+ImageWaitTip.GetWidth()-36, nYPos+130),DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);
		DrawNumberString(pDC, ImageNumberGreen, m_pMatchWaitTip->wPlayingTable, nXPos+160,nYPos+110,TA_RIGHT);
		strDrawText.Format(TEXT("您已经率先完成本轮，处于本桌第%d名。请等待其他桌游戏结束后重新排名。"),m_pMatchWaitTip->wCurTableRank);
		pDC->DrawText(strDrawText, &CRect(nXPos+18,nYPos+144, nXPos+ImageWaitTip.GetWidth()-36, nYPos+200),DT_TOP|DT_WORDBREAK);

		//清理资源
		pDC->SelectObject(pOldFont);
		DrawFont.DeleteObject();
	}

	if(m_strRollText!="" )
	{
		CFont DrawFont;
		DrawFont.CreateFont(20,0,0,0,300,0,0,0,134,3,2,1,2,TEXT("黑体"));
		CFont * pOldFont=pDC->SelectObject(&DrawFont);

		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_strRollText, &m_rcText,DT_CENTER|DT_WORDBREAK|DT_WORD_ELLIPSIS);

		//清理资源
		pDC->SelectObject(pOldFont);
		DrawFont.DeleteObject();
	}
}

//绘画背景
VOID CGameFrameViewGDI::DrawViewImage(CDC * pDC, CBitImageEx & BitImage, BYTE cbDrawMode)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//无效区域
	CRect rcDirty;
	pDC->GetClipBox(&rcDirty);

	//绘画位图
	switch (cbDrawMode)
	{
	case DRAW_MODE_SPREAD:		//平铺模式
		{
			//加载资源
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//绘画位图
			for (INT nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
			{
				for (INT nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
				{
					bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
					if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);
				}
			}

			return;
		}
	case DRAW_MODE_CENTENT:		//居中模式
		{
			//变量定义
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//位置计算
			INT nXPos=(rcClient.Width()-BitImage.GetWidth())/2;
			INT nYPos=(rcClient.Height()-BitImage.GetHeight())/2;

			//绘画位图
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,nXPos,nYPos,nImageWidth,nImageHeight);
			if (bIntersect==true) BitImage.BitBlt(pDC->m_hDC,nXPos,nYPos);

			return;
		}
	case DRAW_MODE_ELONGGATE:	//拉伸模式
		{
			//变量定义
			INT nImageWidth=BitImage.GetWidth();
			INT nImageHeight=BitImage.GetHeight();

			//绘画位图
			bool bIntersect=BitImage.IsRectIntersect(rcDirty,0,0,rcClient.Width(),rcClient.Height());
			if (bIntersect==true) BitImage.StretchBlt(pDC->m_hDC,0,0,rcClient.Width(),rcClient.Height(),0,0,nImageWidth,nImageHeight);

			return;
		}
	}

	return;
}

//绘画数字
VOID CGameFrameViewGDI::DrawNumberString(CDC * pDC, CPngImageEx &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign)
{
	//加载资源
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/10;

	//计算数目
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos;
	if(cbAlign==TA_RIGHT)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//写字符
VOID CGameFrameViewGDI::DrawText( CDC * pDC, LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat , CDFontEx* pFontEx /*= NULL*/ )
{
	if( RealizeWIN7() && CD2DEngine::GetD2DEngine() )
	{
		LOGFONT logfont;
		COLORREF color = pDC->GetTextColor();
		pDC->GetCurrentFont()->GetLogFont(&logfont);

		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,lpszString,lpRect,color,nFormat);
		else
			CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,lpszString,lpRect,color,nFormat);
	}
	else
	{
		COLORREF color = pDC->GetTextColor();
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,lpszString,lpRect,color,nFormat);
		else
			pDC->DrawText(lpszString, nCount, lpRect, nFormat);
	}
}

//写字符
VOID CGameFrameViewGDI::DrawText( CDC * pDC, const CString& str, LPRECT lpRect, UINT nFormat , CDFontEx* pFontEx /*= NULL*/ )
{
	if( RealizeWIN7() && CD2DEngine::GetD2DEngine() )
	{
		LOGFONT logfont;
		COLORREF color = pDC->GetTextColor();
		pDC->GetCurrentFont()->GetLogFont(&logfont);

		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,str,lpRect,color,nFormat);
		else
			CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,str,lpRect,color,nFormat);
	}
	else
	{
		COLORREF color = pDC->GetTextColor();
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,str,lpRect,color,nFormat);
		else
			pDC->DrawText(str, lpRect, nFormat);
	}
}

//写字符
VOID CGameFrameViewGDI::TextOut( CDC * pDC, int x, int y, LPCTSTR lpszString, int nCount, CDFontEx* pFontEx /*= NULL*/ )
{
	if( RealizeWIN7() && CD2DEngine::GetD2DEngine() )
	{
		LOGFONT logfont;
		COLORREF color = pDC->GetTextColor();
		UINT nAlign = pDC->GetTextAlign();
		pDC->GetCurrentFont()->GetLogFont(&logfont);

		UINT nFormat = 0;

		if ( nAlign&TA_CENTER )
			nFormat |= DT_CENTER;
		else if( nAlign&TA_RIGHT )
			nFormat |= DT_RIGHT;
		else
			nFormat |= DT_LEFT;

		if( nAlign&TA_BOTTOM )
			nFormat |= DT_BOTTOM;
		else 
			nFormat |= DT_TOP;

		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,lpszString,x,y,color,nFormat);
		else
			CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,lpszString,x,y,color,nFormat);
	}
	else
	{
		if ( pFontEx != NULL )
		{
			COLORREF color = pDC->GetTextColor();
			UINT nAlign = pDC->GetTextAlign();
			UINT nFormat = 0;

			if ( nAlign&TA_CENTER )
				nFormat |= DT_CENTER;
			else if( nAlign&TA_RIGHT )
				nFormat |= DT_RIGHT;
			else
				nFormat |= DT_LEFT;

			if( nAlign&TA_BOTTOM )
				nFormat |= DT_BOTTOM;
			else 
				nFormat |= DT_TOP;

			pFontEx->DrawText(pDC,lpszString,x,y,color,nFormat);
		}
		else
		{
			pDC->TextOut(x, y, lpszString, nCount);
		}
	}
}

//写字符
VOID CGameFrameViewGDI::TextOut( CDC * pDC, int x, int y, const CString& str, CDFontEx* pFontEx /*= NULL*/ )
{
	if( RealizeWIN7() && CD2DEngine::GetD2DEngine() )
	{
		LOGFONT logfont;
		COLORREF color = pDC->GetTextColor();
		UINT nAlign = pDC->GetTextAlign();
		pDC->GetCurrentFont()->GetLogFont(&logfont);

		UINT nFormat = 0;

		if ( nAlign&TA_CENTER )
			nFormat |= DT_CENTER;
		else if( nAlign&TA_RIGHT )
			nFormat |= DT_RIGHT;
		else
			nFormat |= DT_LEFT;

		if( nAlign&TA_BOTTOM )
			nFormat |= DT_BOTTOM;
		else 
			nFormat |= DT_TOP;

		if(logfont.lfWidth==0)logfont.lfWidth=12;
		if(logfont.lfWeight==0)logfont.lfWeight=500;
		if(pFontEx != NULL)
			pFontEx->DrawText(pDC,str,x,y,color,nFormat);
		else
			CDFontEx::DrawText(this,pDC,logfont.lfWidth,logfont.lfWeight,str,x,y,color,nFormat);
	}
	else
	{
		if ( pFontEx != NULL )
		{
			COLORREF color = pDC->GetTextColor();
			UINT nAlign = pDC->GetTextAlign();
			UINT nFormat = 0;

			if ( nAlign&TA_CENTER )
				nFormat |= DT_CENTER;
			else if( nAlign&TA_RIGHT )
				nFormat |= DT_RIGHT;
			else
				nFormat |= DT_LEFT;

			if( nAlign&TA_BOTTOM )
				nFormat |= DT_BOTTOM;
			else 
				nFormat |= DT_TOP;

			pFontEx->DrawText(pDC,str,x,y,color,nFormat);
		}
		else
		{
			pDC->TextOut(x, y, str);
		}
	}
}



//////////////////////////////////////////////////////////////////////////////////


