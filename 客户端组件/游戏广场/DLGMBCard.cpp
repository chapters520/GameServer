// DLGMBCard.cpp : implementation file
//

#include "stdafx.h"
#include "DLGMBCard.h"
#include "resource.h"
#include ".\dlgmbcard.h"


// CDLGMBCard dialog

IMPLEMENT_DYNAMIC(CDLGMBCard, CSkinDialog)
CDLGMBCard::CDLGMBCard(CWnd* pParent /*=NULL*/)
	: CSkinDialog(IDD_MBCARD, pParent)
{
	m_uMBCardID=0;
	ZeroMemory(m_szRandLetter,sizeof(m_szRandLetter));
	ZeroMemory(m_szRandNumber,sizeof(m_szRandNumber));
	m_cbErrorCount=0;
}

CDLGMBCard::~CDLGMBCard()
{
}

void CDLGMBCard::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX,IDOK,m_btOK);
	DDX_Control(pDX,IDCANCEL,m_btCancel);
}


BEGIN_MESSAGE_MAP(CDLGMBCard, CSkinDialog)
	//ON_WM_PAINT()
END_MESSAGE_MAP()


//配置消息
BOOL CDLGMBCard::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	SetWindowText(TEXT("密保信息"));

	srand((unsigned)time(NULL));

	RandCoordinate();
	GetDlgItem(IDC_INPUT)->SetFocus();
	((CEdit *)GetDlgItem(IDC_INPUT))->LimitText(15);

	return FALSE; 
}

//绘制消息
void CDLGMBCard::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	//创建字体
	CFont DrawFont;
	DrawFont.CreateFont(-23,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("宋体"));

	//设置环境
	CFont * pOldFont = pDC->SelectObject(&DrawFont);
	COLORREF crOldColor = pDC->SetTextColor(RGB(0,0,255));
	int iOldMode = pDC->SetBkMode(TRANSPARENT);

	//绘制信息
	CString strCoordinate,strTmp;
	for (int i=0; i<3; ++i)
	{
		strTmp.Format(TEXT("%c%d "),m_szRandLetter[i],m_szRandNumber[i]);
		strCoordinate+=strTmp;
	}
	pDC->TextOut(140,46,strCoordinate);

	//还原设置
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(crOldColor);
	pDC->SetBkMode(iOldMode);
	DrawFont.DeleteObject();
}

//控件事件
void CDLGMBCard::OnOK()
{
	//获取信息
	TCHAR szInput[64];
	GetDlgItemText(IDC_INPUT,szInput,CountArray(szInput));

	CString strCoorNumber=TEXT(""),strTmp;
	for (int i=0; i<3; ++i)
	{
		int nIndex1=int(m_szRandLetter[i]-'A');
		int nIndex2=int(m_szRandNumber[i]-1);
		if(m_uCoorNumber[nIndex1][nIndex2]<10)
			strTmp.Format(TEXT("%d00"),m_uCoorNumber[nIndex1][nIndex2]);
		else if(m_uCoorNumber[nIndex1][nIndex2]<100)
			strTmp.Format(TEXT("%d0"),m_uCoorNumber[nIndex1][nIndex2]);
		else
			strTmp.Format(TEXT("%d"),m_uCoorNumber[nIndex1][nIndex2]);
		strCoorNumber+=strTmp;
	}

	//效验信息
	if (strCoorNumber!=szInput) 
	{		
		++m_cbErrorCount;
		if (m_cbErrorCount>=3)
		{
			ShowInformationEx(TEXT("提示消息"),TEXT("密保输错次数超限，请重新登录！"),MB_ICONINFORMATION,0);
			EndDialog(IDCANCEL);
			return;
		}		
		ShowInformationEx(TEXT("提示消息"),TEXT("密保坐标对应数字输入有误！"),MB_ICONINFORMATION,0);
		RandCoordinate();
		Invalidate();
		return;
	}

	EndDialog(IDOK);
}

//随机函数
void CDLGMBCard::RandCoordinate()
{
	for (int i=0; i<CountArray(m_szRandLetter); ++i)
		m_szRandLetter[i]='A'+rand()%4;
	for (int i=0; i<CountArray(m_szRandNumber); ++i) 
		m_szRandNumber[i]=1+rand()%4;
}

//设置密保
void CDLGMBCard::SetMBCard(UINT uMBCardID)
{
	m_uMBCardID=uMBCardID;

	m_uCoorNumber[0][0]=(m_uMBCardID/9)%1000;
	m_uCoorNumber[0][1]=(m_uMBCardID/15)%1000;
	m_uCoorNumber[0][2]=(m_uMBCardID/24)%1000;
	m_uCoorNumber[0][3]=(m_uMBCardID/268)%1000;

	m_uCoorNumber[1][0]=(m_uMBCardID/1)%1000;
	m_uCoorNumber[1][1]=(m_uMBCardID/88)%1000;
	m_uCoorNumber[1][2]=(m_uMBCardID/25)%1000;
	m_uCoorNumber[1][3]=(m_uMBCardID/76)%1000;

	m_uCoorNumber[2][0]=(m_uMBCardID/185)%1000;
	m_uCoorNumber[2][1]=(m_uMBCardID/28)%1000;
	m_uCoorNumber[2][2]=(m_uMBCardID/65)%1000;
	m_uCoorNumber[2][3]=(m_uMBCardID/205)%1000;

	m_uCoorNumber[3][0]=(m_uMBCardID/72)%1000;
	m_uCoorNumber[3][1]=(m_uMBCardID/39)%1000;
	m_uCoorNumber[3][2]=(m_uMBCardID/11)%1000;
	m_uCoorNumber[3][3]=(m_uMBCardID/31)%1000;

	m_uCoorNumber[4][0]=(m_uMBCardID/52)%1000;
	m_uCoorNumber[4][1]=(m_uMBCardID/220)%1000;
	m_uCoorNumber[4][2]=(m_uMBCardID/7)%1000;
	m_uCoorNumber[4][3]=(m_uMBCardID/105)%1000;

	m_uCoorNumber[5][0]=(m_uMBCardID/99)%1000;
	m_uCoorNumber[5][1]=(m_uMBCardID/117)%1000;
	m_uCoorNumber[5][2]=(m_uMBCardID/93)%1000;
	m_uCoorNumber[5][3]=(m_uMBCardID/48)%1000;
}