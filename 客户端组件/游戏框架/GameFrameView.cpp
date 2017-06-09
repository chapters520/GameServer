#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameView.h"
#include "GameFrameEngine.h"


//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGameFrameView * CGameFrameView::m_pGameFrameView=NULL;					//游戏视图

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameView, CWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameView::CGameFrameView()
{
	//位置变量
	ZeroMemory(&m_ptClock,sizeof(m_ptClock));
	ZeroMemory(&m_ptReady,sizeof(m_ptReady));
	ZeroMemory(&m_ptAvatar,sizeof(m_ptAvatar));
	ZeroMemory(&m_ptNickName,sizeof(m_ptNickName));

	//用户变量
	ZeroMemory(m_wUserClock,sizeof(m_wUserClock));
	ZeroMemory(m_pIClientUserItem,sizeof(m_pIClientUserItem));

	//组件接口
	m_pIStringMessage=NULL;
	m_pIGameLevelParser=NULL;

	//设置变量
	ASSERT(m_pGameFrameView==NULL);
	if (m_pGameFrameView==NULL) m_pGameFrameView=this;
	m_wRollTextCount=0;

	//注册组件
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_VIEW,QUERY_ME_INTERFACE(IUnknownEx));

	//比赛变量
	m_pMatchInfo=NULL;
	m_pMatchWaitTip=NULL;
	m_pMatchResult=NULL;
	m_pDlgMatchAwards=NULL;

	return;
}

//析构函数
CGameFrameView::~CGameFrameView()
{
	//设置变量
	ASSERT(m_pGameFrameView==this);
	if (m_pGameFrameView==this) m_pGameFrameView=NULL;

	//删除对象
	if(m_pMatchInfo!=NULL)
		SafeDelete(m_pMatchInfo);

	//删除对象
	if(m_pMatchWaitTip!=NULL)
		SafeDelete(m_pMatchWaitTip);

	//删除对象
	if(m_pMatchResult!=NULL)
		SafeDelete(m_pMatchResult);

	if(m_pDlgMatchAwards!=NULL && m_pDlgMatchAwards->m_hWnd!=NULL)
		m_pDlgMatchAwards->DestroyWindow();

	if(m_pDlgMatchAwards!=NULL)
		SafeDelete(m_pDlgMatchAwards);

	return;
}

//接口查询
VOID * CGameFrameView::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameView,Guid,dwQueryVer);
	return NULL;
}

//窗口句柄
HWND CGameFrameView::GetGameViewHwnd()
{
	return m_hWnd;
}

//创建窗口
bool CGameFrameView::CreateGameViewWnd(CWnd * pParentWnd, UINT nID)
{
	//效验状态
	ASSERT(m_hWnd==NULL);
	if (m_hWnd!=NULL) return false;

	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//创建窗口
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rcCreate,pParentWnd,nID);

	//查询接口
	m_pIGameLevelParser=(IGameLevelParser *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_LEVEL_PARSER,IID_IGameLevelParser,VER_IGameLevelParser);

	return true;
}

//切换状态
VOID CGameFrameView::SwitchToReadyStatus()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//重置界面
	ResetGameView();

	//用户变量
	ZeroMemory(m_wUserClock,sizeof(m_wUserClock));
	ZeroMemory(m_pIClientUserItem,sizeof(m_pIClientUserItem));

	//更新界面
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//获取时间
WORD CGameFrameView::GetUserClock(WORD wChairID)
{
	//效验参数
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return 0;

	//获取时间
	return m_wUserClock[wChairID];
}

//设置时间
VOID CGameFrameView::SetUserClock(WORD wChairID, WORD wUserClock)
{
	//设置变量
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<MAX_CHAIR;i++)
		{
			m_wUserClock[i]=wUserClock;
		}
	}
	else
	{
		m_wUserClock[wChairID]=wUserClock;
	}

	//更新界面
	if(IsWindow(m_hWnd))
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
	}

	return;
}

//获取用户
IClientUserItem * CGameFrameView::GetClientUserItem(WORD wChairID)
{
	//效验参数
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return NULL;

	//获取用户
	return m_pIClientUserItem[wChairID];
}

//设置用户
VOID CGameFrameView::SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(wChairID<CountArray(m_pIClientUserItem));
	if (wChairID>=CountArray(m_pIClientUserItem)) return;

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置变量
	m_pIClientUserItem[wChairID]=pIClientUserItem;

	//更新界面
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());

	return;
}

//播放声音
bool CGameFrameView::PlayGameSound(LPCTSTR pszSoundName)
{
	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(pszSoundName);

	return true;
}

//播放声音
bool CGameFrameView::PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName)
{
	//播放声音
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	if (pGlobalUnits!=NULL) pGlobalUnits->PlayGameSound(hInstance,pszSoundName);

	return true;
}

//获取等级
LPCTSTR CGameFrameView::GetLevelDescribe(IClientUserItem * pIClientUserItem)
{
	//效验状态
	ASSERT(m_pIGameLevelParser!=NULL);
	if (m_pIGameLevelParser==NULL) return TEXT("");
	
	//获取等级
	return m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem);
}

//发送消息
LRESULT CGameFrameView::SendEngineMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	ASSERT(CGameFrameEngine::GetInstance()!=NULL);
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();

	//窗口效验
	ASSERT(IsWindow(pGameFrameEngine->GetSafeHwnd()));
	if (IsWindow(pGameFrameEngine->GetSafeHwnd())==FALSE) return 0L;

	//发送消息
	return pGameFrameEngine->SendMessage(uMessage,wParam,lParam);
}

//投递消息
LRESULT CGameFrameView::PostEngineMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//变量定义
	ASSERT(CGameFrameEngine::GetInstance()!=NULL);
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();

	//窗口效验
	ASSERT(IsWindow(pGameFrameEngine->GetSafeHwnd()));
	if (IsWindow(pGameFrameEngine->GetSafeHwnd())==FALSE) return 0L;

	//发送消息
	return pGameFrameEngine->PostMessage(uMessage,wParam,lParam);
}

//位置变化
VOID CGameFrameView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);
	
	m_wTextLeft=(cx-500)/2;

	//调整控件
	RectifyControl(cx,cy);

	//更新界面
	InvalidGameView(0,0,cx,cy);

	return;
}

//创建消息
INT CGameFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置背景
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//变量定义
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//设置接口
	m_pIGameLevelParser=pGlobalUnits->m_GameLevelParserModule.GetInterface();

	//头像
	m_ImageUserFace.LoadImage(this, GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("USER_FACE_IMAGE"));

	return 0;
}

//鼠标消息
VOID CGameFrameView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//显示菜单
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		if (m_pIClientUserItem[i]!=NULL)
		{
			//位置判断
			if (Point.x<=m_ptAvatar[i].x) continue;
			if (Point.y<=m_ptAvatar[i].y) continue;
			if (Point.x>=(m_ptAvatar[i].x+FACE_CX)) continue;
			if (Point.y>=(m_ptAvatar[i].y+FACE_CY)) continue;

			//查询接口
			IGameFrameControl * pIGameFrameControl=NULL;
			pIGameFrameControl=(IGameFrameControl *)g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_CONTROL,IID_IGameFrameControl,VER_IGameFrameControl);

			//显示菜单
			ASSERT(pIGameFrameControl!=NULL);
			if (pIGameFrameControl!=NULL) pIGameFrameControl->TrackUserItemMenu(m_pIClientUserItem[i]);

			break;
		}
	}

	return;
}

//鼠标消息
VOID CGameFrameView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//设置焦点
	SetFocus();

	return;
}

//银行消息
bool CGameFrameView::OnEventInsureMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	ASSERT(pData);

	switch(wSubCmdID)
	{
	case SUB_GR_USER_INSURE_INFO:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GR_S_UserInsureInfo));
			if (wDataSize<sizeof(CMD_GR_S_UserInsureInfo)) return false;

			//变量定义
			CMD_GR_S_UserInsureInfo * pUserInsureInfo=(CMD_GR_S_UserInsureInfo *)pData;
			
			//设定变量
			m_DlgInsureGame.m_wRevenueTake = pUserInsureInfo->wRevenueTake;
			m_DlgInsureGame.m_wRevenueTransfer = pUserInsureInfo->wRevenueTransfer;
			m_DlgInsureGame.m_lGameGold=pUserInsureInfo->lUserScore;
			m_DlgInsureGame.m_lStorageGold=pUserInsureInfo->lUserInsure;

			//更新界面
			m_DlgInsureGame.UpdateView();
		}
		break;
	case SUB_GR_USER_INSURE_SUCCESS:
		{
			//变量定义
			CMD_GR_S_UserInsureSuccess *pUserInsureSuccess = (CMD_GR_S_UserInsureSuccess *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString)));
			if (wDataSize<(sizeof(CMD_GR_S_UserInsureSuccess)-sizeof(pUserInsureSuccess->szDescribeString))) return false;

			//设置变量
			m_DlgInsureGame.m_lGameGold=pUserInsureSuccess->lUserScore;
			m_DlgInsureGame.m_lStorageGold=pUserInsureSuccess->lUserInsure;

			//显示消息
			if (pUserInsureSuccess->szDescribeString[0]!=0)
			{
				CInformation Information(this);
				Information.ShowMessageBox(pUserInsureSuccess->szDescribeString,MB_ICONINFORMATION,60);
			}
		}
		break;
	case SUB_GR_USER_INSURE_FAILURE:
		{
			//变量定义
			CMD_GR_S_UserInsureFailure *pUserInsureFailure = (CMD_GR_S_UserInsureFailure *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_GP_UserInsureFailure)-sizeof(pUserInsureFailure->szDescribeString))) return false;

			//显示消息
			if (pUserInsureFailure->szDescribeString[0]!=0)
			{
				CInformation Information(this);
				Information.ShowMessageBox(pUserInsureFailure->szDescribeString,MB_ICONERROR,60);
			}
		}
		break;
	}

	return true;
}

//设置属性
void CGameFrameView::SetInsureOption(bool blCanGetOnlyFree, bool blCanStore)
{
	//设置变量
	m_DlgInsureGame.m_blCanGetOnlyFree = blCanGetOnlyFree;
	m_DlgInsureGame.m_blCanStore = blCanStore;

	//更新显示
	if(m_DlgInsureGame.m_hWnd !=NULL)
	{
		m_DlgInsureGame.ShowItem();
	}
}

//银行存
void CGameFrameView::ShowInsureSave(IClientKernel *	pIClientKernel, CPoint pos)
{
	ASSERT(pIClientKernel!=NULL);
	if(pIClientKernel==NULL)return;

	if (NULL==m_DlgInsureGame.m_hWnd) m_DlgInsureGame.Create(IDD_BANK_STORAGE,this);

	if (!m_DlgInsureGame.IsWindowVisible() || !m_DlgInsureGame.m_bBankStorage)
	{
		m_DlgInsureGame.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserItem());
		m_DlgInsureGame.SetBankerActionType(true);
		m_DlgInsureGame.SetPostPoint(pos);

		m_DlgInsureGame.ShowWindow(SW_SHOW);
	}
	else
		m_DlgInsureGame.ShowWindow(SW_HIDE);
}

//银行取
void CGameFrameView::ShowInsureGet(IClientKernel * pIClientKernel, CPoint pos)
{
	ASSERT(pIClientKernel!=NULL);
	if(pIClientKernel==NULL)return;

	if (NULL==m_DlgInsureGame.m_hWnd) m_DlgInsureGame.Create(IDD_BANK_STORAGE,this);

	if (!m_DlgInsureGame.IsWindowVisible() || m_DlgInsureGame.m_bBankStorage)
	{
		m_DlgInsureGame.SetSendInfo(pIClientKernel,pIClientKernel->GetMeUserItem());
		m_DlgInsureGame.SetBankerActionType(false);
		m_DlgInsureGame.SetPostPoint(pos);

		m_DlgInsureGame.ShowWindow(SW_SHOW);
	}
	else
		m_DlgInsureGame.ShowWindow(SW_HIDE);
}
//比赛信息
void CGameFrameView::SetMatchInfo(CMD_GR_Match_Info *pMatchInfo)
{
	//创建对象
	if(m_pMatchInfo==NULL)
		m_pMatchInfo=new CMD_GR_Match_Info;

	//拷贝对象
	CopyMemory(m_pMatchInfo, pMatchInfo, sizeof(CMD_GR_Match_Info));
}

//等待提示
void CGameFrameView::SetMatchWaitTip(CMD_GR_Match_Wait_Tip *pMatchWaitTip)
{
	if(pMatchWaitTip==NULL)
	{
		if(m_pMatchWaitTip!=NULL)
		{
			SafeDelete(m_pMatchWaitTip);
		}
	}
	else
	{
		//创建对象
		if(m_pMatchWaitTip==NULL && pMatchWaitTip!=NULL)
			m_pMatchWaitTip=new CMD_GR_Match_Wait_Tip;

		//拷贝对象
		CopyMemory(m_pMatchWaitTip,pMatchWaitTip,sizeof(CMD_GR_Match_Wait_Tip));
	}

	CRect rcClient;
	GetClientRect(&rcClient);
	InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
}

//比赛结果
void CGameFrameView::SetMatchResult(CMD_GR_MatchResult *pMatchResult)
{
	if(m_pMatchResult==NULL)
		m_pMatchResult=new CMD_GR_MatchResult;

	CopyMemory(m_pMatchResult,pMatchResult,sizeof(CMD_GR_MatchResult));

	if(m_pDlgMatchAwards==NULL)
		m_pDlgMatchAwards=new CDlgMatchAwards;

	if(m_pDlgMatchAwards->m_hWnd==NULL)
		m_pDlgMatchAwards->Create(IDD_DLG_MATCH_AWARDS, this);

	m_pDlgMatchAwards->SetMatchResult(m_pMatchResult);
	m_pDlgMatchAwards->ShowWindow(SW_SHOW);
}
//////////////////////////////////////////////////////////////////////////////////


