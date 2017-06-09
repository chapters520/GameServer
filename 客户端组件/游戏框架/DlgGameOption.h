#ifndef DLG_GAME_OPTION_HEAD_FILE
#define DLG_GAME_OPTION_HEAD_FILE

#pragma once

#include "GameOptionItem.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////

//子项数组
typedef CWHArray<IGameOptionItem *, IGameOptionItem *> CGameOptionItemArray;

//////////////////////////////////////////////////////////////////////////

//常规选项
class CGeneralOptionItem : public CGameOptionItem
{
	//旁观选项
public:
	bool							m_bAllowLookon;						//允许旁观
	bool							m_bAllowFriendLookon;				//允许旁观

	//其他选项
public:
	bool							m_bAllowSound;						//允许声音
	bool							m_bAllowBackGroundSound;			//允许背景声音
	bool							m_bFullGameView;					//全屏模式
	bool							m_bDebarDetestChat;					//屏蔽消息
	bool							m_bNotifyUserInOut;					//进出消息

	//函数定义
public:
	//构造函数
	CGeneralOptionItem();

	//信息函数
protected:
	//获取标题
	virtual LPCTSTR __cdecl GetItemTitle() { return TEXT("常规选项"); }

	//管理函数
protected:
	//保存配置
	virtual bool __cdecl SaveParameter();
	//默认参数
	virtual bool __cdecl DefaultParameter();
	//创建控件
	virtual bool __cdecl CreateOptionItem(CWnd * pParentWnd);

	//内部函数
private:
	//更新控制
	VOID UpdateItemControl();
};

//////////////////////////////////////////////////////////////////////////

//游戏帮助
class CDlgGameOption : public CSkinDialog
{
	//变量定义
protected:
	IGameOptionItem *				m_pIGameOptionItem;					//当前选项
	CGameOptionItemArray			m_GameOptionItemArray;				//配置子项

	//控件变量
public:
	CSkinButton						m_btOk;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮
	CSkinButton						m_btDefault;						//默认按钮
	CSkinTabCtrl					m_SkinTabControl;					//子项选择
	CGeneralOptionItem				m_GeneralOptionItem;				//常规选项

	//函数定义
public:
	//构造函数
	CDlgGameOption();
	//析构函数
	virtual ~CDlgGameOption();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual VOID OnOK();

	//功能函数
public:
	//游戏配置
	bool CreateGameOption(IUnknownEx * pIUnknownEx[], WORD wItemCount);

	//按钮消息
protected:
	//默认按钮
	VOID OnBnClickedDefault();

	//消息映射
protected:
	//销毁消息
	VOID OnNcDestroy();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//子项改变
	VOID OnTcnSelchangeTabControl(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif