#include "StdAfx.h"
#include "GlobalUnits.h"
#include "GameFrameAvatar.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGameFrameAvatar *	CGameFrameAvatar::m_pGameFrameAvatar=NULL;			//引擎接口

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameAvatar::CGameFrameAvatar()
{
	//设置变量
	m_wD3DLineCount=0;
	m_wD3DFullCount=0;

	//设置接口
	ASSERT(m_pGameFrameAvatar==NULL);
	if (m_pGameFrameAvatar==NULL) m_pGameFrameAvatar=this;

	return;
}

//析构函数
CGameFrameAvatar::~CGameFrameAvatar()
{
	//设置接口
	ASSERT(m_pGameFrameAvatar==this);
	if (m_pGameFrameAvatar==this) m_pGameFrameAvatar=NULL;

	//销毁资源
	if (m_D3DTextureAvatar.IsNull()==false) m_D3DTextureAvatar.Destory();

	return;
}

//配置函数
VOID CGameFrameAvatar::Initialization(CD3DDevice * pD3DDevice)
{
	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//获取资源
	tagAvatarResource AvatarResource;
	pGlobalUnits->m_FaceItemControlModule->GetAvatarResource(AvatarResource);

	//加载资源
	m_D3DTextureAvatar.LoadImage(pD3DDevice,AvatarResource.hResInstance,AvatarResource.pszResource,TEXT("PNG"));

	//设置变量
	m_wD3DLineCount=m_D3DTextureAvatar.GetWidth()/FACE_CX;
	m_wD3DFullCount=m_wD3DLineCount*m_D3DTextureAvatar.GetHeight()/(FACE_CY*2);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)
{
	//绘画头像
	DrawUserAvatar(pDC,nXPos,nYPos,FACE_CX,FACE_CX,wFaceID);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//绘画头像
	DrawUserAvatar(pDC,nXPos,nYPos,FACE_CX,FACE_CX,pIClientUserItem);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)
{
	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//获取接口
	IFaceItemControl * pIFaceItemControl=pGlobalUnits->m_FaceItemControlModule.GetInterface();

	//绘画头像
	ASSERT(pIFaceItemControl!=NULL);
	if (pIFaceItemControl!=NULL) pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,nWidth,nHeight,wFaceID);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, IClientUserItem * pIClientUserItem)
{
	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//获取接口
	IFaceItemControl * pIFaceItemControl=pGlobalUnits->m_FaceItemControlModule.GetInterface();

#ifdef _DEBUG

	//调试头像
	if (pIClientUserItem==NULL)
	{
		pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,nWidth,nHeight,WORD(0));
		return;
	}

#endif

	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	WORD wFaceID=pIClientUserItem->GetFaceID();
	bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

	//绘画头像
	if ((pUserInfo->dwCustomID!=0)&&(pCustomFaceInfo->dwDataSize!=0))
	{
		//绘画头像
		if (bOffLine==false)
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,nWidth,nHeight,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pIFaceItemControl->DrawFaceOffLine(pDC,nXPos,nYPos,nWidth,nHeight,pCustomFaceInfo->dwCustomFace);
		}
	}
	else
	{
		//变量定义
		WORD wFaceID=pUserInfo->wFaceID;
		if (wFaceID>=pIFaceItemControl->GetFaceCount()) wFaceID=0;

		//绘画头像
		if (bOffLine==false)
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,nWidth,nHeight,wFaceID);
		}
		else
		{
			pIFaceItemControl->DrawFaceOffLine(pDC,nXPos,nYPos,nWidth,nHeight,wFaceID);
		}
	}

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wFaceID)
{
	//绘画头像
	DrawUserAvatar(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,wFaceID);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//绘画头像
	DrawUserAvatar(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,pIClientUserItem);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)
{
	//效验参数
	ASSERT(m_D3DTextureAvatar.IsNull()==false);
	if (m_D3DTextureAvatar.IsNull()==true) return;

	//调整参数
	if (wFaceID>=m_wD3DFullCount) wFaceID=0;

	//绘画头像
	INT nXImagePos=wFaceID%m_wD3DLineCount;
	INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
	m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,nWidth,nHeight,nXImagePos*nWidth,nYImagePos*nHeight,FACE_CX,FACE_CY);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, INT nWidth, INT nHeight, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(m_D3DTextureAvatar.IsNull()==false);
	if (m_D3DTextureAvatar.IsNull()==true) return;

#ifdef _DEBUG

	//调试头像
	if (pIClientUserItem==NULL)
	{
		m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,nWidth,nHeight,0,0,FACE_CX,FACE_CY);
		return;
	}

#endif

	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//绘画头像
	if ((pUserInfo->dwCustomID!=0)&&(pCustomFaceInfo->dwDataSize!=0))
	{
		if (pUserInfo->cbUserStatus!=US_OFFLINE)
		{
			DWORD dwTempCustomFace[FACE_CX*FACE_CY]={0};

			//创建区域
			for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
			{
				for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
				{
					//设置颜色
					DWORD dwImageTarget=nYImagePos*FACE_CX+nXImagePos;
					COLORREF crImageTarget=pCustomFaceInfo->dwCustomFace[dwImageTarget];

					//提取颜色
					BYTE cbColorR=GetRValue(crImageTarget);
					BYTE cbColorG=GetGValue(crImageTarget);
					BYTE cbColorB=GetBValue(crImageTarget);

					//设置颜色
					dwTempCustomFace[dwImageTarget]=RGB(cbColorB,cbColorG,cbColorR);
				}
			}
			CD3DTexture::DrawImage(pD3DDevice,nXPos,nYPos,nWidth,nHeight,dwTempCustomFace,FACE_CX,FACE_CY);
		}
		else
		{
			DWORD dwTempCustomFace[FACE_CX*FACE_CY]={0};

			//创建区域
			for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
			{
				for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
				{
					//设置颜色
					DWORD dwImageTarget=nYImagePos*FACE_CX+nXImagePos;
					COLORREF crImageTarget=pCustomFaceInfo->dwCustomFace[dwImageTarget];

					//提取颜色
					BYTE cbColorR=GetRValue(crImageTarget);
					BYTE cbColorG=GetGValue(crImageTarget);
					BYTE cbColorB=GetBValue(crImageTarget);
					BYTE cbColorGray=(BYTE)(cbColorR*0.30+cbColorG*0.59+cbColorB*0.11);

					//设置颜色
					dwTempCustomFace[dwImageTarget]=RGB(cbColorGray,cbColorGray,cbColorGray);
				}
			}

			//绘制头像
			CD3DTexture::DrawImage(pD3DDevice,nXPos,nYPos,nWidth,nHeight,dwTempCustomFace,FACE_CX,FACE_CY);
		}
	}
	else
	{
		//变量定义
		WORD wFaceID=pUserInfo->wFaceID;
		if (wFaceID>=m_wD3DFullCount) wFaceID=0;

		//绘画头像
		if (pUserInfo->cbUserStatus!=US_OFFLINE)
		{
			INT nXImagePos=wFaceID%m_wD3DLineCount;
			INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
			m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY,FACE_CX,FACE_CY);
		}
		else
		{
			INT nXImagePos=wFaceID%m_wD3DLineCount;
			INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
			m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY+FACE_CY,FACE_CX,FACE_CY);
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
