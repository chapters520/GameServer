#ifndef STOCK_MANAGER_HEAD_FILE
#define STOCK_MANAGER_HEAD_FILE

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//库存管理
class CStockManager
{
	//变量定义
protected:
	SCORE							m_lStockScore;						//库存数目
	DOUBLE							m_dDecreaseRate;					//减少比率

	//函数定义
public:
	//构造函数
	CStockManager();
	//析构函数
	virtual ~CStockManager();

	//查询函数
public:
	//获取库存
	SCORE GetStockScore() { return m_lStockScore; }
	//减少比率
	DOUBLE GetDecreaseRate() { return m_dDecreaseRate; }

	//设置函数
public:
	//设置库存
	SCORE SetStockScore(SCORE lStockScore);
	//设置比率
	DOUBLE SetDecreaseRate(DOUBLE dDecreaseRate);

	//功能函数
public:
	//修改库存
	VOID ModifyStockScore(SCORE lVariationScore);
	//设置存款
	VOID InitializationStock(SCORE lStockScore, DOUBLE dDecreaseRate);
};

//////////////////////////////////////////////////////////////////////////////////

#endif