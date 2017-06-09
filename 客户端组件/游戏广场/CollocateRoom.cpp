// CollocateRoom.cpp : implementation file
//

#include "stdafx.h"
#include "CollocateRoom.h"
#include "Resource.h"
#include ".\collocateroom.h"


// CCollocateRoom dialog

IMPLEMENT_DYNAMIC(CCollocateRoom, CDialog)
CCollocateRoom::CCollocateRoom()
	: CSkinDialog(IDD_COLLOCATE_ROOM)
{
	//配置变量
	m_pParameterGame=NULL;
	m_pParameterServer=NULL;
}

CCollocateRoom::~CCollocateRoom()
{
}

//配置参数
bool CCollocateRoom::InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer)
{
	//设置变量
	m_pParameterGame=pParameterGame;
	m_pParameterServer=pParameterServer;

	//默认参数
	DefaultParameter();

	return true;
}

void CCollocateRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_WIN_RATE, m_edWinRate);
	DDX_Control(pDX, IDC_FLEE_RATE, m_edFleeRate);
	DDX_Control(pDX, IDC_GAME_SCORE_MIN, m_edScoreMin);
	DDX_Control(pDX, IDC_GAME_SCORE_MAX, m_edScoreMax);
	DDX_Control(pDX, IDC_TABLE_PASSWORD, m_edPassword);
}


BEGIN_MESSAGE_MAP(CCollocateRoom, CSkinDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCollocateRoom message handlers

BOOL CCollocateRoom::OnInitDialog()
{
	__super::OnInitDialog();

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置标题
	SetWindowText(TEXT("房间设置"));

	//变量定义
	INT nTViewBorder=m_SkinAttribute.m_EncircleInfoView.nTBorder;
	INT nBViewBorder=m_SkinAttribute.m_EncircleInfoView.nBBorder;
	INT nLViewBorder=m_SkinAttribute.m_EncircleInfoView.nLBorder;
	INT nRViewBorder=m_SkinAttribute.m_EncircleInfoView.nRBorder;
	INT nBFrameBorder=m_SkinAttribute.m_EncircleInfoFrame.nBBorder;

	//移动窗口
	rcClient.bottom=nTViewBorder+nBFrameBorder+185;
	SetWindowPos(NULL,0,0,rcClient.Width(),rcClient.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//限制输入
	((CEdit *)GetDlgItem(IDC_WIN_RATE))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_FLEE_RATE))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_GAME_SCORE_MAX))->LimitText(10);
	((CEdit *)GetDlgItem(IDC_GAME_SCORE_MIN))->LimitText(11);
	((CEdit *)GetDlgItem(IDC_TABLE_PASSWORD))->LimitText(LEN_PASSWORD-1);

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//房间设置
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//游戏配置
		m_wMinWinRate=m_pParameterGame->m_wMinWinRate;
		m_wMaxFleeRate=m_pParameterGame->m_wMaxFleeRate;
		m_bLimitWinRate=m_pParameterGame->m_bLimitWinRate;
		m_lMaxGameScore=m_pParameterGame->m_lMaxGameScore;
		m_lMinGameScore=m_pParameterGame->m_lMinGameScore;
		m_bLimitFleeRate=m_pParameterGame->m_bLimitFleeRate;
		m_bLimitGameScore=m_pParameterGame->m_bLimitGameScore;

		//其他配置
		m_bLimitDetest=pParameterGlobal->m_bLimitDetest;
		m_bLimitSameIP=pParameterGlobal->m_bLimitSameIP;

		//房间配置
		m_bTakePassword=m_pParameterServer->m_bTakePassword;
		lstrcpyn(m_szPassword,m_pParameterServer->m_szPassword,CountArray(m_szPassword));
	}

	//更新控件
	UpdateControlStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//保存参数
bool CCollocateRoom::SaveParameter()
{
	//获取参数
	m_bLimitDetest=(IsDlgButtonChecked(IDC_LIMIT_DETEST)==BST_CHECKED)?true:false;
	m_bLimitSameIP=(IsDlgButtonChecked(IDC_LIMIT_SAME_IP)==BST_CHECKED)?true:false;
	m_bTakePassword=(IsDlgButtonChecked(IDC_TAKE_PASSWORD)==BST_CHECKED)?true:false;
	m_bLimitWinRate=(IsDlgButtonChecked(IDC_LIMIT_WIN_RATE)==BST_CHECKED)?true:false;
	m_bLimitFleeRate=(IsDlgButtonChecked(IDC_LIMIT_FLEE_RATE)==BST_CHECKED)?true:false;
	m_bLimitGameScore=(IsDlgButtonChecked(IDC_LIMIT_GAME_SCORE)==BST_CHECKED)?true:false;

	//最低胜率
	TCHAR szBuffer[128]=TEXT("");
	GetDlgItemText(IDC_WIN_RATE,szBuffer,CountArray(szBuffer));
	m_wMinWinRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//最高逃率
	GetDlgItemText(IDC_FLEE_RATE,szBuffer,CountArray(szBuffer));
	m_wMaxFleeRate=(WORD)(_tstof(szBuffer)*100.0)%10000;

	//积分限制
	m_lMaxGameScore=GetDlgItemInt(IDC_GAME_SCORE_MAX,NULL,TRUE);
	m_lMinGameScore=GetDlgItemInt(IDC_GAME_SCORE_MIN,NULL,TRUE);
	if ((m_bLimitGameScore)&&(m_lMinGameScore>=m_lMaxGameScore))
	{
		//显示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("积分限制范围参数不正确，请正确设置积分限制范围！"),MB_ICONINFORMATION);

		//设置焦点
		GetDlgItem(IDC_GAME_SCORE_MAX)->SetFocus();

		return false;
	}

	//携带密码
	GetDlgItemText(IDC_TABLE_PASSWORD,m_szPassword,CountArray(m_szPassword));
	if ((m_bTakePassword==true)&&(m_szPassword[0]==0))
	{
		//显示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("桌子携带密码没有设置，请输入设置桌子携带密码！"),MB_ICONINFORMATION);

		//设置焦点
		GetDlgItem(IDC_TABLE_PASSWORD)->SetFocus();

		return false;
	}

	return true;
}

//默认参数
bool CCollocateRoom::DefaultParameter()
{
	//胜率限制
	m_wMinWinRate=0;
	m_bLimitWinRate=false;

	//逃率限制
	m_wMaxFleeRate=5000;
	m_bLimitFleeRate=false;

	//积分限制
	m_bLimitGameScore=false;
	m_lMaxGameScore=2147483647L;
	m_lMinGameScore=-2147483646L;

	//其他配置
	m_bLimitDetest=false;
	m_bLimitSameIP=false;

	//携带密码
	m_bTakePassword=false;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));

	return true;
}

//更新控制
bool CCollocateRoom::UpdateControlStatus()
{
	//设置控件
	CheckDlgButton(IDC_LIMIT_DETEST,(m_bLimitDetest==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_SAME_IP,(m_bLimitSameIP==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_TAKE_PASSWORD,(m_bTakePassword==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_WIN_RATE,(m_bLimitWinRate==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_FLEE_RATE,(m_bLimitFleeRate==true)?BST_CHECKED:BST_UNCHECKED);
	CheckDlgButton(IDC_LIMIT_GAME_SCORE,(m_bLimitGameScore==true)?BST_CHECKED:BST_UNCHECKED);

	//最低胜率
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMinWinRate))/100.0);
	SetDlgItemText(IDC_WIN_RATE,szBuffer);

	//最高逃率
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%.2f"),((DOUBLE)(m_wMaxFleeRate))/100.0);
	SetDlgItemText(IDC_FLEE_RATE,szBuffer);

	//积分限制
	SetDlgItemInt(IDC_GAME_SCORE_MAX,m_lMaxGameScore,TRUE);
	SetDlgItemInt(IDC_GAME_SCORE_MIN,m_lMinGameScore,TRUE);

	//携带密码
	SetDlgItemText(IDC_TABLE_PASSWORD,m_szPassword);

	return true;
}

void CCollocateRoom::OnBnClickedOk()
{
	//保存参数
	if(SaveParameter()==false)return;

	//变量定义
	ASSERT(CParameterGlobal::GetInstance()!=NULL);
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();

	//房间配置
	if ((m_pParameterServer!=NULL)&&(m_pParameterGame!=NULL))
	{
		//其他配置
		pParameterGlobal->m_bLimitDetest=m_bLimitDetest;
		pParameterGlobal->m_bLimitSameIP=m_bLimitSameIP;

		//游戏配置
		m_pParameterGame->m_wMinWinRate=m_wMinWinRate;
		m_pParameterGame->m_wMaxFleeRate=m_wMaxFleeRate;
		m_pParameterGame->m_lMaxGameScore=m_lMaxGameScore;
		m_pParameterGame->m_lMinGameScore=m_lMinGameScore;
		m_pParameterGame->m_bLimitWinRate=m_bLimitWinRate;
		m_pParameterGame->m_bLimitFleeRate=m_bLimitFleeRate;
		m_pParameterGame->m_bLimitGameScore=m_bLimitGameScore;

		//房间配置
		m_pParameterServer->m_bTakePassword=m_bTakePassword;
		lstrcpyn(m_pParameterServer->m_szPassword,m_szPassword,CountArray(m_pParameterServer->m_szPassword));
	}

	OnOK();
}
