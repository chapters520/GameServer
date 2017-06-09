#ifndef MATCH_SERVICE_HEAD_H
#define MATCH_SERVICE_HEAD_H
#include "resource.h"
#pragma once

#include "Stdafx.h"
#include "DistributeManager.h"
#include "TableFrame.h"
#include "afxwin.h"

//////////////////////////////////////////////////////////////////////////
//数组说明
typedef CWHArray<CTableFrame *>		CTableFrameArray;					//桌子数组
typedef CMap<CTableFrame*,CTableFrame*,DWORD,DWORD> CTableFrameMap;		//桌子容器
typedef CMap<DWORD,DWORD,IServerUserItem*,IServerUserItem*> CIServerUserItemMap;
typedef CWHArray <CMD_GR_Match_Info*> CMatchInfoArray;

//////////////////////////////////////////////////////////////////////////
//常量定义
#define MAX_MATCH_COUNT				(512/(sizeof(ULARGE_INTEGER)*2))	//比赛场次

//比赛状态
#define MATCH_STATUS_NOT_STARTED			0							//没有开始
#define MATCH_STATUS_MATCHING				1							//比赛中
#define MATCH_STATUS_WAIT_FINISH			2							//等待结束
#define MATCH_STATUS_WAIT_START				3							//等待开始
#define MATCH_STATUS_FINISH					4							//比赛结束

#ifdef _UNICODE
#define VER_IMatchService INTERFACE_VERSION(1,1)
static const GUID IID_IMatchService={0x6e733049,0x0f23,0x40c2,0x00ad,0xec,0xcb,0x87,0x76,0xb6,0x3e,0xf5};
#else
#define VER_IMatchService INTERFACE_VERSION(1,1)
static const GUID IID_IMatchService={0x24725b64,0x0e46,0x4ca3,0x0091,0xc7,0x4a,0x8a,0x78,0x69,0x06,0x83};
#endif

//比赛接口
interface IMatchService:public IUnknownEx
{
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx)=NULL;
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)=NULL;
	//设置参数
	virtual bool ModifyMatchParameter()=NULL;
	//设置接口
	virtual bool InitMatchService(IMainServiceFrame *pIMainServiceFrame, IDataBaseEngine *pIKernelDataBaseEngine,ITimerEngine *pITimerEngine,
		tagGameServiceAttrib *pGameServiceAttrib,tagGameServiceOption *pGameServiceOption,CTableFrameArray &TableFrameArray)=NULL;
	//用户报名
	virtual bool OnUserJoinMatch(IServerUserItem * pIServerUserItem)=NULL;
	//用户退赛
	virtual bool OnUserQuitMatch(IServerUserItem * pIServerUserItem)=NULL;
	//命令消息
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID)=NULL;
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)=NULL;
	//报名人数
	virtual DWORD GetSignUpUserCount()=NULL;
	//设置规则
	virtual VOID  SetDistributeRule(BYTE cbDistributeRule)=NULL;
	//插入分配
	virtual bool InsertDistribute(IServerUserItem * pIServerUserItem)=NULL;
	//寒事信息
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem)=NULL;
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
#ifdef _UNICODE
#define VER_IMatchTableFrameNotify INTERFACE_VERSION(1,1)
static const GUID IID_IMatchTableFrameNotify={0x726112e4,0x7820,0x4e49,0x00af,0x69,0xe5,0x39,0x6e,0x71,0xb2,0xfb};
#else
#define VER_IMatchTableFrameNotify INTERFACE_VERSION(1,1)
static const GUID IID_IMatchTableFrameNotify={0xdfc52f50,0x0692,0x430a,0x00aa,0x44,0x4d,0xda,0x2d,0xd3,0x08,0xd8};
#endif


interface IMatchTableFrameNotify: public IUnknownEx
{
	//用户准备
	virtual bool OnUserReay(IServerUserItem * pIServerUserItem,WORD wTableID,WORD wChairID)=NULL;
	//游戏开始
	virtual bool OnEventGameStart(WORD wTableID)=NULL;
	//游戏结束
	virtual bool OnEventGameEnd(WORD wTableID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
class CMatchParameterDlg:public CDialog
{
	enum {IDD=IDD_DLG_MATCH_PARAMETER};

public:
	WORD							m_wMatchID;							//比赛ID
	BYTE							&m_cbMatchTime;						//比赛场次
	ULARGE_INTEGER					*m_puiMatchDate;					//比赛时间
	CMatchInfoArray					&m_MatchInfo;						//赛事信息
	TCHAR							*m_pszMatchRoom;					//房间名称
	DWORD							&m_dwMatchFee;						//报名费用
	LONG							&m_lInitScore;						//初始积分
	WORD							&m_wMatchGameCount;					
	
	//控件变量
protected:
	CListCtrl						m_ListMatch;						//比赛列表
	CListCtrl						m_ListMatchInfo;					//比赛信息

public:
	CMatchParameterDlg(CMatchInfoArray &MatchInfo,ULARGE_INTEGER *puiMatchDate,BYTE &cbMatchTime, TCHAR *pszMatchRoom,DWORD &dwMatchFee,
						LONG &lInitScore, WORD &wMatchGameCount,WORD wMatchID);
    virtual ~CMatchParameterDlg();

	//重载函数
public:
	//初始化函数
	virtual BOOL OnInitDialog();
	//数据交换
	virtual void DoDataExchange(CDataExchange* pDX);

	//消息映射
public:
	//添加时间
	VOID OnBnClickedAdd();
	//修改时间
	VOID OnBnClickedModify();
	//删除时间
	VOID OnBnClickedDelete();
	//添加信息
	VOID OnBnClickedAddInfo();
	//修改信息
	VOID OnBnClickedModifyInfo();
	//删除信息
	VOID OnBnClickedDeleteInfo();


	//辅助函数
private:
	//更新控件
	VOID UpdateMatchList();
	//更新控件
	VOID UpdateMatchInfoList();
	//获取时间字符串
	LPCTSTR GetTimeString( SYSTEMTIME &st );
	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
};

//////////////////////////////////////////////////////////////////////////
class CTimeRankingsMatch:public IMatchService, public IMatchTableFrameNotify
{
	//变量定义
public:	
	DWORD							m_dwMatchFee;						//比赛费用
	IMainServiceFrame *				m_pIMainServiceFrame;				//服务接口
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//数据引擎
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置
	CDistributeManager              m_DistributeManage;                 //分组管理
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	CTableFrameArray				m_TableFrameArray;					//桌子数组

	//比赛配置
public:
	CMatchInfoArray					m_MatchInfo;						//赛事信息
	CIServerUserItemMap				m_ServerUserItemMap;				//参赛人员
	BYTE							m_cbMatchTime;						//比赛场次
	ULARGE_INTEGER					m_uiMatchDate[MAX_MATCH_COUNT*2];	//比赛时间
	CMatchParameterDlg				*m_pParameterDlg;					//配置参数
	TCHAR							m_szMatchName[32];					//比赛名称
	WORD							m_wMatchID;							//比赛ID
	LONG							m_lInitScore;						//初始积分
	WORD							m_wMatchGameCount;					//比赛局数
	CMD_GR_MatchRank *				m_pMatchRank;						//比赛得名
	WORD 							m_wRankCount;						//获奖人数

	//状态变量
protected:
	BYTE							m_cbMatchStatus;					//比赛状态
	CTime							m_TimeMatchStart;					//比赛开始
	CTime							m_TimeMatchOver;					//比赛结束
	BYTE							m_cbCurMatch;						//当前比赛

	//函数定义
public:
	//构造函数
	CTimeRankingsMatch(WORD wMatchID);
	//析构函数
	virtual ~CTimeRankingsMatch(void);

	//基础接口
public:
	//释放对象
	virtual VOID Release(){delete this;}
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//比赛接口
public:
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);
	//设置参数
	virtual bool ModifyMatchParameter();
	//设置接口
	virtual bool InitMatchService(IMainServiceFrame *pIMainServiceFrame, IDataBaseEngine *pIKernelDataBaseEngine,ITimerEngine *pITimerEngine,
		tagGameServiceAttrib *pGameServiceAttrib,tagGameServiceOption *pGameServiceOption,CTableFrameArray &TableFrameArray);
	//用户报名
	virtual bool OnUserJoinMatch(IServerUserItem * pIServerUserItem);
	//用户退赛
	virtual bool OnUserQuitMatch(IServerUserItem * pIServerUserItem);
	//命令消息
	virtual bool OnEventSocketMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem, DWORD dwSocketID);
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//报名人数
	virtual DWORD GetSignUpUserCount(){return (DWORD)m_ServerUserItemMap.GetCount();}
	//设置规则
	virtual VOID  SetDistributeRule(BYTE cbDistributeRule){m_DistributeManage.SetDistributeRule(cbDistributeRule);}
	//插入分配
	virtual bool InsertDistribute(IServerUserItem * pIServerUserItem);
	//寒事信息
	virtual bool SendMatchInfo(IServerUserItem * pIServerUserItem);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//桌子通知
public:
	//用户准备
	virtual bool OnUserReay(IServerUserItem * pIServerUserItem,WORD wTableID,WORD wChairID);
	//游戏开始
	virtual bool OnEventGameStart(WORD wTableID){return true;}
	//游戏结束
	virtual bool OnEventGameEnd(WORD wTableID);

	//运行函数
protected:
	//开始下一场比赛
	VOID StartNextMatch();
	//分配用户
	bool PerformDistribute();
	//获取时间字符串
	LPCTSTR GetTimeString( SYSTEMTIME &st );
};
//////////////////////////////////////////////////////////////////////////

//添加比赛日期类
class  CDlgAddMatchDate : public CDialog
{
	//定义变量
public:
	CTime					m_TimeBegin;				//开始时间
	CTime					m_TimeEnd;					//结束时间

	//控件变量
protected:
	CDateTimeCtrl			m_CtrlDateBegin;			//开始日期
	CDateTimeCtrl			m_CtrlTimeBegin;			//开始时间
	CDateTimeCtrl			m_CtrlDateEnd;				//结束日期
	CDateTimeCtrl			m_CtrlTimeEnd;				//结束时间

	//函数定义
public:
	//构造函数
	CDlgAddMatchDate();   
	//析构函数
	virtual ~CDlgAddMatchDate();

	//重载函数
protected:
	//数据交换
	virtual void DoDataExchange(CDataExchange* pDX); 
	//初始化
	virtual BOOL OnInitDialog();
	//确定函数
	VOID OnOK();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////
//添加比赛信息类
class  CDlgAddMatchInfo : public CDialog
{
	enum {IDD=IDD_MATCH_INFO};
	//控件变量
public:
	CString					m_strInfoTitle;				//信息标题
	CString					m_strInfoDesc;				//信息描述
	int						m_nInfoTitleLen;			//标题长度
	int						m_nInfoDescLen;				//描述长度

	//函数定义
public:
	//构造函数
	CDlgAddMatchInfo();   
	//析构函数
	virtual ~CDlgAddMatchInfo();

	//重载函数
protected:
	//数据交换
	virtual void DoDataExchange(CDataExchange* pDX); 
	//初始化
	virtual BOOL OnInitDialog();
	//确定函数
	VOID OnOK();

	DECLARE_MESSAGE_MAP()


};

class CMatchGroup
{
	//变量定义
protected:
	DWORD							m_dwMatchID;						//比赛ID
	CTableFrameArray				m_TableFrameArray;					//桌子数组
	CIServerUserItemMap				m_ServerUserItemMap;				//参赛人员
	BYTE							m_cbCurRound;						//当前轮数
};
//////////////////////////////////////////////////////////////////////////////////
#endif
