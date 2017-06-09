#include "StdAfx.h"
#include "PngImageEx.h"
#include "D2DEngine.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CPngImageEx::CPngImageEx()
{
	//设置变量
	m_pImage = NULL;
	m_pWnd = NULL;
	ZeroMemory(m_strImageName, sizeof(m_strImageName) );

	return;
}

//析构函数
CPngImageEx::~CPngImageEx()
{
	//销毁图片
	DestroyImage(); 

	return;
}

//是否加载
bool CPngImageEx::IsNull()
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
		return m_pWnd == NULL;

	//加载判断
	if (m_pImage==NULL) return true;
	if (m_pImage->GetLastStatus()!=Ok) return true;

	return false;
}

//获取宽度
INT CPngImageEx::GetWidth()
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
		return CD2DEngine::GetD2DEngine()->GetWidth(m_pWnd, m_strImageName);

	//加载判断
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//获取宽度
	return m_pImage->GetWidth();
}

//获取高度
INT CPngImageEx::GetHeight()
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
		return CD2DEngine::GetD2DEngine()->GetHeight(m_pWnd, m_strImageName);

	//加载判断
	ASSERT(IsNull()==false);
	if (IsNull()==true) return 0;

	//获取高度
	return m_pImage->GetHeight();
}

//销毁图片
bool CPngImageEx::DestroyImage()
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() && m_pWnd!= NULL )
	{
		CD2DEngine::GetD2DEngine()->DeleteImage(m_pWnd, m_strImageName);
		m_pWnd = NULL;
		return true;
	}

	//删除对象
	if (m_pImage!=NULL) SafeDelete(m_pImage);

	return true;
}

//加载图片
bool CPngImageEx::LoadImage(CWnd* pWnd, LPCTSTR pszFileName)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		m_pWnd = pWnd;
		CD2DEngine::GetD2DEngine()->LaodImage(pWnd, pszFileName, m_strImageName);
		return true;
	}

	//加载判断
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//加载文件
	CT2CW strFileName(pszFileName);
	m_pImage=Image::FromFile((LPCWSTR)strFileName);

	//错误判断
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
	{
		DestroyImage();
		return false;
	}

	return true;
}

//加载图片
bool CPngImageEx::LoadImage(CWnd* pWnd, HINSTANCE hInstance, LPCTSTR pszResourceName)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		m_pWnd = pWnd;
		CD2DEngine::GetD2DEngine()->LaodImage(pWnd, hInstance, pszResourceName, TEXT("PNG"), m_strImageName);
		return true;
	}

	//加载判断
	ASSERT(m_pImage==NULL);
	if (m_pImage!=NULL) return false;

	//查找资源
	HRSRC hResource=FindResource(hInstance,pszResourceName,TEXT("PNG"));
	if (hResource==NULL) return false;

	//读取资源
	DWORD dwImageSize=SizeofResource(hInstance,hResource);
	LPVOID pImageBuffer=LoadResource(hInstance,hResource);

	//创建数据
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		ASSERT(FALSE);
		return false;
	}

	//写入数据
	pIStream->Write(pImageBuffer,dwImageSize,NULL);

	//创建位图
	m_pImage=Image::FromStream(pIStream);

	//释放资源
	SafeRelease(pIStream);

	//错误判断
	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//绘画图像
bool CPngImageEx::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->DrawImage(m_pWnd, m_strImageName, nXPos, nYPos, 255  );
		return true;
	}

	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//获取属性
	INT nImageWidth=m_pImage->GetWidth();
	INT nImageHeight=m_pImage->GetHeight();

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXPos;
	rcDrawRect.Y=(REAL)nYPos;
	rcDrawRect.Width=(REAL)nImageWidth;
	rcDrawRect.Height=(REAL)nImageHeight;

	//绘画图像
	graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel);

	return true;
}

//绘画图像
bool CPngImageEx::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->DrawImage(m_pWnd, m_strImageName, nXDest, nYDest, nDestWidth, nDestHeight, nXScr, nYSrc, 255  );
		return true;
	}

	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//绘画图像
	graphics.DrawImage(m_pImage,rcDrawRect,(REAL)nXScr,(REAL)nYSrc,(REAL)nDestWidth,(REAL)nDestHeight,UnitPixel);

	return true;
}

//绘画图像
bool CPngImageEx::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->DrawImage(m_pWnd, m_strImageName, nXDest, nYDest, nDestWidth, nDestHeight, nXScr, nYSrc, nSrcWidth, nSrcHeight, 255 );
		return true;
	}

	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDC!=NULL);
	Graphics graphics(pDC->GetSafeHdc());

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)nXDest;
	rcDrawRect.Y=(REAL)nYDest;
	rcDrawRect.Width=(REAL)nDestWidth;
	rcDrawRect.Height=(REAL)nDestHeight;

	//绘画图像
	graphics.DrawImage(m_pImage,rcDrawRect,(REAL)nXScr,(REAL)nYSrc,(REAL)nSrcWidth,(REAL)nSrcHeight,UnitPixel);

	return true;
}

//混合绘画
bool CPngImageEx::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, BYTE cbAlphaDepth)
{
	//透明绘画
	AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,cbAlphaDepth);

	return true;
}

//混合绘画
bool CPngImageEx::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, BYTE cbAlphaDepth)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->DrawImage(m_pWnd, m_strImageName, xDest, yDest, cxDest, cyDest, xSrc, ySrc, cbAlphaDepth  );
		return true;
	}

	//加载判断
	ASSERT(m_pImage!=NULL);
	if (m_pImage==NULL) return false;

	//创建屏幕
	ASSERT(pDestDC!=NULL);
	Graphics graphics(pDestDC->GetSafeHdc());

	//构造位置
	RectF rcDrawRect;
	rcDrawRect.X=(REAL)xDest;
	rcDrawRect.Y=(REAL)yDest;
	rcDrawRect.Width=(REAL)cxDest;
	rcDrawRect.Height=(REAL)cyDest;

	//透明矩阵
	ColorMatrix Matrix=
	{
		1.0f,0.0f,0.0f,0.0f,0.0f, 
			0.0f,1.0f,0.0f,0.0f,0.0f, 
			0.0f,0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,cbAlphaDepth/255.0f,0.0f, 
			0.0f,0.0f,0.0f,0.0f,1.0f
	};

	//设置属性
	ImageAttributes Attributes; 
	Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

	//绘画图像
	graphics.DrawImage(m_pImage,rcDrawRect,(REAL)xSrc,(REAL)ySrc,(REAL)cxDest,(REAL)cyDest,UnitPixel,&Attributes);

	return true;
}

//混合绘画
bool CPngImageEx::AlphaDrawImage(CDC * pDestDC, INT xDest, INT yDest, INT cxDest, INT cyDest, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, BYTE cbAlphaDepth)
{
	//WIN 7 执行
	if ( CD2DEngine::GetD2DEngine() )
	{
		CD2DEngine::GetD2DEngine()->DrawImage(m_pWnd, m_strImageName, xDest, yDest, cxDest, cyDest, xSrc, ySrc, cxSrc, cySrc, cbAlphaDepth  );
		return true;
	}

	//创建缓冲
	if ((cxDest!=cxSrc)||(cyDest!=cySrc))
	{
		//加载判断
		ASSERT(m_pImage!=NULL);
		if (m_pImage==NULL) return false;

		//创建屏幕
		ASSERT(pDestDC!=NULL);
		Graphics graphics(pDestDC->GetSafeHdc());

		//构造位置
		RectF rcDrawRect;
		rcDrawRect.X=(REAL)xDest;
		rcDrawRect.Y=(REAL)yDest;
		rcDrawRect.Width=(REAL)cxDest;
		rcDrawRect.Height=(REAL)cyDest;

		//透明矩阵
		ColorMatrix Matrix=
		{
			1.0f,0.0f,0.0f,0.0f,0.0f, 
				0.0f,1.0f,0.0f,0.0f,0.0f, 
				0.0f,0.0f,1.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,cbAlphaDepth/255.0f,0.0f, 
				0.0f,0.0f,0.0f,0.0f,1.0f
		};

		//设置属性
		ImageAttributes Attributes; 
		Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 

		//绘画图像
		graphics.DrawImage(m_pImage,rcDrawRect,(REAL)xSrc,(REAL)ySrc,(REAL)cxSrc,(REAL)cySrc,UnitPixel,&Attributes);	
	}
	else
	{
		//普通调用
		AlphaDrawImage(pDestDC,xDest,yDest,cxDest,cyDest,xSrc,ySrc,cbAlphaDepth);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
