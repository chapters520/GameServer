#include "Stdafx.h"
#include "Resource.h"
#include "VideoServiceControl.h"

//////////////////////////////////////////////////////////////////////////

//窗口标识
#define IDC_VIDEO_SNED				100									//发送窗口
#define IDC_VIDEO_RCEC				100									//接收窗口
#define IDC_VIDEO_SR				100									//视频窗口

//按钮标识
#define IDC_FLOWER					103									//送花按钮
#define IDC_VIDEO					104									//视频按钮
#define IDC_AUDIO					105									//音频按钮
#define IDC_CLONE					106									//克隆按钮
#define IDC_MORE					107									//更多按钮


//位置标识
#define X_SMALL_POS					5									//起始坐标
#define Y_SMALL_POS					4									//起始坐标
#define X_LARGE_POS					5									//起始坐标
#define Y_LARGE_POS					5									//起始坐标

//宽度标识
#define W_SMALL_POS					90									//宽度坐标
#define H_SMALL_POS					60									//高度坐标
#define W_LARGE_POS					161									//宽度坐标
#define H_LARGE_POS					120									//高度坐标

//////////////////////////////////////////////////////////////////////////

//菜单命令
#define	IDM_UM_COPY_NAME			WM_USER+100							//拷贝名字
#define	IDM_UM_SET_CHAT				WM_USER+101							//设置聊天
#define	IDM_UM_CANECL_CHAT			WM_USER+102							//取消聊天
#define	IDM_UM_ALLOW_LOOKON			WM_USER+103							//旁观控制
#define	IDM_UM_ENJOIN_LOOKON		WM_USER+104							//旁观控制


//鲜花菜单
#define	IDM_UM_FLOWER				WM_USER+306							//赠送礼物


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CVideoServiceControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_FLOWER,OnBnClickedFlower)
	ON_BN_CLICKED(IDC_VIDEO,OnBnClickedVideo)
	ON_BN_CLICKED(IDC_AUDIO,OnBnClickedAudio)
	ON_BN_CLICKED(IDC_CLONE,OnBnClickedClone)
	ON_BN_CLICKED(IDC_MORE,OnBnClickedMore)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CVideoServiceManager, CWnd)
	ON_MESSAGE(WM_GV_CONNECT,OnGVClientConnect)
	ON_MESSAGE(WM_GV_LOGINSYSTEM,OnGVClientLogin)
	ON_MESSAGE(WM_GV_ENTERROOM,OnGVClientEnterRoom)
	ON_MESSAGE(WM_GV_ONLINEUSER,OnGVClientOnlineUser)
	ON_MESSAGE(WM_GV_USERATROOM,OnGVClientUserAtRoom)
	ON_MESSAGE(WM_GV_LINKCLOSE,OnGVClientLinkClose)
	ON_MESSAGE(WM_GV_CAMERASTATE,OnGVClientCameraState)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//静态变量
CVideoServiceManager * CVideoServiceManager::m_pVideoServiceManager=NULL;//管理对象
HINSTANCE CVideoServiceManager::m_hResInstance=NULL;
//tagVideoFrameImage CVideoServiceManager::m_VideoFrameImage;
//tagPropertyMenuImage CVideoServiceManager::m_PropertyMenuImage;


//全局变量
bool							g_bClientClosed=true;

//////////////////////////////////////////////////////////////////////////
//回调函数

//视频数据回调函数
void CALLBACK VideoDataCallBack(DWORD dwUserID, LPVOID lpBuf, DWORD dwLen, BITMAPINFOHEADER bmiHeader, LPVOID lpUserValue)
{
	//关闭判断
	if (g_bClientClosed) return;

	CVideoServiceManager* pVideoServiceManager=(CVideoServiceManager*)lpUserValue;

	if(pVideoServiceManager&&pVideoServiceManager->m_hWnd!=NULL) 
	{
		//获取玩家
		IClientKernel *pIClientKernel=pVideoServiceManager->GetClientKernel();
		if (pIClientKernel==NULL) return;
		if (pIClientKernel->IsServiceStatus()==false) return;
 		IClientUserItem *pClientUserItem=pVideoServiceManager->GetClientKernel()->SearchUserByUserID(dwUserID);
		if (pClientUserItem==NULL) return;

		//绘画视频
		WORD wVideoIndex=pVideoServiceManager->GetVideoIndex(pClientUserItem->GetUserInfo());
		if (wVideoIndex==MAX_CHAIR) return;
		CVideoServiceControl *pVideoServiceControl=pVideoServiceManager->GetVideoServiceControl(wVideoIndex);
		pVideoServiceControl->DrawUserVideo(dwUserID,lpBuf,dwLen,bmiHeader);
	}
}

//音频回调
void CALLBACK AudioDataCallBack(DWORD dwUserID, LPVOID lpBuf, DWORD dwLen, WAVEFORMATEX waveFormatEx, LPVOID lpUserValue)
{
	//关闭判断
	if (g_bClientClosed) return;

	CVideoServiceManager* pVideoServiceManager=(CVideoServiceManager*)lpUserValue;
	if(pVideoServiceManager) 
	{
		//获取玩家
		IClientUserItem *pUserData=pVideoServiceManager->GetClientKernel()->SearchUserByGameID(dwUserID);
		if (pUserData==NULL) return;

		//获取对象		
		WORD wVideoIndex=pVideoServiceManager->GetVideoIndex(pUserData->GetUserInfo());
		if (wVideoIndex==MAX_CHAIR) return;
		CVideoServiceControl *pVideoServiceControl=pVideoServiceManager->GetVideoServiceControl(wVideoIndex);
		ASSERT(pVideoServiceControl!=NULL);
		if (pVideoServiceControl==NULL) return;
		ASSERT(pVideoServiceControl->GetSafeHwnd()!=NULL);
		if (pVideoServiceControl->GetSafeHwnd()==NULL) return;

		// 计算音量
		USHORT* pData = (USHORT*)lpBuf;
		DWORD	datalen = dwLen/2;
		LONGLONG totalvolume = 0;
		for(int i=0; i<(int)datalen; i++)
		{
			totalvolume += *(pData + i);
		}
		DWORD dwCurrentVolume=(DWORD)((totalvolume% (0xFFFF-1))*100)/(0xFFFF-1);

		//绘画音波
		pVideoServiceControl->SetVolume(dwCurrentVolume);
	}
}

//构造函数
CVideoServiceControl::CVideoServiceControl()
{
	//内核变量
	m_bInitVideo=false;
	m_bShowVideo=false;

	//控制变量
	m_bSmallMode=false;
	m_bLocalMode=false;
	m_bEnableVideo=true;
	m_bEnableAudio=true;

	//状态变量
	m_fVolumePercent=0.0f;

	//送花变量
	m_dwTargerUserID = 0;

	//用户信息
	m_dwUserID=0L;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//送花变量
	m_pUserData=NULL;

	//加载资源
	HINSTANCE hResInstance=GetModuleHandle(VIDEO_SERVICE_DLL_NAME);
	m_ImageSmallPhoto.LoadFromResource(hResInstance,IDB_SMALL_PHOTO);
	m_ImageLargePhoto.LoadFromResource(hResInstance,IDB_LARGE_PHOTO);
	m_ImageSmallFrame.LoadFromResource(hResInstance,IDB_SMALL_FRAME);
	m_ImageLargeFrame.LoadFromResource(hResInstance,IDB_LARGE_FRAME);

	m_ImageLargeVolume.LoadFromResource	 (hResInstance,IDB_LARGE_VOLUME);
	m_ImageLargeVolumeEx.LoadFromResource(hResInstance,IDB_LARGE_VOLUME_EX);
	m_ImageSmallVolumeEx.LoadFromResource(hResInstance,IDB_SMALL_VOLUME_EX);
	m_ImageSmallVolume.LoadFromResource  (hResInstance,IDB_SMALL_VOLUME);	

	return;
}

//析构函数
CVideoServiceControl::~CVideoServiceControl()
{
	g_bClientClosed=true;
}

//消息解释
BOOL CVideoServiceControl::PreTranslateMessage(MSG * pMsg)
{
	//提示消息
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//设置音波
void CVideoServiceControl::SetVolume(DWORD dwCurrentVolume)
{
	//计算比例
	m_fVolumePercent = dwCurrentVolume/(100.f);
	if (m_fVolumePercent>1.0) m_fVolumePercent=1.0f;

	//更新界面
	InvalidateRect(&m_rcPaintVolume);
}

//大小模式
bool CVideoServiceControl::SetSmallMode(bool bSmallMode)
{
	//设置变量
	if (m_bSmallMode!=bSmallMode)
	{
		//设置变量
		m_bSmallMode=bSmallMode;

		//调整窗口
		RectifyControl();
	}

	return true;
}

//获取大小
void CVideoServiceControl::GetVideoSize(CRect &rcVideo)
{
	if (m_bSmallMode==true) rcVideo.SetRect(X_SMALL_POS,Y_SMALL_POS,X_SMALL_POS+W_SMALL_POS,Y_SMALL_POS+H_SMALL_POS);
	else rcVideo.SetRect(X_LARGE_POS,Y_LARGE_POS,X_LARGE_POS+W_LARGE_POS,Y_LARGE_POS+H_LARGE_POS);
}

//设置用户
bool CVideoServiceControl::SetVideoUserInfo(DWORD dwUserID, LPCTSTR pszAccounts)
{
	//设置变量
	m_dwUserID=dwUserID;
	lstrcpyn(m_szAccounts,pszAccounts,CountArray(m_szAccounts));

	//刷新窗口
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return true;
}

//配置视频
bool CVideoServiceControl::InitVideoService(bool bLocalMode, bool bSmallMode)
{
	//效验状态
	ASSERT((m_hWnd!=NULL)&&(m_bInitVideo==false));
	if ((m_hWnd==NULL)||(m_bInitVideo==true)) return false;

	//获取对象
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

	//设置变量
	m_bInitVideo=true;
	m_bSmallMode=bSmallMode;
	m_bLocalMode=bLocalMode;

	//设置按钮
	HINSTANCE hResInstance=GetModuleHandle(VIDEO_SERVICE_DLL_NAME);
	m_btMore.SetButtonImage(m_bSmallMode?IDB_BT_SMALL_MORE:IDB_BT_BIG_MORE,hResInstance,false,false);
	m_btVideo.SetButtonImage(m_bSmallMode?IDB_BT_SMALL_SEE:IDB_BT_BIG_SEE,hResInstance,false,false);
	m_btAudio.SetButtonImage(m_bSmallMode?IDB_BT_SMALL_TALK:IDB_BT_BIG_TALK,hResInstance,false,false);
	m_btClone.SetButtonImage(m_bSmallMode?IDB_BT_SMALL_RESORE:IDB_BT_BIG_RESORE,hResInstance,false,false);
	m_btFlower.SetButtonImage(m_bSmallMode?IDB_BT_SMALL_FLOWER:IDB_BT_BIG_FLOWER,hResInstance,false,false);


	//调整窗口
	RectifyControl();

	//音波区域	
	if (m_bSmallMode==true)
	{
		//语音图片
// 		CImageHandle HandleFrame(&m_ImageSmallFrame);
// 		CImageHandle ImageVolume(&m_ImageSmallVolume);

		m_rcPaintVolume.left = (m_ImageSmallFrame.GetWidth()-m_ImageSmallVolume.GetWidth())/2;
		m_rcPaintVolume.top = H_SMALL_POS+8;
		m_rcPaintVolume.right = m_rcPaintVolume.left + m_ImageSmallVolume.GetWidth();
		m_rcPaintVolume.bottom = m_rcPaintVolume.top + m_ImageSmallVolume.GetHeight();
	}
	else
	{
		//语音图片
// 		CImageHandle HandleFrame(&m_ImageLargeFrame);
// 		CImageHandle ImageVolume(&m_ImageLargeVolume);

		m_rcPaintVolume.left = (m_ImageLargeFrame.GetWidth()-m_ImageLargeVolume.GetWidth())/2;;
		m_rcPaintVolume.top = H_LARGE_POS+10;
		m_rcPaintVolume.right = m_rcPaintVolume.left + m_ImageLargeVolume.GetWidth();
		m_rcPaintVolume.bottom = m_rcPaintVolume.top + m_ImageLargeVolume.GetHeight();
	}
	return true;
}

//调整窗口
VOID CVideoServiceControl::RectifyControl()
{
	//窗口判断
	if (m_hWnd==NULL) return;

	//调整窗口
	if (m_bInitVideo==true)
	{
		//加载资源
		CBitImage *pImageFrame;
		if (m_bSmallMode==true) pImageFrame=&m_ImageSmallFrame;
		if (m_bSmallMode==false) pImageFrame=&m_ImageLargeFrame;

		//调整窗口
		INT nFrameWidth=pImageFrame->GetWidth();
		INT nFrameHeight=pImageFrame->GetHeight();
		SetWindowPos(NULL,0,0,nFrameWidth,nFrameHeight,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);

		//变量定义
		INT nSpace=m_bSmallMode?1:10;
		INT nXCenterPos=pImageFrame->GetWidth()/2;
		INT nYPos=m_bSmallMode?H_SMALL_POS+13:H_LARGE_POS+18;
		const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

		//按钮位置
		CRect rcButton;
		m_btFlower.GetWindowRect(&rcButton);

		//移动按钮
		HDWP hDwp=BeginDeferWindowPos(32);
		DeferWindowPos(hDwp,m_btFlower,NULL,nXCenterPos-rcButton.Width()*5/2-nSpace*2,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btVideo,NULL,nXCenterPos-rcButton.Width()*3/2-nSpace,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btAudio,NULL,nXCenterPos-rcButton.Width()/2,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btClone,NULL,nXCenterPos+rcButton.Width()/2+nSpace,nYPos,0,0,uFlags);
		DeferWindowPos(hDwp,m_btMore,NULL,nXCenterPos+rcButton.Width()*3/2+nSpace*2,nYPos,0,0,uFlags);
		EndDeferWindowPos(hDwp);
	}
	else
	{
		//缩小窗口
		SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOCOPYBITS);
	}

	//重画窗口
	Invalidate(TRUE);

	return;
}

//克隆按钮
VOID CVideoServiceControl::OnBnClickedClone()
{
	if (m_bShowVideo==true)
	{
		if (m_DlgVideoClone.m_hWnd==NULL) m_DlgVideoClone.Create(IDD_VIDEO_CLONE,this);
		m_DlgVideoClone.ShowWindow(SW_SHOW);
		m_DlgVideoClone.CenterWindow();
		m_DlgVideoClone.BringWindowToTop();
	}
}

//重画函数
VOID CVideoServiceControl::OnPaint()
{
	CPaintDC dcPaint(this);

	//配置判断
	ASSERT(m_bInitVideo==true);
	if (m_bInitVideo==false) return;

	//获取大小
	CRect rcClient;
	GetClientRect(&rcClient);

	//内存DC
	CDC dcBuffer;
	dcBuffer.CreateCompatibleDC(&dcPaint);
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(&dcPaint, rcClient.Width(), rcClient.Height());
	dcBuffer.SelectObject(&BufferBmp);

	//区域判断
	CRect rcClipBox;
	dcPaint.GetClipBox(rcClipBox);
	if ( rcClipBox != m_rcPaintVolume )
	{
		//绘画背景
		if (m_bSmallMode==true)
		{
//			CImageHandle HandleFrame(&m_ImageSmallFrame);
			m_ImageSmallFrame.BitBlt(dcBuffer,0,0);

			//语音图片
//			CImageHandle ImageVolume(&m_ImageSmallVolume);

			INT nXPos=(m_ImageSmallFrame.GetWidth()-m_ImageSmallVolume.GetWidth())/2;
			INT nYPos=H_SMALL_POS+8;
			m_ImageSmallVolume.BitBlt(dcBuffer,nXPos, nYPos);
		}
		else
		{
			//CImageHandle HandleFrame(&m_ImageLargeFrame);
			m_ImageLargeFrame.BitBlt(dcBuffer,0,0);

			//语音图片
			//CImageHandle ImageVolume(&m_ImageLargeVolume);

			INT nXPos=(m_ImageLargeFrame.GetWidth()-m_ImageLargeVolume.GetWidth())/2;
			INT nYPos=H_LARGE_POS+10;
			m_ImageLargeVolume.BitBlt(dcBuffer,nXPos, nYPos);
		}

		//视频标识
		if (m_bShowVideo==false)
		{
			if (m_bSmallMode==true)
			{
				m_ImageSmallPhoto.BitBlt(dcBuffer,X_SMALL_POS+(W_SMALL_POS-m_ImageSmallPhoto.GetWidth())/2,Y_SMALL_POS+(H_SMALL_POS-m_ImageSmallPhoto.GetHeight())/2);
			}
			else
			{
				m_ImageLargePhoto.BitBlt(dcBuffer,X_LARGE_POS+(W_LARGE_POS-m_ImageLargePhoto.GetWidth())/2,Y_LARGE_POS+(H_LARGE_POS-m_ImageLargePhoto.GetHeight())/2);
			}
		}

		dcPaint.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcBuffer, 0, 0, SRCCOPY);
	}
	else
	{
		if (m_bSmallMode==true)
		{
			//语音图片
			m_ImageSmallVolume.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top);
			m_ImageSmallVolumeEx.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top,int(m_rcPaintVolume.Width()*m_fVolumePercent),m_rcPaintVolume.Height(),0,0);
		}
		else
		{
			//语音图片
			m_ImageLargeVolume.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top);
			m_ImageLargeVolumeEx.BitBlt(dcBuffer,m_rcPaintVolume.left, m_rcPaintVolume.top,int(m_rcPaintVolume.Width()*m_fVolumePercent),m_rcPaintVolume.Height(),0,0);
		}

		dcPaint.BitBlt(rcClipBox.left, rcClipBox.top, rcClipBox.Width(), rcClipBox.Height(), &dcBuffer, rcClipBox.left, rcClipBox.top, SRCCOPY);
	}

	//清理资源
	dcBuffer.DeleteDC();
	BufferBmp.DeleteObject();	

	return;
}

//创建函数
INT CVideoServiceControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//修改窗口属性
	ModifyStyle(0,WS_CLIPCHILDREN);

	//创建按钮
	CRect rcCreate(0,0,0,0);
	m_btMore.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MORE);
	m_btVideo.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_VIDEO);
	m_btAudio.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUDIO);
	m_btClone.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLONE);
	m_btFlower.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_FLOWER);

	//建立提示
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMore, TEXT("查看信息"));
	m_ToolTipCtrl.AddTool(&m_btVideo, TEXT("禁止视频"));
	m_ToolTipCtrl.AddTool(&m_btAudio, TEXT("禁止语音"));
	m_ToolTipCtrl.AddTool(&m_btClone, TEXT("缩放视频"));
	m_ToolTipCtrl.AddTool(&m_btFlower, TEXT("赠送鲜花"));

	//调整窗口
	RectifyControl();

	return 0;
}

//送花按钮
VOID CVideoServiceControl::OnBnClickedFlower()
{
// 		//获取目标
// 	m_dwTargerUserID = GetVideoUserID();
// 	if ( m_dwTargerUserID == 0 ) return;
// 
// 	//菜单命令
// 	UINT uCommands[FLOWER_COUNT] ;
// 	for ( WORD wCmdIdx = 0; wCmdIdx < FLOWER_COUNT; ++wCmdIdx )
// 		uCommands[wCmdIdx] = IDM_UM_FLOWER + wCmdIdx;
// 
// 	//菜单位图
// 	CBitmap MenuBmps[FLOWER_COUNT];
// 	AfxSetResourceHandle(CVideoServiceManager::m_hResInstance);
// 	tagPropertyMenuImage & PropertyMenuImage=CVideoServiceManager::m_PropertyMenuImage;
// 
// 	//加载位图
// 	for(WORD i=0;i<FLOWER_COUNT;i++)
// 	{
// 		MenuBmps[i].LoadBitmap( PropertyMenuImage.uMenuGift[i] );
// 	}
// 
// 	AfxSetResourceHandle(GetModuleHandle(NULL));
//     
//     //创建菜单
// 	CMenu GiftMenu;
// 	GiftMenu.CreatePopupMenu();
// 
// 	//设置菜单
// 	for ( UINT uMenuItem = 0; uMenuItem < FLOWER_COUNT; ++uMenuItem )
// 	{
// 		//菜单信息
// 		CString strMenuItem = g_FlowerDescribe[uMenuItem].szName;
// 		CString strGiftPay;
// 		strGiftPay.Format("%ld", CPropertyBar::m_FlowerInfo[uMenuItem].lPrice);
// 		strMenuItem += strGiftPay;
// 
// 		//增加菜单
// 		GiftMenu.AppendMenu( 0, uCommands[uMenuItem], strMenuItem);
// 		GiftMenu.SetMenuItemBitmaps(uCommands[uMenuItem], MF_BYCOMMAND,&MenuBmps[uMenuItem], &MenuBmps[uMenuItem] );
// 	}	
// 
// 	//获取位置
// 	CPoint MousePoint;
// 	GetCursorPos(&MousePoint);
// 
// 	GiftMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
// 
// 	//释放资源
// 	for ( INT nBmpIdx = 0; nBmpIdx < FLOWER_COUNT; ++nBmpIdx ) MenuBmps[nBmpIdx].DeleteObject();
// 	GiftMenu.DestroyMenu();

	return;
}

//菜单命令
BOOL CVideoServiceControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//提取命令
	UINT uCmd = LOWORD(wParam);

// 	if ( IDM_UM_FLOWER <= uCmd && uCmd <= IDM_UM_FLOWER + FLOWER_COUNT )
// 	{
// 		//索引转换
// 		UINT uBaseCmd = IDM_UM_FLOWER;
// 		UINT uFlowerIdx = uCmd - uBaseCmd;
// 
// 		//构造信息
// 		CString strMsg;
// 		LONG lValue = CPropertyBar::m_FlowerInfo[uFlowerIdx].lPrice;
// 		strMsg.Format(TEXT("你选择了赠送［%s］，将会扣除[%ld]金豆。\n按“是”继续\n按“否”取消这次操作"),g_FlowerDescribe[uFlowerIdx].szName, lValue, lValue );
// 		if(IDYES!=MessageBox(strMsg,TEXT("温馨提示"),MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2))
// 			return TRUE;
// 
// 		//存在判断
// 		DWORD dwRcvUserID = GetVideoUserID();
// 		if ( dwRcvUserID == 0 || m_dwTargerUserID != dwRcvUserID )
// 		{
// 			MessageBox(strMsg,TEXT("温馨提示"), MB_ICONINFORMATION );
// 			return TRUE;
// 		}
// 
// 		//获取对象
// 		ASSERT(CVideoServiceManager::GetInstance()!=NULL);
// 		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
// 		IClientKernel *pIClientKernel = pVideoServiceManager->m_pIClientKernel;
// 		ASSERT( pIClientKernel != NULL );
// 
// 		//构造数据
// 		CMD_GF_Gift Gift;
// 		ZeroMemory( &Gift, sizeof( Gift ) );
// 
// 		Gift.cbSendLocation = LOCATION_GAME_ROOM;
// 		Gift.wFlowerCount=1;
// 		Gift.dwRcvUserID = GetVideoUserID();
// 		Gift.dwSendUserID = pVideoServiceManager->m_dwLocalUserID;
// 		Gift.wGiftID = uFlowerIdx;
// 
// 		//发送消息
// 		pIClientKernel->SendSocketData( MDM_GF_PRESENT,SUB_GF_FLOWER,&Gift, sizeof(Gift));
// 
// 		return TRUE;
// 	}

	//获取对象
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	ASSERT(pVideoServiceManager!=NULL) ;

	//发送数据
	IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
	ASSERT(pIClientKernel!=NULL) ;

	switch (LOWORD(wParam))
	{
	case IDM_UM_COPY_NAME:		//拷贝名字
		{
			if ( m_pUserData == NULL ) return TRUE;

			//寻找玩家
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//打开剪切板
			if (OpenClipboard()==FALSE) return TRUE;
			if (EmptyClipboard()==FALSE) 
			{
				CloseClipboard();
				return TRUE;
			}

			//复制数据
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,LEN_NICKNAME);
			if (hData==NULL) 
			{
				CloseClipboard();
				return TRUE;
			}
			LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
			lstrcpy(szMemName,m_pUserData->szNickName);
			SetClipboardData(CF_TEXT,hData);
			GlobalUnlock(hData);
			CloseClipboard();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//视频按钮
VOID CVideoServiceControl::OnBnClickedVideo()
{
	//合法判断
	if (m_dwUserID==0) return ;

	//获取设备
	CHAR szDeviceName[128];
	DWORD dwReturnValue=BRAC_GetCurVideoCapture(szDeviceName,CountArray(szDeviceName));
	if (dwReturnValue!=GV_ERR_SUCCESS || szDeviceName[0]==0) return;

	//设置变量
	m_bEnableVideo=!m_bEnableVideo;

	//设置按钮
	HINSTANCE hInstance=GetModuleHandle(VIDEO_SERVICE_DLL_NAME);
	if (m_bSmallMode==false) m_btVideo.SetButtonImage((m_bEnableVideo==true)?IDB_BT_BIG_SEE:IDB_BT_BIG_NOSEE,hInstance,false,false);
	if (m_bSmallMode==true) m_btVideo.SetButtonImage((m_bEnableVideo==true)?IDB_BT_SMALL_SEE:IDB_BT_SMALL_NOSEE,hInstance,false,false);

	if ( m_bEnableVideo ) m_ToolTipCtrl.AddTool(&m_btVideo, TEXT("禁止视频"));
	else m_ToolTipCtrl.AddTool(&m_btVideo, TEXT("启用视频"));

	BRAC_UserCameraControl(m_dwUserID,m_bEnableVideo);
	m_bShowVideo=m_bEnableVideo;

	//重画控件
	Invalidate(FALSE);

	return;
}

//音频按钮
VOID CVideoServiceControl::OnBnClickedAudio()
{
	//合法判断
	if (m_dwUserID==0) return ;

	//设置变量
	m_bEnableAudio=!m_bEnableAudio;

	//设置按钮
	HINSTANCE hInstance=GetModuleHandle(VIDEO_SERVICE_DLL_NAME);
	if (m_bSmallMode==false) m_btAudio.SetButtonImage((m_bEnableAudio==true)?IDB_BT_BIG_TALK:IDB_BT_BIG_NOTALK,hInstance,false,false);
	if (m_bSmallMode==true) m_btAudio.SetButtonImage((m_bEnableAudio==true)?IDB_BT_SMALL_TALK:IDB_BT_SMALL_NOTALK,hInstance,false,false);

	if ( m_bEnableAudio ) m_ToolTipCtrl.AddTool(&m_btAudio, TEXT("禁止语音"));
	else m_ToolTipCtrl.AddTool(&m_btAudio, TEXT("启用语音"));

	//音频控制
	BRAC_UserSpeakControl(m_dwUserID,m_bEnableAudio);

	//设置变量
	m_fVolumePercent=0.f;

	//更新界面
	InvalidateRect(m_rcPaintVolume);
	
	return;
}

//更多按钮
VOID CVideoServiceControl::OnBnClickedMore()
{
	//获取位置
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	//获取对象
	ASSERT(CVideoServiceManager::GetInstance()!=NULL);
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	ASSERT( pVideoServiceManager != NULL ) ;

	//获取接口
	IClientKernel * pIClientKernel=pVideoServiceManager->m_pIClientKernel;
	ASSERT( pIClientKernel != NULL ) ;

	//获取用户
	TCHAR szBuffer[512]=TEXT("");
	IClientUserItem *pUserItem=pIClientKernel->SearchUserByUserID(m_dwUserID);
	if(pUserItem==NULL) return;
	m_pUserData=pUserItem->GetUserInfo();
	if(m_pUserData==NULL) return;

	//预先计算
	double dFleeRate=0.0;
	DWORD dwPlayCount=m_pUserData->dwWinCount+m_pUserData->dwLostCount+m_pUserData->dwDrawCount+m_pUserData->dwFleeCount;
	if (m_pUserData->dwFleeCount>0)
	{
		dFleeRate=(double)(m_pUserData->dwFleeCount*100)/(double)dwPlayCount;
		if (dFleeRate<0.01) dFleeRate=0.0;
	}

	//构造菜单
	CSkinMenu UserInfoMenu;
	UserInfoMenu.CreateMenu();

	//功能菜单
	UserInfoMenu.AppendMenu(IDM_UM_COPY_NAME,TEXT("拷贝用户名"));
	UserInfoMenu.AppendSeparator();

	//用户称谓
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("用户名：%s"),m_pUserData->szNickName);
	UserInfoMenu.AppendMenu(0,szBuffer);

	//用户 ID
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("游戏 ID：%ld"),m_pUserData->dwUserID);
	UserInfoMenu.AppendMenu(0,szBuffer);

	//用户经验
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("经验值：%ld"),m_pUserData->dwExperience);
	UserInfoMenu.AppendMenu(0,szBuffer);

	//用户积分
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("积分：%ld  总局数：%ld  逃跑率：%.2f%%"),m_pUserData->lScore,dwPlayCount,dFleeRate);
	UserInfoMenu.AppendMenu(0,szBuffer);

	//获取用户
	const tagUserInfo* pMeUserData=pIClientKernel->GetMeUserItem()->GetUserInfo();

	//显示菜单
	UserInfoMenu.TrackPopupMenu(/*TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,*/this);

	return;
}

//绘制视频
void CVideoServiceControl::DrawUserVideo(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, BITMAPINFOHEADER bmiHeader)
{
	if (m_hWnd==NULL) return;

	//克隆判断
	if (m_DlgVideoClone.m_hWnd!=NULL) m_DlgVideoClone.DrawUserVideo(dwUserid, lpBuf, dwLen, bmiHeader);

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
	if (m_bSmallMode==true) rcVideo.SetRect(X_SMALL_POS,Y_SMALL_POS,X_SMALL_POS+W_SMALL_POS,Y_SMALL_POS+H_SMALL_POS);
	else rcVideo.SetRect(X_LARGE_POS,Y_LARGE_POS,X_LARGE_POS+W_LARGE_POS,Y_LARGE_POS+H_LARGE_POS);

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

	//设置标识
	m_bShowVideo=true;
}

//进入事件
bool CVideoServiceControl::OnEventUserEnter()
{
	return true;
}

//离开事件
bool CVideoServiceControl::OnEventUserLeave()
{
	CString strLog;
	strLog.Format(TEXT("%s 离开"),m_szAccounts);
	CVideoServiceManager::GetInstance()->AppendLogString(strLog);

	//设置标识
	m_bShowVideo=false;

	//用户信息
	m_dwUserID=0;	
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//更新界面
	InvalidateRect(NULL);
    
	return true;
}

//重置事件
bool CVideoServiceControl::OnEventReset()
{
	//内核变量
	m_bShowVideo=false;

	//控制变量
	m_bEnableVideo=true;
	m_bEnableAudio=true;

	//状态变量
	m_fVolumePercent=0.0f;

	//用户信息
	m_dwUserID=0L;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//送花变量
	m_pUserData=NULL;

	return true;
}

//摄像头事件
void CVideoServiceControl::OnEventCameraState(BYTE cbState)
{
	//设置标识
	if (cbState==GV_CAMERA_STATE_OPEN) m_bShowVideo=true;
	else m_bShowVideo=false;

	//更新界面
	InvalidateRect(NULL);
}
//////////////////////////////////////////////////////////////////////////

//构造函数
CVideoServiceManager::CVideoServiceManager()
{
	//视频信息
	m_wVideoPort=0;
	m_dwVideoServer=0L;
	m_dwLocalUserID=0L;
	ZeroMemory(m_dwUserID,sizeof(m_dwUserID));
	m_dwVideoRoomID=0;

	//组件变量
	m_pIClientKernel=NULL;
	m_pIMessageProxy=NULL;
	ZeroMemory(m_VideoServiceControl,sizeof(m_VideoServiceControl));

	//设置对象
	ASSERT(m_pVideoServiceManager==NULL);
	if (m_pVideoServiceManager==NULL) m_pVideoServiceManager=this;

	//功能按钮
// 	m_VideoFrameImage.uBtBigFlower  = DEF_IDB_BT_VIDEO_BIG_FLOWER  ; 
// 	m_VideoFrameImage.uBtBigMore    = DEF_IDB_BT_VIDEO_BIG_MORE    ;  
// 	m_VideoFrameImage.uBtBigNosee   = DEF_IDB_BT_VIDEO_BIG_NOSEE   ;  
// 	m_VideoFrameImage.uBtBigNotalk  = DEF_IDB_BT_VIDEO_BIG_NOTALK  ; 
// 	m_VideoFrameImage.uBtBigResore  = DEF_IDB_BT_VIDEO_BIG_RESORE  ; 
// 	m_VideoFrameImage.uBtBigSee     = DEF_IDB_BT_VIDEO_BIG_SEE     ;  
// 	m_VideoFrameImage.uBtBigTalk    = DEF_IDB_BT_VIDEO_BIG_TALK    ;  
// 	m_VideoFrameImage.uBtSmallFlower= DEF_IDB_BT_VIDEO_SMALL_FLOWER; 
// 	m_VideoFrameImage.uBtSmallMore  = DEF_IDB_BT_VIDEO_SMALL_MORE  ; 
// 	m_VideoFrameImage.uBtSmallNosee = DEF_IDB_BT_VIDEO_SMALL_NOSEE ; 
// 	m_VideoFrameImage.uBtSmallNotalk= DEF_IDB_BT_VIDEO_SMALL_NOTALK; 
// 	m_VideoFrameImage.uBtSmallResore= DEF_IDB_BT_VIDEO_SMALL_RESORE; 
// 	m_VideoFrameImage.uBtSmallSee   = DEF_IDB_BT_VIDEO_SMALL_SEE   ;  
// 	m_VideoFrameImage.uBtSmallTalk  = DEF_IDB_BT_VIDEO_SMALL_TALK  ; 
// 
// 	//框架资源
// 	m_VideoFrameImage.uBigFemale		=	DEF_IDB_VIDEO_LARGE_FEMALE	  ; 
// 	m_VideoFrameImage.uBigFrame		 =	DEF_IDB_VIDEO_LARGE_FRAME	  ; 
// 	m_VideoFrameImage.uBigMale		 =	DEF_IDB_VIDEO_LARGE_MALE	  ; 
// 	m_VideoFrameImage.uBigPhoto		=	DEF_IDB_VIDEO_LARGE_PHOTO	  ; 
// 	m_VideoFrameImage.uBigVolume		=	DEF_IDB_VIDEO_LARGE_VOLUME	  ; 
// 	m_VideoFrameImage.uBigVolumeEx	 =	DEF_IDB_VIDEO_LARGE_VOLUME_EX ;
// 	m_VideoFrameImage.uSmallFemale	 =	DEF_IDB_VIDEO_SMALL_FEMALE	  ; 
// 	m_VideoFrameImage.uSmallFrame		=	DEF_IDB_VIDEO_SMALL_FRAME	 ; 
// 	m_VideoFrameImage.uSmallMale		=	DEF_IDB_VIDEO_SMALL_MALE	  ; 
// 	m_VideoFrameImage.uSmallPhoto		=	DEF_IDB_VIDEO_SMALL_PHOTO	  ; 
// 	m_VideoFrameImage.uSmallVolume	=	DEF_IDB_VIDEO_SMALL_VOLUME	 ; 
// 	m_VideoFrameImage.uSmallVolumeEx =	DEF_IDB_VIDEO_SMALL_VOLUME_EX; 


	return;
}

//析构函数
CVideoServiceManager::~CVideoServiceManager()
{
	g_bClientClosed=true;
}

//设置接口
bool CVideoServiceManager::SetClientKernel(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernel)!=NULL);
		m_pIClientKernel=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IClientKernel);

		//成功判断
		if (m_pIClientKernel==NULL) return false;
	}
	else m_pIClientKernel=NULL;

	//创建窗体
	if (m_hWnd==NULL)
	{
		Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),GetDesktopWindow(),300);	
		g_bClientClosed=false;
	}

	return true;
}

//服务信息
bool CVideoServiceManager::SetVideoServiceInfo(DWORD dwLoaclUserID, DWORD dwVideoServer, WORD wVideoPort)
{
	//设置变量
	m_wVideoPort=wVideoPort;
	m_dwVideoServer=dwVideoServer;
	m_dwLocalUserID=dwLoaclUserID;

	return true;
}

//设置对象
bool CVideoServiceManager::SetVideoServiceControl(WORD wVideoIndex, CVideoServiceControl * pVideoServiceControl)
{
	//获取对象
	ASSERT(wVideoIndex<MAX_CHAIR);
	if (wVideoIndex>=MAX_CHAIR) return false;

	//设置对象
	m_VideoServiceControl[wVideoIndex]=pVideoServiceControl;

	return true;
}

//获取对象
CVideoServiceControl *CVideoServiceManager::GetVideoServiceControl(WORD wVideoIndex)
{
	//返回对象
	ASSERT(wVideoIndex<MAX_CHAIR);
	if (wVideoIndex>=MAX_CHAIR) return NULL;
	return m_VideoServiceControl[wVideoIndex];
}

//设置接口
bool CVideoServiceManager::SetMessageProxy(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIMessageProxy=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IStringMessage);
	ASSERT(m_pIMessageProxy!=NULL);
	return (m_pIMessageProxy!=NULL);
}

//登陆视频
void CVideoServiceManager::LoginVideoSystem()
{
	//重置判断
	if (m_dwVideoRoomID!=0) 
	{
		OnEventGameReset();
		return;
	}

	//旁观判断
	ASSERT(m_pIClientKernel!=NULL);
	if(m_pIClientKernel==NULL) return;
	IClientUserItem *pUserItem=m_pIClientKernel->GetMeUserItem();
	if(pUserItem==NULL) return;
	const tagUserInfo *pUserData=pUserItem->GetUserInfo();
	if (pUserData==NULL || pUserData->cbUserStatus==US_LOOKON) return;

	//配置视频
	DWORD dwFuncMode = BRAC_FUNC_VIDEO_CBDATA | BRAC_FUNC_AUDIO_AUTOPLAY | BRAC_FUNC_AUDIO_CBDATA;
	ASSERT(m_hWnd!=NULL);
	BRAC_InitSDK(m_hWnd,dwFuncMode);
	BRAC_SetVideoDataCallBack(BRAC_PIX_FMT_RGB24,VideoDataCallBack,this);
	BRAC_SetAudioDataCallBack(AudioDataCallBack,this);
	WORD wVideoIndex=GetVideoIndex(pUserData);
	if (wVideoIndex==MAX_CHAIR) return;
	CVideoServiceControl *pVideoServiceControl=GetVideoServiceControl(wVideoIndex);

	//地址转换
	BYTE *pByte=(BYTE*)&m_dwVideoServer;
	char szServerIPAddr[30] = {0};
	_snprintf(szServerIPAddr,CountArray(szServerIPAddr),"%d.%d.%d.%d",pByte[0],pByte[1],pByte[2],pByte[3]);

	//连接服务器
	DWORD dwReturnValue=0;
	dwReturnValue=BRAC_Connect((LPCTSTR)szServerIPAddr,m_wVideoPort);

	CString strLog;
	strLog.Format(TEXT("IP:%s,Port:%d"),CA2CT(szServerIPAddr),m_wVideoPort);
	//输出日志
	AppendLogString(strLog);

	if (dwReturnValue==0) strLog=TEXT("连接视频服务器...");
	else strLog.Format(TEXT("连接服务器失败，错误代码未：%ld"), dwReturnValue);

	//输出日志
	AppendLogString(strLog);

}

//重置视频
bool CVideoServiceManager::OnEventGameReset()
{
	//离开房间
	if (m_dwVideoRoomID!=0)
	{
		BRAC_LeaveRoom(m_dwVideoRoomID);
		m_dwVideoRoomID=0;
	}

	//旁观判断
	ASSERT(m_pIClientKernel!=NULL);
	if(m_pIClientKernel==NULL) return true;
	IClientUserItem *pUserItem=m_pIClientKernel->GetMeUserItem();
	if(pUserItem==NULL) return true;
	const tagUserInfo *pUserData=pUserItem->GetUserInfo();
	if (pUserData==NULL || pUserData->cbUserStatus==US_LOOKON) return true;

	//房间编号
	const tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
	m_dwVideoRoomID=pServerAttribute->wServerID*1000+pUserData->wTableID;

	//进入房间
	CString strLog;
	DWORD dwReturnValue=BRAC_EnterRoom(m_dwVideoRoomID,NULL,0);

	if (dwReturnValue==0) strLog=TEXT("登陆视频房间...");
	else strLog.Format(TEXT("登陆视频房间失败，错误代码未：%ld"), dwReturnValue);

	//输出日志
	AppendLogString(strLog);

	return true;
}


//显示日志
void CVideoServiceManager::AppendLogString(CString strLog)
{
#ifdef _DEBUG
	if (m_pIMessageProxy!=NULL) m_pIMessageProxy->InsertSystemString(strLog);
#endif
}

//获取索引
WORD CVideoServiceManager::GetVideoIndex(tagUserInfo const*pagUserData)
{
	ASSERT(pagUserData!=NULL);
	if (pagUserData==NULL) return MAX_CHAIR;
	WORD wChairID=pagUserData->wChairID;
	WORD wVideoIndex=m_pIClientKernel->SwitchViewChairID(wChairID);

	return wVideoIndex;
}

//获取索引
WORD CVideoServiceManager::GetVideoIndex(DWORD dwUserID)
{
	ASSERT(dwUserID!=0);
	for (WORD i=0; i<MAX_CHAIR; ++i) 
		if (m_dwUserID[i]==dwUserID)
			return i;

	ASSERT(FALSE);
	return MAX_CHAIR;
}

//连接事件
LRESULT CVideoServiceManager::OnGVClientConnect(WPARAM wParam, LPARAM lParam)
{
	AppendLogString(wParam ? TEXT("连接视频服务器成功..."):TEXT("连接视频服务器失败..."));

	if(wParam)
	{
		//登陆系统
		DWORD dwReturnValue=0;
		CString strLog;
		//旁观判断
		ASSERT(m_pIClientKernel!=NULL);
		if(m_pIClientKernel==NULL) return TRUE;
		IClientUserItem *pUserItem=m_pIClientKernel->GetMeUserItem();
		if(pUserItem==NULL) return TRUE;
		const tagUserInfo *pUserData=pUserItem->GetUserInfo();
		if (pUserData==NULL || pUserData->cbUserStatus==US_LOOKON) return TRUE;

		char szUserID[32], szVideoPassword[33];
		_snprintf(szUserID,CountArray(szUserID),"%ld",pUserData->dwUserID);
		//CWHEncrypt::MD5Encrypt(szUserID,szVideoPassword);
		szVideoPassword[32]=0;//ID的md5前17位
		dwReturnValue=BRAC_Login((LPCTSTR)szUserID,(LPCTSTR)szVideoPassword,0);

		if (dwReturnValue==0) strLog=TEXT("登陆视频服务器...");
		else strLog.Format(TEXT("登陆视频服务器失败，错误代码未：%ld"), dwReturnValue); 

		//输出日志
		AppendLogString(strLog);
	}

	return TRUE;
}

//登陆事件
LRESULT CVideoServiceManager::OnGVClientLogin(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	if (lParam==0)
	{	
		//合法判断
		ASSERT(m_dwLocalUserID==wParam);
		if (m_dwLocalUserID==wParam) strLog=TEXT("登陆视频服务器成功");
		else strLog.Format(TEXT("视频服务器返回用户标识和本地标识不一致，返回:%ld，本地:%ld"),wParam,m_dwLocalUserID);	

		//房间编号
		DWORD dwReturnValue=0;
		const tagServerAttribute *pServerAttribute=m_pIClientKernel->GetServerAttribute();
		if(m_pIClientKernel==NULL) return 1;
		IClientUserItem *pUserItem=m_pIClientKernel->GetMeUserItem();
		if(pUserItem==NULL) return 1;
		const tagUserInfo *pUserData=pUserItem->GetUserInfo();
		if (pUserData==NULL || pUserData->cbUserStatus==US_LOOKON) return 1;
//#ifndef _DEBUG
		m_dwVideoRoomID=pServerAttribute->wServerID*1000+pUserData->wTableID;
// #else
// 		m_dwVideoRoomID=pUserData->wTableID;
// #endif

		//进入房间
		dwReturnValue=BRAC_EnterRoom(m_dwVideoRoomID,NULL,0); 

		if (dwReturnValue==0) strLog=TEXT("登陆视频房间...");
		else strLog.Format(TEXT("登陆视频房间失败，错误代码未：%ld"), dwReturnValue);

		//输出日志
		AppendLogString(strLog);
	}
	else
	{
		strLog.Format(TEXT("登录视频服务器失败，出错代码为：%d "),lParam);
		m_dwLocalUserID=0;
	}

	//输出日志
	AppendLogString(strLog);

	return TRUE;
}

//进入事件
LRESULT CVideoServiceManager::OnGVClientEnterRoom(WPARAM wParam, LPARAM lParam)
{
	CString strLog;
	if (lParam==0)
	{
		strLog.Format(TEXT("成功进入编号为：%d 的视频房间"),wParam);

		//设置视频
		CRect rcVideo;
		if(m_pIClientKernel==NULL) return TRUE;
		IClientUserItem *pUserItem=m_pIClientKernel->GetMeUserItem();
		if(pUserItem==NULL) return TRUE;
		const tagUserInfo *pUserData=pUserItem->GetUserInfo();
		if (pUserData==NULL || pUserData->cbUserStatus==US_LOOKON) return TRUE;

		WORD wVideoIndex=GetVideoIndex(pUserData);
		if (wVideoIndex==MAX_CHAIR) return TRUE;
		CVideoServiceControl *pVideoServiceControl=m_VideoServiceControl[wVideoIndex];
		pVideoServiceControl->GetVideoSize(rcVideo);
		BRAC_SetVideoPos(m_dwLocalUserID,pVideoServiceControl->GetSafeHwnd(),rcVideo.left,rcVideo.top,rcVideo.right,rcVideo.bottom);
		BRAC_UserCameraControl(m_dwLocalUserID,TRUE);
		BRAC_UserSpeakControl(m_dwLocalUserID,TRUE);

		//事件通知
		pVideoServiceControl->SetVideoUserInfo(m_dwLocalUserID,pUserData->szNickName);
		pVideoServiceControl->OnEventUserEnter();

		//保存标识
		m_dwUserID[wVideoIndex]=m_dwLocalUserID;
	}
	else
	{
		strLog.Format(TEXT("申请进入房间失败，出错代码为： %d "),lParam);
	}

	//输出日志
	AppendLogString(strLog);

	return TRUE;
}

//列表事件
LRESULT CVideoServiceManager::OnGVClientOnlineUser(WPARAM wParam, LPARAM lParam)
{
	//获取人数
	DWORD dwUserNum = 0;
	BRAC_GetOnlineUser(NULL,dwUserNum);
	if(!dwUserNum) return TRUE;

	//获取列表
	DWORD dwUserList[MAX_CHAIR];
	BRAC_GetOnlineUser(dwUserList,dwUserNum);

	for(INT i=0; i< (INT)dwUserNum; i++)
	{
		//获取玩家
		DWORD dwUserID=dwUserList[i];
		IClientUserItem *pUserItem=m_pIClientKernel->SearchUserByUserID(dwUserID);
		if (pUserItem==NULL)
		{
			//输出日志
			CString strLog;
			strLog.Format(TEXT("ID为：%ld 的玩家在进入游戏平台前进入视频系统，视频显示失败！"),dwUserID);
			AppendLogString(strLog);
			continue;
		}
		tagUserInfo *pUserData=pUserItem->GetUserInfo();
		if(pUserData==NULL) continue;

		//设置视频
		CRect rcVideo;
		WORD wVideoIndex=GetVideoIndex(pUserData);
		if (wVideoIndex==MAX_CHAIR) return TRUE;
		CVideoServiceControl *pVideoServiceControl=m_VideoServiceControl[wVideoIndex];
		pVideoServiceControl->GetVideoSize(rcVideo);
		BRAC_SetVideoPos(dwUserID,pVideoServiceControl->GetSafeHwnd(),rcVideo.left,rcVideo.top,rcVideo.right,rcVideo.bottom);
		BRAC_UserCameraControl(dwUserID,TRUE);
		BRAC_UserSpeakControl(dwUserID,TRUE);

		//保存标识
		m_dwUserID[wVideoIndex]=dwUserID;

		//事件通知
		pVideoServiceControl->SetVideoUserInfo(pUserData->dwUserID,pUserData->szNickName);
		pVideoServiceControl->OnEventUserEnter();
	}

	return TRUE;
}

//进入事件
LRESULT CVideoServiceManager::OnGVClientUserAtRoom(WPARAM wParam, LPARAM lParam)
{	
	DWORD dwUserID=(DWORD)wParam;
	BOOL bEnter=(BOOL)lParam;

#ifdef _DEBUG
	CString strLog;
	strLog.Format(TEXT("编号为：%ld 的用户 %s 房间"),dwUserID, bEnter ? TEXT("进入") : TEXT("离开"));
	AppendLogString(strLog);
#endif	


	if (bEnter)
	{
		//获取玩家
		IClientUserItem *pUserItem=m_pIClientKernel->SearchUserByUserID(dwUserID);
		ASSERT(pUserItem!=NULL);
		if (pUserItem==NULL)
		{
			//输出日志
			CString strLog;
			strLog.Format(TEXT("ID为：%ld 的玩家在进入游戏平台前进入视频系统，视频显示失败！"),dwUserID);
			AppendLogString(strLog);
			return TRUE;
		}
		tagUserInfo *pUserData=pUserItem->GetUserInfo();
		if(pUserData==NULL) return TRUE;

		//获取控件
		CRect rcVideo;
		WORD wVideoIndex=GetVideoIndex(pUserData);
		if (wVideoIndex==MAX_CHAIR) return TRUE;
		CVideoServiceControl *pVideoServiceControl=m_VideoServiceControl[wVideoIndex];

		//设置视频
		pVideoServiceControl->GetVideoSize(rcVideo);
		BRAC_SetVideoPos(dwUserID,pVideoServiceControl->GetSafeHwnd(),rcVideo.left,rcVideo.top,rcVideo.right,rcVideo.bottom);
		BRAC_UserCameraControl(dwUserID,TRUE);
		BRAC_UserSpeakControl(dwUserID,TRUE);

		//事件通知
		pVideoServiceControl->SetVideoUserInfo(pUserData->dwUserID,pUserData->szNickName);
		pVideoServiceControl->OnEventUserEnter();

		//保存标识
		m_dwUserID[wVideoIndex]=dwUserID;
	}
	else
	{
		//获取对象
		WORD wVideoIndex=GetVideoIndex(dwUserID);
		if (wVideoIndex==MAX_CHAIR) return TRUE;
		CVideoServiceControl *pVideoServiceControl=m_VideoServiceControl[wVideoIndex];
		ASSERT(pVideoServiceControl!=NULL);
		if (pVideoServiceControl==NULL) return TRUE;

		//事件通知
		m_dwUserID[wVideoIndex]=0;
		pVideoServiceControl->OnEventUserLeave();
		pVideoServiceControl->OnEventReset();
	}

	return TRUE;
}

//断网事件
LRESULT CVideoServiceManager::OnGVClientLinkClose(WPARAM wParam, LPARAM lParam)
{
	//输出日志
	AppendLogString(TEXT("你与视频服务器断开连接，暂时不能视频"));

	//获取玩家
	if(m_pIClientKernel==NULL) return TRUE;
	IClientUserItem *pUserItem=m_pIClientKernel->GetMeUserItem();
	if(pUserItem==NULL) return TRUE;
	const tagUserInfo *pUserData=pUserItem->GetUserInfo();
	if (pUserData==NULL || pUserData->cbUserStatus==US_LOOKON) return TRUE;

	WORD wVideoIndex=GetVideoIndex(pUserData);
	if (wVideoIndex==MAX_CHAIR) return TRUE;
	CVideoServiceControl *pVideoServiceControl=m_VideoServiceControl[wVideoIndex];

	//事件通知
	pVideoServiceControl->OnEventUserLeave();

	return TRUE;
}

//摄像头事件
LRESULT CVideoServiceManager::OnGVClientCameraState(WPARAM wParam, LPARAM lParam)
{
	//玩家标识
	DWORD dwUserID=(DWORD)wParam;

	//获取对象
	WORD wVideoIndex=GetVideoIndex(dwUserID);
	if (wVideoIndex==MAX_CHAIR) return TRUE;
	CVideoServiceControl *pVideoServiceControl=GetVideoServiceControl(wVideoIndex);
	ASSERT(pVideoServiceControl!=NULL);
	if (pVideoServiceControl==NULL) return TRUE;

	//事件通知
	pVideoServiceControl->OnEventCameraState(BYTE(lParam));

	return TRUE;
}

//设置资源
void CVideoServiceManager::SetSkinResource(HINSTANCE hResInstance/*,tagVideoFrameImage *pVideoFrameImage,tagPropertyMenuImage *pPropertyMenuImage*/)
{
	//资源变量
	m_hResInstance=hResInstance;
//	CopyMemory(&m_VideoFrameImage,pVideoFrameImage,sizeof(m_VideoFrameImage));
//	CopyMemory(&m_PropertyMenuImage,pPropertyMenuImage,sizeof(m_PropertyMenuImage));

	return ;
}

//登出系统
void CVideoServiceManager::LogoutVideoSystem()
{
	//离开房间
	if (m_dwVideoRoomID!=0)
	{
		BRAC_LeaveRoom(m_dwVideoRoomID);
		m_dwVideoRoomID=0;
	}

	//注销系统
	BRAC_Logout();

	g_bClientClosed=true;
}
//////////////////////////////////////////////////////////////////////////
