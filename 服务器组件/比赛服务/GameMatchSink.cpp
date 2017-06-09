#include "StdAfx.h"
#include "GameMatchSink.h"


//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CGameMatchSink::m_wPlayerCount=MAX_CHAIR;			//游戏人数

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameMatchSink::CGameMatchSink()
{
	//游戏变量
	
	//运行信息
	
	
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameEventSink=NULL;

	return;
}

//析构函数
CGameMatchSink::~CGameMatchSink(void)
{
	SafeRelease(m_pGameEventSink);
	SafeRelease(m_pITableFrame);
}

//接口查询
void *  CGameMatchSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameMatchSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	//QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	//QUERYINTERFACE(IMainServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameMatchSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool  CGameMatchSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//游戏开始
bool CGameMatchSink::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{
	if(m_pGameEventSink!=NULL)
		m_pGameEventSink->OnEventGameStart(pITableFrame, wChairCount);
	return true;
}

//游戏结束
bool  CGameMatchSink::OnEventGameEnd(ITableFrame *pITableFrame, WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason )
{
	if(m_pGameEventSink!=NULL)
		m_pGameEventSink->OnEventGameEnd(pITableFrame,wChairID,pIServerUserItem,cbReason);
	return false;
}

//发送场景
bool  CGameMatchSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	return true;
}

bool CGameMatchSink::SetGameEventSink(IUnknownEx * pIUnknownEx)
{
	m_pGameEventSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameEventSink);
	return true;
}

//玩家是否可以起立 
bool CGameMatchSink::IsCanStanUp(IServerUserItem *pUserItem)
{
	if(m_pGameEventSink==NULL)
		return true;
	return m_pGameEventSink->IsCanStanUp(pUserItem);
}

//玩家断线或重连 
void CGameMatchSink::SetUserOffline(WORD wChairID, bool bOffline)
{
	if(m_pGameEventSink==NULL)
		return ;	
	m_pGameEventSink->SetUserOffline(m_pITableFrame->GetTableID(),m_pITableFrame->GetTableUserItem(wChairID),bOffline);
}

//用户坐下
bool CGameMatchSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(m_pGameEventSink!=NULL)  
		return m_pGameEventSink->OnActionUserSitDown(m_pITableFrame->GetTableID(),wChairID, pIServerUserItem, bLookonUser);
	else
	{
		CTraceService::TraceString(TEXT("玩家坐下桌子非比赛桌子"),TraceLevel_Exception);
	}

	return false;
}

//用户起来
bool CGameMatchSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(m_pGameEventSink!=NULL) 
		return m_pGameEventSink->OnActionUserStandUp(m_pITableFrame->GetTableID(),wChairID, pIServerUserItem, bLookonUser);

	return false;
}

//用户同意
bool CGameMatchSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	if(m_pGameEventSink!=NULL)
		return m_pGameEventSink->OnActionUserOnReady(m_pITableFrame->GetTableID(),wChairID, pIServerUserItem ,pData ,wDataSize);

	return false;
}

//////////////////////////////////////////////////////////////////////////
