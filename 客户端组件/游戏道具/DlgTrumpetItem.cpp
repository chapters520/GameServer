#include "Stdafx.h"
#include "resource.h"
#include "DlgTrumpetItem.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgTrumpetItem, CSkinDialog)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_COLOR_SET, OnBnClickedColorSet)
	ON_BN_CLICKED(IDC_RADIO_TRUMPET, OnBnClickedTrumpet)
	ON_BN_CLICKED(IDC_RADIO_TYPHON, OnBnClickedTyphon)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

//构造函数
CDlgTrumpetItem::CDlgTrumpetItem() : CSkinDialog(IDD_DLG_TRUMPET)
{
	//设置变量
	m_crChatTX = RGB(0,0,0);
	m_wRadioSelected=0;
   
	//接口变量
	m_pGameTrumpetSink=NULL;
	m_pIClientUserItem=NULL;

	//控件变量
	m_pExpressionControl=NULL;
}

//析构函数
CDlgTrumpetItem::~CDlgTrumpetItem()
{
	//销毁窗口
	if(m_pExpressionControl && IsWindow(m_pExpressionControl->m_hWnd)) m_pExpressionControl->DestroyWindow();

	//删除对象
	SafeDelete(m_pExpressionControl);
}

//控件绑定
void CDlgTrumpetItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);
	DDX_Control(pDX, IDC_InputChat, m_InputChat);
	DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);

	DDX_Text(pDX, IDC_InputChat, m_strInuptChat);
	DDV_MaxChars(pDX, m_strInuptChat, TRUMPET_MAX_CHAR);
	DDX_Text(pDX, IDC_PROPERTY_INFO, m_strPropertyInfo);

	return;
}

//更新数据
void  CDlgTrumpetItem::UpdateControlSurface()
{
	//变量判断
	if(m_pIClientUserItem==NULL) return;

	//道具管理
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//格式字符
	CString strTitle ;
	if(((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->GetCheck()==BST_CHECKED)
	{
		//获取道具
		CGamePropertyItem * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET); 
		if(pPropertyItem==NULL) return;
		tagPropertyInfo * pPropertyInfo = pPropertyItem->GetPropertyInfo();

		//计算价格
		DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
		if(m_pIClientUserItem->GetMemberOrder()>0) lPrice=lPrice*pPropertyInfo->wDiscount/100;

		//购买数目
		TCHAR szBuyCountInfo[32]=TEXT("");
		DWORD dwCanBuyCount= __max((DWORD)(m_pIClientUserItem->GetUserInsure()/lPrice),0);	

		//设置文字
		strTitle.Format(TEXT("您能购买的小喇叭数目为[ %d ] 条"), dwCanBuyCount);       
	}
	else
	{
		//获取道具
		CGamePropertyItem * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON); 
		if(pPropertyItem==NULL) return;
		tagPropertyInfo * pPropertyInfo = pPropertyItem->GetPropertyInfo();

		//计算价格
		DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
		if(m_pIClientUserItem->GetMemberOrder()>0) lPrice=lPrice*pPropertyInfo->wDiscount/100;

		//购买数目
		TCHAR szBuyCountInfo[32]=TEXT("");
		DWORD dwCanBuyCount= __max((DWORD)(m_pIClientUserItem->GetUserInsure()/lPrice),0);	

		//设置文字
		strTitle.Format(TEXT("您能购买的大喇叭数目为[ %d ] 条"), dwCanBuyCount);		
	}

	//设置标题
	SetWindowText(strTitle);
}

//获取内容
void CDlgTrumpetItem::GetTrumpetContent(LPTSTR pszTrumpetContent)
{
	//参数判断
	if(pszTrumpetContent==NULL) return;

	//拷贝数据
	CopyMemory(pszTrumpetContent,m_strInuptChat,sizeof(TCHAR)*TRUMPET_MAX_CHAR);
}

//初始框架
BOOL CDlgTrumpetItem::OnInitDialog()
{
	CSkinDialog::OnInitDialog();	

	//设置资源
	m_btExpression.SetButtonImage(IDB_BT_EXPRESSION,GetModuleHandle(GAME_PROPERTY_DLL_NAME),true,false);
	m_btColorSet.SetButtonImage(IDB_BT_COLOR_SET,GetModuleHandle(GAME_PROPERTY_DLL_NAME),true,false);

	//输入限制
	m_InputChat.LimitText(TRUMPET_MAX_CHAR);
	
	//默认喇叭
	//((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->SetCheck(TRUE);
	//OnBnClickedTrumpet();	

	//更新界面
	//UpdateControlSurface();

	return TRUE;
}


//颜色按钮
void CDlgTrumpetItem::OnBnClickedColorSet()
{
	//设置颜色
	CColorDialog ColorDialog(m_crChatTX,CC_FULLOPEN);
	
	if (ColorDialog.DoModal()==IDOK)
	{
		m_crChatTX=ColorDialog.GetColor();

		//设置界面
		m_InputChat.SetFocus();
	}

	return;
}

//表情按钮
void CDlgTrumpetItem::OnBnClickedExpression()
{
	//创建控件
	if (m_pExpressionControl==NULL)
	{
		m_pExpressionControl = new CExpressionControl;
	}

	//移动窗口
	CRect rcButton;
	m_btExpression.GetWindowRect(&rcButton);

	//显示窗口
	m_pExpressionControl->ShowExpression(this,rcButton.left,rcButton.top,this);
	m_pExpressionControl->SetFocus();

	return;
}

//喇叭按钮
void CDlgTrumpetItem::OnBnClickedTrumpet()
{
	//道具管理
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//获取道具
	CGamePropertyItem * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET); 
	if(pPropertyItem==NULL) return;
	tagPropertyInfo * pPropertyInfo = pPropertyItem->GetPropertyInfo();

	m_strPropertyInfo.Format(TEXT("小喇叭%I64d游戏币一条。每条消息不得超过%d个字符，消息将在房间内的聊天框内以醒目的方式显示。"),	pPropertyInfo->lPropertyGold,TRUMPET_MAX_CHAR);	

	//更新数据
	UpdateData(FALSE);

	//更新控件
	UpdateControlSurface();
}

//喇叭按钮
void CDlgTrumpetItem::OnBnClickedTyphon()
{
	//道具管理
	CGamePropertyManager * pGamePropertyManager = CGamePropertyManager::GetInstance();

	//获取道具
	CGamePropertyItem * pPropertyItem = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON); 
	if(pPropertyItem==NULL) return;
	tagPropertyInfo * pPropertyInfo = pPropertyItem->GetPropertyInfo();

	m_strPropertyInfo.Format(TEXT("大喇叭%I64d游戏币一条。每条消息不得超过%d个字符，消息将在房间内的聊天框内以醒目的方式显示。"),	pPropertyInfo->lPropertyGold,TRUMPET_MAX_CHAR);

	//更新数据
	UpdateData(FALSE);

	//更新控件
	UpdateControlSurface();
}

//选择事件
VOID CDlgTrumpetItem::OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)
{
	if (pExpressionInfo!=NULL)
	{
		CString strChatMsg;
		m_InputChat.GetWindowText(strChatMsg);
		strChatMsg+="/";
		strChatMsg+=pExpressionInfo->szExpression[1];
		m_InputChat.SetWindowText(strChatMsg);
		m_InputChat.SetFocus();
		m_InputChat.SetSel(strChatMsg.GetLength(),strChatMsg.GetLength());
	}
}

//设置接口
bool CDlgTrumpetItem::SetGameTrumpetSink(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertyUseSink)!=NULL);
		m_pGameTrumpetSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGamePropertyUseSink);

		//成功判断
		if (m_pGameTrumpetSink==NULL) return false;
	}
	else m_pGameTrumpetSink=NULL;

	return true;
}

//显示窗口
void CDlgTrumpetItem::ShowTrumpetWindow(CWnd * pParentWnd,IClientUserItem  * pIClientUserItem)
{
	//效验状态
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//状态判断
	if(m_hWnd!=NULL && IsWindowVisible()==TRUE) return;

	//检查窗口
	if(m_hWnd==NULL) 
	{
		//设置接口
		m_pIClientUserItem = pIClientUserItem;

		//创建窗口
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		Create(IDD_DLG_TRUMPET,pParentWnd);
		AfxSetResourceHandle(GetModuleHandle(NULL));		
	}

	//获取位置
	CRect rcParentRect(0,0,0,0);
	pParentWnd->GetWindowRect(&rcParentRect);
	
	//获取区域
	CRect rcWindowRect(0,0,0,0);
	GetWindowRect(&rcWindowRect);

	//设置位置
	INT nXPos = (rcParentRect.Width()-rcWindowRect.Width())/2;
	INT nYPos = (rcParentRect.Height()-rcWindowRect.Height())/2;
	SetWindowPos(&CWnd::wndTopMost,nXPos,nYPos,rcWindowRect.Width(),rcWindowRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER|SWP_SHOWWINDOW);

	//变量定义
	CGamePropertyManager * pGamePropertyManager=CGamePropertyManager::GetInstance();
	CGamePropertyItem * pGamePropertyTrumpet = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TRUMPET);
	CGamePropertyItem * pGamePropertyTyphon  = pGamePropertyManager->GetPropertyItem(PROPERTY_ID_TYPHON);

	//默认喇叭
	if(pGamePropertyTrumpet!=NULL && pGamePropertyTyphon==NULL)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->EnableWindow();
		((CButton*)GetDlgItem(IDC_RADIO_TYPHON))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_TYPHON))->EnableWindow(FALSE);
		OnBnClickedTrumpet();
	}

	//默认喇叭
	if(pGamePropertyTrumpet==NULL && pGamePropertyTyphon!=NULL)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_TYPHON))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_TYPHON))->EnableWindow();
		OnBnClickedTyphon();
	}

	//默认喇叭
	if(pGamePropertyTrumpet!=NULL && pGamePropertyTyphon!=NULL)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->EnableWindow();
		((CButton*)GetDlgItem(IDC_RADIO_TYPHON))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_TYPHON))->EnableWindow();
		OnBnClickedTrumpet();
	}

	//设置变量
	m_strInuptChat=TEXT("");
	UpdateData(FALSE);

	//更新界面
	UpdateControlSurface();
}

//确认消息
void CDlgTrumpetItem::OnBnClickedOk()
{
	//获取道具
	WORD wPropertyIndex = ((CButton*)GetDlgItem(IDC_RADIO_TRUMPET))->GetCheck()==BST_CHECKED?PROPERTY_ID_TRUMPET:PROPERTY_ID_TYPHON;

	//获取信息
	m_InputChat.GetWindowText(m_strInuptChat);
	if ( m_strInuptChat.IsEmpty() )
	{
		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("输入内容不能为空！"),MB_ICONWARNING);
		return ;
	}

	if ( TRUMPET_MAX_CHAR < m_strInuptChat.GetLength() )
	{ 
		CString strMsg;
		strMsg.Format(TEXT("输入内容最多为%d个字符！"), TRUMPET_MAX_CHAR);

		CInformation Information2(this);
		Information2.ShowMessageBox(TEXT("输入内容最多为%d个字符！"),TRUMPET_MAX_CHAR,MB_ICONWARNING);
		return ;
	}	

	//回调接口
	if(m_pGameTrumpetSink!=NULL)  m_pGameTrumpetSink->OnEventUseProperty(wPropertyIndex);

	OnOK();
}

//控件颜色
HBRUSH CDlgTrumpetItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor == CTLCOLOR_EDIT )
	{
		pDC->SetTextColor(m_crChatTX);
	}
	return hbr;
}


//绘画函数
void CDlgTrumpetItem::OnPaint()
{
	CPaintDC dc(this); 

	//绘画边框
	CRect rcWindowRect(0,0,0,0);
	m_InputChat.GetWindowRect(&rcWindowRect);

	//区域膨胀
	ScreenToClient(&rcWindowRect);
	rcWindowRect.InflateRect(1,1,1,1);

	//绘制边框
	dc.MoveTo(rcWindowRect.left,rcWindowRect.top);
	dc.LineTo(rcWindowRect.right,rcWindowRect.top);
	dc.MoveTo(rcWindowRect.right,rcWindowRect.top);
	dc.LineTo(rcWindowRect.right,rcWindowRect.bottom);
	dc.MoveTo(rcWindowRect.right,rcWindowRect.bottom);
	dc.LineTo(rcWindowRect.left,rcWindowRect.bottom);
	dc.MoveTo(rcWindowRect.left,rcWindowRect.bottom);
	dc.LineTo(rcWindowRect.left,rcWindowRect.top);

	return;
}

//////////////////////////////////////////////////////////////////////

