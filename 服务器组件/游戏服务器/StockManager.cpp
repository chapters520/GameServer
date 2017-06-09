#include "StdAfx.h"
#include "StockManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CStockManager::CStockManager()
{
	//设置变量
	m_lStockScore=0L;
	m_dDecreaseRate=1.0;

	return;
}

//析构函数
CStockManager::~CStockManager()
{
}

//设置库存
SCORE CStockManager::SetStockScore(SCORE lStockScore)
{
	//设置变量
	m_lStockScore=lStockScore;

	return m_lStockScore;
}

//设置比率
DOUBLE CStockManager::SetDecreaseRate(DOUBLE dDecreaseRate)
{
	//设置变量
	m_dDecreaseRate=dDecreaseRate;

	return m_dDecreaseRate;
}

//修改库存
VOID CStockManager::ModifyStockScore(SCORE lVariationScore)
{
	//效验参数
	m_lStockScore-=lVariationScore;

	//设置减少
	if (m_lStockScore>0L)
	{
		m_lStockScore=(SCORE)(m_lStockScore*m_dDecreaseRate);
	}

	return;
}

//设置存款
VOID CStockManager::InitializationStock(SCORE lStockScore, DOUBLE dDecreaseRate)
{
	//设置变量
	m_lStockScore=lStockScore;
	m_dDecreaseRate=dDecreaseRate;

	return;
}

//////////////////////////////////////////////////////////////////////////////////
