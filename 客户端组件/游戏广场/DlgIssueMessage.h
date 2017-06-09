#ifndef DLG_ISSUE_MESSAGE_HEAD_FILE
#define DLG_ISSUE_MESSAGE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//房间管理
class CDlgIssueMessage : public CSkinDialog
{
	//变量定义
public:
	CString							m_strMessage;						//信息提示
	BOOL							m_bSendALLRoom;						//房间消息
	BOOL							m_bSendRoom;						//房间消息
	BOOL							m_bSendGame;						//游戏消息

	//控制变量
protected:
	CSkinButton						m_btOK;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

	//函数定义
public:
	//构造函数
	CDlgIssueMessage();
	//析构函数
	virtual ~CDlgIssueMessage();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////


#endif