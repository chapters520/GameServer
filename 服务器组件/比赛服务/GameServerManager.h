#ifndef GAME_SERVER_MANAGER_HEAD_FILE
#define GAME_SERVER_MANAGER_HEAD_FILE

#pragma once

//导出定义
#ifndef MATCH_SERVICE_CLASS
#ifdef  MATCH_SERVICE_DLL
#define MATCH_SERVICE_CLASS _declspec(dllexport)
#else
#define MATCH_SERVICE_CLASS _declspec(dllimport)
#endif
#endif

//模块定义
#ifndef _DEBUG
#define MATCH_SERVICE_DLL_NAME	TEXT("MatchService.dll")		//组件名字
#else
#define MATCH_SERVICE_DLL_NAME	TEXT("MatchServiceD.dll")		//组件名字
#endif

#include "Stdafx.h"
#include "ChampionshipMatch.h"
 
class CGameMatchSink;
//////////////////////////////////////////////////////////////////////////
//定时器子项
struct tagMatchTimerItem
{
	DWORD								dwTimerID;						//定时器 ID
	DWORD								dwRealID;						//实际ID
	DWORD								dwRepeatTimes;					//重复次数
	WPARAM								wBindParam;						//绑定参数
};

//比赛奖励
struct tagMatchReward
{
	DWORD								dwGold;							//金币奖励
	DWORD								dwMedal;						//奖牌奖励
	DWORD								dwExperience;					//经验奖励
};

//类说明
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;
typedef CWHArray<CChampionshipMatch*>CMatchGroupArray;
typedef CWHArray<tagMatchReward*> CMatchRewardArray;
typedef CWHArray<tagMatchTimerItem *> CTimerItemPtr;


//游戏服务器管理类
class MATCH_SERVICE_CLASS CGameServiceManager : public IGameMatchServiceManager,IChampionshipMatchSink
{
	//变量定义
protected:
	CMatchUserItemArray					m_OnMatchUserItem;				//参赛用户
	CMatchUserItemArray					m_OutUserItem;					//淘汰的用户
	tagGameServiceOption *				m_pGameServiceOption;			//服务配置
	tagGameServiceAttrib *				m_pGameServiceAttrib;			//服务属性

	//接口变量
protected:
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;		//网络接口
	IDataBaseEngine *					m_pIDataBaseEngine;				//数据引擎
	ITimerEngine *						m_pITimerEngine;				//时间引擎
	IMainServiceFrame				*  m_pIGameServiceFrame;			//功能接口
	IServerUserManager *				m_pIServerUserManager;			//用户管理
	ITableFrame							** m_ppITableFrame;				//框架接口
	IAndroidUserManager					*m_pAndroidUserManager;			//机器管理
	bool								*m_bTableUsed;					//桌子是否已经分配了

	CMatchGroupArray					m_MatchGroup;	
	CChampionshipMatch					*m_pCurMatch;
	tagMatchRule						m_MatchRule;
	CMatchRewardArray					m_RewardArr;						//奖励
	WORD								m_wMatchIDCount;
	CMatchGroupArray					m_OverGroup;					//已经结束待回收桌子的分组
	DWORD								m_dwStartTimeID;				//开始定时
	CMD_GR_MatchDesc					m_MatchDesc;					//信息描述
	CWHArray<WORD,WORD>					m_UnRecovery;					//没有回收的桌子


	CTimerItemPtr						m_TimerItemFree;				//空闲数组
	CTimerItemPtr						m_TimerItemActive;				//活动数组
	//函数定义
public:
	//构造函数
	CGameServiceManager(void);
	//析构函数
	virtual ~CGameServiceManager(void);

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { }
	//接口查询
	virtual VOID *  QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//控制接口
public:
	//停止服务
	virtual bool  StopService();
	//启动服务
	virtual bool  StartService();
	//管理接口
public:
	//初始化接口
	virtual bool  InitMatchInterface(ITCPNetworkEngineEvent *pTCPNetworkEngine,IDataBaseEngine *pIDataBaseEngine, IServerUserManager *pIServerUserManager,
		IMainServiceFrame *pIMainServiceFrame,ITimerEngine *pITimerEngine,IAndroidUserManager *pAndroidUserManager);
	//初始化桌子框架
	virtual bool  InitTableFrame(ITableFrame * pTableFrame,WORD wChairID);
	
	//系统事件
public:
	//时间事件
	virtual bool  OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//参数接口
public:
	//修改参数
	virtual bool  RectifyServiceOption(tagGameServiceOption * pGameServiceOption,tagGameServiceAttrib *pGameServiceAttrib);
	//排序用户
	virtual void  UserSort(WORD wTeamID);
	//信息接口
public:
	//用户参加比赛
	virtual bool  OnUserJoinGame(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//用户退出比赛
	virtual bool  OnUserQuitGame(IServerUserItem * pIServerUserItem, BYTE cbReason,WORD *pBestRank=NULL, DWORD dwContextID=INVALID_WORD);
	//用户比赛成绩
	virtual bool  OnUserMatchResult(DWORD dwUserID, WORD wKindID, WORD wServerID, WORD wSortID);
	//获取比赛信息
//	virtual bool  OnGetGameMatchInfo(CMD_GR_GameMatchInfo &MatchInfo);

	//创建接口
public:
	//建立比赛模式
	virtual VOID *  CreateGameMatchSink(REFGUID Guid, DWORD dwQueryVer);
	//网络接口
public:
	//发送数据
	virtual bool  SendDataToServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	//命令消息
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);

	//内部功能
private:
	//删除用户
	bool DeleteUserItem(DWORD dwUserIndex);
	//发送消息
	bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
// 	//发送游戏消息
// 	bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//分配桌子
	bool AssignTable(CChampionshipMatch *pMatch, WORD wTableCount);
	//读取比赛规则
	bool ReadMatchOption(LPCTSTR lpFileName);
	
public:
	//报名人满，开始比赛
	virtual bool  OnMatchStart(CChampionshipMatch *pMatch);
	//等待超时
	virtual bool  OnWaitTimeOut(DWORD dwMatchNO);
	//用户淘汰
	virtual bool  OnUserOut(IServerUserItem *pUserItem,LPCTSTR szReason,WORD wLen,WORD wRank,CChampionshipMatch *pMatch);
	//发送数据
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//定时器
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch);
	//发送消息
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType);
	//写入奖励
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, WORD wRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch);
	//比赛结束
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch);
	//发送用户分数
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);
	//发送状态
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch);
	//发送数据到服务器 
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据到一组用户 
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch);
	//获取一个空闲的机器人
	virtual IAndroidUserItem * GetFreeAndroidUserItem();
};

//////////////////////////////////////////////////////////////////////////
//组件创建

//////////////////////////////////////////////////////////////////////////

#endif