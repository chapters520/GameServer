#ifndef DLG_MANAGE_USER_HEAD_FILE
#define DLG_MANAGE_USER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//房间管理
class CDlgManageUser : public CSkinDialog
{
	//变量定义
protected:
	DWORD							m_dwMasterRight;					//管理权限

	//接口定义
protected:
	ITCPSocket *					m_pITCPSocket;						//网络接口

	//控件变量
protected:
	CSkinButton						m_btKickAllUser;					//控制按钮
	CSkinButton						m_btDismissGame;					//控制按钮
	CSkinButton						m_btSeeUserIP;					    //控制按钮

	//函数定义
public:
	//构造函数
	CDlgManageUser();
	//析构函数
	virtual ~CDlgManageUser();

	//重载函数 
protected:
	//菜单命令
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//功能函数
public:
	//设置接口
	bool SetTCPSocket(IUnknownEx * pIUnknownEx);
	//设置权限
	VOID SetMasterRight( DWORD dwMasterRight );

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////


#endif