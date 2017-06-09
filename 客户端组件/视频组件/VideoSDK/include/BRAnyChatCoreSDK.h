#if !defined(BR_ANYCHAT_CORE_SDK_H__INCLUDED_)
#define BR_ANYCHAT_CORE_SDK_H__INCLUDED_

#include <mmsystem.h>
#include <wingdi.h>


/**
 *	AnyChat Core SDK Include File
 */


#pragma once

#include "GVSDK.h"

#define BRAC_API extern "C" __declspec(dllexport)

// 视频图像格式定义
enum BRAC_PixelFormat{
	BRAC_PIX_FMT_RGB24 = 0,						///< Packed RGB 8:8:8, 24bpp, RGBRGB...（MEDIASUBTYPE_RGB24）
	BRAC_PIX_FMT_RGB32,							///< 对应于：MEDIASUBTYPE_RGB32，Packed RGB 8:8:8, 32bpp, (msb)8A 8R 8G 8B(lsb), in cpu endianness
	BRAC_PIX_FMT_YV12,							///< 对应于：MEDIASUBTYPE_YV12，Planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
	BRAC_PIX_FMT_YUY2,							///< 对应于：MEDIASUBTYPE_YUY2，Packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
};

// 音频设备定义
enum BRAC_AudioDevice{
	BRAC_AD_WAVEIN = 0,							///< 输入设备：Mic
	BRAC_AD_WAVEOUT,							///< 输出设备：Wave
};

// 功能模式定义
#define BRAC_FUNC_VIDEO_CBDATA		0x00000001L	///< 通过回调函数输出视频数据
#define BRAC_FUNC_VIDEO_AUTODISP	0x00000002L	///< 由SDK包处理视频，将视频显示在指定的窗口上
#define BRAC_FUNC_AUDIO_CBDATA		0x00000004L	///< 通过顺调函数输出音频数据
#define BRAC_FUNC_AUDIO_AUTOPLAY	0x00000008L	///< 由SDK包处理音频，直接播放


// 视频数据回调函数定义
typedef void (CALLBACK * BRAC_VideoData_CallBack)(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, BITMAPINFOHEADER bmiHeader, LPVOID lpUserValue);
// 音频数据回调函数定义
typedef void (CALLBACK * BRAC_AudioData_CallBack)(DWORD dwUserid, LPVOID lpBuf, DWORD dwLen, WAVEFORMATEX waveFormatEx, LPVOID lpUserValue);
// 文字消息回调函数定义
typedef void (CALLBACK * BRAC_TextMessage_CallBack)(DWORD dwFromUserid, DWORD dwToUserid, BOOL bSecret, LPCTSTR lpMsgBuf, DWORD dwLen, LPVOID lpUserValue);
// 透明通道数据回调函数定义
typedef void (CALLBACK * BRAC_TransBuffer_CallBack)(DWORD dwUserid, LPBYTE lpBuf, DWORD dwLen, LPVOID lpUserValue);
// 音量变化回调函数定义
typedef void (CALLBACK * BRAC_VolumeChange_CallBack)(BRAC_AudioDevice device, DWORD dwCurrentVolume, LPVOID lpUserValue);

/**
 *	API方法定义
 */

// 获取SDK版本信息
BRAC_API DWORD BRAC_GetSDKVersion(DWORD& dwMainVer, DWORD& dwSubVer, LPSTR lpCompileTime, DWORD dwBufLen);

// 初始化系统
BRAC_API DWORD BRAC_InitSDK(HWND hWnd, DWORD dwFuncMode);

// 设置视频数据回调函数
BRAC_API DWORD BRAC_SetVideoDataCallBack(BRAC_PixelFormat pixFmt, BRAC_VideoData_CallBack lpFunction, LPVOID lpUserValue=NULL);
// 设置音频数据回调函数
BRAC_API DWORD BRAC_SetAudioDataCallBack(BRAC_AudioData_CallBack lpFunction, LPVOID lpUserValue=NULL);
// 设置文字消息回调函数
BRAC_API DWORD BRAC_SetTextMessageCallBack(BRAC_TextMessage_CallBack lpFunction, LPVOID lpUserValue=NULL);
// 设置透明通道数据回调函数
BRAC_API DWORD BRAC_SetTransBufferCallBack(BRAC_TransBuffer_CallBack lpFunction, LPVOID lpUserValue=NULL);
// 设置音量变化回调函数
BRAC_API DWORD BRAC_SetVolumeChangeCallBack(BRAC_VolumeChange_CallBack lpFunction, LPVOID lpUserValue=NULL);

// 连接服务器
BRAC_API DWORD BRAC_Connect(LPCTSTR lpServerAddr, DWORD dwPort);
// 登录系统
BRAC_API DWORD BRAC_Login(LPCTSTR lpUserName, LPCTSTR lpPassword, DWORD dwPassEncType); 
// 进入房间
BRAC_API DWORD BRAC_EnterRoom(DWORD dwRoomid, LPCTSTR lpRoomPass, DWORD dwPassEncType);
// 离开房间
BRAC_API DWORD BRAC_LeaveRoom(DWORD dwRoomid);
// 注销系统
BRAC_API DWORD BRAC_Logout(VOID);

// 释放所有资源
BRAC_API DWORD BRAC_Release(VOID);

// 获取当前房间在线用户列表
BRAC_API DWORD BRAC_GetOnlineUser(LPDWORD lpUserIDArray, DWORD& dwUserNum);
// 查询用户摄像头的状态
BRAC_API DWORD BRAC_GetCameraState(DWORD dwUserid, DWORD& dwState);
// 查询用户发言状态
BRAC_API DWORD BRAC_GetSpeakState(DWORD dwUserid, DWORD& dwState);
// 显示本地视频画面调节对话框
BRAC_API DWORD BRAC_ShowLVProperty(HWND hParent, LPCTSTR szCaption=NULL, DWORD dwX=0, DWORD dwY=0);


// 枚举本地视频采集设备
BRAC_API DWORD BRAC_EnumVideoCapture(CHAR** lpDeviceName, DWORD& dwDeviceNum);
// 选择指定的视频采集设备
BRAC_API DWORD BRAC_SelectVideoCapture(LPCTSTR szCaptureName);
// 获取当前使用的视频采集设备
BRAC_API DWORD BRAC_GetCurVideoCapture(CHAR* lpDeviceName, DWORD dwLen);
// 枚举本地音频采集设备
BRAC_API DWORD BRAC_EnumAudioCapture(CHAR** lpDeviceName, DWORD& dwDeviceNum);
// 选择指定的音频采集设备
BRAC_API DWORD BRAC_SelectAudioCapture(LPCTSTR szCaptureName);
// 获取当前使用的音频采集设备
BRAC_API DWORD BRAC_GetCurAudioCapture(CHAR* lpDeviceName, DWORD dwLen);


// 操作用户视频
BRAC_API DWORD BRAC_UserCameraControl(DWORD dwUserid, BOOL bOpen);
// 操作用户语音
BRAC_API DWORD BRAC_UserSpeakControl(DWORD dwUserid, BOOL bOpen);
// 设置视频显示位置
BRAC_API DWORD BRAC_SetVideoPos(DWORD dwUserid, HWND hWnd, DWORD dwLeft, DWORD dwTop, DWORD dwRight, DWORD dwBottom);

// 获取指定音频设备的当前音量
BRAC_API DWORD BRAC_AudioGetVolume(BRAC_AudioDevice device, DWORD& dwVolume);
// 设置指定音频设备的音量
BRAC_API DWORD BRAC_AudioSetVolume(BRAC_AudioDevice device, DWORD dwVolume);

// 透明通道传送缓冲区
BRAC_API DWORD BRAC_TransBuffer(DWORD dwUserid, LPBYTE lpBuf, DWORD dwLen);
// 传送文本消息
BRAC_API DWORD BRAC_SendTextMessage(DWORD dwUserid, BOOL bSecret, LPCTSTR lpMsgBuf, DWORD dwLen);

// 更改当前的聊天模式
BRAC_API DWORD BRAC_ChangeChatMode(DWORD dwChatMode);
// 获取指定用户当前的聊天模式
BRAC_API DWORD BRAC_GetUserChatMode(DWORD dwUserid, DWORD& dwChatMode);
// 请求与对方私聊，向对方发起私聊请求
BRAC_API DWORD BRAC_PrivateChatRequest(DWORD dwUserid);
// 回复对方的私聊请求
BRAC_API DWORD BRAC_PrivateChatEcho(DWORD dwUserid, DWORD dwRequestid,BOOL bAccept);
// 回复对方的私聊请求（扩展，可以附带出错代码）
BRAC_API DWORD BRAC_PrivateChatEchoEx(DWORD dwUserid, DWORD dwRequestid,DWORD dwErrorCode);
// 退出与某用户的私聊，或者将某用户从自己的私聊列表中清除
BRAC_API DWORD BRAC_PrivateChatExit(DWORD dwUserid);



#endif //BR_ANYCHAT_CORE_SDK_H__INCLUDED_
