#pragma once

//////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC ÎÄ¼þ
#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

//////////////////////////////////////////////////////////////////////////////////
//Á´½Ó´úÂë

//¶àÃ½Ìå¿â
#pragma comment (lib,"Winmm.lib")

//Á´½Ó´úÂë
#ifndef _DEBUG
#ifndef _UNICODE
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/WHImage.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/GameEngine.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/ServiceCore.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/SkinControl.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/ShareControl.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/PlatformData.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/GameProperty.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/AvatarControl.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/VideoService.lib")
#else
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/WHImage.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/GameEngine.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/ServiceCore.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/SkinControl.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/ShareControl.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/PlatformData.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/GameProperty.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/AvatarControl.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/VideoService.lib")
#endif
#else
#ifndef _UNICODE
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/WHImageD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/GameEngineD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/ServiceCoreD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/SkinControlD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/ShareControlD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/PlatformDataD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/GamePropertyD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/AvatarControlD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Ansi/VideoServiceD.lib")
#else
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/WHImageD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/GameEngineD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/ServiceCoreD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/SkinControlD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/ShareControlD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/PlatformDataD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/GamePropertyD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/AvatarControlD.lib")
	#pragma comment (lib,"../../Á´½Ó¿â/Unicode/VideoServiceD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
