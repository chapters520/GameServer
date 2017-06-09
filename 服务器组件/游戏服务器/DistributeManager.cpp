#include "StdAfx.h"
#include "DistributeManager.h"

//////////////////////////////////////////////////////////////////////////////////

//内存管理
tagDistributeNode * CDistributeNodePool::m_pHeadOfFreeList=NULL;
const int CDistributeNodePool::BLOCK_SIZE = 20;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDistributeNodePool::CDistributeNodePool()
{
}

//析构函数
CDistributeNodePool::~CDistributeNodePool()
{
	if(m_pHeadOfFreeList!=NULL)
	{
		tagDistributeNode * pDistributeNode=m_pHeadOfFreeList;
		while(pDistributeNode!=NULL)
		{
			//安全释放
			m_pHeadOfFreeList = pDistributeNode->pNextDistributeNode;
			SafeDelete(pDistributeNode);
			pDistributeNode=m_pHeadOfFreeList;
		}
	}
}

//分配结点
tagDistributeNode * CDistributeNodePool::AllocNode()
{
	//获取头结点
	tagDistributeNode *pDistributeNode = m_pHeadOfFreeList; 
	if(pDistributeNode!=NULL)
		m_pHeadOfFreeList = pDistributeNode->pNextDistributeNode;
	else 
	{
		//分配大块内存
		for(int nIndex=0;nIndex<BLOCK_SIZE;nIndex++)
		{
			tagDistributeNode * pNewBlock = new tagDistributeNode;
			pNewBlock->pNextDistributeNode=m_pHeadOfFreeList;
			m_pHeadOfFreeList=pNewBlock;
		}
		
		//设置结点
		pDistributeNode = m_pHeadOfFreeList;
		m_pHeadOfFreeList=pDistributeNode->pNextDistributeNode;
	}

	return pDistributeNode;
}

//释放结点
VOID CDistributeNodePool::FreeNode(void * pNode)
{
	//归还结点
	tagDistributeNode * pDeadNode =  static_cast<tagDistributeNode*>(pNode);
	pDeadNode->pNextDistributeNode = m_pHeadOfFreeList;
	m_pHeadOfFreeList = pDeadNode;
	pNode=NULL;

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDistributeManager::CDistributeManager()
{
	//设置变量
	m_pHeadNode=NULL;
	m_wNodeCount=0;
	m_wAndroidCount=0;
	m_wRealCount=0;
	m_cbDistributeRule=0;
}

//析构函数
CDistributeManager::~CDistributeManager()
{
	RemoveAll();
}


//插入结点
BOOL CDistributeManager::InsertDistributeNode(const tagDistributeInfo & DistributeInfo)
{
	//查找用户
	if(SearchNode(DistributeInfo.pIServerUserItem)!=NULL) return false;

	//头部判断
	if(m_pHeadNode==NULL)
	{
		//分配结点
		m_pHeadNode = m_DistributeNodePool.AllocNode();
		ASSERT(m_pHeadNode!=NULL);
		if(m_pHeadNode==NULL) return false;

		//设置变量
		m_pHeadNode->pNextDistributeNode=NULL;
		m_pHeadNode->pNextSameTableNode=NULL;
		m_pHeadNode->pPrepDistributeNode=NULL;
		CopyMemory(&m_pHeadNode->DistributeInfo,&DistributeInfo,sizeof(DistributeInfo));
		m_pHeadNode->DistributeInfo.pPertainNode=m_pHeadNode;
	}
	else
	{
		//分配结点
		tagDistributeNode * pDistributeNode = m_DistributeNodePool.AllocNode();
		ASSERT(pDistributeNode!=NULL);
		if(pDistributeNode==NULL) return false;

		//设置结点
		pDistributeNode->pNextDistributeNode=NULL;
		pDistributeNode->pPrepDistributeNode=NULL;
		pDistributeNode->pNextSameTableNode=NULL;
		CopyMemory(&pDistributeNode->DistributeInfo,&DistributeInfo,sizeof(DistributeInfo));
		pDistributeNode->DistributeInfo.pPertainNode=pDistributeNode;
		
		//同桌限制
		tagDistributeNode  * pMoveNode=NULL;
		if((m_cbDistributeRule&DISTRIBUTE_LAST_TABLE)==0)
		{
			//查找同桌链表头			
			pMoveNode=m_pHeadNode;
			while(pMoveNode!=NULL)
			{
				if(pMoveNode->DistributeInfo.wLastTableID==pDistributeNode->DistributeInfo.wLastTableID &&
					pDistributeNode->DistributeInfo.wLastTableID!=INVALID_TABLE)
					break;
				pMoveNode=pMoveNode->pNextDistributeNode;
			}
		}

		//设置结点
		if(pMoveNode!=NULL)
		{
			//同桌结点
			pDistributeNode->pNextSameTableNode=pMoveNode->pNextSameTableNode;
			pDistributeNode->pPrepDistributeNode=pMoveNode;
			pMoveNode->pNextSameTableNode=pDistributeNode;			
		}
		else 
		{
			//表头结点
			if(m_pHeadNode->pNextDistributeNode!=NULL)
			{
				m_pHeadNode->pNextDistributeNode->pPrepDistributeNode=pDistributeNode;
				pDistributeNode->pNextDistributeNode=m_pHeadNode->pNextDistributeNode;
			}
			
			pDistributeNode->pPrepDistributeNode=m_pHeadNode;
			m_pHeadNode->pNextDistributeNode=pDistributeNode;
		}
	}

	//更新数目
	if(DistributeInfo.pIServerUserItem->IsAndroidUser())
		++m_wAndroidCount;
	else
		++m_wRealCount;
	++m_wNodeCount;

	return true;
}

//移除结点
VOID  CDistributeManager::RemoveDistributeNode(const IServerUserItem  * pIServerUserItem)
{
	//查找结点
	tagDistributeNode  *pDistributeNode=SearchNode(pIServerUserItem);
	if(pDistributeNode!=NULL) 
		RemoveDistributeNode(pDistributeNode);

	return;
}

//移除结点
VOID  CDistributeManager::RemoveDistributeNode(tagDistributeNode * pDistributeNode)
{
	//参数校验
	if(pDistributeNode==NULL) return;

	//变量定义
	tagDistributeNode  *pPrepNode=pDistributeNode->pPrepDistributeNode;
	tagDistributeNode  *pNextNode=pDistributeNode->pNextDistributeNode;
	tagDistributeNode  *pNextSameTableNode=pDistributeNode->pNextSameTableNode;

	//删除结点
	if(pPrepNode!=NULL)
	{
		if(pNextNode!=NULL)
		{
			//同桌链表头结点
			if(pNextSameTableNode!=NULL)
			{
				pPrepNode->pNextDistributeNode=pNextSameTableNode;
				pNextSameTableNode->pPrepDistributeNode=pPrepNode;
				pNextSameTableNode->pNextDistributeNode=pNextNode;
			}
			else
			{
				pPrepNode->pNextDistributeNode=pNextNode;
				pNextNode->pPrepDistributeNode=pPrepNode;
			}
		}
		else
		{
			if(pNextSameTableNode!=NULL)
			{
				//表头链接
				if(pPrepNode->pNextSameTableNode==pDistributeNode)
					pPrepNode->pNextSameTableNode=pNextSameTableNode;

				//同IP链接
				if(pPrepNode->pNextDistributeNode==pDistributeNode)
					pPrepNode->pNextDistributeNode=pNextSameTableNode;

				pNextSameTableNode->pPrepDistributeNode=pPrepNode;
			}
			else
			{
				//表头链接
				if(pPrepNode->pNextDistributeNode==pDistributeNode)
					pPrepNode->pNextDistributeNode=NULL;

				//同IP链接
				if(pPrepNode->pNextSameTableNode==pDistributeNode)
					pPrepNode->pNextSameTableNode=NULL;				
			}
		}
	}
	else
	{
		if(pNextNode!=NULL)
		{
			if(pNextSameTableNode!=NULL)
			{
				//设置结点
				pNextSameTableNode->pPrepDistributeNode=NULL;
				pNextSameTableNode->pNextDistributeNode=pNextNode;

				//重置表头
				m_pHeadNode=pNextSameTableNode;
			}
			else
			{
				pNextNode->pPrepDistributeNode=NULL;

				//重置表头
				m_pHeadNode=pNextNode;
			}
		}
		else
		{
			if(pNextSameTableNode!=NULL)
			{
				//设置结点
				pNextSameTableNode->pPrepDistributeNode=NULL;
				pNextSameTableNode->pNextDistributeNode=NULL;

				//重置表头
				m_pHeadNode=pNextSameTableNode;
			}
			else
				m_pHeadNode=NULL;
		}
	}

    //更新数目
	if(pDistributeNode->DistributeInfo.pIServerUserItem->IsAndroidUser())
		--m_wAndroidCount;
	else
		--m_wRealCount;
	--m_wNodeCount;

	//安全释放
	m_DistributeNodePool.FreeNode(pDistributeNode);
}

//移除结点
VOID CDistributeManager::RemoveAll()
{
	//释放内存
	while(m_pHeadNode!=NULL) RemoveDistributeNode(m_pHeadNode);

	//重置变量
	m_pHeadNode=NULL;
	m_wNodeCount=0;
	m_wAndroidCount=0;
	m_wRealCount=0;

	return;
}

//执行分组
WORD  CDistributeManager::PerformDistribute(CDistributeInfoArray & DistributeInfoArray,WORD wNeedCount)
{
	//定义变量
	tagDistributeNode  * pMoveNode=m_pHeadNode;
	if(pMoveNode!=NULL)
	{
		DistributeInfoArray.Add(pMoveNode->DistributeInfo);
		pMoveNode=pMoveNode->pNextDistributeNode;
	}

	//获取用户
	while(pMoveNode!=NULL)
	{
		//定义变量
		BOOL bFirstSuccess=TRUE;

		//等级过滤
		if(DistributeInfoArray.GetCount()>0 && DistributeInfoArray[0].wDistribute!=pMoveNode->DistributeInfo.wDistribute)
			bFirstSuccess=FALSE;

		//机器过滤
		if(bFirstSuccess==TRUE && DistributeInfoArray.GetCount()==wNeedCount-1 && 
			FilterRuleIsAllAndroid(DistributeInfoArray,pMoveNode->DistributeInfo.pIServerUserItem))
			bFirstSuccess=FALSE;

		//同IP过滤
		if(bFirstSuccess==TRUE && (m_cbDistributeRule&DISTRIBUTE_SAME_ADDRESS)==0 &&  
			FilterRuleExitsIPAddr(DistributeInfoArray,pMoveNode->DistributeInfo.dwClientAddr)==TRUE)
		{
			bFirstSuccess=FALSE;

			//搜索同桌链表
			tagDistributeNode  * pSameTableNode=pMoveNode->pNextSameTableNode;
			while(pSameTableNode!=NULL)
			{
				//定义变量
				BOOL bSecondSuccess=TRUE;

				//等级过滤
				if(DistributeInfoArray.GetCount()>0 && DistributeInfoArray[0].wDistribute!=pMoveNode->DistributeInfo.wDistribute)
					bSecondSuccess=FALSE;

				//机器过滤
				if(bFirstSuccess==TRUE && DistributeInfoArray.GetCount()==wNeedCount-1 && 
					FilterRuleIsAllAndroid(DistributeInfoArray,pSameTableNode->DistributeInfo.pIServerUserItem))
					bFirstSuccess=FALSE;

				//同IP过滤
				if(bSecondSuccess==TRUE && FilterRuleExitsIPAddr(DistributeInfoArray,pMoveNode->DistributeInfo.dwClientAddr)==TRUE)
					bSecondSuccess=FALSE;

				//获取成功
				if(bSecondSuccess==TRUE) 
				{
					DistributeInfoArray.Add(pMoveNode->DistributeInfo);
					break;
				}

				pSameTableNode=pSameTableNode->pNextSameTableNode;
			}
		}

		//获取成功
		if(bFirstSuccess==TRUE) DistributeInfoArray.Add(pMoveNode->DistributeInfo);

		//向前推进
		pMoveNode=pMoveNode->pNextDistributeNode;

		//成功判断
		if(DistributeInfoArray.GetCount()==wNeedCount) break;
	}

	return (WORD)DistributeInfoArray.GetCount();
}

//查找结点
tagDistributeNode * CDistributeManager::SearchNode(const IServerUserItem  * const pIServerUserItem)
{
	if(m_pHeadNode==NULL) return NULL;

	//设置变量
	tagDistributeNode *pMoveNode=m_pHeadNode;
	tagDistributeNode *pNextSameTableNode=NULL;

	//查找结点
	while(pMoveNode!=NULL)
	{
		pNextSameTableNode=pMoveNode;
		while(pNextSameTableNode!=NULL)
		{
			//接口判断
			if(pNextSameTableNode->DistributeInfo.pIServerUserItem==pIServerUserItem)
				return pNextSameTableNode;

			//向前推进
            pNextSameTableNode=pNextSameTableNode->pNextSameTableNode;
		}

		//向前推进
		pMoveNode=pMoveNode->pNextDistributeNode;
	}

	return NULL;
}

//IP同址
BOOL  CDistributeManager::FilterRuleExitsIPAddr(const CDistributeInfoArray & DistributeInfoArray,DWORD dwClientAddr)
{
	//查找同IP
	for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
	{
		if(DistributeInfoArray[nIndex].dwClientAddr==dwClientAddr)
			return TRUE;
	}

	return FALSE;
}

//机器过滤
BOOL CDistributeManager::FilterRuleIsAllAndroid(const CDistributeInfoArray & DistributeInfoArray,IServerUserItem  * const pIServerUserItem)
{
	//参数校验
	if(pIServerUserItem==NULL || DistributeInfoArray.GetCount()==0) return FALSE;

	//变量定义
	WORD wAndroidCount=0;

	//统计机器
	for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
	{
		if(DistributeInfoArray[nIndex].pIServerUserItem->IsAndroidUser()==true)
			++wAndroidCount;
	}

	return (wAndroidCount==DistributeInfoArray.GetCount()) && pIServerUserItem->IsAndroidUser();
}

//////////////////////////////////////////////////////////////////////////////////
