#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "DlgGameOption.h"

//////////////////////////////////////////////////////////////////////////

//位置定义
#define SPACE_TOP					10									//间距大小
#define SPACE_LEFT					10									//间距大小
#define SPACE_RIGHT					10									//间距大小
#define SPACE_BOTTOM				40									//间距大小

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgGameOption, CSkinDialog)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_DEFAULT, OnBnClickedDefault)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ITEM_TAB, OnTcnSelchangeTabControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGeneralOptionItem::CGeneralOptionItem()
{
	//默认参数
	DefaultParameter();

	return;
}

//创建控件
bool __cdecl CGeneralOptionItem::CreateOptionItem(CWnd * pParentWnd)
{
	//创建窗口
	if (m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
		Create(IDD_GENERAL_OPTION,pParentWnd);
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}

	//更新控制
	UpdateItemControl();

	return true;
}

//保存配置
bool __cdecl CGeneralOptionItem::SaveParameter()
{
	//获取参数
	if (m_hWnd!=NULL)
	{
		//旁观选项
		m_bAllowLookon=(IsDlgButtonChecked(IDC_ALLOW_LOOKON)==BST_CHECKED)?true:false;
		m_bAllowFriendLookon=(IsDlgButtonChecked(IDC_ALLOW_FRIEND_LOOKON)==BST_CHECKED)?true:false;

		//其他选项
		m_bAllowSound=(IsDlgButtonChecked(IDC_ALLOW_SOUND)==BST_CHECKED)?true:false;
		m_bAllowBackGroundSound=(IsDlgButtonChecked(IDC_ALLOW_BACK_SOUND)==BST_CHECKED)?true:false;
		m_bFullGameView=(IsDlgButtonChecked(IDC_FULL_GAME_VIEW)==BST_CHECKED)?true:false;
		m_bDebarDetestChat=(IsDlgButtonChecked(IDC_DEBAR_DETEST_CHAT)==BST_CHECKED)?true:false;
		m_bNotifyUserInOut=(IsDlgButtonChecked(IDC_NOTIFY_USER_INOUT)==BST_CHECKED)?true:false;
	}

	return true;
}

//默认参数
bool __cdecl CGeneralOptionItem::DefaultParameter()
{
	//旁观选项
	m_bAllowLookon=false;
	m_bAllowFriendLookon=false;
	
	//其他选项
	m_bAllowSound=true;
	m_bAllowBackGroundSound=true;
	m_bFullGameView=true;
	m_bDebarDetestChat=true;
	m_bNotifyUserInOut=true;

	//更新控制
	UpdateItemControl();

	return true;
}

//更新控制
VOID CGeneralOptionItem::UpdateItemControl()
{
	//设置控件
	if (m_hWnd!=NULL)
	{
		//旁观选项
		CheckDlgButton(IDC_ALLOW_LOOKON,(m_bAllowLookon==true)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(IDC_ALLOW_FRIEND_LOOKON,(m_bAllowFriendLookon==true)?BST_CHECKED:BST_UNCHECKED);

		//其他选项
		CheckDlgButton(IDC_ALLOW_SOUND,(m_bAllowSound==true)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(IDC_ALLOW_BACK_SOUND,(m_bAllowBackGroundSound==true)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(IDC_FULL_GAME_VIEW,(m_bFullGameView==true)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(IDC_DEBAR_DETEST_CHAT,(m_bDebarDetestChat==true)?BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(IDC_NOTIFY_USER_INOUT,(m_bNotifyUserInOut==true)?BST_CHECKED:BST_UNCHECKED);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgGameOption::CDlgGameOption() : CSkinDialog(IDD_GAME_OPTION)
{
	//设置变量
	m_pIGameOptionItem=NULL;

	return;
}

//析构函数
CDlgGameOption::~CDlgGameOption()
{
	return;
}

//控件绑定
VOID CDlgGameOption::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_DEFAULT, m_btDefault);
	DDX_Control(pDX, IDC_ITEM_TAB, m_SkinTabControl);
}

//初始化函数
BOOL CDlgGameOption::OnInitDialog()
{
	__super::OnInitDialog();

	//变量定义
	CSize ItemSize(0,0);
	m_SkinTabControl.SetItemSize(CSize(150,25));

	//子项处理
	for (INT_PTR i=0;i<m_GameOptionItemArray.GetCount();i++)
	{
		//获取子项
		IGameOptionItem * pIGameOptionItem=m_GameOptionItemArray[i];

		//插入子项
		pIGameOptionItem->CreateOptionItem(&m_SkinTabControl);
		m_SkinTabControl.InsertItem((INT)i,pIGameOptionItem->GetItemTitle());

		//计算位置
		CSize CurrentSize;
		pIGameOptionItem->GetControlMinSize(CurrentSize);
		ItemSize.SetSize(__max(CurrentSize.cx,ItemSize.cx),__max(CurrentSize.cy,ItemSize.cy));
	}

	//调整位置
	CRect rcControl(0,0,ItemSize.cx,ItemSize.cy);
	m_SkinTabControl.AdjustRect(TRUE,&rcControl);

	//显示首项
	if (m_GameOptionItemArray.GetCount()>0)
	{
		m_pIGameOptionItem=m_GameOptionItemArray[0];
		::ShowWindow(m_pIGameOptionItem->GetWindowHwnd(),SW_SHOW);
	}

	//变量定义
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	
	//调整控件
	INT nWindowWidth=rcControl.Width()+SPACE_RIGHT+SPACE_LEFT+nLViewBorder*2;
	INT nWindowHeight=nTViewBorder+rcControl.Height()+SPACE_TOP+SPACE_BOTTOM+nBViewBorder;
	SetWindowPos(NULL,0,0,nWindowWidth,nWindowHeight,SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);

	//居中窗口
	CenterWindow(AfxGetMainWnd());

	//设置标题
	SetWindowText(TEXT("游戏设置："));

	return FALSE;
}

//确定消息
VOID CDlgGameOption::OnOK() 
{
	//保存数据
	for (INT_PTR i=0;i<m_GameOptionItemArray.GetCount();i++)
	{
		//获取子项
		IGameOptionItem * pIGameOptionItem=m_GameOptionItemArray[i];

		//插入子项
		if (pIGameOptionItem->SaveParameter()==false)
		{
			//显示子项
			if (m_pIGameOptionItem!=pIGameOptionItem)
			{
				//显示子项
				::ShowWindow(pIGameOptionItem->GetWindowHwnd(),SW_SHOW);
				if (m_pIGameOptionItem!=NULL) ::ShowWindow(m_pIGameOptionItem->GetWindowHwnd(),SW_HIDE);

				//设置变量
				m_pIGameOptionItem=pIGameOptionItem;
			}

			return;
		}
	}

	__super::OnOK();
}

//游戏配置
bool CDlgGameOption::CreateGameOption(IUnknownEx * pIUnknownEx[], WORD wItemCount)
{
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	//设置参数
	m_GeneralOptionItem.m_bAllowLookon=pGlobalUnits->m_bAllowLookon;
	m_GeneralOptionItem.m_bAllowFriendLookon=pGlobalUnits->m_bAllowFriendLookon;

	//其他选项
	m_GeneralOptionItem.m_bAllowSound=pGlobalUnits->m_bAllowSound;
	m_GeneralOptionItem.m_bAllowBackGroundSound=pGlobalUnits->m_bAllowBackGroundSound;
	m_GeneralOptionItem.m_bFullGameView=pGlobalUnits->m_bFullGameView;
	m_GeneralOptionItem.m_bDebarDetestChat=pGlobalUnits->m_bDebarDetestChat;
	m_GeneralOptionItem.m_bNotifyUserInOut=pGlobalUnits->m_bNotifyUserInOut;

	//常规子项
	m_GameOptionItemArray.Add(QUERY_OBJECT_INTERFACE(m_GeneralOptionItem,IGameOptionItem));

	//游戏子项
	for (WORD i=0;i<wItemCount;i++)
	{
		//查询接口
		ASSERT(pIUnknownEx[i]!=NULL);
		IGameOptionItem * pIGameOptionItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx[i],IGameOptionItem);

		//插入子项
		ASSERT(pIGameOptionItem!=NULL);
		if (pIGameOptionItem!=NULL) m_GameOptionItemArray.Add(pIGameOptionItem);
	}

	//显示窗口
	if (DoModal()==IDOK)
	{
		//旁观选项
		pGlobalUnits->m_bAllowLookon=m_GeneralOptionItem.m_bAllowLookon;
		pGlobalUnits->m_bAllowFriendLookon=m_GeneralOptionItem.m_bAllowFriendLookon;

		//其他选项
		pGlobalUnits->m_bAllowSound=m_GeneralOptionItem.m_bAllowSound;
		pGlobalUnits->m_bAllowBackGroundSound=m_GeneralOptionItem.m_bAllowBackGroundSound;
		pGlobalUnits->m_bFullGameView=m_GeneralOptionItem.m_bFullGameView;
		pGlobalUnits->m_bDebarDetestChat=m_GeneralOptionItem.m_bDebarDetestChat;
		pGlobalUnits->m_bNotifyUserInOut=m_GeneralOptionItem.m_bNotifyUserInOut;

		//查询接口
		ASSERT(g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd));
		IGameFrameWnd * pIGameFrameWnd=(IGameFrameWnd *)g_GlobalUnits.QueryGlobalModule(MODULE_GAME_FRAME_WND,IID_IGameFrameWnd,VER_IGameFrameWnd);

		//应用选项
		if (pIGameFrameWnd!=NULL) 
		{
			pIGameFrameWnd->OnGameOptionChange();
		}

		return true;
	}

	return false;
}

//默认按钮
VOID CDlgGameOption::OnBnClickedDefault()
{
	//默认参数
	for (INT_PTR i=0;i<m_GameOptionItemArray.GetCount();i++)
	{
		IGameOptionItem * pIGameOptionItem=m_GameOptionItemArray[i];
		if (pIGameOptionItem!=NULL) pIGameOptionItem->DefaultParameter();
	}

	return;
}

//销毁消息
VOID CDlgGameOption::OnNcDestroy()
{
	//删除对象
	m_pIGameOptionItem=NULL;
	m_GameOptionItemArray.RemoveAll();

	__super::OnNcDestroy();
}

//位置消息
VOID CDlgGameOption::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//变量定义
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;

	//移动子项
	if (m_SkinTabControl.m_hWnd!=NULL)
	{
		//移动选择
		m_SkinTabControl.MoveWindow(SPACE_LEFT+nLViewBorder,nTViewBorder+SPACE_TOP,
			cx-SPACE_LEFT-SPACE_RIGHT-nLViewBorder*2,cy-nTViewBorder-SPACE_TOP-SPACE_BOTTOM-nRViewBorder);

		//计算位置
		CRect rcItem;
		m_SkinTabControl.GetClientRect(&rcItem);
		m_SkinTabControl.AdjustRect(FALSE,&rcItem);

		//调整子项
		for (INT_PTR i=0;i<m_GameOptionItemArray.GetCount();i++)
		{
			IGameOptionItem * pIGameOptionItem=m_GameOptionItemArray[i];
			::MoveWindow(pIGameOptionItem->GetWindowHwnd(),1,27,rcItem.Width()-2,rcItem.Height()-28,FALSE);
		}
	}

	//移动按钮
	if (m_btOk.m_hWnd!=NULL)
	{
		//获取大小
		CRect rcButton;
		m_btOk.GetWindowRect(&rcButton);

		//移动按钮
		INT nButtonSpace=(cx-rcButton.Width()*3-nLViewBorder*2)/6;
		INT nYButtonPos=cy-nRViewBorder-SPACE_BOTTOM/2-rcButton.Height()/2;
		m_btOk.SetWindowPos(NULL,nLViewBorder+nButtonSpace*2,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_btCancel.SetWindowPos(NULL,rcButton.Width()+nLViewBorder+nButtonSpace*3,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_btDefault.SetWindowPos(NULL,rcButton.Width()*2+nLViewBorder+nButtonSpace*4,nYButtonPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}

	return;
}

//子项改变
VOID CDlgGameOption::OnTcnSelchangeTabControl(NMHDR * pNMHDR, LRESULT * pResult)
{
	//获取选项
	INT nCurrentSel=m_SkinTabControl.GetCurSel();
	IGameOptionItem * pISelectItem=m_GameOptionItemArray[nCurrentSel];

	//改变处理
	if (pISelectItem!=m_pIGameOptionItem)
	{
		//显示子项
		::ShowWindow(pISelectItem->GetWindowHwnd(),SW_SHOW);
		if (m_pIGameOptionItem!=NULL) ::ShowWindow(m_pIGameOptionItem->GetWindowHwnd(),SW_HIDE);

		//设置变量
		m_pIGameOptionItem=pISelectItem;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
