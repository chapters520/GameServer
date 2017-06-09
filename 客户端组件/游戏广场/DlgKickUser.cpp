#include "StdAfx.h"
#include "Resource.h"
#include "DlgKickUser.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgKickUser, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgKickUser::CDlgKickUser() : CSkinDialog(IDD_DLG_KICK_USER)
{
}

//析构函数
CDlgKickUser::~CDlgKickUser()
{
}

//控件绑定
VOID CDlgKickUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control( pDX,IDOK,m_btOK );
	DDX_Control( pDX,IDCANCEL,m_btCancel );
	DDX_Text(pDX,IDC_EDT_MESSAGE,m_strKickMessage);
}

//初始化函数
BOOL CDlgKickUser::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("提示"));
	
	//默认提示
	m_strKickMessage = TEXT("抱歉，您被管理员请出房间，若有任何疑问，请联系游戏客服！");

	UpdateData(FALSE);

	return TRUE;
}

//确定函数
VOID CDlgKickUser::OnOK()
{
	UpdateData();
	
	return __super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
