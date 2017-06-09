#ifndef VIDEO_SERVICE_HEAD_FILE
#define VIDEO_SERVICE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////
//包含文件

//平台文件
#include "..\..\全局定义\macro.h"
//#include "..\..\公共文件\GlobalDef.h"

//组件头文件
#include "..\..\全局定义\module.h"
#include "..\..\客户端组件\游戏框架\GameFrameHead.h"
#include "..\..\客户端组件\界面控件\SkinControlHead.h"
#include "..\..\客户端组件\游戏道具\GamePropertyHead.h"
//#include "..\..\客户端组件\鲜花道具\PropertyBar.h"

//////////////////////////////////////////////////////////////////////////
//公共宏定义

//导出定义
#ifndef VIDEO_SERVICE_CLASS
	#ifdef  VIDEO_SERVICE_DLL
		#define VIDEO_SERVICE_CLASS _declspec(dllexport)
	#else
		#define VIDEO_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define VIDEO_SERVICE_DLL_NAME	TEXT("VideoService.dll")			//组件 DLL 名字
#else
	#define VIDEO_SERVICE_DLL_NAME	TEXT("VideoServiceD.dll")			//组件 DLL 名字
#endif

//////////////////////////////////////////////////////////////////////////
//结构定义

//连接信息
struct tagVideoNetwordInfo
{
	WORD							wNatPort;							//网络端口
	WORD							wLocalPort;							//网络端口
	DWORD							dwNatAddr;							//网络地址
	DWORD							dwLocalAddr;						//本地地址
};

//导出类头文件
#include "VideoServiceControl.h"

//////////////////////////////////////////////////////////////////////////

#endif
