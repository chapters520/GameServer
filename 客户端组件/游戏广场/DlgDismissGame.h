#pragma once

#include "Stdafx.h"

// CDlgDismissGame dialog

class CDlgDismissGame : public CSkinDialog
{
	//变量定义
protected:
	WORD							m_wDismissTable;					//踢出提示

	//控制变量
protected:
	CSkinButton						m_btOK;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

public:
	CDlgDismissGame(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDismissGame();

// Dialog Data
	enum { IDD = IDD_DLG_DISMISSGAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_DYNAMIC(CDlgDismissGame)
	DECLARE_MESSAGE_MAP()

public:
	WORD GetDismissTableNum(){return m_wDismissTable;}
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
