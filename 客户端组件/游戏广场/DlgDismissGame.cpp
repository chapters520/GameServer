// DlgDismissGame.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgDismissGame.h"
#include ".\dlgdismissgame.h"


// CDlgDismissGame dialog

IMPLEMENT_DYNAMIC(CDlgDismissGame, CSkinDialog)
CDlgDismissGame::CDlgDismissGame(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CDlgDismissGame::IDD, pParent)
{
}

CDlgDismissGame::~CDlgDismissGame()
{
}

void CDlgDismissGame::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control( pDX,IDOK,m_btOK );
	DDX_Control( pDX,IDCANCEL,m_btCancel );
}


BEGIN_MESSAGE_MAP(CDlgDismissGame, CSkinDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDismissGame message handlers

void CDlgDismissGame::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strTableNum;
	GetDlgItem(IDC_EDIT_TABLE_NUM)->GetWindowText(strTableNum);
	m_wDismissTable=(WORD)StrToInt(strTableNum);
	if(m_wDismissTable > 0) m_wDismissTable -= 1;

	OnOK();
}

BOOL CDlgDismissGame::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("解散桌子"));

	//输入限制
	((CEdit*)GetDlgItem(IDC_EDIT_TABLE_NUM))->SetLimitText(2);

	return TRUE;
}
