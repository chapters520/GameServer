#ifndef GAME_OPTION_ITEM_HEAD_FILE
#define GAME_OPTION_ITEM_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////

//设置子项
class GAME_FRAME_CLASS CGameOptionItem : public CDialog, public IGameOptionItem
{
	//变量定义
protected:
	CSize							m_LogoSize;							//视图大小
	CSize							m_ControlSize;						//控件大小
	CBitImage						m_ImageOptionLogo;					//视图资源

	//函数定义
public:
	//构造函数
	CGameOptionItem();
	//析构函数
	virtual ~CGameOptionItem();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//窗口接口
public:
	//获取句柄
	virtual HWND __cdecl GetWindowHwnd() { return m_hWnd; }
	//创建控件
	virtual bool __cdecl CreateOptionItem(CWnd * pParentWnd)=NULL;

	//扩展接口
public:
	//调整控件
	virtual VOID __cdecl RectifyControl(INT nWidth, INT nHeight);
	//绘画控件
	virtual VOID __cdecl DrawControlFace(CDC * pDC, INT nWidth, INT nHeight);

	//信息接口
public:
	//保存配置
	virtual bool __cdecl SaveParameter()=NULL;
	//默认参数
	virtual bool __cdecl DefaultParameter()=NULL;
	//获取位置
	virtual VOID __cdecl GetControlMinSize(CSize & ControlSize) { ControlSize=m_ControlSize; }

	//重载函数
protected:
	//确定消息
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//消息映射
public:
	//绘画消息
	VOID OnPaint();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	//控件颜色
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif