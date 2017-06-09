#ifndef GAME_PLAZA_HEAD_HEAD_FILE
#define GAME_PLAZA_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//类型说明

//类说明
class CTableResource;

//接口说明
interface ITableView;
interface ITableViewFrame;
interface IServerViewItem;

//结构说明
struct tagTableMirrorAttrib;

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerViewItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerViewItem={0xe1a242f0,0xed48,0x47c0,0xa4,0xa9,0x0b,0x6f,0x90,0x2f,0x12,0xbd};
#else
	#define VER_IServerViewItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerViewItem={0x404c4520,0x6f4e,0x44b0,0x8d,0xc7,0xe5,0x8e,0x78,0x72,0x31,0x89};
#endif

//房间接口
interface IServerViewItem : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessManager INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManager={0xf5a2e8a6,0xdb72,0x4a54,0xa0,0xb9,0xf1,0x6f,0x82,0xb9,0xa8,0x53};
#else
	#define VER_IProcessManager INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManager={0x68b8eac6,0x366f,0x4c5c,0xb8,0xd1,0xf4,0x7e,0x13,0xf3,0x55,0x88};
#endif

//进程管理
interface IProcessManager : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManagerSink={0x61ec395e,0xcb90,0x4965,0xb5,0x81,0x5b,0x22,0x49,0x8a,0xdd,0x6e};
#else
	#define VER_IProcessManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManagerSink={0xf3a80930,0x377c,0x40ad,0xa1,0x65,0xf9,0x02,0x76,0x09,0x49,0x85};
#endif

//游戏管理
interface IProcessManagerSink : public IUnknownEx
{
	//进程错误
	virtual bool OnGameProcessError()=NULL;
	//进程关闭
	virtual bool OnGameProcessClose(DWORD dwExitCode)=NULL;
	//进程启动
	virtual bool OnGameProcessCreate(bool bAlreadyExist)=NULL;
	//进程数据
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableView INTERFACE_VERSION(1,1)
	static const GUID IID_ITableView={0xa12ff990,0xa860,0x4406,0x94,0x6b,0x44,0x59,0xbe,0xa2,0x6f,0x82};
#else
	#define VER_ITableView INTERFACE_VERSION(1,1)
	static const GUID IID_ITableView={0xdcd15997,0xcbd4,0x4f9b,0xa5,0xe0,0x54,0x63,0x9c,0xb3,0xe5,0x47};
#endif

//桌子视图
interface ITableView : public IUnknownEx
{
	//功能接口
public:
	//空椅子数
	virtual WORD GetNullChairCount(WORD & wNullChairID)=NULL;
	//配置函数
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableFrameView)=NULL;

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wChairID)=NULL;
	//设置信息
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)=NULL;

	//设置接口
public:
	//鼠标按下
	virtual VOID SetMouseDown(bool bMouseDown)=NULL;
	//焦点框架
	virtual VOID SetFocusFrame(bool bFocusFrame)=NULL;
	//盘旋位置
	virtual VOID SetHoverStatus(WORD wHoverChairID)=NULL;
	//桌子状态 
	virtual VOID SetTableStatus(bool bPlaying, bool bLocker)=NULL;

	//查询接口
public:
	//游戏标志
	virtual bool GetPlayFlag()=NULL;
	//密码标志
	virtual bool GetLockerFlag()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableViewFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrame={0xe63e8758,0x96b0,0x4ab3,0x92,0xfc,0x3a,0x29,0x82,0x18,0x84,0xf8};
#else
	#define VER_ITableViewFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrame={0x067aff04,0x7527,0x41da,0x9b,0x83,0x0e,0x6f,0x41,0x8d,0xcd,0x87};
#endif

//桌子框架
interface ITableViewFrame : public IUnknownEx
{
	//配置接口
public:
	//创建桌子
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)=NULL;
	//配置桌子
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID, LPCTSTR pszResDirectory)=NULL;

	//信息接口
public:
	//桌子数目
	virtual WORD GetTableCount()=NULL;
	//椅子数目
	virtual WORD GetChairCount()=NULL;

	//用户接口
public:
	//获取用户
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID)=NULL;
	//设置信息
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)=NULL;

	//状态接口
public:
	//游戏标志
	virtual bool GetPlayFlag(WORD wTableID)=NULL;
	//密码标志
	virtual bool GetLockerFlag(WORD wTableID)=NULL;
	//焦点框架
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame)=NULL;
	//桌子状态 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker)=NULL;

	//视图控制
public:
	//桌子可视
	virtual bool VisibleTable(WORD wTableID)=NULL;
	//闪动桌子
	virtual bool FlashGameTable(WORD wTableID)=NULL;
	//闪动椅子
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID)=NULL;

	//功能接口
public:
	//更新桌子
	virtual bool UpdateTableView(WORD wTableID)=NULL;
	//获取桌子
	virtual ITableView * GetTableViewItem(WORD wTableID)=NULL;
	//空椅子数
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableViewFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrameSink={0xb0b88935,0xc7fd,0x42ff,0x90,0x62,0x2a,0x2d,0xa9,0x35,0x5b,0xc8};
#else
	#define VER_ITableViewFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrameSink={0xf52f47d4,0xeabf,0x49b5,0xa3,0x0c,0x37,0x61,0x3f,0x81,0x21,0x3c};
#endif

//桌子通知
interface ITableViewFrameSink : public IUnknownEx
{
	//鼠标双击
	virtual VOID OnDButtonHitTable(WORD wTableID)=NULL;
	//左键按下
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID)=NULL;
	//右键按下
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID)=NULL;
	//桌子按钮
	virtual VOID OnHitTableButton(BYTE cbButton)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define VER_IChatMsgItem INTERFACE_VERSION(1,1)
static const GUID IID_IChatMsgItem={0x3a168ac6,0xd09c,0x4d26,0x00aa,0x45,0x1b,0xa,0x62,0x6,0x29,0x66};
#else
#define VER_IChatMsgItem INTERFACE_VERSION(1,1)
static const GUID IID_IChatMsgItem={0x8f04a9a0,0x5737,0x43e5,0x009a,0x1b,0xea,0x81,0x79,0x89,0xb0,0xa0};
#endif

//消息接口
interface IChatMsgItem : public IUnknownEx
{
	//功能接口
public:
	//重置项目
	virtual VOID __cdecl ResetChatMsg()=NULL;
	//获取玩家
	virtual LPCTSTR __cdecl GetSendUserAccounts()=NULL;
	//获取发送消息
	virtual LPCTSTR __cdecl GetMsg()=NULL;
	//获取字体颜色
	virtual COLORREF __cdecl GetChatColor()=NULL;
	//是否自己消息
	virtual bool __cdecl IsSelfMsg()=NULL;
	//获取时间
	virtual const SYSTEMTIME& GetMsgTime()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define VER_IChatHistory INTERFACE_VERSION(1,1)
static const GUID IID_IChatHistory={0xb0a24da4,0x7e14,0x47ef,0x00ab,0xdc,0xd0,0xc3,0x65,0x48,0xc8,0xf9};
#else
#define VER_IChatHistory INTERFACE_VERSION(1,1)
static const GUID IID_IChatHistory={0xf00991f3,0xd009,0x4ca9,0x008b,0xd1,0x78,0x24,0x95,0xe,0xbf,0x8};
#endif

//聊天记录
interface IChatHistory : public IUnknownEx
{
	//查询函数
public:
	//获取记录ID
	virtual DWORD __cdecl GetChatID()=NULL;
	//获取消息数
	virtual INT_PTR __cdecl GetMsgItemCount()=NULL;
	//枚举消息
	virtual IChatMsgItem* __cdecl EnumChatMsgItem(INT_PTR nEnumIndex)=NULL;

	//功能函数
public:
	//重置记录
	virtual VOID __cdecl ResetChatHistory()=NULL;
	//添加消息
	virtual bool __cdecl AddMsgItem(LPCTSTR lpszSendUser,LPCTSTR lpszMsg,COLORREF crColor,bool bMySelfString)=NULL;
	//清除记录
	virtual bool __cdecl ClearHistory()=NULL;

	//文件操作
public:
	//保存记录
	virtual bool __cdecl Save()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define VER_IChatHistoryManager INTERFACE_VERSION(1,1)
static const GUID IID_IChatHistoryManager={0x94afc65f,0x2c55,0x46d3,0x00a0,0x84,0x6c,0x17,0x84,0x66,0xf0,0x15};
#else
#define VER_IChatHistoryManager INTERFACE_VERSION(1,1)
static const GUID IID_IChatHistoryManager={0x4e39ada4,0x239,0x4209,0x00a0,0x9b,0xc3,0xeb,0xd2,0x77,0xae,0xb4};
#endif

//聊天记录管理接口
interface IChatHistoryManager : public IUnknownEx
{
	//配置接口
public:
	//初始化
	virtual VOID SetMyselftUserItem(IClientUserItem *pIMyselfUserItem)=NULL;

	//查询接口
public:
	//获取玩家接口
	virtual IClientUserItem * __cdecl GetMyselftUserItem()=NULL;

	//功能接口
public:
	//激活记录
	virtual IChatHistory* __cdecl ActiveChatHistory(DWORD dwChatID)=NULL;
	//关闭记录
	virtual bool __cdecl CloseChatHistory(DWORD dwChatID)=NULL;
	//保存记录
	virtual bool __cdecl SaveChatHistory(DWORD dwChatID)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#endif