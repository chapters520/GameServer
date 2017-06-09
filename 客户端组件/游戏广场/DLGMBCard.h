#pragma once
#include "stdafx.h"

// CDLGMBCard dialog

class CDLGMBCard : public CSkinDialog
{
public:
	UINT								m_uMBCardID;						//机器序列
	CHAR								m_szRandLetter[3];
	BYTE								m_szRandNumber[3];
	BYTE								m_cbErrorCount;
	UINT								m_uCoorNumber[6][4];

	CSkinButton							m_btOK;
	CSkinButton							m_btCancel;

	//函数定义
public:
	CDLGMBCard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGMBCard();

	//函数定义
public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//重载函数
public:
	//配置消息
	virtual BOOL OnInitDialog();
	//按钮消息
	void OnOK();
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//辅助函数
public:
	void RandCoordinate();
	void SetMBCard(UINT uMBCardID);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDLGMBCard)
};
