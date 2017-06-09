#include "StdAfx.h"
#include "Resource.h"
#include "GamePropertyCtrl.h"
#include "GamePropertyManager.h"
#include ".\gamepropertyctrl.h"

//////////////////////////////////////////////////////////////////////////////////

#define  IDC_BT_LEFT_MOVE                 100                    //左移按钮
#define  IDC_BT_RIGHT_MOVE                101                    //右移按钮  

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePropertyCtrl, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()

	ON_BN_CLICKED(IDC_BT_LEFT_MOVE, OnBtnLeftMoveClicked)
	ON_BN_CLICKED(IDC_BT_RIGHT_MOVE, OnBtnRightMoveClicked)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyCtrl::CGamePropertyCtrl()
{
	//状态变量
	m_bHovering=false;
	m_wDownItem=INVALID_WORD;
	m_wHoverItem=INVALID_WORD;
	m_cbCurrType=0;

	//设置变量
	m_pIGamePropertySink=NULL;
	m_pCurrPropertyItem=NULL;

	//设置位置
	m_SizeImageItem.SetSize(0,0);
	ZeroMemory(&m_PropertyResource,sizeof(m_PropertyResource));

	//设置变量
	m_cbPresentStartIndex=0;
	m_cbPropertyStartIndex=0;
	m_cbPresentCount=0;
	m_cbPropertyCount=0;
	ZeroMemory(m_pGamePropertyItem,sizeof(m_pGamePropertyItem));
	ZeroMemory(m_pGamePreSentItem,sizeof(m_pGamePreSentItem));	

	return;
}

//析构函数
CGamePropertyCtrl::~CGamePropertyCtrl()
{
	//通知退出
	if(m_pCurrPropertyItem!=NULL)
		m_pCurrPropertyItem->OnEventPerformCommand(CMD_CLOSE_WND,NULL);
}

//设置接口
bool CGamePropertyCtrl::SetGamePropertySink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertySink)!=NULL);
		m_pIGamePropertySink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertySink);

		//成功判断
		if (m_pIGamePropertySink==NULL) return false;
	}
	else m_pIGamePropertySink=NULL;

	return true;
}

//设置资源
bool CGamePropertyCtrl::SetPropertyResource(tagPropertyResource & PropertyResource)
{
	//设置变量
	m_PropertyResource=PropertyResource;

	//加载资源
	CImage ImageGamePropertyItem;
	ImageGamePropertyItem.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageItem);

	//设置变量
	m_SizeImageItem.SetSize(ImageGamePropertyItem.GetWidth(),ImageGamePropertyItem.GetHeight());

	return true;
}

//设置道具
bool CGamePropertyCtrl::SetGamePropertyInfo(tagPropertyInfo PropertyInfo[], BYTE cbPropertyCount, WORD wIssueArea)
{
	//设置变量
	m_cbPropertyCount=0;
	m_cbPresentCount=0;
	m_cbPresentStartIndex=0;
	m_cbPropertyStartIndex=0;
	ZeroMemory(m_pGamePropertyItem,sizeof(m_pGamePropertyItem));
	ZeroMemory(m_pGamePreSentItem,sizeof(m_pGamePreSentItem));

	//设置道具
	for (BYTE i=0;i<cbPropertyCount;i++)
	{
		if ((PropertyInfo[i].wIssueArea&wIssueArea)!=0)
		{
			//溢出判断
			ASSERT(m_cbPropertyCount<CountArray(m_pGamePropertyItem));
			if (m_cbPropertyCount>=CountArray(m_pGamePropertyItem)) break;

			//溢出判断
			ASSERT(m_cbPresentCount<CountArray(m_pGamePreSentItem));
			if (m_cbPresentCount>=CountArray(m_pGamePreSentItem)) break;

			//创建对象
			CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
			CGamePropertyItem * pGamePropertyItem=pGamePropertyManager->CreatePropertyItem(PropertyInfo[i]);

			//插入对象
			if (pGamePropertyItem!=NULL)
			{
				//礼物类型
				if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PRESENT)
					m_pGamePreSentItem[m_cbPresentCount++]=pGamePropertyItem;

				//道具类型
				if(pGamePropertyItem->GetPropertyAttrib()->wPropertyType==PT_TYPE_PROPERTY)
					m_pGamePropertyItem[m_cbPropertyCount++]=pGamePropertyItem;
			}
		}
	}

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return true;
}

//改变事件
VOID  CGamePropertyCtrl::OnSelectedUserItemChange(IClientUserItem * pIClintUserItem)
{
	//参数校验
	if(pIClintUserItem==NULL) return;

	//道具判断
	if(m_pCurrPropertyItem==NULL) return;

	//事件调用
	m_pCurrPropertyItem->OnEventPerformCommand(CMD_USER_CHANGE,(VOID *)pIClintUserItem);
}

//点击判断
WORD CGamePropertyCtrl::SwitchItemFromPoint(CPoint MousePoint)
{
	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	CSize SizeExcursion=m_PropertyResource.SizeExcursion;
	CPoint PointExcursion=m_PropertyResource.PointExcursion;

	//计算位置
	INT nItemCount=(rcClient.Width()-20)/m_SizeImageItem.cx;
	INT nItemStart=(rcClient.Width()-nItemCount*m_SizeImageItem.cx)/2L;

	//范围判断
	if ((MousePoint.x<nItemStart)||(MousePoint.x>(nItemStart+nItemCount*SizeExcursion.cx))) return INVALID_WORD;
	if ((MousePoint.y<PointExcursion.y)||(MousePoint.y>(PointExcursion.y+2L*SizeExcursion.cy))) return INVALID_WORD;

	//溢出判断
	if (((MousePoint.x-nItemStart)%SizeExcursion.cx)>PROPERTY_IMAGE_CX) return INVALID_WORD;
	if (((MousePoint.y-PointExcursion.y)%SizeExcursion.cy)>PROPERTY_IMAGE_CY) return INVALID_WORD;

	//横向索引
	INT nXIndex=(MousePoint.x-nItemStart)/SizeExcursion.cx;
	INT nYIndex=(MousePoint.y-PointExcursion.y)/SizeExcursion.cy;

	//子项类型
	m_cbCurrType = nYIndex==0?PT_TYPE_PROPERTY:PT_TYPE_PRESENT;

	//计算索引
	WORD wSwitchIndex=/*nYIndex*nItemCount+*/nXIndex;
	if (m_cbCurrType==PT_TYPE_PROPERTY && wSwitchIndex>=m_cbPropertyCount) return INVALID_WORD;
	if (m_cbCurrType==PT_TYPE_PRESENT && wSwitchIndex>=m_cbPresentCount) return INVALID_WORD;	

	return wSwitchIndex;
}

//调整控件
VOID CGamePropertyCtrl::RectifyControl(INT nWidth, INT nHeight)
{
	//变量定义
	if(m_SizeImageItem.cx==0) return;
	INT nItemCount=(nWidth-20)/m_SizeImageItem.cx;

	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;	

	//调整按钮
	CRect rcButton(0,0,0,0);
	m_btLeftMove.GetWindowRect(&rcButton);
	INT nTempWidth = (nWidth-nItemCount*m_SizeImageItem.cx)/2;
	DeferWindowPos(hDwp,m_btLeftMove,NULL,(nTempWidth-rcButton.Width())/2+2,(nHeight-rcButton.Height())/2,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btRightMove,NULL,nWidth-(nTempWidth-rcButton.Width())/2-rcButton.Width()-3,(nHeight-rcButton.Height())/2,0,0,uFlags|SWP_NOSIZE);

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	//调整索引
	if(m_cbPresentStartIndex+nItemCount>=m_cbPresentCount)
		m_cbPresentStartIndex=__max(m_cbPresentCount-nItemCount,0);

	//调整索引
	if(m_cbPropertyStartIndex+nItemCount>=m_cbPropertyCount)
		m_cbPropertyStartIndex=__max(m_cbPropertyCount-nItemCount,0);

	
	//左移按钮
	if(m_cbPresentStartIndex+nItemCount>=m_cbPresentCount && m_cbPropertyStartIndex+nItemCount>=m_cbPropertyCount)
		m_btRightMove.EnableWindow(FALSE);
	else
		m_btRightMove.EnableWindow(TRUE);

	//右移按钮
	if(m_cbPresentStartIndex==0 && m_cbPropertyStartIndex==0)
		m_btLeftMove.EnableWindow(FALSE);
	else
		m_btLeftMove.EnableWindow(TRUE);

	return;
}

//绘画背景
BOOL CGamePropertyCtrl::OnEraseBkgnd(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//设置缓冲
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextColor(RGB(250,250,250));
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//加载资源
	CImage ImageGamePropertyL;
	CImage ImageGamePropertyM;
	CImage ImageGamePropertyR;
	CImage ImageGamePropertyItem;
	ImageGamePropertyL.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageL);
	ImageGamePropertyM.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageM);
	ImageGamePropertyR.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageR);
	ImageGamePropertyItem.LoadFromResource(m_PropertyResource.hResInstance,m_PropertyResource.uImageItem);

	//变量定义
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//渲染资源
	pSkinRenderManager->RenderImage(ImageGamePropertyL);
	pSkinRenderManager->RenderImage(ImageGamePropertyM);
	pSkinRenderManager->RenderImage(ImageGamePropertyR);
	pSkinRenderManager->RenderImage(ImageGamePropertyItem);

	//绘画框架
	ImageGamePropertyL.BitBlt(BufferDC,0,0);
	ImageGamePropertyR.BitBlt(BufferDC,rcClient.Width()-ImageGamePropertyR.GetWidth(),0);

	//变量定义
	INT nXStartPos=ImageGamePropertyL.GetWidth();
	INT nXConcludePos=rcClient.Width()-ImageGamePropertyR.GetWidth();

	//计算位置
	INT nItemCount=(rcClient.Width()-20)/m_SizeImageItem.cx;
	INT nItemStart=(rcClient.Width()-nItemCount*m_SizeImageItem.cx)/2;

	//绘画中间
	for (INT nXPos=nXStartPos;nXPos<nXConcludePos;nXPos+=ImageGamePropertyM.GetWidth())
	{
		INT nCXDraw=__min(ImageGamePropertyM.GetWidth(),nXConcludePos-nXPos);
		ImageGamePropertyM.BitBlt(BufferDC,nXPos,0,nCXDraw,ImageGamePropertyM.GetHeight(),0,0);
	}

	//绘画子项
	for (INT i=0;i<nItemCount;i++)
	{
		ImageGamePropertyItem.BitBlt(BufferDC,nItemStart+i*ImageGamePropertyItem.GetWidth(),0);
	}

	//变量定义
	CSize SizeExcursion=m_PropertyResource.SizeExcursion;
	CPoint PointExcursion=m_PropertyResource.PointExcursion;
	INT nXHoverPos=-1;
	INT nYHoverPos=-1;

	//绘画道具
	for (BYTE i=0;i<m_cbPropertyCount;i++)
	{
		//效验对象
		ASSERT(m_pGamePropertyItem[i]!=NULL);
		if (m_pGamePropertyItem[i]==NULL) break;

		//获取信息
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem[i]->GetImageInfo(PropertyImage);

		//加载资源
		CPngImage ImageProperty;
		ImageProperty.LoadImage(PropertyImage.hResInstance,PropertyImage.pszImageID);

		//索引判断
		if(i<m_cbPropertyStartIndex) continue;
		BYTE cbIndex=i-m_cbPropertyStartIndex;

		//数目判断
		if(cbIndex>=nItemCount) break;

		//绘画界面
		if (ImageProperty.IsNull()==false)
		{
			//获取大小
			CSize SizeProperty;
			SizeProperty.SetSize(ImageProperty.GetWidth()/3L,ImageProperty.GetHeight());

			//计算位置
			INT nImagePos=0;
			if (m_wDownItem==cbIndex && m_cbCurrType==PT_TYPE_PROPERTY) nImagePos=SizeProperty.cx;
			if ((m_wDownItem!=cbIndex)&&(m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PROPERTY)) nImagePos=SizeProperty.cx*2;

			//绘画图标
			INT nXDrawPos=(cbIndex%nItemCount)*SizeExcursion.cx+PointExcursion.x;
			INT nYDrawPos=PointExcursion.y;
			ImageProperty.DrawImage(pBufferDC,nItemStart+nXDrawPos,nYDrawPos,PROPERTY_IMAGE_CX,PROPERTY_IMAGE_CY,nImagePos,0,SizeProperty.cx,SizeProperty.cy);

			//绘画框架
			if (m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PROPERTY)
			{
				//加载资源
				CPngImage ImageHoverFrame;
				ImageHoverFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("HOVER_FRAME"));

				//获取大小
				CSize SizeHoverFrame;
				SizeHoverFrame.SetSize(ImageHoverFrame.GetWidth(),ImageHoverFrame.GetHeight());

				//绘画框架
				nXHoverPos=nXDrawPos+(PROPERTY_IMAGE_CX-SizeHoverFrame.cx)/2L;
				nYHoverPos=nYDrawPos+(PROPERTY_IMAGE_CY-SizeHoverFrame.cy)/2L;
				ImageHoverFrame.DrawImage(pBufferDC,nItemStart+nXHoverPos,nYHoverPos);
			}
		}
	}

	//绘画礼物
	for (BYTE i=0;i<m_cbPresentCount;i++)
	{
		//效验对象
		ASSERT(m_pGamePreSentItem[i]!=NULL);
		if (m_pGamePreSentItem[i]==NULL) break;

		//获取信息
		tagPropertyImage PropertyImage;
		m_pGamePreSentItem[i]->GetImageInfo(PropertyImage);

		//加载资源
		CPngImage ImageProperty;
		ImageProperty.LoadImage(PropertyImage.hResInstance,PropertyImage.pszImageID);

		//索引判断
		if(i<m_cbPresentStartIndex) continue;
		BYTE cbIndex=i-m_cbPresentStartIndex;

		//数目判断
		if(cbIndex>=nItemCount) break;

		//绘画界面
		if (ImageProperty.IsNull()==false)
		{
			//获取大小
			CSize SizeProperty;
			SizeProperty.SetSize(ImageProperty.GetWidth()/3L,ImageProperty.GetHeight());

			//计算位置
			INT nImagePos=0;
			if (m_wDownItem==cbIndex && m_cbCurrType==PT_TYPE_PRESENT) nImagePos=SizeProperty.cx;
			if ((m_wDownItem!=cbIndex)&&(m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PRESENT)) nImagePos=SizeProperty.cx*2;

			//绘画图标
			INT nXDrawPos=(cbIndex%nItemCount)*SizeExcursion.cx+PointExcursion.x;
			INT nYDrawPos=SizeExcursion.cy+PointExcursion.y;
			ImageProperty.DrawImage(pBufferDC,nItemStart+nXDrawPos,nYDrawPos,PROPERTY_IMAGE_CX,PROPERTY_IMAGE_CY,nImagePos,0,SizeProperty.cx,SizeProperty.cy);

			//绘画框架
			if (m_wHoverItem==cbIndex && m_cbCurrType==PT_TYPE_PRESENT)
			{
				//加载资源
				CPngImage ImageHoverFrame;
				ImageHoverFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("HOVER_FRAME"));

				//获取大小
				CSize SizeHoverFrame;
				SizeHoverFrame.SetSize(ImageHoverFrame.GetWidth(),ImageHoverFrame.GetHeight());

				//绘画框架
				nXHoverPos=nXDrawPos+(PROPERTY_IMAGE_CX-SizeHoverFrame.cx)/2L;
				nYHoverPos=nYDrawPos+(PROPERTY_IMAGE_CY-SizeHoverFrame.cy)/2L;
				ImageHoverFrame.DrawImage(pBufferDC,nItemStart+nXHoverPos,nYHoverPos);
			}
		}
	}

	//提示
	if(m_wHoverItem != INVALID_WORD && nXHoverPos != -1 && nYHoverPos != -1)
	{
		//获取信息
		TCHAR *pName=NULL;
		if(m_cbCurrType==PT_TYPE_PROPERTY)
		{
			pName = m_pGamePropertyItem[m_wHoverItem+m_cbPropertyStartIndex]->GetPropertyAttrib()->szPropertyName;
		}
		else if(m_cbCurrType == PT_TYPE_PRESENT)
		{
			pName = m_pGamePreSentItem[m_wHoverItem+m_cbPresentStartIndex]->GetPropertyAttrib()->szPropertyName;
		}

		//位置修正
		if(m_wHoverItem <= nItemCount/2) nXHoverPos += (PROPERTY_IMAGE_CX+20);
		else nXHoverPos -= (PROPERTY_IMAGE_CX+20);
		nYHoverPos += (PROPERTY_IMAGE_CY/2)+5;

		//绘制信息
		CRect rcTip(nXHoverPos,nYHoverPos,nXHoverPos+80,nYHoverPos+20);
		pBufferDC->FillSolidRect(&rcTip, RGB(240,240,240));
		pBufferDC->Draw3dRect(&rcTip,RGB(153,149,103),RGB(153,149,103));
		if(pName)
		{
			pBufferDC->SetTextColor(RGB(0,0,0));
			pBufferDC->DrawText(pName, -1, &rcTip, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}


	//绘画界面
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//位置消息
VOID CGamePropertyCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//鼠标消息
VOID CGamePropertyCtrl::OnMouseMove(UINT nFlags, CPoint MousePoint)
{
	__super::OnMouseMove(nFlags, MousePoint);

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	return;
}

//鼠标消息
VOID CGamePropertyCtrl::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//释放捕获
	ReleaseCapture();

	//动作处理
	if (m_wDownItem!=INVALID_WORD)
	{
		//变量定义
		WORD wLastDown=m_wDownItem;
		BYTE cbLastType=m_cbCurrType;

		//设置变量
		m_wDownItem=INVALID_WORD;
		m_wHoverItem=SwitchItemFromPoint(MousePoint);

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

		//动作处理
		if (wLastDown==m_wHoverItem && cbLastType==m_cbCurrType)
		{
			//道具类型
			if(m_cbCurrType==PT_TYPE_PROPERTY)
			{
				ASSERT(m_pGamePropertyItem[m_cbPropertyStartIndex+wLastDown]!=NULL);
				if(m_cbPropertyStartIndex+wLastDown>=m_cbPropertyCount) return;
				m_pCurrPropertyItem = m_pGamePropertyItem[m_cbPropertyStartIndex+wLastDown];
				m_pCurrPropertyItem->ShowBuyPropertyWnd(m_pIGamePropertySink);				
			}

            //礼物类型
			if(m_cbCurrType==PT_TYPE_PRESENT)
			{
				ASSERT(m_pGamePreSentItem[m_cbPresentStartIndex+wLastDown]!=NULL);
				if(m_cbPresentStartIndex+wLastDown>=m_cbPresentCount) return;
				m_pCurrPropertyItem = m_pGamePreSentItem[m_cbPresentStartIndex+wLastDown];
				m_pCurrPropertyItem->ShowBuyPropertyWnd(m_pIGamePropertySink);				
			}

		}
	}

	return;
}

//鼠标消息
VOID CGamePropertyCtrl::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//鼠标扑获
	SetCapture();

	//动作处理
	if (m_wDownItem==INVALID_WORD)
	{
		//设置变量
		m_wDownItem=SwitchItemFromPoint(MousePoint);

		//更新界面
		if (m_wDownItem!=INVALID_WORD)
		{
			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//鼠标消息
LRESULT CGamePropertyCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bHovering=false;

	//设置界面
	if (m_wHoverItem!=INVALID_WORD) 
	{
		//设置变量
		m_wHoverItem=INVALID_WORD;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return 0;
}

//光标消息
BOOL CGamePropertyCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//点击判断
	WORD wLastItem=m_wHoverItem;
	WORD wHoverItem=SwitchItemFromPoint(MousePoint);

	//相同判断
	if (wHoverItem!=m_wHoverItem)
	{
		//设置变量
		m_wHoverItem=wHoverItem;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	//设置光标
	if (m_wHoverItem!=INVALID_WORD)
	{
		SetCursor(LoadCursor(GetModuleHandle(GAME_PROPERTY_DLL_NAME),MAKEINTRESOURCE(IDC_HAND_CUR)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//左移按钮
VOID CGamePropertyCtrl::OnBtnLeftMoveClicked()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	INT nItemCount=(rcClient.Width()-20)/m_SizeImageItem.cx;

	//礼物索引
	if(m_cbPresentStartIndex>0)
		m_cbPresentStartIndex--;

	//道具索引
	if(m_cbPropertyStartIndex>0)
		m_cbPropertyStartIndex--;

	//调整控件
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
}

//右移按钮
VOID CGamePropertyCtrl::OnBtnRightMoveClicked()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	INT nItemCount=(rcClient.Width()-20)/m_SizeImageItem.cx;

	//礼物索引
	if(m_cbPresentStartIndex+nItemCount<m_cbPresentCount)
		m_cbPresentStartIndex++;

	//道具索引
	if(m_cbPropertyStartIndex+nItemCount<m_cbPropertyCount)
		m_cbPropertyStartIndex++;

	//调整控件
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
}

//创建对象
int CGamePropertyCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//变量定义
	CRect rcCreate(0,0,0,0);

	//创建按钮
	m_btLeftMove.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_LEFT_MOVE);
	m_btRightMove.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_RIGHT_MOVE);

	//加载资源
	m_btLeftMove.SetButtonImage(IDB_BT_LEFT,GetModuleHandle(GAME_PROPERTY_DLL_NAME),true,false);
    m_btRightMove.SetButtonImage(IDB_BT_RIGHT,GetModuleHandle(GAME_PROPERTY_DLL_NAME),true,false);

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////