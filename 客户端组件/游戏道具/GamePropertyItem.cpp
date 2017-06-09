#include "StdAfx.h"
#include "DlgBuyProperty.h"
#include "GamePropertyItem.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyItem::CGamePropertyItem()
{
	//设置变量
	ZeroMemory(&m_PropertyInfo,sizeof(m_PropertyInfo));
	ZeroMemory(&m_PropertyAttrib,sizeof(m_PropertyAttrib));

	return;
}

//析构函数
CGamePropertyItem::~CGamePropertyItem()
{
}

//提示信息
LPCTSTR CGamePropertyItem::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	return szResultMessage;
}

//使用道具
VOID CGamePropertyItem::ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink)
{
	//购买窗口
	CDlgBuyProperty::ShowBuyPropertyWnd(this,pIGamePropertySink);

	return;
}

//改变事件
VOID CGamePropertyItem::OnEventPerformCommand(WORD wCommandID,VOID * pCmdData)
{
	//购买窗口
	CDlgBuyProperty::OnEventPerformCommand(wCommandID,pCmdData);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyCar::CGamePropertyCar()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_CAR;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("汽车礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有汽车刹车的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyCar::~CGamePropertyCar()
{
}

//图片信息
VOID CGamePropertyCar::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("汽车.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_CAR");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_CAR");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyCar::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 送给 [ %s ] %d 辆豪华跑车，好拉风啊！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyEgg::CGamePropertyEgg()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_EGG;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("臭蛋礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有仍臭蛋的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyEgg::~CGamePropertyEgg()
{
}

//图片信息
VOID CGamePropertyEgg::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("臭蛋.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_EGG");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_EGG");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyEgg::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR  szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 丢给 [ %s ] %d 颗臭鸡蛋，臭名远扬了！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyClap::CGamePropertyClap()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_CLAP;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("鼓掌礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在使用者头像附近，有拍手动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyClap::~CGamePropertyClap()
{
}

//图片信息
VOID CGamePropertyClap::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("鼓掌.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_CLAP");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_CLAP");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyClap::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR   szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 为 [ %s ]热烈鼓掌，你是最棒的！"),szSourceNickName,szTargerNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyKiss::CGamePropertyKiss()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_KISS;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("香吻礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有吻的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyKiss::~CGamePropertyKiss()
{
}

//图片信息
VOID CGamePropertyKiss::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("香吻.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_KISS");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_KISS");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyKiss::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 献给 [ %s ] %d 个香吻，好害羞呀！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyBeer::CGamePropertyBeer()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_BEER;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("啤酒礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有喝啤酒的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyBeer::~CGamePropertyBeer()
{
}

//图片信息
VOID CGamePropertyBeer::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("啤酒.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_BEER");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_BEER");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyBeer::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR  szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 与 [ %s ]干杯，一醉方休！"),szSourceNickName,szTargerNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyCake::CGamePropertyCake()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_CAKE;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("蛋糕礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有手捧蛋糕献上的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyCake::~CGamePropertyCake()
{
}

//图片信息
VOID CGamePropertyCake::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("蛋糕.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_CAKE");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_CAKE");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyCake::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR  szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 送给 [ %s ] %d 块蛋糕，友谊天长地久！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyRing::CGamePropertyRing()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_RING;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("钻戒礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有钻戒闪耀的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyRing::~CGamePropertyRing()
{
}

//图片信息
VOID CGamePropertyRing::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("钻戒.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_RING");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_RING");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyRing::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR  szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 送给 [ %s ] %d 玫钻戒，幸福甜蜜的人儿！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyBeat::CGamePropertyBeat()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_BEAT;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("暴打礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有一人物被暴打的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyBeat::~CGamePropertyBeat()
{
}

//图片信息
VOID CGamePropertyBeat::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("暴打.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_BEAT");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_BEAT");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyBeat::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 把 [ %s ]暴打了一顿，这个社会不太和谐！"),szSourceNickName,szTargerNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyBomb::CGamePropertyBomb()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_BOMB;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("炸弹礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有炸弹爆炸的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyBomb::~CGamePropertyBomb()
{
}

//图片信息
VOID CGamePropertyBomb::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("炸弹.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_BOMB");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_BOMB");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyBomb::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR  szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 丢了 %d 颗炸弹给 [%s]，可恶的家伙！"),szSourceNickName,wItemCount,szTargerNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertySmoke::CGamePropertySmoke()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_SMOKE;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("香烟礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有抽烟的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertySmoke::~CGamePropertySmoke()
{
}

//图片信息
VOID CGamePropertySmoke::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("香烟.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_SMOKE");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_SMOKE");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertySmoke::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 递给 [ %s ] %d 支香烟，大家趣味相投！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyVilla::CGamePropertyVilla()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_VILLA;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("别墅礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有房子出现的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyVilla::~CGamePropertyVilla()
{
}

//图片信息
VOID CGamePropertyVilla::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("别墅.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_VILLA");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_VILLA");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyVilla::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 送给 [ %s ] %d 栋别墅，霸气外露！"),szSourceNickName,szTargerNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyBrick::CGamePropertyBrick()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_BRICK;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("砖头礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有仍砖头的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyBrick::~CGamePropertyBrick()
{
}

//图片信息
VOID CGamePropertyBrick::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("砖头.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_BRICK");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_BRICK");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyBrick::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 拍了 [ %s ] %d 砖头，[%s] 你晕了吗？"),szSourceNickName,szTargerNickName,wItemCount,szTargerNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyFlower::CGamePropertyFlower()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_FLOWER;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PRESENT;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("鲜花礼物"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("出现在获赠者头像附近，有手持花的动画声效"),CountArray(m_PropertyAttrib.szRegulationsInfo));
	
	return;
}

//析构函数
CGamePropertyFlower::~CGamePropertyFlower()
{
}

//图片信息
VOID CGamePropertyFlower::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("鲜花.swf");
	PropertyImage.pszImageID=TEXT("IMAGE_FLOWER");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_FLOWER");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyFlower::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] 献给 [ %s ] %d 束鲜花，[%s]脸蛋儿红通通！"),szSourceNickName,szTargerNickName,wItemCount,szTargerNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyTwoScoreCard::CGamePropertyTwoScoreCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_TWO_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("小时"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("双倍积分卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("拥有双倍积分卡，游戏赢分将获得双倍积分，输分不多扣！（此道具卡只能在积分房间购买，且仅在购买的游戏房间生效。）"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyTwoScoreCard::~CGamePropertyTwoScoreCard()
{
}

//图片信息
VOID CGamePropertyTwoScoreCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_TWO_SCORE_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_TWO_SCORE_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyTwoScoreCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，双倍积分卡即时生效，%d小时内您游戏赢分将获得双倍积分，输分不多扣。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您双倍积分卡道具，即时生效。%d小时内您游戏赢分将获得双倍积分，\n输分不多扣。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyFourScoreCard::CGamePropertyFourScoreCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_FOUR_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER|PT_SERVICE_AREA_LOOKON;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("小时"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("四倍积分卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("拥有四倍积分卡，游戏赢分将获得四倍积分，输分不多扣！（此道具卡只能在积分房间购买，且仅在购买的游戏房间生效。）"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyFourScoreCard::~CGamePropertyFourScoreCard()
{
}

//图片信息
VOID CGamePropertyFourScoreCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_FOUR_SCORE_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_FOUR_SCORE_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyFourScoreCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，四倍积分卡即时生效，%d小时内您游戏赢分将获得四倍积分，输分不多扣。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您四倍积分卡道具，即时生效。%d小时内您游戏赢分将获得四倍积分，\n输分不多扣。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyScoreClear::CGamePropertyScoreClear()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_SCORE_CLEAR;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("张"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("负分清零卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("使用该道具后，您的游戏积分将恢复至初始状态，不再为负分而烦恼！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyScoreClear::~CGamePropertyScoreClear()
{
}

//图片信息
VOID CGamePropertyScoreClear::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_SCORE_CLEAR");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_SCORE_CLEAR");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyScoreClear::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName,szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，负分清零即时生效，您的游戏积分已经恢复初始状态。"),szSourceNickName,wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您负分清零道具，即时生效。您的游戏积分已经恢复初始状态。 "),szSourceNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyEscapeClear::CGamePropertyEscapeClear()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_ESCAPE_CLEAR;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("张"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("逃跑率清零卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("使用该道具后，您的逃跑率将恢复至初始状态，不再为逃跑而烦恼！（仅限于在购买本款游戏房间生效）"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyEscapeClear::~CGamePropertyEscapeClear()
{
}

//图片信息
VOID CGamePropertyEscapeClear::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_ESCAPE_CLEAR");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_ESCAPE_CLEAR");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyEscapeClear::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName,szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，逃跑清零即时生效，您的逃跑率已经恢复初始状态。"),szSourceNickName,wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您逃跑清零道具，即时生效。您的逃跑率已经恢复初始状态。 "),szSourceNickName);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyTrumpet::CGamePropertyTrumpet()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_TRUMPET;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("小喇叭"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("您发送的消息将在房间及房间内的所有游戏里的聊天框内以醒目的方式显示！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyTrumpet::~CGamePropertyTrumpet()
{
}

//图片信息
VOID CGamePropertyTrumpet::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_TRUMPET");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_TRUMPET");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyTrumpet::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您，[ %s ]玩家了赠送了您%d个小喇叭道具，即送即用。该道具作用您发送的消息将在房间内的聊天框内以醒目的方式显示。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyTyphon::CGamePropertyTyphon()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_TYPHON;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("个"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("大喇叭"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("您发送的消息将在大厅和全服的所有游戏里的聊天框内以醒目的方式显示！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyTyphon::~CGamePropertyTyphon()
{
}

//图片信息
VOID CGamePropertyTyphon::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_TYPHON");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_TYPHON");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyTyphon::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您，[ %s ]玩家了赠送了您%d个大喇叭道具，即送即用。该道具作用您发送的消息将在大厅和全服的所有游戏里以醒目的方式显示。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyGuardKickCard::CGamePropertyGuardKickCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_GUARDKICK_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("小时"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("防踢卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("购买该道具后，您不用担心被会员踢出游戏桌。"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyGuardKickCard::~CGamePropertyGuardKickCard()
{
}

//图片信息
VOID CGamePropertyGuardKickCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_GUARDKICK_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_GUARDKICK_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyGuardKickCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，防踢卡即时生效，%d小时内您不用担心被会员踢出游戏桌。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT(" 恭喜您！[ %s ]玩家赠送了您防踢卡道具，即时生效。%d小时内，您不用担心被会员踢出游戏桌。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyPossess::CGamePropertyPossess()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_POSSESS;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("小时"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("护身符"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("购买该道具后,您游戏输分不扣分,赢分正常得分,不再为扣分而烦恼! (此道具卡只能在积分房间购买,且仅在购买的游戏房间生效. )"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyPossess::~CGamePropertyPossess()
{
}

//图片信息
VOID CGamePropertyPossess::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_POSSESS");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_POSSESS");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyPossess::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，护身符即时生效，%d小时内您游戏输分不扣分，赢分正常得分。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您护身符道具，即时生效。%d小时内您游戏输分不扣分，赢分正常得分。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}


//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyBlueRingCard::CGamePropertyBlueRingCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_BLUERING_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("月"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("蓝钻会员卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("会员购买道具礼物时可享受折扣优惠，可踢普通玩家、蓝钻会员，有黄钻会员特殊标志，会员期间，名字ID在用户列表中以红色字体突出显示，并可优先进入满员房间！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyBlueRingCard::~CGamePropertyBlueRingCard()
{
}

//图片信息
VOID CGamePropertyBlueRingCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_BLUERING_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_BLUERING_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyBlueRingCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，蓝钻会员卡即时生效，%d月内您可以享受蓝钻会员的尊贵权限。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您蓝钻会员卡道具，即时生效。%d月内您可以享受蓝钻会员的尊贵权限。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyYellowRingCard::CGamePropertyYellowRingCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_YELLOWRING_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("月"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("黄钻会员卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("会员购买道具礼物时可享受折扣优惠，可踢普通玩家、蓝钻会员，有黄钻会员特殊标志，会员期间，名字ID在用户列表中以红色字体突出显示，并可优先进入满员房间！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyYellowRingCard::~CGamePropertyYellowRingCard()
{
}

//图片信息
VOID CGamePropertyYellowRingCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_YELLOWRING_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_YELLOWRING_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyYellowRingCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，黄钻会员卡即时生效，%d月内您可以享受黄钻会员的尊贵权限。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您黄钻会员卡道具，即时生效。%d月内您可以享受黄钻会员的尊贵权限。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyWhiteRingCard::CGamePropertyWhiteRingCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_WHITERING_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("月"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("白钻会员卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("会员购买道具礼物时可享受折扣优惠，可踢普通玩家、蓝钻会员和黄钻会员，有白钻会员特殊标志，会员期间，名字ID在用户列表中以红色字体突出显示，并可优先进入满员房间！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyWhiteRingCard::~CGamePropertyWhiteRingCard()
{
}

//图片信息
VOID CGamePropertyWhiteRingCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_WHITERING_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_WHITERING_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyWhiteRingCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，白钻会员卡即时生效，%d月内您可以享受白钻会员的尊贵权限。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您白钻会员卡道具，即时生效。%d月内您可以享受白钻会员的尊贵权限。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyRedRingCard::CGamePropertyRedRingCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_REDRING_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("月"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("红钻会员卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("会员购买道具礼物时可享受折扣优惠，可踢普通玩家、蓝钻会员、黄钻会员和白钻会员，有红钻会员特殊标志，会员期间，名字ID在用户列表中以红色字体突出显示，并可优先进入满员房间！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyRedRingCard::~CGamePropertyRedRingCard()
{
}

//图片信息
VOID CGamePropertyRedRingCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_REDRING_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_REDRING_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyRedRingCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，红钻会员卡即时生效，%d月内您可以享受红钻会员的尊贵权限。"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您红钻会员卡道具，即时生效。%d月内您可以享受红钻会员的尊贵权限。"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyVipRoomCard::CGamePropertyVipRoomCard()
{
	//设置变量
	m_PropertyAttrib.wIndex=PROPERTY_ID_VIPROOM_CARD;
	m_PropertyAttrib.wPropertyType=PT_TYPE_PROPERTY;
	m_PropertyAttrib.wServiceArea=PT_SERVICE_AREA_MESELF|PT_SERVICE_AREA_PLAYER;
	lstrcpyn(m_PropertyAttrib.szMeasuringunit,TEXT("月"),CountArray(m_PropertyAttrib.szMeasuringunit));
	lstrcpyn(m_PropertyAttrib.szPropertyName,TEXT("VIP会员卡"),CountArray(m_PropertyAttrib.szPropertyName));
	lstrcpyn(m_PropertyAttrib.szRegulationsInfo,TEXT("购买该道具的用户，可以进入特定的VIP房间游戏，购买道具礼物时可享受折扣优惠，有特殊的VIP特色标志，会员期间，名字ID在用户列表中以红色字体突出显示，并可优先进入满员房间！"),CountArray(m_PropertyAttrib.szRegulationsInfo));
}

//析构函数
CGamePropertyVipRoomCard::~CGamePropertyVipRoomCard()
{
}

//图片信息
VOID CGamePropertyVipRoomCard::GetImageInfo(tagPropertyImage & PropertyImage)
{
	//设置变量
	PropertyImage.pszFlash=TEXT("");
	PropertyImage.pszImageID=TEXT("IMAGE_VIPROOM_CARD");
	PropertyImage.pszSymbolID=TEXT("SYMBOL_VIPROOM_CARD");
	PropertyImage.hResInstance=GetModuleHandle(GAME_PROPERTY_DLL_NAME);

	return;
}

//提示信息
LPCTSTR CGamePropertyVipRoomCard::GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage)
{
	//定义变量
	TCHAR szMessage[128]=TEXT("");
	if(szSourceNickName==szTargerNickName)
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！道具购买成功，VIP会员卡即时生效，%d月内您可以享受VIP会员的尊贵权限！"),wItemCount);
	else
		_sntprintf(szMessage,CountArray(szMessage),TEXT("恭喜您！[ %s ]玩家赠送了您VIP会员卡道具，即时生效。%d月内您可以享受VIP会员的尊贵权限！"),szSourceNickName,wItemCount);

	//拷贝数据
	CopyMemory(szResultMessage,szMessage,sizeof(szMessage));

	return szResultMessage;
}

//////////////////////////////////////////////////////////////////////////////////