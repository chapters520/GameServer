#include "StdAfx.h"
#include "GamePropertyManager.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CGamePropertyManager * CGamePropertyManager::m_pGamePropertyManager=NULL;//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGamePropertyManager::CGamePropertyManager()
{
	//设置变量
	ASSERT(m_pGamePropertyManager==NULL);
	if (m_pGamePropertyManager==NULL) m_pGamePropertyManager=this;

	return;
}

//析构函数
CGamePropertyManager::~CGamePropertyManager()
{
	//设置变量
	ASSERT(m_pGamePropertyManager==this);
	if (m_pGamePropertyManager==this) m_pGamePropertyManager=NULL;

	return;
}

//获取道具
CGamePropertyItem * CGamePropertyManager::GetPropertyItem(WORD wIndex)
{
	//查找道具
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_MapPropertyItem.Lookup(wIndex,pGamePropertyItem);

	return pGamePropertyItem;
}

//创建道具
CGamePropertyItem * CGamePropertyManager::CreatePropertyItem(tagPropertyInfo & PropertyInfo)
{
	//查找现存
	CGamePropertyItem * pGamePropertyItem=NULL;
	m_MapPropertyItem.Lookup(PropertyInfo.wIndex,pGamePropertyItem);

	//现存判断
	if (pGamePropertyItem!=NULL)
	{
		//设置道具
		pGamePropertyItem->m_PropertyInfo=PropertyInfo;

		//喇叭过滤
		if(PropertyInfo.wIndex==PROPERTY_ID_TRUMPET || PropertyInfo.wIndex==PROPERTY_ID_TYPHON)
			return NULL;
		else
			return pGamePropertyItem;
	}

	//创建道具
	switch (PropertyInfo.wIndex)
	{
	case PROPERTY_ID_CAR:			//汽车礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyCar;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_EGG:			//臭蛋礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyEgg;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_CLAP:			//鼓掌礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyClap;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_KISS:			//香吻礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyKiss;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BEER:			//啤酒礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBeer;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_CAKE:			//蛋糕礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyCake;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_RING:			//钻戒礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyRing;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BEAT:			//暴打礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBeat;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BOMB:			//炸弹礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBomb;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_SMOKE:			//香烟礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertySmoke;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_VILLA:			//别墅礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyVilla;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BRICK:			//砖头礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBrick;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_FLOWER:		//鲜花礼物
		{
			try
			{
				pGamePropertyItem=new CGamePropertyFlower;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_TWO_CARD:    //双倍积分
		{
			try
			{
				pGamePropertyItem=new CGamePropertyTwoScoreCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_FOUR_CARD:    //四倍积分
		{
			try
			{
				pGamePropertyItem=new CGamePropertyFourScoreCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_SCORE_CLEAR:    //负分清零
		{
			try
			{
				pGamePropertyItem=new CGamePropertyScoreClear;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_ESCAPE_CLEAR:    //逃跑清零
		{
			try
			{
				pGamePropertyItem=new CGamePropertyEscapeClear;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_TRUMPET:		  //小喇叭
		{
			try
			{
				pGamePropertyItem=new CGamePropertyTrumpet;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_TYPHON:		  //大喇叭
		{
			try
			{
				pGamePropertyItem=new CGamePropertyTyphon;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_GUARDKICK_CARD:   //防踢卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyGuardKickCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_POSSESS:		  //护身符
		{
			try
			{
				pGamePropertyItem=new CGamePropertyPossess;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_BLUERING_CARD:	  //蓝钻会员卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyBlueRingCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_YELLOWRING_CARD:	//黄钻会员卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyYellowRingCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_WHITERING_CARD:	//白钻会员卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyWhiteRingCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_REDRING_CARD:		//红钻会员卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyRedRingCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	case PROPERTY_ID_VIPROOM_CARD:		//VIP房卡
		{
			try
			{
				pGamePropertyItem=new CGamePropertyVipRoomCard;
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

			break;
		}
	}

	//创建判断
	if (pGamePropertyItem!=NULL)
	{
		//设置道具
		pGamePropertyItem->m_PropertyInfo=PropertyInfo;
		m_MapPropertyItem[PropertyInfo.wIndex]=pGamePropertyItem;

		//喇叭过滤
		if(PropertyInfo.wIndex==PROPERTY_ID_TRUMPET || PropertyInfo.wIndex==PROPERTY_ID_TYPHON)
			return NULL;
		else
			return pGamePropertyItem;
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
