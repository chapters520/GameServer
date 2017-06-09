#include "StdAfx.h"
#include "DlgWhisper.h"
#include "PlatformFrame.h"
#include "ServerViewItem.h"

//////////////////////////////////////////////////////////////////////////////////
//位置定义
#define MIN_WIDTH						545								//最小宽度
#define MIN_HEIGHT						425								//最大宽度
#define NORMAL_HISTORY_HEGIHT			180								//聊天高度
#define NORMAL_HISTORY_OFFSET		    35								//聊天高度

//用户信息
#define USER_INFO_CX				188									//信息宽度
#define USER_INFO_CY				180									//信息高度
#define CHAT_INPUT_CY				100									//输入高度

//颜色选择
#define MAX_CHAT_COLOR				16									//最大数目
#define IDM_SELECT_CHAT_COLOR		(WM_USER+200)						//选择颜色

//菜单命令
#define IDM_SHOW_ALL					2000							//显示所有
#define IDM_HIDE_DETEST					2001							//屏蔽厌恶
#define IDM_ONLY_FRIEND					2002							//接收好友
#define IDM_HIDE_ALL					2003							//屏蔽所有

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgWhisper, CSkinDialog)

	//系统命令
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_GETMINMAXINFO()

	//按钮命令
	ON_BN_CLICKED(IDC_BT_USER_MENU, OnBnClickedUserMenu)
	ON_BN_CLICKED(IDC_BT_SEND_CHAT, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_BT_SEND_CONTROL, OnBnClickedSendControl)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_1, OnBnClickedChatButton1)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_2, OnBnClickedChatButton2)
	ON_BN_CLICKED(IDC_BT_CHAT_BUTTON_3, OnBnClickedChatButton3)
	ON_BN_CLICKED(IDC_BT_CHAT_HISTORY, OnBnClickedChatHistory)

	//控制按钮
	ON_BN_CLICKED(IDC_SHIELD_THIS_CHAT,OnBnClickedShieldThisChat)
	ON_BN_CLICKED(IDC_ADD_FRIEND, OnBnClickAddFriend)
	ON_BN_CLICKED(IDC_SET_DETEST, OnBnClickSetDetest)
	ON_BN_CLICKED(IDC_MESSAGE_SETTING, OnBnClickMessageSetting)

	//菜单命令
	ON_COMMAND(IDM_MORE_COLOR, OnSelectMoreColor)
	ON_COMMAND_RANGE(IDM_SELECT_CHAT_COLOR, (IDM_SELECT_CHAT_COLOR+MAX_CHAT_COLOR), OnSelectChatColor)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgWhisper::CDlgWhisper() : CSkinDialog(IDD_DLG_WHISPER)
{
	//状态变量
	m_bCreateFlag=false;
	m_bShowHistory=false;
	m_dwConversationID=0L;
	m_dwChatHistroyID = 0L;

	m_dwUserRight = 0;

	//接口变量
	m_pITCPSocket=NULL;
	m_pIMySelfUserItem=NULL;
	m_pIUserOrderParser=NULL;
	m_pIGameLevelParser=NULL;

	//组件变量
	m_pServerViewItem=NULL;
	m_pExpressionControl=NULL;

	//记录接口
	m_pIChatHistory = NULL;
	m_pIChatHistroyManager = NULL;

    //输入资源
	tagEncircleResource	EncircleChatInput;
	EncircleChatInput.pszImageTL=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TL);
	EncircleChatInput.pszImageTM=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TM);
	EncircleChatInput.pszImageTR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_TR);
	EncircleChatInput.pszImageML=MAKEINTRESOURCE(IDB_INPUT_WHISPER_ML);
	EncircleChatInput.pszImageMR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_MR);
	EncircleChatInput.pszImageBL=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BL);
	EncircleChatInput.pszImageBM=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BM);
	EncircleChatInput.pszImageBR=MAKEINTRESOURCE(IDB_INPUT_WHISPER_BR);
	m_EncircleChatInput.InitEncircleResource(EncircleChatInput,AfxGetInstanceHandle());

	//信息资源
	tagEncircleResource	EncircleChatMessage;
	EncircleChatMessage.pszImageTL=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TL);
	EncircleChatMessage.pszImageTM=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TM);
	EncircleChatMessage.pszImageTR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_TR);
	EncircleChatMessage.pszImageML=MAKEINTRESOURCE(IDB_CHAT_WHISPER_ML);
	EncircleChatMessage.pszImageMR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_MR);
	EncircleChatMessage.pszImageBL=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BL);
	EncircleChatMessage.pszImageBM=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BM);
	EncircleChatMessage.pszImageBR=MAKEINTRESOURCE(IDB_CHAT_WHISPER_BR);
	m_EncircleChatMessage.InitEncircleResource(EncircleChatMessage,AfxGetInstanceHandle());

	//信息资源
	tagEncircleResource	EncircleHistoryEdit;
	EncircleHistoryEdit.pszImageTL=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_TL);
	EncircleHistoryEdit.pszImageTM=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_TM);
	EncircleHistoryEdit.pszImageTR=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_TR);
	EncircleHistoryEdit.pszImageML=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_ML);
	EncircleHistoryEdit.pszImageMR=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_MR);
	EncircleHistoryEdit.pszImageBL=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_BL);
	EncircleHistoryEdit.pszImageBM=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_BM);
	EncircleHistoryEdit.pszImageBR=MAKEINTRESOURCE(IDB_HISTORY_WHISPER_BR);
	m_EncircleHistoryEdit.InitEncircleResource(EncircleHistoryEdit,AfxGetInstanceHandle());

	return;
}

//析构函数
CDlgWhisper::~CDlgWhisper()
{
	//销毁窗口
	if(m_pExpressionControl && IsWindow(m_pExpressionControl->m_hWnd)) m_pExpressionControl->DestroyWindow();

	//删除对象
	SafeDelete(m_pExpressionControl);

	return;
}

//控件绑定
VOID CDlgWhisper::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//聊天按钮
	DDX_Control(pDX, IDC_BT_CHAT_HISTORY, m_btChatHistory);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_1, m_btChatButton1);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_2, m_btChatButton2);
	DDX_Control(pDX, IDC_BT_CHAT_BUTTON_3, m_btChatButton3);

	//控制按钮
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_BT_USER_MENU, m_btUserMenu);
	DDX_Control(pDX, IDC_BT_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_BT_SEND_CONTROL, m_btSendControl);

	//控制按钮
	DDX_Control(pDX, IDC_SHIELD_THIS_CHAT, m_btShieldThisChat);
	DDX_Control(pDX, IDC_ADD_FRIEND, m_btAddFriend);
	DDX_Control(pDX, IDC_SET_DETEST, m_btSetDetest);
	DDX_Control(pDX, IDC_MESSAGE_SETTING, m_btMessageSetting);

	//聊天控件
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
	DDX_Control(pDX, IDC_EDT_HISTORY, m_HistoryEdit);
}

//消息解释
BOOL CDlgWhisper::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	//发送控制
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//变量定义
		bool bLControlDown=((GetAsyncKeyState(VK_LCONTROL)&0xFFF0)!=0);
		bool bRControlDown=((GetAsyncKeyState(VK_RCONTROL)&0xFFF0)!=0);

		//变量定义
		ASSERT(CParameterGlobal::GetInstance()!=NULL);
		CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

		//发送控制
		if ((pParameterGlobal->m_bSendWhisperByEnter==true)&&(bLControlDown==false)&&(bRControlDown==false))
		{
			OnBnClickedSendChat();
			return TRUE;
		}

		//发送控制
		if ((pParameterGlobal->m_bSendWhisperByEnter==false)&&((bLControlDown==true)||(bRControlDown==true)))
		{
			OnBnClickedSendChat();
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CDlgWhisper::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	SetWindowText(TEXT("私聊"));
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WHISPER)),TRUE);

	//变量定义
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//聊天控件
	m_ChatInput.SetLimitText(LEN_USER_CHAT-1);
	m_ChatMessage.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));
	m_HistoryEdit.SetExpressionManager(CExpressionManager::GetInstance(),RGB(255,255,255));

	//聊天按钮
	m_btUserMenu.SetButtonImage(IDB_BT_WHISPER_USER_MENU,hInstance,false,false);
	m_btSendControl.SetButtonImage(IDB_BT_WHISPER_SEND_CONTROL,hInstance,false,false);
	m_btChatHistory.SetButtonImage(IDB_BT_WHISPER_HISTORY,TEXT("BT_WHISPER_HISTORY"),hInstance,false,false);
	m_btChatButton1.SetButtonImage(IDB_BT_WHISPER_BUTTON_1,TEXT("BT_WHISPER_BUTTON_1"),hInstance,false,false);
	m_btChatButton2.SetButtonImage(IDB_BT_WHISPER_BUTTON_2,TEXT("BT_WHISPER_BUTTON_2"),hInstance,false,false);
	m_btChatButton3.SetButtonImage(IDB_BT_WHISPER_BUTTON_3,TEXT("BT_WHISPER_BUTTON_3"),hInstance,false,false);
	m_btMessageSetting.SetButtonImage(IDB_MESSAGE_SETTING,hInstance,false,false);

	//设置变量
	m_bCreateFlag=true;

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//提示信息
	m_ChatMessage.InsertPromptString(TEXT("请勿在私聊中相信任何形式的中奖信息，请勿向任何人透露您的密码！\r\n"));

	return TRUE;
}

//确定消息
VOID CDlgWhisper::OnOK()
{
	return;
}

//取消消息
VOID CDlgWhisper::OnCancel()
{
	//关闭窗口
	DestroyWindow();

	return;
}

//命令函数
BOOL CDlgWhisper::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//命令处理
	switch (LOWORD(wParam))
	{
	case IDM_SEND_BY_ENTER:
		{
			//设置变量
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if (pParameterGlobal!=NULL) pParameterGlobal->m_bSendWhisperByEnter=true;

			//设置焦点
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_SEND_BY_CTRL_ENTER:
		{
			//设置变量
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if (pParameterGlobal!=NULL) pParameterGlobal->m_bSendWhisperByEnter=false;

			//设置焦点
			m_ChatInput.SetFocus();

			return TRUE;
		}
	}

	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	switch (LOWORD(wParam))
	{
	case IDM_SHOW_ALL:
		pParameterGlobal->m_cbMessageMode=MESSAGE_MODE_ALL;
		break;
	case IDM_HIDE_DETEST:
		pParameterGlobal->m_cbMessageMode=MESSAGE_MODE_DETEST;
		break;
	case IDM_ONLY_FRIEND:
		pParameterGlobal->m_cbMessageMode=MESSAGE_MODE_FRIEND;
		break;
	case IDM_HIDE_ALL:
		pParameterGlobal->m_cbMessageMode=MESSAGE_MODE_NONE;
		break;
	}

	//颜色选择

	return __super::OnCommand(wParam,lParam);
}

//表情事件
VOID CDlgWhisper::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	//设置焦点
	m_ChatInput.SetFocus();

	//发送表情
	if (EfficacyUserChat(NULL,pExpression->GetIndex())==true)
	{
		if (m_ClientUserItemArray.GetCount()==2L)
		{
			//私聊信息
			ASSERT(m_ClientUserItemArray[1]!=NULL);
			SendExpressionPacket(m_ClientUserItemArray[1],pExpression->GetIndex());
		}
		else if (m_ClientUserItemArray.GetCount()>=3L)
		{
			//群聊信息
			IClientUserItem * * pIClientUserItem=m_ClientUserItemArray.GetData();
		//	SendExpressionPacket(pIClientUserItem,(WORD)m_ClientUserItemArray.GetCount(),strMessage);
		}
		else
		{
			//构造提示
			TCHAR szMessage[LEN_USER_CHAT+128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("本次会话的所有用户已经离开，消息无法发送"));

			//插入信息
			m_ChatMessage.InsertString(TEXT("\r\n"),0);
			m_ChatMessage.InsertSystemString(szMessage);
		}
	}

	return;
}

//绘画消息
VOID CDlgWhisper::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//尺寸调整
	if(m_bShowHistory)nHeight -= NORMAL_HISTORY_HEGIHT;

	//输入位置
	CRect rcChatInput;
	rcChatInput.left=nLBorder;
	rcChatInput.bottom=nHeight-nBBorder;
	rcChatInput.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatInput.top=nHeight-CHAT_INPUT_CY-nBBorder;

	//信息位置
	CRect rcChatMessage;
	rcChatMessage.top=nTBorder;
	rcChatMessage.left=nLBorder;
	rcChatMessage.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatMessage.bottom=nHeight-CHAT_INPUT_CY-nBBorder;

	//控制区域
	CRect rcChatControl;
	rcChatControl.right=nWidth-5;
	rcChatControl.bottom=nHeight-9;
	rcChatControl.top=nTBorder+USER_INFO_CY+5;
	rcChatControl.left=nWidth-USER_INFO_CX-nRBorder;

	//绘画背景
	COLORREF crBackGround=RGB(198,208,220);
	int cy = m_bShowHistory ? (nHeight-nTBorder-nBBorder+NORMAL_HISTORY_OFFSET) : (nHeight-nTBorder-nBBorder);
	pDC->FillSolidRect(nLBorder,nTBorder,nWidth-nLBorder-nRBorder,cy,crBackGround);
	pDC->FillSolidRect(rcChatMessage.right,nTBorder+USER_INFO_CY,USER_INFO_CX,5,RGB(215,223,228));

	//绘画环绕
	m_EncircleChatInput.DrawEncircleFrame(pDC,rcChatInput);
	m_EncircleChatMessage.DrawEncircleFrame(pDC,rcChatMessage);

	//控制信息
	DrawUserInfoArea(pDC,nWidth-USER_INFO_CX-nRBorder,nTBorder,USER_INFO_CX,USER_INFO_CY);
	DrawFunctionArea(pDC,rcChatControl.left,rcChatControl.top,rcChatControl.Width(),rcChatControl.Height());

	//历史记录
	if(m_bShowHistory)
	{
		//历史记录
		CRect rcHistory;
		rcHistory.left = nLBorder;
		rcHistory.right = nWidth - nLBorder;
		rcHistory.top = rcChatInput.bottom+NORMAL_HISTORY_OFFSET;
		rcHistory.bottom = nHeight + NORMAL_HISTORY_HEGIHT - 10;

		m_EncircleHistoryEdit.DrawEncircleFrame(pDC,rcHistory);
	}

	return;
}

//废弃判断
bool CDlgWhisper::DisuseEstimate()
{
	//废弃判断
	if (m_hWnd==NULL) return true;
	if (m_ClientUserItemArray.GetCount()<=1L) return true;

	return false;
}

//用户比较
bool CDlgWhisper::CompareUserItem(DWORD dwUserID[], WORD wUserCount)
{
	//数目对比
	if (m_ClientUserItemArray.GetCount()!=wUserCount)
	{
		return false;
	}

	//用户对比
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		//用户标识
		DWORD dwUserItemID=m_ClientUserItemArray[i]->GetUserID();

		//用户对比
		WORD j;
		for (j=0;j<wUserCount;j++) 
		{
			if (dwUserID[j]==dwUserItemID) break;
		}

		//结果判断
		if (j==wUserCount) return false;
	}

	return true;
}

//聊天效验
bool CDlgWhisper::EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex)
{
	if( !m_pIMySelfUserItem ) return false;

	WORD wUserCount = (WORD)m_ClientUserItemArray.GetCount();

	//是否没有管理员
	bool bAllNotMaster = true;
	if( bAllNotMaster )
	{
		for( WORD i = 0; i < wUserCount; i++ )
		{
			if( m_ClientUserItemArray[i]->GetMasterOrder() > 0 )
			{
				bAllNotMaster = false;
				break;
			}
		}
	}

	//是否都是游戏状态
	bool bAllPlaying = true;
	if( bAllPlaying )
	{
		for( WORD i = 0; i < wUserCount; i++ )
		{
			if( m_ClientUserItemArray[i]->GetUserStatus() != US_PLAYING )
			{
				bAllPlaying = false;
				break;
			}
		}
	}

	//是否都同一桌
	WORD wTableId = m_pIMySelfUserItem->GetTableID();
	bool bAllSameTable = (wTableId==INVALID_TABLE?false:true);
	if( bAllSameTable )
	{
		for( WORD i = 0; i < wUserCount; i++ )
		{
			if( wTableId != m_ClientUserItemArray[i]->GetTableID() )
			{
				bAllSameTable = false;
				break;
			}
		}
	}

	//权限判断
	if (CUserRight::CanWisper(m_dwUserRight)==false && bAllNotMaster )
	{
		ShowInformation(TEXT("很抱歉，您没有发送私聊信息的权限！"),MB_OK,10);
		return false;
	}

	//房间禁止私聊
	DWORD dwServerRule=m_pServerViewItem->GetServerRule();
	if( CServerRule::IsForfendWisperChat(dwServerRule) && bAllNotMaster )
	{
		//if( CServerRule::IsMemberCanChat(m_dwServerRule) == false || m_pIMyselfUserItem->GetMemberOrder() == 0 )
		{
			ShowInformation(TEXT("抱歉，此游戏房间不允许用户私聊！"),MB_OK,10);
			return false;
		}
	}

	//同桌游戏私聊
	if( CServerRule::IsForfendWisperOnGame(dwServerRule) && bAllSameTable && bAllNotMaster && bAllPlaying &&
		!CUserRight::CanGameChat(m_dwUserRight))
	{
		ShowInformation(TEXT("抱歉，此游戏房间不允许在游戏中与同桌的玩家私聊！"),MB_OK,10);
		return false;
	}

	return true;
}

//创建窗口
bool CDlgWhisper::CreateWhisperWnd(DWORD dwConversationID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd)
{
	//设置变量
	m_ClientUserItemArray.RemoveAll();
	m_dwConversationID=dwConversationID;

	//用户收集
	WORD wOppositeCount = 0;
	DWORD dwOppositeGameID = 0;
	IClientUserItem *pOppositeUserItem=NULL;
	for (WORD i=0;i<wUserCount;i++)
	{
		//终止判断
		if (wUserCount>=MAX_WHISPER_USER) 
		{
			ASSERT(FALSE);
			break;
		}

		//插入用户
		ASSERT(pIClientUserItem[i]!=NULL);
		m_ClientUserItemArray.Add(pIClientUserItem[i]);

		if( pIClientUserItem[i] != m_pIMySelfUserItem )
		{
			wOppositeCount++;
			dwOppositeGameID = pIClientUserItem[i]->GetGameID();
			pOppositeUserItem= pIClientUserItem[i];
		}
	}

	//创建窗口
	if (m_hWnd!=NULL)
	{
		//变量定义
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//更新窗口
		RectifyTitle();
	}
	else 
	{
		//创建窗口
		Create(IDD_DLG_WHISPER,pParentWnd);
	}

	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btAddFriend.SetButtonImage(IDB_SET_FRIEND,hInstance,false,false);
	m_btSetDetest.SetButtonImage(IDB_SET_DETEST, hInstance,false,false);
	m_btShieldThisChat.SetButtonImage(IDB_SHIELD_WHISPER, hInstance,false, false);

	//加载记录
	if( m_pIChatHistory == NULL && m_pIChatHistroyManager && m_pIMySelfUserItem &&
		wOppositeCount == 1 && dwOppositeGameID != 0L )
	{
		//设置ID
		m_dwChatHistroyID = dwOppositeGameID;

		//激活记录接口
		m_pIChatHistory = m_pIChatHistroyManager->ActiveChatHistory(m_dwChatHistroyID);

		//更新按钮
		BYTE cbSourCompanion=pOppositeUserItem->GetUserCompanion();

		if(cbSourCompanion==CP_FRIEND)
			m_btAddFriend.SetButtonImage(IDB_CANCEL_FRIEND,hInstance,false,false);
		else
			m_btAddFriend.SetButtonImage(IDB_SET_FRIEND,hInstance,false,false);

		if(cbSourCompanion==CP_DETEST)
			m_btSetDetest.SetButtonImage(IDB_CANCEL_DETEST, hInstance,false,false);
		else
			m_btSetDetest.SetButtonImage(IDB_SET_DETEST, hInstance,false,false);

		if(cbSourCompanion==CP_SHIELD)
			m_btShieldThisChat.SetButtonImage(IDB_RECEVE_WHISPER, hInstance,false, false);
		else
			m_btShieldThisChat.SetButtonImage(IDB_SHIELD_WHISPER, hInstance,false, false);
	}

	//置顶窗口
	SetActiveWindow();
	BringWindowToTop();
	SetForegroundWindow();

	return true;
}

//用户进入
VOID CDlgWhisper::OnEventUserEnter(IClientUserItem * pIClientUserItem)
{
	//用户搜索
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (pIClientUserItem==m_ClientUserItemArray[i])
		{
			ASSERT(FALSE);
			return;
		}
	}

	//设置数据
	m_ClientUserItemArray.Add(pIClientUserItem);

	//事件通知
	m_ChatMessage.InsertUserAccounts(pIClientUserItem->GetNickName());
	m_ChatMessage.InsertString(TEXT("加入了本次会话\r\n"),COLOR_EVENT);

	//更新标题
	RectifyTitle();

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

	return;
}

//用户离开
VOID CDlgWhisper::OnEventUserLeave(IClientUserItem * pIClientUserItem)
{
	//用户搜索
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (pIClientUserItem==m_ClientUserItemArray[i])
		{
			//设置数据
			m_ClientUserItemArray.RemoveAt(i);

			//会话设置
			if (m_ClientUserItemArray.GetCount()<=2)
			{
				m_dwConversationID=0L;
			}

			//保存并关闭记录
			CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
			if( m_ClientUserItemArray.GetCount() < 2 && m_pIChatHistory && pParameterGlobal->m_bSaveWhisperChat)
			{
				ASSERT(m_dwChatHistroyID>0);
				m_pIChatHistory->Save();
				SafeRelease(m_pIChatHistory);
				m_dwChatHistroyID = 0L;
			}

			//插入换行
			LONG lTextLength=m_ChatMessage.GetWindowTextLength();
			if (lTextLength!=0L) m_ChatMessage.InsertString(TEXT("\r\n"),COLOR_EVENT);

			//事件通知
			m_ChatMessage.InsertUserAccounts(pIClientUserItem->GetNickName());
			m_ChatMessage.InsertString(TEXT("离开了本次会话\r\n"),COLOR_EVENT);

			//更新标题
			RectifyTitle();

			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

			return;
		}
	}

	return;
}

//用户状态
VOID CDlgWhisper::OnEventUserStatus(IClientUserItem * pIClientUserItem)
{
	//状态更新
	for (INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if (m_ClientUserItemArray[i]==pIClientUserItem)
		{
			//更新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);

			break;
		}
	}

	return;
}

//用户表情
VOID CDlgWhisper::OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	CExpressionManager * pExpressionManager=CExpressionManager::GetInstance();
	CExpression * pExpressionItem=pExpressionManager->GetExpressionItem(wItemIndex);

	//插入消息
	if (pExpressionItem!=NULL)
	{
		//状态变量
		DWORD dwSendUserID=pIClientUserItem->GetUserID();
		bool bMySelfString=(pGlobalUserData->dwUserID==dwSendUserID);

		//获取路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		pExpressionItem->GetExpressionPath(szImagePath,CountArray(szImagePath));

		//显示消息
		LPCTSTR pszNiceName=pIClientUserItem->GetNickName();
		m_ChatMessage.InsertExpression(pszNiceName,szImagePath,bMySelfString);
	}

	//显示控制
	if (IsWindowVisible()==FALSE)
	{
		if (pParameterGlobal->m_bAutoShowWhisper==true)
		{
			//显示窗口
			ShowWindow(SW_RESTORE);

			//置顶窗口
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();
		}
		else
		{
			//收集窗口
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->InsertWhisperItem(this);
		}
	}
	else
	{
		//激活窗口
		if (IsChild(GetFocus())==FALSE)
		{
			FlashWindow(TRUE);
		}
	}

	return;
}

//用户私聊
VOID CDlgWhisper::OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//状态变量
	DWORD dwSendUserID=pIClientUserItem->GetUserID();
	bool bMySelfString=(pGlobalUserData->dwUserID==dwSendUserID);

	//显示消息
	LPCTSTR pszNiceName=pIClientUserItem->GetNickName();
	m_ChatMessage.InsertWhisperChat(pszNiceName,pszString,rcColor,bMySelfString);

	//聊天记录
	if( m_pIChatHistory )
	{
		m_pIChatHistory->AddMsgItem(pszNiceName,pszString,rcColor,bMySelfString);
	}

	//显示控制
	if (IsWindowVisible()==FALSE)
	{
		if (pParameterGlobal->m_bAutoShowWhisper==true)
		{
			//显示窗口
			ShowWindow(SW_RESTORE);

			//置顶窗口
			SetActiveWindow();
			BringWindowToTop();
			SetForegroundWindow();
		}
		else
		{
			//收集窗口
			CPlatformFrame * pPlatformFrame=CPlatformFrame::GetInstance();
			if (pPlatformFrame!=NULL) pPlatformFrame->InsertWhisperItem(this);
		}
	}
	else
	{
		//激活窗口
		if (IsChild(GetFocus())==FALSE)
		{
			FlashWindow(TRUE);
		}
	}

	return;
}

//更新标题
VOID CDlgWhisper::RectifyTitle()
{
	//变量定义
	TCHAR szTitle[MAX_PATH]=TEXT("");
	LPCTSTR pszServerName=m_pServerViewItem->GetServerName();

	//构造标题
	if (m_ClientUserItemArray.GetCount()<=2)
	{
		_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] － 私聊"),pszServerName);
	}
	else
	{
		_sntprintf(szTitle,CountArray(szTitle),TEXT("[ %s ] － 多人聊天"),pszServerName);
	}

	//设置标题
	SetWindowText(szTitle);

	return;
}

//调整控件
VOID CDlgWhisper::RectifyControl(INT nWidth, INT nHeight)
{
	//状态判断
	if (m_bCreateFlag==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//尺寸调整
	if(m_bShowHistory)nHeight -= NORMAL_HISTORY_HEGIHT;

	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//输入位置
	CRect rcChatInput;
	rcChatInput.left=nLBorder;
	rcChatInput.bottom=nHeight-nBBorder;
	rcChatInput.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatInput.top=nHeight-CHAT_INPUT_CY-nBBorder;

	//信息位置
	CRect rcChatMessage;
	rcChatMessage.top=nTBorder;
	rcChatMessage.left=nLBorder;
	rcChatMessage.right=nWidth-USER_INFO_CX-nRBorder;
	rcChatMessage.bottom=nHeight-CHAT_INPUT_CY-nBBorder;

	//历史记录
	CRect rcHistory;
	rcHistory.left = nLBorder;
	rcHistory.right = nWidth - nLBorder;
	rcHistory.top = rcChatInput.bottom+NORMAL_HISTORY_OFFSET;
	rcHistory.bottom = nHeight + NORMAL_HISTORY_HEGIHT - 10;

	//移动控件
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//环绕资源
	m_EncircleChatInput.DeferWindowPos(&m_ChatInput,hDwp,rcChatInput);
	m_EncircleChatMessage.DeferWindowPos(&m_ChatMessage,hDwp,rcChatMessage);
	m_EncircleHistoryEdit.DeferWindowPos(&m_HistoryEdit,hDwp,rcHistory);

	//用户菜单
	DeferWindowPos(hDwp,m_btUserMenu,NULL,nWidth-123-nRBorder,nTBorder+13,0,0,uFlags|SWP_NOSIZE);

	//聊天按钮
	CRect rcChatButton;
	m_btChatButton1.GetWindowRect(&rcChatButton);
	DeferWindowPos(hDwp,m_btChatButton1,NULL,rcChatInput.left+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatButton2,NULL,rcChatInput.left+rcChatButton.Width()+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btChatButton3,NULL,rcChatInput.left+rcChatButton.Width()*2+10,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);

	//聊天记录
	CRect rcChatHistory;
	m_btChatHistory.GetWindowRect(&rcChatHistory);
	DeferWindowPos(hDwp,m_btChatHistory,NULL,rcChatInput.right-rcChatHistory.Width()-6,rcChatInput.top+3,0,0,uFlags|SWP_NOSIZE);

	//控制按钮
	CRect rcControl;
	CRect rcSendControl;
	m_btCancel.GetWindowRect(&rcControl);
	m_btSendControl.GetWindowRect(&rcSendControl);
	DeferWindowPos(hDwp,m_btSendControl,NULL,rcChatInput.right-rcSendControl.Width()-3,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancel,NULL,rcChatInput.right-rcSendControl.Width()-rcControl.Width()*2-13,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btSendChat,NULL,rcChatInput.right-rcSendControl.Width()-rcControl.Width()*1-8,nHeight-rcControl.Height()-12,0,0,uFlags|SWP_NOSIZE);

	int ny = nTBorder+USER_INFO_CY+15;
	DeferWindowPos(hDwp, m_btAddFriend, NULL, rcChatMessage.right+10, ny, 0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btSetDetest, NULL, rcChatMessage.right+10, ny+27, 0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btShieldThisChat, NULL, rcChatMessage.right+10, ny+54, 0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btMessageSetting, NULL, rcChatMessage.right+10, ny+81, 0,0,uFlags|SWP_NOSIZE);

	//移动结束
	EndDeferWindowPos(hDwp);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE);

	return;
}

//排序函数
VOID CDlgWhisper::SortUserIDData(DWORD dwUserID[], WORD wItemCount)
{
	//变量定义
	bool bSorted=true;
	DWORD dwTempUserID=0L;
	WORD wLastIndex=wItemCount-1;

	//排序操作
	do
	{
		//设置变量
		bSorted=true;

		//排序操作
		for (WORD i=0;i<wLastIndex;i++)
		{
			//交换位置
			if (dwUserID[i]<dwUserID[i+1])
			{
				bSorted=false;
				dwTempUserID=dwUserID[i];
				dwUserID[i]=dwUserID[i+1];
				dwUserID[i+1]=dwTempUserID;
			}	
		}

		//设置索引
		wLastIndex--;

	} while (bSorted==false);

	return;
}

//绘画控制
VOID CDlgWhisper::DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//变量定义
	ASSERT(CSkinRenderManager::GetInstance()!=NULL);
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//加载资源
	CBitImage ImageControlT;
	CBitImage ImageControlM;
	CBitImage ImageControlB;
	ImageControlT.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_T);
	ImageControlM.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_M);
	ImageControlB.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_CONTRUL_B);

	//渲染资源
	pSkinRenderManager->RenderImage(ImageControlT);
	pSkinRenderManager->RenderImage(ImageControlM);
	pSkinRenderManager->RenderImage(ImageControlB);

	//用户信息
	ImageControlT.BitBlt(pDC->m_hDC,nXPos,nYPos);
	ImageControlB.BitBlt(pDC->m_hDC,nXPos,nYPos+nHeight-ImageControlB.GetHeight());

	//绘画中间
	INT nYStart=nYPos+ImageControlT.GetHeight();
	INT nYTerminate=nYPos+nHeight-ImageControlB.GetHeight();
	for (INT nYDrawPos=nYStart;nYDrawPos<nYTerminate;nYDrawPos+=ImageControlM.GetHeight())
	{
		INT nWidth=ImageControlM.GetWidth();
		ImageControlM.BitBlt(pDC->m_hDC,nXPos,nYDrawPos,nWidth,__min(nYTerminate-nYDrawPos,nWidth),0,0);
	}

	return;
}

//绘画用户
VOID CDlgWhisper::DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//加载资源
	CBitImage ImageUserInfoT;
	CBitImage ImageUserInfoM;
	CBitImage ImageUserInfoB;
	ImageUserInfoT.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_T);
	ImageUserInfoM.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_M);
	ImageUserInfoB.LoadFromResource(AfxGetInstanceHandle(),IDB_WHISPER_USER_INFO_B);

	//用户信息
	ImageUserInfoT.BitBlt(pDC->m_hDC,nXPos,nYPos);
	ImageUserInfoB.BitBlt(pDC->m_hDC,nXPos,nYPos+nHeight-ImageUserInfoB.GetHeight());

	//绘画中间
	INT nYStart=nYPos+ImageUserInfoT.GetHeight();
	INT nYTerminate=nYPos+nHeight-ImageUserInfoB.GetHeight();
	for (INT nYDrawPos=nYStart;nYDrawPos<nYTerminate;nYDrawPos+=ImageUserInfoM.GetHeight())
	{
		INT nWidth=ImageUserInfoM.GetWidth();
		ImageUserInfoM.BitBlt(pDC->m_hDC,nXPos,nYDrawPos,nWidth,__min(nYTerminate-nYDrawPos,nWidth),0,0);
	}

	//用户信息
	if (m_ClientUserItemArray.GetCount()>=2L)
	{
		//变量定义
		ASSERT(CFaceItemControl::GetInstance()!=NULL);
		IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();

		//变量定义
		ASSERT(CUserItemElement::GetInstance()!=NULL);
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

		//获取用户
		IClientUserItem * pIClientUserItem=m_ClientUserItemArray[1];
		tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

		//游戏状态
		BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
		LPCTSTR pszUserStatus[]={TEXT("空闲"),TEXT("入座"),TEXT("准备"),TEXT("旁观"),TEXT("游戏"),TEXT("断线")};

		//绘画头像
		if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos+13,nYPos+14,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos+13,nYPos+14,pIClientUserItem->GetFaceID());
		}

		//设置 DC
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));

		//经验信息
		DWORD dwExperience=pIClientUserItem->GetUserExperience();
		WORD wExperienceLevel=pUserItemElement->GetExperienceLevel(dwExperience);

		//等级信息
		LPCTSTR pszKindName=m_pIGameLevelParser->GetKindName();
		LPCTSTR pszGameLevel=m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem);

		//用户信息
		TCHAR szString[128]=TEXT("");
		LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
		LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

		//信息位置
		CRect rcNickName(nXPos+80,nYPos+18,nXPos+nWidth-5,nYPos+18+12);
		CRect rcUserGameID(nXPos+80,nYPos+40,nXPos+nWidth-5,nYPos+40+12);

		//信息位置
		CRect rcGameLevel(nXPos+11,nYPos+74,nXPos+nWidth-5,nYPos+74+12);
		CRect rcExperience(nXPos+11,nYPos+95,nXPos+nWidth-5,nYPos+95+12);
		CRect rcGameScore(nXPos+11,nYPos+116,nXPos+nWidth-5,nYPos+116+12);
		CRect rcUserStatus(nXPos+11,nYPos+137,nXPos+nWidth-5,nYPos+137+12);
		CRect rcUnderWrite(nXPos+11,nYPos+158,nXPos+nWidth-5,nYPos+158+12);

		//用户信息
		_sntprintf(szString,CountArray(szString),TEXT("%s"),pIClientUserItem->GetNickName());
		pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户标识
		_sntprintf(szString,CountArray(szString),TEXT("%ld"),pIClientUserItem->GetGameID());
		pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//游戏等级
		_sntprintf(szString,CountArray(szString),TEXT("级别：%s [ %s ]"),pszGameLevel,pszKindName);
		pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户经验
		_sntprintf(szString,CountArray(szString),TEXT("经验：%d 级"),wExperienceLevel);
		pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//经验标志
		CSize SizeExperience=pDC->GetTextExtent(szString,lstrlen(szString));
		pUserItemElement->DrawExperience(pDC,rcExperience.left+SizeExperience.cx+6,rcExperience.top-4L,dwExperience);

		//用户成绩
		_sntprintf(szString,CountArray(szString),TEXT("成绩：%I64d"),pIClientUserItem->GetUserScore());
		pDC->DrawText(szString,lstrlen(szString),&rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//游戏状态
		_sntprintf(szString,CountArray(szString),TEXT("状态：%s"),pszUserStatus[cbUserStatus-1]);
		pDC->DrawText(szString,lstrlen(szString),&rcUserStatus,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//用户签名
		_sntprintf(szString,CountArray(szString),TEXT("签名：%s"),pIClientUserItem->GetUnderWrite());
		pDC->DrawText(szString,lstrlen(szString),&rcUnderWrite,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	return;
}

//发送表情
bool CDlgWhisper::SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex)
{
	//变量定义
	CMD_GR_C_WisperExpression WisperExpression;
	ZeroMemory(&WisperExpression,sizeof(WisperExpression));

	//构造信息
	WisperExpression.wItemIndex=wItemIndex;
	WisperExpression.dwTargetUserID=pIClientUserItem->GetUserID();

	//发送命令
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));

	return true;
}

//发送聊天
bool CDlgWhisper::SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造信息
	CMD_GR_C_WisperChat WisperChat;
	lstrcpyn(WisperChat.szChatString,pszChatString,CountArray(WisperChat.szChatString));

	//构造数据
	WisperChat.dwChatColor=pParameterGlobal->m_crWhisperTX;
	WisperChat.dwTargetUserID=pIClientUserItem->GetUserID();
	WisperChat.wChatLength=CountStringBuffer(WisperChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(WisperChat)-sizeof(WisperChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));

	return true;
}

//发送会话
bool CDlgWhisper::SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString)
{
	//变量定义
	CMD_GR_ColloquyChat ColloquyChat;
	ZeroMemory(&ColloquyChat,sizeof(ColloquyChat));

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//构造用户
	for (WORD i=0;i<wUserCount;i++)
	{
		//效验数据
		ASSERT(i<CountArray(ColloquyChat.dwTargetUserID));
		if (i>=CountArray(ColloquyChat.dwTargetUserID)) break;

		//设置数据
		ColloquyChat.dwTargetUserID[i]=pIClientUserItem[i]->GetUserID();
	}

	//数据排序
	SortUserIDData(ColloquyChat.dwTargetUserID,wUserCount);

	//会话信息
	lstrcpyn(ColloquyChat.szChatString,pszChatString,CountArray(ColloquyChat.szChatString));

	//会话属性
	ColloquyChat.dwConversationID=m_dwConversationID;
	ColloquyChat.dwSendUserID=pGlobalUserData->dwUserID;
	ColloquyChat.dwChatColor=pParameterGlobal->m_crWhisperTX;
	ColloquyChat.wChatLength=CountStringBuffer(ColloquyChat.szChatString);

	//发送命令
	WORD wHeadSize=sizeof(ColloquyChat)-sizeof(ColloquyChat.szChatString);
	m_pITCPSocket->SendData(MDM_GR_USER,SUB_GR_COLLOQUY_CHAT,&ColloquyChat,wHeadSize+ColloquyChat.wChatLength*sizeof(ColloquyChat.szChatString[0]));

	return true;
}

//用户菜单
VOID CDlgWhisper::OnBnClickedUserMenu()
{
	//弹出菜单
	if (m_ClientUserItemArray.GetCount()>=2L)
	{
		ASSERT(m_ClientUserItemArray[1]!=NULL);
		m_pServerViewItem->TrackUserItemMenu(m_ClientUserItemArray[1]);
	}

	return;
}

//发送消息
VOID CDlgWhisper::OnBnClickedSendChat()
{
	//获取信息
	CString strMessage;
	m_ChatInput.GetWindowText(strMessage);

	//发送信息
	if (strMessage.IsEmpty()==FALSE)
	{
		if (m_ClientUserItemArray.GetCount()==2L)
		{
			//私聊信息
			ASSERT(m_ClientUserItemArray[1]!=NULL);
			SendWhisperChatPacket(m_ClientUserItemArray[1],strMessage);
		}
		else if (m_ClientUserItemArray.GetCount()>=3L)
		{
			//群聊信息
			IClientUserItem * * pIClientUserItem=m_ClientUserItemArray.GetData();
			SendColloquyChatPacket(pIClientUserItem,(WORD)m_ClientUserItemArray.GetCount(),strMessage);
		}
		else
		{
			//构造提示
			TCHAR szMessage[LEN_USER_CHAT+128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("本次会话的所有用户已经离开，“%s”消息无法发送"),strMessage);

			//插入信息
			m_ChatMessage.InsertString(TEXT("\r\n"),0);
			m_ChatMessage.InsertSystemString(szMessage);
		}
	}
	else
	{
		//插入信息
		m_ChatMessage.InsertCustomString(TEXT("发送内容不能为空，请输入内容"),RGB(125,125,125));
	}

	//设置焦点
	m_ChatInput.SetFocus();
	m_ChatInput.SetWindowText(TEXT(""));

	return;
}

//发送控制
VOID CDlgWhisper::OnBnClickedSendControl()
{
	//构造菜单
	CSkinMenu Menu;
	Menu.CreateMenu();

	//构造菜单
	Menu.AppendMenu(IDM_SEND_BY_ENTER,TEXT("按 Enter 键发送消息"));
	Menu.AppendMenu(IDM_SEND_BY_CTRL_ENTER,TEXT("按 Ctrl+Enter 键发送消息"));

	//设置菜单
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if (pParameterGlobal->m_bSendWhisperByEnter==true) Menu.CheckMenuItem(IDM_SEND_BY_ENTER,MF_BYCOMMAND|MF_CHECKED);
	if (pParameterGlobal->m_bSendWhisperByEnter==false) Menu.CheckMenuItem(IDM_SEND_BY_CTRL_ENTER,MF_BYCOMMAND|MF_CHECKED);

	//获取位置
	CRect rcButton;
	m_btSendControl.GetWindowRect(&rcButton);

	//弹出菜单
	Menu.TrackPopupMenu(rcButton.left,rcButton.bottom,this);

	return;
}

//聊天按钮
VOID CDlgWhisper::OnBnClickedChatButton1()
{
	//创建表情
	CRect rcButton;
	m_btChatButton1.GetWindowRect(&rcButton);

	//创建窗口
	if (m_pExpressionControl==NULL)
	{
		m_pExpressionControl=new CExpressionControl;
	}

	//显示窗口
	m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);

	return;
}

//聊天按钮
VOID CDlgWhisper::OnBnClickedChatButton2()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	INT nWidth=72,nHeight=16;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//构造颜色
	CImage ImageColor[CountArray(crColor)];
	for (INT i=0;i<CountArray(ImageColor);i++)
	{
		//创建位图
		ImageColor[i].Create(nWidth,nHeight,16);

		//获取设备
		CImageDC BufferDC(ImageColor[i]);
		CDC * pDC=CDC::FromHandle(BufferDC);

		//绘画位图
		pDC->FillSolidRect(2,2,nWidth-4,nHeight-4,crColor[i]);
		pDC->Draw3dRect(0,0,nWidth,nHeight,RGB(0,0,0),RGB(0,0,0));
		pDC->Draw3dRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255),RGB(255,255,255));
	}

	//创建菜单
	CSkinMenu ColorMenu;
	ColorMenu.CreateMenu();

	//构造菜单
	ColorMenu.AppendMenu(IDM_MORE_COLOR,TEXT("选择其他颜色"));

	//颜色选项
	ColorMenu.AppendSeparator();
	for (INT i=0;i<CountArray(ImageColor);i++) 
	{
		ColorMenu.AppendMenu(IDM_SELECT_CHAT_COLOR+i,ImageColor[i]);
		if (pParameterGlobal->m_crWhisperTX==crColor[i]) ColorMenu.CheckMenuItem(IDM_SELECT_CHAT_COLOR+i,MF_BYCOMMAND|MF_CHECKED);
	}

	//弹出菜单
	CRect rcButton;
	m_btChatButton2.GetWindowRect(&rcButton);
	ColorMenu.TrackPopupMenu(rcButton.right,rcButton.top,this);

	return;
}

//聊天按钮
VOID CDlgWhisper::OnBnClickedChatButton3()
{
	//设置焦点
	m_ChatInput.SetFocus();

	//聊天清屏
	m_ChatMessage.CleanScreen();

	return;
}

//聊天记录
VOID CDlgWhisper::OnBnClickedChatHistory()
{
	//变量定义
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置变量
	//m_bRectifyHistory = false;
	if( m_bShowHistory )
	{
		m_bShowHistory = false;

		INT nChangeHeight = NORMAL_HISTORY_HEGIHT;//+Y_SPLITTER_HISTORY;

		//改变窗口
		SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height()-nChangeHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);
	}
	else
	{
		m_bShowHistory = true;

		INT nChangeHeight = NORMAL_HISTORY_HEGIHT;//+Y_SPLITTER_HISTORY;

		//改变窗口
		SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height()+nChangeHeight,SWP_NOMOVE|SWP_NOZORDER|SWP_NOCOPYBITS);

		//加载聊天记录
		LoadChatHistory();
	}

	return;
}

//更多颜色
VOID CDlgWhisper::OnSelectMoreColor()
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//设置颜色
	CColorDialog ColorDialog(pParameterGlobal->m_crWhisperTX,CC_FULLOPEN,this);
	if (ColorDialog.DoModal()==IDOK) pParameterGlobal->m_crWhisperTX=ColorDialog.GetColor();

	//设置界面
	m_ChatInput.SetFocus();

	return;
}

//选择颜色
VOID CDlgWhisper::OnSelectChatColor(UINT nCommandID)
{
	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//变量定义
	UINT nIndex=nCommandID-IDM_SELECT_CHAT_COLOR;
	COLORREF crColor[]={RGB(255,0,0),RGB(255,255,0),RGB(128,255,0),RGB(0,255,255),
		RGB(0,0,128),RGB(0,128,0),RGB(128,0,128),RGB(128,0,0),RGB(0,0,0)};

	//选择颜色
	ASSERT(nIndex<CountArray(crColor));
	if (nIndex<CountArray(crColor)) pParameterGlobal->m_crWhisperTX=crColor[nIndex];

	//设置焦点
	m_ChatInput.SetFocus();

	return;
}

//销毁消息
VOID CDlgWhisper::OnNcDestroy()
{
	//状态变量
	m_bCreateFlag=false;
	m_bShowHistory=false;

	//设置变量
	m_dwConversationID=0L;
	m_ClientUserItemArray.RemoveAll();

	//保存记录
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	if( m_pIChatHistory )
	{
		if(pParameterGlobal->m_bSaveWhisperChat)
			m_pIChatHistory->Save();
		SafeRelease(m_pIChatHistory);
	}
	m_dwChatHistroyID = 0L;

	//删除对象
	SafeDelete(m_pExpressionControl);

	__super::OnNcDestroy();
}

//位置消息
VOID CDlgWhisper::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//获取大小
VOID CDlgWhisper::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = MIN_WIDTH;
	lpMMI->ptMinTrackSize.y = MIN_HEIGHT+(m_bShowHistory?NORMAL_HISTORY_HEGIHT:0);
}

//加载聊天记录
VOID CDlgWhisper::LoadChatHistory()
{
	//重置控件
	m_HistoryEdit.SetSel(0,-1);
	m_HistoryEdit.ReplaceSel(NULL);

	if( m_pIChatHistory && m_dwChatHistroyID != 0 )
	{
		if( m_pIChatHistory->GetMsgItemCount() > 0 )
		{
			INT_PTR nIndex = 0;
			IChatMsgItem *pIChatMsgItem = NULL;
			SYSTEMTIME ChatTime = {0}, CurTime = {0};
			GetLocalTime(&CurTime);
			while( pIChatMsgItem = m_pIChatHistory->EnumChatMsgItem(nIndex++) )
			{
				//插入时间
				const SYSTEMTIME &MsgTime = pIChatMsgItem->GetMsgTime();
				if( MsgTime.wYear != ChatTime.wYear || MsgTime.wMonth != ChatTime.wMonth || MsgTime.wDay != ChatTime.wDay )
				{
					ChatTime = MsgTime;
					TCHAR szTime[64] = {0};
					_sntprintf( szTime,CountArray(szTime),TEXT("----------------日期：%4d-%2d-%2d-------------\r\n"),MsgTime.wYear,MsgTime.wMonth,MsgTime.wDay );
					m_HistoryEdit.InsertCustomString( szTime,COLOR_EVENT );
				}

				m_HistoryEdit.InsertWhisperChat(pIChatMsgItem->GetSendUserAccounts(),pIChatMsgItem->GetMsg(),
					pIChatMsgItem->GetChatColor(),pIChatMsgItem->IsSelfMsg(),pIChatMsgItem->GetMsgTime());
			}
		}
	}
}

//屏蔽私聊
VOID CDlgWhisper::OnBnClickedShieldThisChat()
{
	//获取用户
	IClientUserItem *pTargetUser=NULL;
	for(INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		pTargetUser=m_ClientUserItemArray.GetAt(i);
		if(pTargetUser!=m_pIMySelfUserItem)
		{			
			break;
		}
	}
	if(pTargetUser!=NULL)
	{
		//变量定义
		ASSERT(CUserInformation::GetInstance()!=NULL);
		CUserInformation * pUserInformation=CUserInformation::GetInstance();
		BYTE cbCompanion=pTargetUser->GetUserCompanion();

		if(cbCompanion==CP_SHIELD)
		{
			pUserInformation->InsertCompanionInfo(pTargetUser,CP_NORMAL);
			m_btShieldThisChat.SetButtonImage(IDB_SHIELD_WHISPER, AfxGetInstanceHandle(),false, false);
			pTargetUser->SetUserCompanion(CP_NORMAL);
		}
		else
		{
			m_btShieldThisChat.SetButtonImage(IDB_RECEVE_WHISPER, AfxGetInstanceHandle(),false, false);
			pUserInformation->InsertCompanionInfo(pTargetUser,CP_SHIELD);
			pTargetUser->SetUserCompanion(CP_SHIELD);
		}
	}
}

//加为好友
VOID CDlgWhisper::OnBnClickAddFriend()
{
	//获取用户
	IClientUserItem *pTargetUser=NULL;
	for(INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if(m_ClientUserItemArray.GetAt(i)!=m_pIMySelfUserItem)
		{
			pTargetUser=m_ClientUserItemArray.GetAt(i);
			break;
		}
	}
	if(pTargetUser!=NULL)
	{
		BYTE cbCompanion=pTargetUser->GetUserCompanion();
		if(cbCompanion==CP_FRIEND)
			m_btAddFriend.SetButtonImage(IDB_SET_FRIEND,AfxGetInstanceHandle(),false,false);
		else
		{
			m_btAddFriend.SetButtonImage(IDB_CANCEL_FRIEND,AfxGetInstanceHandle(),false,false);
			m_btSetDetest.SetButtonImage(IDB_SET_DETEST, AfxGetInstanceHandle(),false,false);
		}
		::PostMessage(m_pServerViewItem->GetSafeHwnd(),WM_ADD_FRIEND,(WPARAM)pTargetUser,0);
	}
}

//设为厌恶
VOID CDlgWhisper::OnBnClickSetDetest()
{
	//获取用户
	IClientUserItem *pTargetUser=NULL;
	for(INT_PTR i=0;i<m_ClientUserItemArray.GetCount();i++)
	{
		if(m_ClientUserItemArray.GetAt(i)!=m_pIMySelfUserItem)
		{
			pTargetUser=m_ClientUserItemArray.GetAt(i);
			break;
		}
	}
	if(pTargetUser!=NULL)
	{
		BYTE cbCompanion=pTargetUser->GetUserCompanion();
		if(cbCompanion==CP_DETEST)
			m_btSetDetest.SetButtonImage(IDB_SET_DETEST, AfxGetInstanceHandle(),false,false);
		else
		{
			m_btSetDetest.SetButtonImage(IDB_CANCEL_DETEST, AfxGetInstanceHandle(),false,false);			
			m_btAddFriend.SetButtonImage(IDB_SET_FRIEND,AfxGetInstanceHandle(),false,false);
		}

		::PostMessage(m_pServerViewItem->GetSafeHwnd(),WM_SET_DETEST,(WPARAM)pTargetUser,0);
	}
}

//屏蔽私聊
VOID CDlgWhisper::OnBnClickMessageSetting()
{
	//变量定义
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//构造菜单
	CSkinMenu MessageSetting;
	MessageSetting.CreateMenu();

	//发送私聊
	MessageSetting.AppendMenu(IDM_SHOW_ALL,TEXT("显示全部的聊天信息"),(pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_ALL)?MF_CHECKED:0);
	MessageSetting.AppendMenu(IDM_HIDE_DETEST,TEXT("屏蔽厌恶玩家的聊天信息"),(pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_DETEST)?MF_CHECKED:0);
	MessageSetting.AppendMenu(IDM_ONLY_FRIEND,TEXT("只显示好友和社团成员的聊天信息"),(pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_FRIEND)?MF_CHECKED:0);
	//MessageSetting.AppendMenu(IDM_HIDE_ALL,TEXT("屏蔽所有玩家的私聊消息"),(pParameterGlobal->m_cbMessageMode==MESSAGE_MODE_NONE)?MF_CHECKED:0);

	//弹出菜单
	MessageSetting.TrackPopupMenu(this);
	return;
}

//////////////////////////////////////////////////////////////////////////
