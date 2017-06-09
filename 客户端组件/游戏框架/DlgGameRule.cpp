#include "Stdafx.h"
#include "Resource.h"
#include "DlgGameRule.h"
#include "GlobalUnits.h"
#include "GlobalUnits.h"
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgGameRule, CSkinDialog)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgGameRule::CDlgGameRule() : CSkinDialog(IDD_GAME_RULE)
{
	//设置变量
	m_pHtmlBrowser=NULL;

	return;
}

//析构函数
CDlgGameRule::~CDlgGameRule()
{
	//删除控件
	SafeDelete(m_pHtmlBrowser);
	
	return;
}

//控件绑定
VOID CDlgGameRule::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

//初始化函数
BOOL CDlgGameRule::OnInitDialog()
{
	__super::OnInitDialog();

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//变量定义
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//变量定义
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	tagGameAttribute * pGameAttribute=pGlobalUnits->m_ClientKernelModule->GetGameAttribute();

	//创建控件
	try
	{
		//位置定义
		CRect rcBrowse;
		rcBrowse.left=nLViewBorder;
		rcBrowse.top=nTViewBorder;
		rcBrowse.right=rcClient.Width()-nLViewBorder;
		rcBrowse.bottom=rcClient.Height()-nBViewBorder;

		//创建控件
		m_pHtmlBrowser=new CWebBrowser;
		m_pHtmlBrowser->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcBrowse,this,101);

		//构造地址
		TCHAR szRuleUrl[256]=TEXT("");
		_sntprintf(szRuleUrl,CountArray(szRuleUrl),TEXT("%s/GameRules.aspx?KindID=%ld"),szPlatformLink,pGameAttribute->wKindID);

		//浏览规则
		m_pHtmlBrowser->Navigate(szRuleUrl,NULL,NULL,NULL,NULL);
	}
	catch (...) { ASSERT(FALSE); }

	//构造标题
	TCHAR szTitle[128]=TEXT("");
	_sntprintf(szTitle,CountArray(szTitle),TEXT("“%s”规则说明"),pGameAttribute->szGameName);

	//设置窗口
	SetWindowText(szTitle);
//	EnableButton(WS_MAXIMIZEBOX|WS_MINIMIZEBOX);

	return TRUE;
}

//确定消息
VOID CDlgGameRule::OnOK() 
{
	return;
}

//取消消息
VOID CDlgGameRule::OnCancel()
{
	//销毁窗口
	DestroyWindow(); 

	return;
}

//销毁消息
VOID CDlgGameRule::OnNcDestroy()
{
	//销毁控件
	if(m_pHtmlBrowser && m_pHtmlBrowser->GetSafeHwnd()) m_pHtmlBrowser->DestroyWindow();

	//删除控件
	SafeDelete(m_pHtmlBrowser);

	__super::OnNcDestroy();
}

//位置消息
VOID CDlgGameRule::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	if (m_pHtmlBrowser->GetSafeHwnd()!=NULL)
	{
		m_pHtmlBrowser->MoveWindow(m_SkinAttribute.m_EncircleInfoView.nLBorder,m_SkinAttribute.m_EncircleInfoView.nTBorder,
			cx-m_SkinAttribute.m_EncircleInfoView.nLBorder*2,cy-m_SkinAttribute.m_EncircleInfoView.nTBorder-m_SkinAttribute.m_EncircleInfoView.nRBorder);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
