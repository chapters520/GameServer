#ifndef CHAMPIONSHIP_MATCH_HEAD_FILE
#define CHAMPIONSHIP_MATCH_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameMatchSink.h"
#include "MatchServiceHead.h"

typedef CMap<DWORD,DWORD,IServerUserItem *, IServerUserItem *>CUserItemMap;
typedef CMap<IServerUserItem *, IServerUserItem *, DWORD, DWORD>CUserSeatMap;

#define IDI_TURN_MATCH_START		(IDI_MATCH_MODULE_START+1)
#define IDI_DELETE_OUT_USER		 	(IDI_MATCH_MODULE_START+2)					//延迟一段时间删除淘汰用户
#define IDI_START_MATCH_GAME		(IDI_MATCH_MODULE_START+3)					//开始一盘比赛  
#define IDI_DELETE_OVER_MATCH		(IDI_MATCH_MODULE_START+5)					//删除结束的比赛组
#define IDI_NORMAL_TIMER			(IDI_MATCH_MODULE_START+6)					//常规定时器
#define IDI_START_MATCH_START		(IDI_MATCH_MODULE_START+7)					//开始一场比赛 
#define IDI_START_MATCH_END			(IDI_MATCH_MODULE_START+9)					//开始一场比赛 
#define IDI_HALF_MINUTE				(IDI_MATCH_MODULE_START+10)					//30秒
#define IDI_GROUP_START				(IDI_MATCH_MODULE_START+60)					//比赛组内的定时器
#define IDI_STAR_NEXT_ROUND			(IDI_GROUP_START+1)							//开始下一轮比赛
#define IDI_CHECK_MATCH_GROUP		(IDI_GROUP_START+2)							//轮巡所有开赛比赛组 
#define IDI_FINISH_FIRST_ROUND		(IDI_GROUP_START+3)							//强行结束初赛


//第一轮规则
enum enFirstRoundRule
{
	FirstRoundRule_Score,					//定时排名
	FirstRoundRule_Time						//打立出局
};

struct tagMatchRule
{
	DWORD				dwMatchID;							//比赛ID
	TCHAR				szMatchName[LEN_SERVER];			//比赛名称
	WORD				wUserCount;							//开赛人数
	WORD				wAndroidUserCount;					//一场比赛中机器人报名的数量
	WORD				wWaitTime;							//达到淘汰人数后等待多久确定晋级人数，单位：秒
	DWORD				dwInitalBase;						//初始基数
	BYTE				dwGrowthTime;						//增长时间
	BYTE				cbGrowthRange;						//增长幅度
	DWORD				dwFee;								//报名费
	DWORD				dwInitalScore;						//初始积分
	WORD				wRemainUserCount;					//剩余多少人停止淘汰
	DWORD				dwSecRoundUserCount;				//进入下一轮剩下的人数
	BYTE				cbLeastScore;						//最少积分
	BYTE				cbFirScoreRule;						//进入复赛的带分规则
	BYTE				cbFirGameCount;						//初赛N局换一次桌
	BYTE				cbSecGameCount;						//复赛N局换一次桌
	BYTE				cbSecScoreRule;						//复赛带分规则
	BYTE				cbFirRoundCount;					//初赛阶段游戏局数
	BYTE				cbSecRoundCount;					//复赛阶段游戏轮次
	WORD				arrSecRoundUserCount[16];			//复赛阶段各轮游戏人数
	enFirstRoundRule	enFirstRule;						//第一轮规则
	DWORD				dwFirstRoundTime;					//第一轮时长，定时排名有效，单位：秒
	DWORD				dwAheadStatistics;					//提前统计成绩 单位：秒
	
	tagMatchRule()
	{
		ZeroMemory(this, sizeof(tagMatchRule));
	}
};

#ifdef _UNICODE
#define VER_IChampionshipMatch INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatch={0x786c5501,0xa7a4,0x4531,0x0091,0xc5,0x3e,0x07,0x3e,0xa6,0x0a,0xf9};
#else
#define VER_IChampionshipMatch INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatch={0x1a4e0406,0x8ddc,0x4e34,0x0083,0x89,0xb5,0x5e,0x4e,0xfa,0x6b,0xb7};
#endif

interface IChampionshipMatch :public IUnknownEx
{
	//设置比赛规则
	virtual bool SetMatchRule(tagMatchRule *pRule)=NULL;
	//用户报名
	virtual bool OnUserSignUp(IServerUserItem *pUserItem)=NULL;
	//用户退赛
	virtual bool OnUserQuitMatch(IServerUserItem *pUserItem,bool bByUser=false)=NULL;
	//分配桌子
	virtual void SetMatchTable(ITableFrame *pTable[], WORD wTableCount)=NULL;
};

#ifdef _UNICODE
#define VER_IChampionshipMatchSink INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatchSink={0x30e4794a,0x610e,0x4873,0x00bd,0x23,0x9c,0xc7,0x2c,0xbe,0xd4,0xc5};
#else
#define VER_IChampionshipMatchSink INTERFACE_VERSION(1,1)
static const GUID IID_IChampionshipMatchSink={0xfc8dc7b5,0x2318,0x411f,0x00b9,0x09,0x6c,0xdd,0x39,0x38,0x0d,0x18};
#endif

class CChampionshipMatch;

interface IChampionshipMatchSink:public IUnknownEx
{
	//报名人满，开始比赛
	virtual bool  OnMatchStart(CChampionshipMatch *pMatch)=NULL;
	//等待超时
	virtual bool  OnWaitTimeOut(DWORD dwMatchNO)=NULL;
	//用户淘汰
	virtual bool  OnUserOut(IServerUserItem *pUserItem,LPCTSTR szReason,WORD wLen,WORD wRank,CChampionshipMatch *pMatch)=NULL;
	//发送数据
	virtual bool  SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//定时器
	virtual bool  SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter,CChampionshipMatch *pMatch)=NULL;
	//发送消息
	virtual bool  SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//写入奖励
	virtual bool  WriteUserAward(IServerUserItem *pIServerUserItem, WORD wRank,DWORD dwMatchNO,LPCTSTR szDescribe,CChampionshipMatch *pMatch)=NULL;
	//比赛结束
	virtual bool  OnEventMatchOver(CChampionshipMatch *pMatch)=NULL;
	//发送用户分数
	virtual bool  SendGroupUserScore(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//发送状态
	virtual bool  SendGroupUserStatus(IServerUserItem * pIServerUserItem,CChampionshipMatch *pMatch)=NULL;
	//发送数据到服务器 
	virtual bool  SendDataToGameServer(DWORD dwAndroidID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
	//发送数据到一组用户
	virtual bool  SendGroupData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize,CChampionshipMatch *pMatch)=NULL;
	//获取一个空闲的机器人
	virtual IAndroidUserItem * GetFreeAndroidUserItem()=NULL;
	//设置替补机器人
	virtual bool SetReplaceAndroidItem(IServerUserItem *pIServerUserItem)=NULL;
};
typedef CWHArray<IServerUserItem *> CMatchUserItemArray;

struct ITableFramEx 
{
	ITableFrame *pTableFrame;
	BYTE cbGameCount;
	DWORD dwBase;
	BYTE cbOnSeatUserCount;
	bool bOffline[MAX_CHAIR];
	BYTE cbStartTimes;
};
typedef CWHArray<ITableFramEx *>CTableFrameMananerArray;
struct tagMatchScore
{
	DWORD dwUserID;
	IServerUserItem *pUserItem;
	SCORE  lScore;
};

typedef CWHArray<tagMatchScore *> CUserMatchScoreArray;

//比赛状态
enum enMatchStatus
{
	MS_NULL_STATUS=0,
	MS_SIGNUPING,
	MS_STOP_OUT,
	MS_FIRST_ROUND,
	MS_START_NEXT_ROUND,
	MS_SECOND_ROUND
};

class CChampionshipMatch:public IGameEventSink
{
	friend class CGameMatchServiceManager;

protected:
	static WORD						m_wChairCount;					//椅子数目
	DWORD							m_dwMatchNO;
	CUserItemMap					m_OnMatchUserMap;				//参赛用户
	CUserItemMap					m_QuitUserMap;					//掉线和强退的用户
	tagMatchRule					*m_pMatchRule;		
	IChampionshipMatchSink			*m_pMatchSink;
	ITableFramEx					*m_pAssignTable;
	WORD							m_wTableCount;					//分配的桌子的个数
	DWORD							m_dwCurBase;					// 当前的基数
	bool							m_bAssignTable;					//桌子分配与否

	DWORD							m_dwTimeCount;
	bool							m_bStarted;
	
	bool							m_bGameEnd;	
	CTableFrameMananerArray			m_WaitStartTable;				//等待开始的桌子	
	enMatchStatus					m_enMatchStatus;				//比赛阶段
	WORD							m_wPlayingTableCount;			//达到淘汰人数时正在游戏的桌子数
	WORD							m_wCurRountSec;					//当前正处于复赛的哪阶段
	WORD							m_wFinishTable;					//完成比赛的桌子数
	CMatchUserItemArray				m_MaybeOutUser;					//有可能淘汰的用户
	CUserMatchScoreArray			m_OutUserArray;					//淘汰用户,等待最后结果
	bool							m_bIsCanStandUp;				//是否可以起立
	CUserSeatMap					m_UserSeatMap;					//用户分配的座位和椅子
	CUserItemMap					m_CanReadyUserMap;				//允许准备
	CTime							m_tMatchStart;					//开始时间
	bool							m_bWillOver;					//即将结束
	WORD							m_wAndroidUserCount;			//机器人个数
	DWORD							m_dwWaitTime;					//等待时间
	bool							m_bException;					//是否发生异常
	
public:
	CChampionshipMatch(tagMatchRule *pMatchRule, DWORD dwMatchNO,IChampionshipMatchSink *pSink);
	virtual ~CChampionshipMatch(void);

	//设置比赛规则
	bool SetMatchRule(tagMatchRule *pRule);
public:
 	//释放对象
 	virtual VOID  Release(){}
 	//接口查询
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);
	//设置回调接口
	 bool SetMatchSink(IChampionshipMatchSink *pSink);
	//用户报名
	 bool OnUserSignUp(IServerUserItem *pUserItem);
	//用户退赛
	 BOOL OnUserQuitMatch(IServerUserItem *pUserItem,bool bByUser=false);
	//分配桌子
	 void SetMatchTable(ITableFrame *pTable[], WORD wTableCount);
	 //获取空座位
	 ITableFramEx* GetEmptyChair(WORD &wTableID, WORD &wChairID);
	 //开始比赛
	 bool StartMatch(ITableFramEx *pTableFrame=NULL);
	 //定时器
	 bool OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter);

	 //游戏开始
	 virtual bool  OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount);
	 //游戏结束
	 virtual bool  OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	 //是否可以离开座位 
	 virtual bool  IsCanStanUp(IServerUserItem *pUserItem);
	 //玩家断线或重连 
	 virtual void  SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline);
	 //用户坐下
	 virtual bool OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //用户起来
	 virtual bool OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	 //用户同意
	 virtual bool OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

protected:
	//选择晋级玩家
	void SelectPromotionUser();
	//玩家排序
	WORD SortMapUser(tagMatchScore score[]);
	//玩家排序
	WORD SortUserItemArray(tagMatchScore score[], CMatchUserItemArray &MatchUserItemArray);
	//将淘汰的玩家排序
	WORD SortOutUserArray(tagMatchScore *score[]);
	//重新安排所有玩家座位
	void RearrangeUserSeat(tagMatchScore score[], INT_PTR nCount);
	//获取名次
	WORD GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame=NULL);
	//获取用户
	IServerUserItem * GetSeatUserItem(DWORD dwSeatID);
	//定时排名游戏结束
	void FirstRoundRuleTimeGameEnd(ITableFramEx *pITableFrameEx);
	//打立出局游戏结束
	void FirstRoundRuleScoreGameEnd(ITableFramEx *pITableFrameEx,WORD wTableIndex);
    //完成一轮
	bool OnTableFinishRound(ITableFramEx *pITableFrameEx);
	//比赛结束
	void MatchOver();

	//用户数目
	WORD GetSitUserCount(ITableFrame *pTableFrame);
	//获取空椅
	WORD GetNullChairID(ITableFrame *pTableFrame);
	//发送提示
	VOID SendWaitTip(ITableFrame *pTableFrame);
};


#endif