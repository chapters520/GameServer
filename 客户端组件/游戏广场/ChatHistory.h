#ifndef CHAT_HISTORY_HEAD_FILE
#define CHAT_HISTORY_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////

class CChatHistory;
class CChatHistoryManager;

//消息类
class CChatMsgItem: public CObject, public IChatMsgItem
{
	//友元类
	friend class CChatHistory;

	//变量定义
protected:
	CString							m_strSendUser;						//发送玩家
	CString							m_strMsg;							//发送消息
	COLORREF						m_crChatColor;						//字体颜色
	BYTE							m_bSelfMsg;							//是否自己的消息
	SYSTEMTIME						m_Time;								//消息时间

	//函数定义
public:
	//构造函数
	CChatMsgItem();
	//析构函数
	virtual ~CChatMsgItem();

	//重载函数
public:
	//序列化
	virtual void Serialize( CArchive& ar );

	//基础接口
public:
	//释放对象
	virtual VOID  Release(){}
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能函数
public:
	//重置项目
	virtual VOID __cdecl ResetChatMsg();
	//获取玩家
	virtual LPCTSTR __cdecl GetSendUserAccounts() { return m_strSendUser; }
	//获取发送消息
	virtual LPCTSTR __cdecl GetMsg() { return m_strMsg; }
	//获取字体颜色
	virtual COLORREF __cdecl GetChatColor() { return m_crChatColor; }
	//是否自己消息
	virtual bool __cdecl IsSelfMsg() { return (m_bSelfMsg>0); }
	//获取时间
	virtual const SYSTEMTIME& GetMsgTime() { return m_Time; }

protected:
	DECLARE_SERIAL(CChatMsgItem)
};

//////////////////////////////////////////////////////////////////////////

//消息数组
typedef CWHArray<CChatMsgItem *>	CChatMsgItemArray;

//消息记录类
class CChatHistory: public CObject, public IChatHistory
{
	//友元类
	friend class CChatHistoryManager;

	//变量定义
protected:
	DWORD								m_dwChatID;							//记录ID
	CChatMsgItemArray					m_ChatMsgItemActive;				//消息数组
	static CChatMsgItemArray			m_ChatMsgItemStorge;				//消息库存
	IChatHistoryManager*				m_pIChatHistoryManager;				//管理接口

	//函数定义
public:
	//构造函数
	CChatHistory();
	//析构函数
	virtual ~CChatHistory();
	
	//重载函数
public:
	//序列化
	virtual void Serialize( CArchive& ar );

	//基础接口
public:
	//释放对象
	virtual VOID Release();
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//查询函数
public:
	//获取记录ID
	virtual DWORD __cdecl GetChatID() { return m_dwChatID; }
	//获取消息数
	virtual INT_PTR __cdecl GetMsgItemCount() { return m_ChatMsgItemActive.GetCount(); }
	//枚举消息
	virtual IChatMsgItem* __cdecl EnumChatMsgItem(INT_PTR nEnumIndex);

	//功能函数
public:
	//重置记录
	virtual VOID __cdecl ResetChatHistory();
	//添加消息
	virtual bool __cdecl AddMsgItem(LPCTSTR lpszSendUser,LPCTSTR lpszMsg,COLORREF crColor,bool bMySelfString);
	//清除记录
	virtual bool __cdecl ClearHistory();

	//文件操作
public:
	//保存记录
	virtual bool __cdecl Save();

	//辅助函数
protected:
	//激活消息项
	CChatMsgItem* ActiveChatMsgItem();

protected:
	DECLARE_SERIAL(CChatHistory)
};

//////////////////////////////////////////////////////////////////////////

//聊天记录数组
typedef CWHArray<CChatHistory *>	CChatHistoryArray;

//记录映射
typedef CMap<DWORD,DWORD,CChatHistory *,CChatHistory *> CMapChatHistory;

//////////////////////////////////////////////////////////////////////////

//聊天记录管理类
class CChatHistoryManager:public IChatHistoryManager
{
	//变量定义
protected:
	IClientUserItem*						m_pIMyselfUserItem;			//玩家接口
	CChatHistoryArray						m_ChatHistoryActive;		//记录数组
	static CChatHistoryArray				m_ChatHistoryStorge;		//记录库存
	CMapChatHistory							m_MapChatHistory;			//记录映射

	//函数定义
public:
	//构造函数
	CChatHistoryManager();
	//析构函数
	virtual ~CChatHistoryManager();

	//基础接口
public:
	//释放对象
	virtual VOID Release(){}
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置接口
public:
	//初始化
	virtual VOID SetMyselftUserItem(IClientUserItem *pIMyselfUserItem);

	//查询接口
public:
	//获取玩家接口
	virtual IClientUserItem * __cdecl GetMyselftUserItem() { return m_pIMyselfUserItem; }

	//功能接口
public:
	//激活记录
	virtual IChatHistory* __cdecl ActiveChatHistory(DWORD dwChatID);
	//关闭记录
	virtual bool __cdecl CloseChatHistory(DWORD dwChatID);
	//保存记录
	virtual bool __cdecl SaveChatHistory(DWORD dwChatID);
};

//////////////////////////////////////////////////////////////////////////

#endif