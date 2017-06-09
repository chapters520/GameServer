#include "StdAfx.h"
#include "Resource.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CDlgBuyProperty * CDlgBuyProperty::m_pDlgBuyProperty=NULL;				//对象指针

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgBuyProperty, CSkinDialog)
	ON_EN_CHANGE(IDC_PROPERTY_COUNT, OnEnChangePropertyCount)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgBuyProperty::CDlgBuyProperty() : CSkinDialog(IDD_DLG_PROPERTY)
{
	//设置变量
	m_lCanBuyCount=0;

	//设置变量
	m_pGamePropertyItem=NULL;
	m_pIGamePropertySink=NULL;

	//设置对象
	ASSERT(m_pDlgBuyProperty==NULL);
	if (m_pDlgBuyProperty==NULL) m_pDlgBuyProperty=this;

	return;
}

//析构函数
CDlgBuyProperty::~CDlgBuyProperty()
{
	//设置对象
	ASSERT(m_pDlgBuyProperty==this);
	if (m_pDlgBuyProperty==this) m_pDlgBuyProperty=NULL;

	return;
}

//控件绑定
VOID CDlgBuyProperty::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);

	//编辑控件
	DDX_Control(pDX, IDC_NICK_NAME, m_edNickName);
	DDX_Control(pDX, IDC_PROPERTY_COUNT, m_edPropertyCount);
}

//消息解释
BOOL CDlgBuyProperty::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//创建函数
BOOL CDlgBuyProperty::OnInitDialog()
{
	__super::OnInitDialog();

	//更新状态
	UpdateCtrlStatus();

	//输入限制
	m_edNickName.LimitText(LEN_NICKNAME-1);

	//支付方式
	((CButton *)GetDlgItem(IDC_PAY_BY_INSURE))->SetCheck(BST_CHECKED);

	return FALSE;
}

//确定函数
VOID CDlgBuyProperty::OnOK()
{
	//获取信息
	CString strNickName;
	GetDlgItemText(IDC_NICK_NAME,strNickName);
	strNickName.TrimLeft();strNickName.TrimRight();

	//获取数目
	UINT wItemCount=GetDlgItemInt(IDC_PROPERTY_COUNT);

	//昵称判断
	if (strNickName.IsEmpty()==true)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("用户昵称不正确，请重新输入"),MB_ICONERROR);

		//设置焦点
		m_edNickName.SetFocus();

		return;
	}

	//数目判断
	if (wItemCount==0 || wItemCount > m_lCanBuyCount)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("购买数目不正确，请重新输入"),MB_ICONERROR);

		//设置焦点
		m_edPropertyCount.SetFocus();

		return;
	}

	//获取信息
	ASSERT(m_pGamePropertyItem->GetPropertyAttrib()!=NULL);
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//购前检查
	ASSERT(m_pIGamePropertySink!=NULL);
	TCHAR szMessage[128]=TEXT("");
	if(m_pIGamePropertySink->OnEventBuyPropertyPrep(strNickName,pPropertyAttrib->wIndex,szMessage)==false)
	{
		//提示信息
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR);

		return;
	}

	//事件通知	
	if (m_pIGamePropertySink->OnEventBuyProperty(strNickName,wItemCount,pPropertyAttrib->wIndex)==true)
	{
		//关闭窗口
		DestroyWindow();
	}

	return;
}

//取消消息
VOID CDlgBuyProperty::OnCancel()
{
	//关闭窗口
	DestroyWindow();

	return;
}

//绘画消息
VOID CDlgBuyProperty::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//框架位置
	INT nTBorder=m_SkinAttribute.m_EncircleInfoFrame.nTBorder;
	INT nBBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;
	INT nLBorder=m_SkinAttribute.m_EncircleInfoFrame.nLBorder;
	INT nRBorder=m_SkinAttribute.m_EncircleInfoFrame.nRBorder;

	//加载资源
	CPngImage ImagePropertyFrame;
	ImagePropertyFrame.LoadImage(GetModuleHandle(GAME_PROPERTY_DLL_NAME),TEXT("PROPERTY_FRAME"));

	//获取大小
	CSize SizePropertyFrame;
	SizePropertyFrame.SetSize(ImagePropertyFrame.GetWidth(),ImagePropertyFrame.GetHeight());

	//绘画框架
	INT nXItemPos=nLBorder+10;
	INT nYItemPos=nTBorder+10;
	ImagePropertyFrame.DrawImage(pDC,nXItemPos-(SizePropertyFrame.cx-PROPERTY_SYMBOL_CX)/2,nYItemPos-(SizePropertyFrame.cy-PROPERTY_SYMBOL_CY)/2);

	//绘画图标
	if (m_pGamePropertyItem!=NULL)
	{
		//获取资源
		tagPropertyImage PropertyImage;
		m_pGamePropertyItem->GetImageInfo(PropertyImage);

		//绘画图标
		CPngImage ImageSymbol;
		ImageSymbol.LoadImage(PropertyImage.hResInstance,PropertyImage.pszSymbolID);
		ImageSymbol.DrawImage(pDC,nXItemPos,nYItemPos,PROPERTY_SYMBOL_CX,PROPERTY_SYMBOL_CY,0,0,ImageSymbol.GetWidth(),ImageSymbol.GetHeight());
	}

	return;
}

//更新状态
VOID CDlgBuyProperty::UpdateCtrlStatus()
{
	//效验参数
	ASSERT(m_pGamePropertyItem!=NULL);
	ASSERT(m_pIGamePropertySink!=NULL);

	//输入限制
	m_edPropertyCount.SetLimitText(12);

	//获取属性
	tagPropertyInfo * pPropertyInfo=m_pGamePropertyItem->GetPropertyInfo();
	tagPropertyAttrib * pPropertyAttrib=m_pGamePropertyItem->GetPropertyAttrib();

	//设置标签
	if(pPropertyAttrib->wPropertyType==PT_TYPE_PROPERTY)
	{		
		SetDlgItemText(IDC_STATIC_NAME,TEXT("道具名称"));
		SetDlgItemText(IDC_STATIC_PRICE,TEXT("道具价格"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_NAME,TEXT("礼物名称"));
		SetDlgItemText(IDC_STATIC_PRICE,TEXT("礼物价格"));
	}

	//道具价格
	TCHAR szPropertyGold[64]=TEXT("");
	_sntprintf(szPropertyGold,CountArray(szPropertyGold),TEXT("%I64d 游戏币/%s （会员折扣：%ld%%）"),pPropertyInfo->lPropertyGold,pPropertyAttrib->szMeasuringunit,pPropertyInfo->wDiscount);

	//设置标题
	SetWindowText(pPropertyAttrib->szPropertyName);

	//道具价格
	SetDlgItemText(IDC_PROPERTY_GOLD,szPropertyGold);

	//道具单位
	SetDlgItemText(IDC_STATIC_COUNT,pPropertyAttrib->szMeasuringunit);

	//道具属性
	SetDlgItemText(IDC_PROPERTY_NAME,pPropertyAttrib->szPropertyName);
	SetDlgItemText(IDC_PROPERTY_DESCRIBE,pPropertyAttrib->szRegulationsInfo);

	//购买数目
	if(m_pIGamePropertySink!=NULL)	
	{
		IClientUserItem * pClientUserItem = m_pIGamePropertySink->OnQueryClientUserItem();
		if(pClientUserItem!=NULL)
		{
			//计算价格
			DOUBLE lPrice=(DOUBLE)pPropertyInfo->lPropertyGold;
			if(pClientUserItem->GetMemberOrder()>0) lPrice=lPrice*pPropertyInfo->wDiscount/100;

			//购买数目
			TCHAR szBuyCountInfo[32]=TEXT("");
			if(pClientUserItem!=NULL) m_lCanBuyCount=__max((LONGLONG)(pClientUserItem->GetUserInsure()/lPrice),0);	
			_sntprintf(szBuyCountInfo,CountArray(szBuyCountInfo),TEXT("（您最多能购买 %I64d %s）"),m_lCanBuyCount,pPropertyAttrib->szMeasuringunit);
			SetDlgItemText(IDC_MAX_BUYCOUNT,szBuyCountInfo);	

			//变量定义
			UINT  wItemCount = GetDlgItemInt(IDC_PROPERTY_COUNT);
			
			//数目判断
			if(wItemCount>m_lCanBuyCount) 
			{
				//数目设置
				SetDlgItemInt(IDC_PROPERTY_COUNT,(UINT)m_lCanBuyCount);

				//光标设置
				((CEdit *)GetDlgItem(IDC_PROPERTY_COUNT))->SetSel(-1);
			}
		}
	}

	return;
}

//显示窗口
VOID CDlgBuyProperty::ShowBuyPropertyWnd(CGamePropertyItem * pGamePropertyItem, IGamePropertySink * pIGamePropertySink)
{
	//效验参数
	ASSERT(pGamePropertyItem!=NULL);
	ASSERT(pIGamePropertySink!=NULL);

	//变量定义
	CDlgBuyProperty * pDlgBuyProperty=m_pDlgBuyProperty;

	//创建对象
	if (pDlgBuyProperty==NULL)
	{
		try
		{
			pDlgBuyProperty=new CDlgBuyProperty;
		}
		catch (...)
		{
			ASSERT(FALSE);
		}
	}

	//设置变量
	pDlgBuyProperty->m_pGamePropertyItem=pGamePropertyItem;
	pDlgBuyProperty->m_pIGamePropertySink=pIGamePropertySink;

	//创建窗口
	if (pDlgBuyProperty->m_hWnd==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(GAME_PROPERTY_DLL_NAME));
		pDlgBuyProperty->Create(IDD_DLG_PROPERTY,AfxGetMainWnd());
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	else
	{
		//更新状态
		pDlgBuyProperty->UpdateCtrlStatus();
	}

	//显示窗口
	pDlgBuyProperty->ShowWindow(SW_SHOW);

	//激活窗口
	pDlgBuyProperty->SetActiveWindow();
	pDlgBuyProperty->SetForegroundWindow();

	return;
}

//改变事件
VOID CDlgBuyProperty::OnEventPerformCommand(WORD wCommandID,VOID * pCmdData)
{
	//存在判断
	if(m_pDlgBuyProperty==NULL || m_pDlgBuyProperty->m_hWnd==NULL) return;

	//分别处理
	if(wCommandID==CMD_CLOSE_WND)
	{
		//关闭窗口
		m_pDlgBuyProperty->DestroyWindow();
	}
	else if(wCommandID==CMD_USER_CHANGE)
	{
		//提取数据
		IClientUserItem * pIClintUserItem=(IClientUserItem *)pCmdData;

		//参数判断
		if(pIClintUserItem==NULL) return;

		//修改昵称
		m_pDlgBuyProperty->SetDlgItemText(IDC_NICK_NAME,pIClintUserItem->GetNickName());
	}
}

//改变事件
void CDlgBuyProperty::OnEnChangePropertyCount()
{
	//变量定义
	DWORD dwSelPos=((CEdit *)GetDlgItem(IDC_PROPERTY_COUNT))->GetSel();
	CString strCount;
	GetDlgItem(IDC_PROPERTY_COUNT)->GetWindowText(strCount);
	
	//去掉前面的0
	CString strLeft = strCount.Left(1);
	if(strLeft.Find('0') != -1)
	{
		strCount = strCount.Right(strCount.GetLength() - 1);
		SetDlgItemText(IDC_PROPERTY_COUNT,strCount);
	}

	UINT wItemCount = GetDlgItemInt(IDC_PROPERTY_COUNT);

	//数目判断
	if(wItemCount>m_lCanBuyCount) 
		SetDlgItemInt(IDC_PROPERTY_COUNT,(UINT)m_lCanBuyCount);
	
	//设置光标	 
	((CEdit *)GetDlgItem(IDC_PROPERTY_COUNT))->SetSel(dwSelPos);
	
	return;
}

//////////////////////////////////////////////////////////////////////////////////
