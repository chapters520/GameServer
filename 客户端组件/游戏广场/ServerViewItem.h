#ifndef SERVER_VIEW_ITEM_HEAD_FILE
#define SERVER_VIEW_ITEM_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgStatus.h"
#include "DlgWhisper.h"
#include "ServerListData.h"
#include "ProcessManager.h"
#include "DlgSearchTable.h"
#include "TableViewFrame.h"
#include "DlgServerManager.h"
#include "PlatformPublicize.h"
#include "ChatHistory.h"
#include "DlgManageUser.h"

//////////////////////////////////////////////////////////////////////////

//加为好友
#define WM_ADD_FRIEND				(WM_USER+400)						//加为好友
#define WM_SET_DETEST				(WM_USER+401)						//设为厌恶

//////////////////////////////////////////////////////////////////////////////////

//服务状态
enum enServiceStatus
{
	ServiceStatus_Unknow,			//未知状态
	ServiceStatus_Entering,			//进入状态
	ServiceStatus_Validate,			//验证状态
	ServiceStatus_RecvInfo,			//读取状态
	ServiceStatus_ServiceIng,		//服务状态
	ServiceStatus_NetworkDown,		//中断状态
};

//查找桌子
struct tagSearchTable
{
	bool							bOneNull;							//一个空位
	bool							bTwoNull;							//两个空位
	bool							bAllNull;							//全空位置
	bool							bNotFull;							//不全满位
	bool							bFilterPass;						//过滤密码
	WORD							wStartTableID;						//起始桌子
	WORD							wResultTableID;						//结果桌子
	WORD							wResultChairID;						//结果椅子
};

//用户数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//游戏房间
class CServerViewItem : public CDialog, public IServerViewItem, public ITCPSocketSink, public ISplitterEvent,
	public ITableViewFrameSink, public IUserManagerSink, public IProcessManagerSink, public IExpressionSink,
	public IChatControlSink, public IGamePropertySink, public IStatusViewSink, public IFlashControlSink,public
	IGamePropertyUseSink
{
	//界面变量
protected:
	bool							m_bCreateFlag;						//创建标志
	bool							m_bRectifyHor;						//调整标志
	bool							m_bRectifyVor;						//调整标志
	bool							m_bHideUserList;					//隐藏标志

	//辅助变量
protected:
	WORD							m_wReqTableID;						//请求桌子
	WORD							m_wReqChairID;						//请求位置
	WORD							m_wFindTableID;						//查找桌子

	//聊天变量
protected:
	DWORD							m_dwChatTime;						//聊天时间
	CString							m_strChatString;					//发送信息

	//用户属性
protected:
	DWORD							m_dwUserRight;						//用户权限
	DWORD							m_dwMasterRight;					//管理权限
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针

	//房间属性
protected:
	WORD							m_wServerType;						//房间类型
	DWORD							m_dwServerRule;						//房间规则
	enServiceStatus					m_ServiceStatus;					//房间状态
	WORD							m_wAVServerPort;					//视频端口
	DWORD							m_dwAVServerAddr;					//视频地址

	//配置信息
protected:
	BYTE							m_cbColumnCount;					//列表数目
	BYTE							m_cbPropertyCount;					//道具数目
	tagColumnItem					m_ColumnItem[MAX_COLUMN];			//列表描述
	tagPropertyInfo					m_PropertyInfo[MAX_PROPERTY];		//道具描述

	//房间属性
protected:
	tagGameKind						m_GameKind;							//类型信息
	tagGameServer					m_GameServer;						//房间信息

	//配置参数
protected:
	CParameterGame *				m_pParameterGame;					//游戏配置
	CParameterServer *				m_pParameterServer;					//房间配置

	//数组变量
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//用户数组

	//功能组件
protected:
	CDlgStatus						m_DlgStatus;						//状态窗口
	CFlashControl					m_FlashControl;						//动画控件
	CProcessManager					m_ProcessManager;					//进程管理
	CDlgWhisperItemArray			m_DlgWhisperItemArray;				//私聊数组
	CChatHistoryManager				m_ChatHistoryManager;				//聊天记录管理

	//控件变量
protected:
	CDlgSearchTable *				m_pDlgSearchTable;					//查找桌子
	CDlgInsureServer *				m_pDlgInsureServer;					//银行对象
	CDlgServerManager *				m_pDlgServerManager;				//管理对象
	CExpressionControl *			m_pExpressionControl;				//表情窗口
	CDlgTrumpetItem *               m_pTrumpetItem;                     //喇叭窗口 

	//组件变量
protected:
	CTCPSocketHelper				m_TCPSocketModule;					//网络连接
	CGameLevelParserHelper			m_GameLevelParserModule;			//游戏等级
	CPlazaUserManagerHelper			m_PlazaUserManagerModule;			//用户管理

	//界面组件
public:
	CSkinButton						m_btControl;						//控制按钮
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CSkinSplitter					m_VorSplitter;						//拆分控件
	CSkinSplitter					m_HorSplitter;						//拆分控件
	CWndChatControl					m_ChatControl;						//聊天控制
	CUserListControl				m_UserListControl;					//用户列表
	CGamePropertyCtrl				m_GamePropertyCtrl;					//游戏道具
	CPlatformPublicize				m_PlatformPublicize;				//浏览控件

	//游戏桌子
public:
	CSkinButton						m_btQuitServer;						//离开房间
	CSkinButton						m_btTableButton1;					//桌子按钮
	CSkinButton						m_btTableButton2;					//桌子按钮
	CSkinButton						m_btTableButton3;					//桌子按钮
	CSkinButton						m_btTableButton4;					//桌子按钮
	CSkinButton						m_btTableButton5;					//桌子按钮
	CTableViewFrame					m_TableViewFrame;					//桌子框架

	//聊天控件
public:
	CSkinEditEx						m_ChatEdit;							//聊天输入
	CSkinButton						m_btSendChat;						//发送聊天
	CSkinButton						m_btChatShort;						//聊天短语
	CRichEditMessage				m_ChatMessage;						//聊天信息

	//聊天按钮
public:
	CSkinButton						m_btChatButton1;					//聊天按钮
	CSkinButton						m_btChatButton2;					//聊天按钮
	CSkinButton						m_btChatButton3;					//聊天按钮
	CSkinButton						m_btChatButton4;					//聊天按钮

	//资源变量
protected:
	CEncircleBMP					m_ChatEncircle;						//聊天资源
	CEncircleBMP					m_TableEncircle;					//桌子资源
	CEncircleBMP					m_UserListEncircle;					//用户资源

	//管理控件
public:
	//CDlgAndroid *					m_pDlgAndroid;						//管理窗口
	CDlgManageUser *				m_pDlgManageUser;					//用户管理

	//函数定义
public:
	//构造函数
	CServerViewItem();
	//析构函数
	virtual ~CServerViewItem();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//桌子接口
public:
	//鼠标双击
	virtual VOID OnDButtonHitTable(WORD wTableID);
	//左键按下
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID);
	//右键按下
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID);
	//桌子按钮
	virtual VOID OnHitTableButton(BYTE cbButton);

	//网络接口
protected:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//道具接口
protected:
	//播放完成
	virtual VOID OnEventFlashFinish();
	//购前检查
	virtual bool OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage);
	//购买道具
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);
	//查询用户
	virtual IClientUserItem * OnQueryClientUserItem() { return m_pIMySelfUserItem; };
	//使用道具
	virtual bool OnEventUseProperty(WORD wPropertyIndex);	


	//进程接口
public:
	//进程错误
	virtual bool OnGameProcessError();
	//进程关闭
	virtual bool OnGameProcessClose(DWORD dwExitCode);
	//进程启动
	virtual bool OnGameProcessCreate(bool bAlreadyExist);
	//进程数据
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//用户接口
public:
	//用户激活
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem);
	//用户删除
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus);
	//用户更新
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserAttrib & UserAttrib);

	//事件接口
protected:
	//取消连接
	virtual VOID OnStatusCancel();
	//拆分事件
	virtual VOID OnSplitterEvent(UINT uControlID, INT nXPos, INT nYPos);
	//用户选择
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem);
	//表情事件
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//菜单接口
public:
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wUserCount);

	//信息函数
public:
	//房间标识
	virtual WORD GetServerID() { return m_GameServer.wServerID; }
	//房间类型
	virtual WORD GetServerType() { return m_wServerType; }
	//房间规则
	virtual DWORD GetServerRule() { return m_dwServerRule; }
	//类型名字
	virtual LPCTSTR GetKindName() { return m_GameKind.szKindName; }
	//房间名字
	virtual LPCTSTR GetServerName() { return m_GameServer.szServerName; }
	//服务状态
	virtual enServiceStatus GetServiceStatus() { return m_ServiceStatus; }
	//游戏配置
	virtual CParameterGame * GetParameterGame() { return m_pParameterGame; }
	//房间配置
	virtual CParameterServer * GetParameterServer() { return m_pParameterServer; }

	//组件函数
public:
	//网络组件
	ITCPSocket * GetTCPSocket() { return m_TCPSocketModule.GetInterface(); }
	//等级组件
	IGameLevelParser * GetGameLevelParser() { return m_GameLevelParserModule.GetInterface(); }
	//用户组件
	IPlazaUserManager * GetPlazaUserManager() { return m_PlazaUserManagerModule.GetInterface(); }

	//功能函数
public:
	//显示银行
	VOID ShowInsureView();
	//老板事件
	VOID NotifyBossCome(bool bBossCome);
	//查找桌子
	VOID SearchGameTable(tagSearchTable & SearchTable);
	//更新头像
	VOID UpdateUserCustomFace(DWORD dwUserID, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo);
	//查询游戏客户端
	bool IsGameClientReady();
	//自己状态
	bool IsPlayingMySelf();

	//私聊函数
public:
	//私聊会话
	VOID WhisperConversation(IClientUserItem * pIClientUserItem[], WORD wUserCount);
	//创建私聊
	CDlgWhisper * CreateWhisperWnd(DWORD dwConversationID, DWORD dwUserID[], WORD wUserCount, bool bCreate);

	//网络事件
protected:
	//用户处理
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//登录处理
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//配置处理
	bool OnSocketMainConfig(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//状态处理
	bool OnSocketMainStatus(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//银行处理
	bool OnSocketMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//管理处理
	bool OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//系统处理
	bool OnSocketMainSystem(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏处理
	bool OnSocketMainGameFrame(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//比赛处理
	bool OnSocketMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户进入
	bool OnSocketSubUserEnter(VOID * pData, WORD wDataSize);
	//用户积分
	bool OnSocketSubUserScore(VOID * pData, WORD wDataSize);
	//用户状态
	bool OnSocketSubUserStatus(VOID * pData, WORD wDataSize);
	//登录完成
	bool OnSocketSubLogonFinish(VOID * pData, WORD wDataSize);
	//登录成功
	bool OnSocketSubLogonSuccess(VOID * pData, WORD wDataSize);
	//登录失败
	bool OnSocketSubLogonFailure(VOID * pData, WORD wDataSize);
	//更新提示
	bool OnSocketSubUpdateNotify(VOID * pData, WORD wDataSize);
	//请求失败
	bool OnSocketSubRequestFailure(VOID * pData, WORD wDataSize);
	//用户聊天
	bool OnSocketSubUserChat(VOID * pData, WORD wDataSize);
	//用户表情
	bool OnSocketSubExpression(VOID * pData, WORD wDataSize);
	//用户私聊
	bool OnSocketSubWisperUserChat(VOID * pData, WORD wDataSize);
	//私聊表情
	bool OnSocketSubWisperExpression(VOID * pData, WORD wDataSize);
	//系统消息
	bool OnSocketSubSystemMessage(VOID * pData, WORD wDataSize);
	//动作消息
	bool OnSocketSubActionMessage(VOID * pData, WORD wDataSize);
	//邀请玩家
	bool OnSocketSubUserInvite(VOID * pData, WORD wDataSize);
	//道具成功
	bool OnSocketSubPropertySuccess(VOID * pData, WORD wDataSize);
	//道具失败
	bool OnSocketSubPropertyFailure(VOID * pData, WORD wDataSize);
	//道具效应
	bool OnSocketSubPropertyEffect(VOID * pData, WORD wDataSize);
	//道具消息
	bool OnSocketPropertyMessage(VOID * pData, WORD wDataSize);
	//道具喇叭
	bool OnSocketPropertyTrumpet(VOID * pData, WORD wDataSize);
	//比赛费用
	bool OnSocketSubMatchFee(VOID * pData, WORD wDataSize);
	//参赛人数
	bool OnSocketSubMatchNum(VOID *pData, DWORD wDataSize);

	//界面函数
public:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//配置房间
	VOID InitServerViewItem(CGameServerItem * pGameServerItem,WORD wAVServerPort,DWORD dwAVServerAddr);
	//设置界面
	VOID InitServerViewCtrl(CSkinRenderManager * pSkinRenderManager);

	//网络命令
public:
	//发送登录
	bool SendLogonPacket();
	//发送规则
	bool SendUserRulePacket();
	//发送旁观
	bool SendLookonPacket(WORD wTableID, WORD wChairID);
	//发送坐下
	bool SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR lpszPassword=NULL);
	//发送起立
	bool SendStandUpPacket(WORD wTableID, WORD wChairID, BYTE cbForceLeave);
	//发送表情
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);
	//发送聊天
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);

	//动作处理
public:
	//执行旁观
	bool PerformLookonAction(WORD wTableID, WORD wChairID);
	//执行起立
	bool PerformStandUpAction(WORD wTableID, WORD wChairID);
	//执行坐下
	bool PerformSitDownAction(WORD wTableID, WORD wChairID, bool bEfficacyPass);
	//执行购买
	bool PerformBuyProperty(BYTE cbRequestArea,LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);

	//辅助函数
protected:
	//设置状态
	VOID SetServiceStatus(enServiceStatus ServiceStatus);
	//资源目录
	VOID GetGameResDirectory(TCHAR szResDirectory[], WORD wBufferCount);

	//内部函数
protected:
	//聊天效验
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//桌子效验
	bool EfficacyTableRule(WORD wTableID, WORD wChairID, bool bReqLookon, CString & strDescribe);
	//创建管理菜单
	VOID CreateServerManagerMenu( CSkinMenu &ManageMenu );

	//聊天命令
protected:
	//更多颜色
	VOID OnSelectMoreColor();
	//选择颜色
	VOID OnSelectChatColor(UINT nCommandID);
	//选择短语
	VOID OnSelectChatShort(UINT nCommandID);

	//管理命令
protected:
	//禁止用户大厅聊天
	VOID OnLimitUserRoomChat();
	//允许用户大厅聊天
	VOID OnAllowUserRoomChat();
	//禁止用户游戏聊天
	VOID OnLimitUserGameChat();
	//允许用户游戏聊天
	VOID OnAllowUserGameChat();
	//禁止用户私聊
	VOID OnLimitUserWhispChat();
	//允许用户私聊
	VOID OnAllowUserWhispChat();

	//管理命令
protected:
	//管理按钮
	VOID OnBnClickedServerManage();
	//房间管理
	VOID OnManageServer();
	//用户管理
	VOID OnManageUser();
	//系统消息管理
	VOID OnManageSystemMsg();
	//桌子规则管理
	VOID OnManageTableRule();
	//机器人管理
	VOID OnManageUserAndior();

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);

	//控件消息
protected:
	//右键列表
	VOID OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//双击列表
	VOID OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//双击列表
	VOID OnNMClickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//加为好友
	LRESULT OnWhisperAddFriend(WPARAM wParam, LPARAM lPara);
	//设为厌恶
	LRESULT OnWhisperSetDetest(WPARAM wParam, LPARAM lPara);

	DECLARE_MESSAGE_MAP()

	//内部函数
protected:
	//播放短语语音
	bool PlayChatShortSound(BYTE cbGender, BYTE cbIndex);
	//获得短语下标
	BYTE GetChatShortIndex(LPCTSTR pChat);
};

//////////////////////////////////////////////////////////////////////////////////

#endif