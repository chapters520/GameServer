#include "StdAfx.h"
#include "Resource.h"
#include "DlgManageUser.h"
#include "DlgKickUser.h"
#include "DlgDismissGame.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgManageUser, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgManageUser::CDlgManageUser() : CSkinDialog(IDD_DLG_MANAGE_USER)
{
	//设置变量
	m_dwMasterRight = 0;
	m_pITCPSocket=NULL;

	return;
}

//析构函数
CDlgManageUser::~CDlgManageUser()
{
}

//控件绑定
VOID CDlgManageUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KICK_ALL_USER, m_btKickAllUser);
	DDX_Control(pDX, IDC_DISMISS_GAME, m_btDismissGame);
	DDX_Control(pDX, IDC_SEE_USER_IP, m_btSeeUserIP);
}

//初始化函数
BOOL CDlgManageUser::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("用户管理："));
	
	//按钮禁用
	BOOL bEnable = CMasterRight::CanKillUser(m_dwMasterRight)?TRUE:FALSE;
	m_btKickAllUser.EnableWindow(bEnable);
	bEnable = CMasterRight::CanDismissGame(m_dwMasterRight)?TRUE:FALSE;
	m_btDismissGame.EnableWindow(bEnable);
	bEnable = CMasterRight::CanSeeUserIP(m_dwMasterRight)?TRUE:FALSE;
	m_btSeeUserIP.EnableWindow(bEnable);
	m_btSeeUserIP.ShowWindow(SW_HIDE);

	return TRUE;
}

//确定函数
VOID CDlgManageUser::OnOK()
{
	return;
}

//取消消息
VOID CDlgManageUser::OnCancel()
{
	//销毁窗口
	DestroyWindow();

	return;
}

//菜单命令
BOOL CDlgManageUser::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_KICK_ALL_USER:			//踢走玩家
		{
			//权限效验
			ASSERT( CMasterRight::CanKillUser(m_dwMasterRight) );
			if( CMasterRight::CanKillUser(m_dwMasterRight) == false ) return TRUE;

			CDlgKickUser DlgKickUser;
			if(IDOK == DlgKickUser.DoModal())
			{
				CMD_GR_KickAllUser KickAllUser = {};		
				_sntprintf(KickAllUser.szKickMessage,CountArray(KickAllUser.szKickMessage), TEXT("%s"), DlgKickUser.m_strKickMessage);

				ASSERT(m_pITCPSocket!=NULL);
				WORD wSendSize = CountStringBuffer(KickAllUser.szKickMessage);
				m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_KICK_ALL_USER,&KickAllUser,wSendSize);
			}

			return TRUE;
		}
	case IDC_DISMISS_GAME:			//解散游戏
		{
			//权限效验
			ASSERT( CMasterRight::CanDismissGame(m_dwMasterRight) );
			if( CMasterRight::CanDismissGame(m_dwMasterRight) == false ) return TRUE;

			CDlgDismissGame DlgDismissGame;
			if(IDOK == DlgDismissGame.DoModal())
			{
				CMD_GR_DismissGame DismissGame;
				DismissGame.wDismissTableNum=DlgDismissGame.GetDismissTableNum();

				ASSERT(m_pITCPSocket!=NULL);
				m_pITCPSocket->SendData(MDM_GR_MANAGE,SUB_GR_DISMISSGAME,&DismissGame,sizeof(DismissGame));
			}

			return TRUE;
		}
	case IDC_SEE_USER_IP:			//查看玩家IP
		{
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//设置接口
bool CDlgManageUser::SetTCPSocket(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocket)!=NULL);
		m_pITCPSocket=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocket);

		//成功判断
		if (m_pITCPSocket==NULL) return false;
	}
	else m_pITCPSocket=NULL;

	return true;
}

//设置权限
VOID CDlgManageUser::SetMasterRight( DWORD dwMasterRight )
{
	m_dwMasterRight = dwMasterRight;

	if( m_hWnd )
	{
		//按钮禁用
		BOOL bEnable = CMasterRight::CanKillUser(m_dwMasterRight)?TRUE:FALSE;
		m_btKickAllUser.EnableWindow( bEnable );

		bEnable = CMasterRight::CanDismissGame(m_dwMasterRight)?TRUE:FALSE;
		m_btDismissGame.EnableWindow( bEnable );
	}
}

//////////////////////////////////////////////////////////////////////////
