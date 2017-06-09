#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define VER_IGameEventSink INTERFACE_VERSION(1,1)
static const GUID IID_IGameEventSink={0x9d49ab20,0x472c,0x4b3a,0x00bc,0xb4,0x92,0xfe,0x8c,0x41,0xcd,0xaa};
#else
#define VER_IGameEventSink INTERFACE_VERSION(1,1)
static const GUID IID_IGameEventSink={0x91cf29a0,0x04d3,0x48da,0x0083,0x36,0x64,0xb2,0xda,0x6a,0x21,0xdb};
#endif

interface IGameEventSink :public IUnknownEx
{
	//游戏开始
	virtual bool  OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)=NULL;
	//游戏结束
	virtual bool  OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;
	//是否可以离开座位 
	virtual bool  IsCanStanUp(IServerUserItem *pUserItem)=NULL;
	//玩家断线或重连 
	virtual void  SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline)=NULL;

	//用户坐下
	virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//用户起来
	virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//用户同意
	virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};
//游戏比赛类
class CGameMatchSink : public IGameMatchSink,public ITableUserAction
{
	//友元定义
	friend class CGameServiceManager;
	//游戏变量
protected:
	
	//运行信息
protected:
	
	//接口变量
protected:
	ITableFrame						* m_pITableFrame;					//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;				//配置参数
	IGameEventSink					* m_pGameEventSink;	
	
	//属性变量
protected:
	static const WORD				m_wPlayerCount;						//游戏人数

	//函数定义
public:
	//构造函数
	CGameMatchSink();
	//析构函数
	virtual ~CGameMatchSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { }
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CGameMatchSink))?true:false; }
	//接口查询
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool  InitTableFrameSink(IUnknownEx * pIUnknownEx);

	//信息接口
public:	
	//游戏事件
public:
	//游戏开始
	virtual bool  OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	//游戏结束
	virtual bool  OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool  SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool  OnTimerMessage(WORD wTimerID, WPARAM wBindParam){return false;}
	//游戏消息
	virtual bool  OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem){return false;}
	//框架消息
	virtual bool  OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem){return false;}
	//设置桌子事件接口 
	virtual bool  SetGameEventSink(IUnknownEx * pIUnknownEx);
	//玩家是否可以起立
	virtual bool  IsCanStanUp(IServerUserItem *pUserItem);
	//玩家断线或重连
	virtual void  SetUserOffline(WORD wChairID, bool bOffline);

	//用户动作
public:	
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);	
};

//////////////////////////////////////////////////////////////////////////

#endif