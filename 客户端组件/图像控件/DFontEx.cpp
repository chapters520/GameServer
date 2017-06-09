#include "StdAfx.h"
#include ".\dfontex.h"
#include "D2DEngine.h"


CDFontEx::CDFontEx(void)
{
	m_pWnd = NULL;
	m_pD2DFont = NULL;
	m_nWidth = 0;
}

CDFontEx::~CDFontEx(void)
{
	//DeleteFont();
}

//创建字体
void CDFontEx::CreateFont( CWnd* pWnd, LPCTSTR strInfo, int nWidth, int nWeight )
{
	if ( CD2DEngine::GetD2DEngine() )
	{
		m_pD2DFont = CD2DEngine::GetD2DEngine()->CreateFont(strInfo, nWidth, nWeight);
	}
	//else
	//{
		m_GDIFont.CreateFont(nWidth, 0, 0, 0, nWeight, 0, 0, 0, 0, 0, 0, 0, 0, strInfo);
	//}
	m_nWidth = nWidth;
	m_pWnd = pWnd;
}

//获取字符长度
int CDFontEx::GetCharLength(TCHAR chInfo, int nWidth)
{
	if( chInfo >= 0xA0 )//全角 中文 
	{ 
		return nWidth;
	} 
	else					//半角 英文 
	{ 
		return nWidth/2;
	} 
}

//绘画字体
void CDFontEx::DrawText( CDC* pDC, LPCTSTR str, LPRECT lpRect, COLORREF colorref, UINT nFormat)
{
	if( m_pWnd == NULL )
		return;

	if ( CD2DEngine::GetD2DEngine() )
	{	
		//变量定义
		CRect rcDraw(lpRect);

		//转换字符串
		CString strInfo(str);
		int	nLengthX = 0;
		bool bComplete = false;
		for ( int i = 0 ; i < lstrlen(strInfo); ++i )
		{
			nLengthX += GetCharLength(strInfo[i], m_nWidth);
			if( nLengthX >= (lpRect->right - lpRect->left) )
			{				
				LOGFONT LogFont;
				m_GDIFont.GetLogFont(&LogFont);

				if(rcDraw.top+LogFont.lfHeight*2+5>rcDraw.bottom)
				{
					if((nFormat&DT_END_ELLIPSIS)==DT_END_ELLIPSIS)
						strInfo.Insert(i-3,TEXT("..."));
					CD2DEngine::GetD2DEngine()->DrawText(m_pWnd, m_pD2DFont, strInfo.Left(i), rcDraw, colorref, nFormat);
					bComplete=true;
					break;
				}
				else
				{
					CD2DEngine::GetD2DEngine()->DrawText(m_pWnd, m_pD2DFont, strInfo.Left(i), rcDraw, colorref, nFormat);
					rcDraw.top+=LogFont.lfHeight+5;
				}

				strInfo.Delete(0,i);
				nLengthX=0;
				i=0;
			}
		}

		if(!bComplete)
			CD2DEngine::GetD2DEngine()->DrawText(m_pWnd, m_pD2DFont, strInfo, rcDraw, colorref, nFormat);
	}
	else
	{
		UINT uOldAlign = pDC->SetTextAlign(TA_LEFT|TA_TOP);
		COLORREF OldColor = pDC->SetTextColor(colorref);
		CFont* pOldFont = pDC->SelectObject(&m_GDIFont);

		pDC->DrawText(str, lpRect, nFormat);

		pDC->SetTextColor(OldColor);
		pDC->SetTextAlign(uOldAlign);
		pDC->SelectObject(pOldFont);
	}
}
//绘画字体
void CDFontEx::DrawText( CDC* pDC, LPCTSTR str, int XPos, int YPos, COLORREF colorref, UINT nFormat)
{
	if( m_pWnd == NULL )
		return;

	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->DrawText(m_pWnd, m_pD2DFont, str, XPos, YPos, colorref, nFormat);
	}
	else
	{
		UINT uOldAlign = 0;
		UINT uNweAlign = 0;
		COLORREF OldColor = pDC->SetTextColor(colorref);
		CFont* pOldFont = pDC->SelectObject(&m_GDIFont);

		if ( nFormat&DT_CENTER )
			uNweAlign |= TA_CENTER;
		else if( nFormat&DT_RIGHT )
			uNweAlign |= TA_RIGHT;
		else
			uNweAlign |= TA_LEFT;

		if( nFormat&DT_BOTTOM )
			uNweAlign |= TA_BOTTOM;
		else 
			uNweAlign |= TA_TOP;

		uOldAlign = pDC->SetTextAlign(uNweAlign);
		
		pDC->TextOut( XPos, YPos, str );
		
		pDC->SetTextColor(OldColor);
		pDC->SetTextAlign(uOldAlign);
		pDC->SelectObject(pOldFont);
	}
}

//绘画字体
void CDFontEx::DrawText(CWnd* pWnd, CDC* pDC, int nWidth, int nWeight, LPCTSTR str, LPRECT lpRect, COLORREF colorref, UINT nFormat )
{
	//转换字符串
// 	CString strInfo(str);
// 	int	nLengthX = 0;
// 	for ( int i = 0 ; i < lstrlen(strInfo); ++i )
// 	{
// 		nLengthX += GetCharLength(strInfo[i], nWidth);
// 		int nLengthMax = nLengthX;
// 
// 		bool bComplete = false;
// 		for ( int j = i + 1, n = 0; j < lstrlen(strInfo) && n < 3; j++, n++)
// 		{
// 			nLengthMax += GetCharLength(strInfo[j], nWidth);
// 
// 			if( nLengthMax >= (lpRect->right - lpRect->left) )
// 			{
// 				strInfo.Insert(i + 1, TEXT("."));
// 				strInfo.Insert(i + 2, TEXT("."));
// 				strInfo.Insert(i + 3, TEXT("."));
// 				if( lstrlen(strInfo) > i + 4 )
// 				{
// 					strInfo.Delete(i + 4, lstrlen(strInfo) - (i + 4));
// 				}
// 				bComplete = true;
// 			}
// 		}
// 
// 		if ( bComplete )
// 		{
// 			break;
// 		}
// 	}

	if ( CD2DEngine::GetD2DEngine() )
	{
		//创建字体
		void* pD2DFont = CD2DEngine::GetD2DEngine()->CreateFont(TEXT("宋体"), nWidth, nWeight);
		CFont FontHeight;
		FontHeight.CreateFont(nWidth, 0, 0, 0, nWeight, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("宋体"));

		//变量定义
		CRect rcDraw(lpRect);

		//转换字符串
		CString strInfo(str);
		int	nLengthX = 0;
		bool bComplete = false;
		for ( int i = 0 ; i < lstrlen(strInfo); ++i )
		{
			nLengthX += GetCharLength(strInfo[i], nWidth);
			if( nLengthX >= (lpRect->right - lpRect->left) )
			{				
				LOGFONT LogFont;
				FontHeight.GetLogFont(&LogFont);

				if(rcDraw.top+LogFont.lfHeight*2+5>rcDraw.bottom)
				{
					if((nFormat&DT_END_ELLIPSIS)==DT_END_ELLIPSIS)
						strInfo.Insert(i-3,TEXT("..."));
					CD2DEngine::GetD2DEngine()->DrawText(pWnd, pD2DFont, strInfo.Left(i), rcDraw, colorref, nFormat);
					bComplete=true;
					break;
				}
				else
				{
					CD2DEngine::GetD2DEngine()->DrawText(pWnd, pD2DFont, strInfo.Left(i), rcDraw, colorref, nFormat);
					rcDraw.top+=LogFont.lfHeight+5;
				}

				strInfo.Delete(0,i);
				nLengthX=0;
				i=0;
			}
		}

		if(!bComplete)
			CD2DEngine::GetD2DEngine()->DrawText(pWnd, pD2DFont, strInfo, rcDraw, colorref, nFormat);

		//CD2DEngine::GetD2DEngine()->DrawText(pWnd, pD2DFont, strInfo, lpRect, colorref, nFormat);

		//释放资源
		FontHeight.DeleteObject();
		CD2DEngine::GetD2DEngine()->DeleteFont(pD2DFont);
	}
	else
	{
		CFont GDIFont;
		GDIFont.CreateFont(nWidth, 0, 0, 0, nWeight, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("宋体"));
		UINT uOldAlign = pDC->SetTextAlign(TA_LEFT|TA_TOP);
		COLORREF OldColor = pDC->SetTextColor(colorref);
		CFont* pOldFont = pDC->SelectObject(&GDIFont);

		pDC->DrawText(str, lpRect, nFormat);

		pDC->SetTextColor(OldColor);
		pDC->SetTextAlign(uOldAlign);
		pDC->SelectObject(pOldFont);
		GDIFont.DeleteObject();
	}
}

//绘画字体
void CDFontEx::DrawText(CWnd* pWnd, CDC* pDC, int nWidth, int nWeight, LPCTSTR str, int XPos, int YPos, COLORREF colorref, UINT nFormat )
{
	if( pWnd == NULL )
		return;

	if ( CD2DEngine::GetD2DEngine() )
	{
		void* pD2DFont = CD2DEngine::GetD2DEngine()->CreateFont(TEXT("宋体"), nWidth, nWeight);
		CD2DEngine::GetD2DEngine()->DrawText(pWnd, pD2DFont, str, XPos, YPos, colorref, nFormat);
		CD2DEngine::GetD2DEngine()->DeleteFont(pD2DFont);
	}
	else
	{
		CFont GDIFont;
		GDIFont.CreateFont(nWidth, 0, 0, 0, nWeight, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("宋体"));
		UINT uOldAlign = 0;
		UINT uNweAlign = 0;
		COLORREF OldColor = pDC->SetTextColor(colorref);
		CFont* pOldFont = pDC->SelectObject(&GDIFont);

		if ( nFormat&DT_CENTER )
			uNweAlign |= TA_CENTER;
		else if( nFormat&DT_RIGHT )
			uNweAlign |= TA_RIGHT;
		else
			uNweAlign |= TA_LEFT;

		if( nFormat&DT_BOTTOM )
			uNweAlign |= TA_BOTTOM;
		else 
			uNweAlign |= TA_TOP;

		uOldAlign = pDC->SetTextAlign(uNweAlign);

		pDC->TextOut( XPos, YPos, str );

		pDC->SetTextColor(OldColor);
		pDC->SetTextAlign(uOldAlign);
		pDC->SelectObject(pOldFont);
		GDIFont.DeleteObject();
	}
}

//删除字体
void CDFontEx::DeleteFont()
{
	
	if ( CD2DEngine::GetD2DEngine() && m_pD2DFont )
	{
		CD2DEngine::GetD2DEngine()->DeleteFont(m_pD2DFont);
		m_pD2DFont = NULL;
	}
	//else
	//{
		if ( m_GDIFont.GetSafeHandle() )
		{
			m_GDIFont.DeleteObject();
		}
	//}

	
}
