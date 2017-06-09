// DlgBank.cpp : implementation file
//

#include "stdafx.h"
#include "DlgInsureGame.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////

//类声明
//class CGameClientView;
#define IDC_OK				500
#define IDC_CANCEL			501
#define	IDC_FRESH			502

//////////////////////////////////////////////////////////////////////////

//定时器I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//更新界面
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//更新界面

//////////////////////////////////////////////////////////////////////////

//转换字符串
void MakeString(CString &strNum,SCORE lNumber)
{
	CString strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/3); i++)
		strTempNum.Insert(nLength - 3*(i+1), _T(","));
	strNum.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
}

//转换数字
void MakeStringToNum(CString str,SCORE &Num)
{
	CString strNumber = str;
	strNumber.Remove(',');
	strNumber.Trim();

#ifndef _UNICODE
	_snscanf(strNumber.GetBuffer(),strNumber.GetLength(),_TEXT("%I64d"),&Num);
#else
	_snwscanf(strNumber.GetBuffer(),strNumber.GetLength(),_TEXT("%I64d"),&Num);
#endif
	strNumber.ReleaseBuffer();
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDlgInsureGame, CSkinDialog)
CDlgInsureGame::CDlgInsureGame(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_BANK_STORAGE, pParent)
{
	m_wRevenueTake=0;
	m_wRevenueTransfer=0;
	m_lInCount=0;
	m_lGameGold=0;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_bBankStorage=true;
	m_blCanStore=false;
	m_blCanGetOnlyFree=false;
	m_ImageFrame.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("BANK_FRAME"));	
	m_ImageNumber.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("BANKNUM"));
}

CDlgInsureGame::~CDlgInsureGame()
{
}

void CDlgInsureGame::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_USER_PASSWORD, m_Edit);
	DDX_Control(pDX, IDC_STATIC_PASS, m_Static);
}


BEGIN_MESSAGE_MAP(CDlgInsureGame, CSkinDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_FRESH,OnBnClickedFresh)
	ON_BN_CLICKED(IDC_OK,OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL,OnCancelEvent)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnBnClickedCheck)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
	ON_MESSAGE(WM_CREADED,OnCreateEd)

END_MESSAGE_MAP()

//勾选
void CDlgInsureGame::OnBnClickedCheck()
{
	if(IsButtonSelected(IDC_CHECK_ALL))
	{
		if(m_pMeUserData!=NULL)
		{
			//设置信息
			m_lInCount=(m_bBankStorage)?m_lGameGold:m_lStorageGold;

			//更新显示
			CString strlGameGold;
			MakeString(strlGameGold,m_lInCount);
			SetDlgItemText(IDC_IN_COUNT,strlGameGold);
		}
	}
	else
	{
		//更新显示
		SetDlgItemText(IDC_IN_COUNT,_TEXT(""));
		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
	}
}

//绘画数字
VOID CDlgInsureGame::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//转换逗号
	TCHAR szControl[128]=TEXT("");
	CString cs;

	//转换格式
	MakeString(cs,lScore);
	_sntprintf(szControl,CountArray(szControl),TEXT("%s"),cs);

	//变量定义
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//绘画判断
	if (nScoreLength>0L)
	{
		//获取大小
		CSize SizeNumber;
		SizeNumber.SetSize(m_ImageNumber.GetWidth()/12L,m_ImageNumber.GetHeight());

		//绘画数字
		for (INT i=0;i<nScoreLength;i++)
		{
			//绘画逗号
			if (szControl[i]==TEXT(','))
			{
				m_ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//绘画点号
			if (szControl[i]==TEXT('.'))
			{
				m_ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//绘画数字
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				m_ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//设置位置
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

//刷新按钮
void CDlgInsureGame::OnBnClickedFresh()
{
	//变量定义
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	QueryInsureInfo.cbActivityGame=TRUE;

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
}

//提取按钮
void CDlgInsureGame::OnBnClickedOk()
{	
	//数字转换
	CString strInCount;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strInCount);
	MakeStringToNum(strInCount, m_lInCount);

	//参数验证
	if (false==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		//构造消息
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%I64d之间"), m_lStorageGold);
		else strMessage.Format(TEXT("你的存储游戏币数目为0,不能进行提取操作!"));

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(strMessage,MB_ICONERROR,30);

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));
		return;
	}

	//参数验证
	if (true==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		//构造消息
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("你输入的游戏币值必须在1和%I64d之间"),m_lGameGold );
		else strMessage.Format(TEXT("你的当前游戏币数目为0,不能进行存储操作!"));

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(strMessage,MB_ICONERROR,30);

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));
		return;
	}

	//密码验证
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));
	if (szPassword[0]==0&&m_blUsingPassWord)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("密码不能为空，请重新输入密码！"),MB_ICONERROR,30);

		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}

	//密码加密
	if(m_blUsingPassWord)
	{
		TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
		CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
		CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);

	}

	//取款
	if (false==m_bBankStorage)
	{
		//构造数据
		CMD_GR_C_TakeScoreRequest TakeScoreRequest;
		ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));
		TakeScoreRequest.cbActivityGame=TRUE;
		TakeScoreRequest.lTakeScore=m_lInCount;
		CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

		//发送数据
		SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));
	}
	else
	{
		//状态判断
		if (US_PLAYING==m_pMeUserData->GetUserStatus())
		{
			//提示消息
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("请结束游戏后再存款！"),MB_ICONERROR,30);
			return;	
		}

		//构造数据
		CMD_GR_C_SaveScoreRequest SaveScoreRequest;
		SaveScoreRequest.cbActivityGame=TRUE;
		SaveScoreRequest.lSaveScore=m_lInCount;

		//发送数据
		SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));
	}

	//清空密码
	GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));

	return;
}

//初始化函数
BOOL CDlgInsureGame::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	UpdateData(FALSE);
	SetWindowText(TEXT("保险柜"));
	SetBankerActionType(true);	

	//定时更新
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);

	return TRUE;
}

//绘画界面
VOID CDlgInsureGame::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{	
	m_ImageFrame.DrawImage(pDC,30,55);
	DrawNumberString(pDC,m_lGameGold,200,78);
	DrawNumberString(pDC,m_lStorageGold,200,104);
	CRect rcScore(120,80,200,95);
	pDC->DrawText(TEXT("当前游戏币："),rcScore,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
	CRect rcBank(120,105,200,120);
	pDC->DrawText(TEXT("保险柜游戏币："),rcBank,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
	CRect rect(0,315,nWidth,nHeight);
	CString strInfo;
	strInfo.Format(TEXT("温馨提示：存入游戏币免手续费，取出将扣除%ld‰的手续费"), m_wRevenueTake);
	pDC->DrawText(strInfo,rect,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
}

//更新界面
void CDlgInsureGame::UpdateView()
{
	//获取数量
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	SCORE OutNum = 0;
	MakeStringToNum(strinCount,OutNum);

	//更新数据
	if(m_OrInCount!= OutNum)
	{
		m_OrInCount = OutNum;
		if(OutNum!=0)
		{
			MakeString(strinCount,OutNum);
			SetDlgItemText(IDC_IN_COUNT,strinCount);
			((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
		}

		//参数验证
		if (false==m_bBankStorage && (OutNum < 0 || OutNum > m_lStorageGold ))
		{
			return;
		}

		//参数验证
		if (true==m_bBankStorage && (OutNum < 0 || OutNum > m_lGameGold ))
		{
			return;
		}
	}

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
}

//鼠标信息
void CDlgInsureGame::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialog::OnLButtonDown(nFlags, point);
}

//鼠标信息
void CDlgInsureGame::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialog::OnMouseMove(nFlags, point);
}

//命令信息
BOOL CDlgInsureGame::PreTranslateMessage(MSG* pMsg)
{
	//过滤字母
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

//时间消息
void CDlgInsureGame::OnTimer(UINT nIDEvent)
{
	//时间消息
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//更新界面
		{
			//更新界面
			UpdateView();
			return;
		}
	}
	__super::OnTimer(nIDEvent);
}

//输入信息
void CDlgInsureGame::OnEnChangeInCount()
{
	//状态效验
	if(!m_pMeUserData) return;

	//获取数据
	CString strCount;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strCount);

	//去掉前面的0
	CString strLeft = strCount.Left(1);
	if(strLeft.Find('0') != -1)
	{
		strCount = strCount.Right(strCount.GetLength() - 1);
		SetDlgItemText(IDC_IN_COUNT,strCount);
	}

	//设置信息
	SCORE lInCount=(m_bBankStorage)?m_lGameGold:m_lStorageGold;
	MakeStringToNum(strCount, m_lInCount);
	if((m_lInCount > lInCount || m_lInCount < 0L))
	{
		m_lInCount = lInCount;
		CString strlGameGold;
		MakeString(strlGameGold,m_lInCount);
		SetDlgItemText(IDC_IN_COUNT,strlGameGold);
	}

	return ;
}

//是否选中
bool CDlgInsureGame::IsButtonSelected(UINT uButtonID)
{
	return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
}

//创建消息
int CDlgInsureGame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	CRect rect(0, 0, 0, 0);	
	m_btOK.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rect,this,IDC_OK);	
	m_btCancel.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rect,this,IDC_CANCEL);
	m_btFresh.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rect,this,IDC_FRESH);
	m_btOK.SetWindowPos(NULL,110,240,94,32,SWP_NOSIZE);
	m_btCancel.SetWindowPos(NULL,285,240,94,32,SWP_NOSIZE);
	m_btFresh.SetWindowPos(NULL,390,66,64,64,SWP_NOSIZE);
	
	return 0;
}

//创建消息
afx_msg LRESULT  CDlgInsureGame::OnCreateEd(WPARAM wparam,LPARAM lparam)
{
	return 1;
}

//选中按钮
void CDlgInsureGame::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//设置信息
void CDlgInsureGame::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserData)
{
	//参数效验
	ASSERT(pClientKernel!=NULL);
	if(pClientKernel==NULL) return;

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pMeUserData=const_cast<IClientUserItem *>(pMeUserData);

	//更新信息
	OnBnClickedFresh();

	return;
}

//设置位置
void CDlgInsureGame::SetPostPoint(CPoint Point)
{
	CRect Rect;
	GetWindowRect(&Rect);
	SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,/*SWP_NOMOVE|*/SWP_NOZORDER);
}

//发送信息
void CDlgInsureGame::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//参数效验
	ASSERT(m_pIClientKernel!=NULL);

	//发送信息
	if(m_pIClientKernel) m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//显示银行
void CDlgInsureGame::ShowItem()
{
	//设置控件
	HINSTANCE hInstance = GetModuleHandle(GAME_FRAME_DLL_NAME);	
	m_btFresh.SetButtonImage(IDB_BT_FRESH,hInstance,false,false);
	m_btFresh.EnableWindow(TRUE);
	if(m_bBankStorage)
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
	    GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(FALSE);
		GetDlgItem(IDC_USER_PASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_HIDE);
		m_btOK.SetButtonImage(IDB_SAVEGOLD, hInstance, false,false);
		m_btCancel.SetButtonImage(IDB_CLOSE,hInstance,false,false);
		m_btOK.EnableWindow(m_blCanStore);
		m_btCancel.EnableWindow(TRUE);
	}
	else
	{
		BOOL bShow = TRUE;
		if(m_blCanGetOnlyFree && m_pMeUserData->GetUserStatus() == US_PLAYING) bShow = FALSE;
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_SHOW);
		m_btOK.SetButtonImage(IDB_GETGOLD, hInstance, false,false);
		m_btCancel.SetButtonImage(IDB_CLOSE,hInstance,false,false);
		m_btOK.EnableWindow(bShow);
		m_btCancel.EnableWindow(TRUE);
	}

}

//设置类型
void CDlgInsureGame::SetBankerActionType(bool bStorage) 
{
	//设置变量
	m_bBankStorage = bStorage;
	m_lInCount = 0;
	GetDlgItem(IDC_IN_COUNT)->SetWindowText(TEXT(""));
	((CButton*)GetDlgItem(IDC_CHECK_ALL))->SetCheck(0);

	//设置标题
	if (m_bBankStorage)
	{
		m_blUsingPassWord = false;
	}
	else
	{
		m_blUsingPassWord = true;
	}

	//显示密码框
	if(m_blUsingPassWord)
	{
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(m_blCanStore);
	}

	//显示控件
	ShowItem();
}

//关闭
void CDlgInsureGame::OnCancelEvent()
{
	DestroyWindow();
}

//取消
void CDlgInsureGame::OnCancel()
{
	DestroyWindow();
}

//回车
void CDlgInsureGame::OnOK()
{
	OnBnClickedOk();
}
