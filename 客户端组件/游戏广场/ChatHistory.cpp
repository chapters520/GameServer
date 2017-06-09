#include "Stdafx.h"
#include "ChatHistory.h"

//////////////////////////////////////////////////////////////////////////
//静态变量

CChatMsgItemArray CChatHistory::m_ChatMsgItemStorge;					//消息库存
CChatHistoryArray CChatHistoryManager::m_ChatHistoryStorge;				//记录库存

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CChatMsgItem, CObject, VERSIONABLE_SCHEMA|1 )
IMPLEMENT_SERIAL( CChatHistory, CObject, VERSIONABLE_SCHEMA|1 )

//////////////////////////////////////////////////////////////////////////
//消息类

//构造函数
CChatMsgItem::CChatMsgItem()
{
	ResetChatMsg();
}

//析构函数
CChatMsgItem::~CChatMsgItem()
{

}

//序列化
void CChatMsgItem::Serialize( CArchive& ar )
{
	__super::Serialize(ar);

	//写入
	if( ar.IsStoring() )
	{
		ar << m_strSendUser;
		ar << m_strMsg;
		ar << m_crChatColor;
		ar << m_bSelfMsg;
		ar << m_Time.wYear;
		ar << m_Time.wMonth;
		ar << m_Time.wDay;
		ar << m_Time.wHour;
		ar << m_Time.wMinute;
		ar << m_Time.wSecond;
		ar << m_Time.wMilliseconds;
		ar << m_Time.wDayOfWeek;
	}
	//读取
	else
	{
		ar >> m_strSendUser;
		ar >> m_strMsg;
		ar >> m_crChatColor;
		ar >> m_bSelfMsg;
		ar >> m_Time.wYear;
		ar >> m_Time.wMonth;
		ar >> m_Time.wDay;
		ar >> m_Time.wHour;
		ar >> m_Time.wMinute;
		ar >> m_Time.wSecond;
		ar >> m_Time.wMilliseconds;
		ar >> m_Time.wDayOfWeek;
	}
}

//接口查询
VOID * CChatMsgItem::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IChatMsgItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IChatMsgItem,Guid,dwQueryVer);
	return NULL;
}

//重置项目
VOID __cdecl CChatMsgItem::ResetChatMsg()
{
	//重置变量
	m_strSendUser = TEXT("");
	m_strMsg = TEXT("");
	m_crChatColor = RGB(0,0,0);
	m_bSelfMsg = FALSE;
	GetLocalTime(&m_Time);
}

//////////////////////////////////////////////////////////////////////////
//消息记录类

//构造函数
CChatHistory::CChatHistory()
{
	ResetChatHistory();
}

//析构函数
CChatHistory::~CChatHistory()
{
	//删除数组
	m_ChatMsgItemStorge.Append(m_ChatMsgItemActive);
	m_ChatMsgItemActive.RemoveAll();
}

//序列化
void CChatHistory::Serialize( CArchive& ar )
{
	__super::Serialize(ar);

	//写入
	if( ar.IsStoring() )
	{
		//消息数目
		ar << (INT)m_ChatMsgItemActive.GetCount();

		for( INT_PTR i = 0; i < m_ChatMsgItemActive.GetCount(); i++ )
		{
			CChatMsgItem *pChatMsgItem = m_ChatMsgItemActive[i];

			pChatMsgItem->Serialize(ar);
		}
	}
	//读取
	else
	{
		//消息数目
		INT nItemCount = 0;
		ar >> nItemCount;

		for( INT i = 0; i < nItemCount; i++ )
		{
			CChatMsgItem *pChatMsgItem = ActiveChatMsgItem();
			ASSERT(pChatMsgItem);
			if( !pChatMsgItem ) break;

			pChatMsgItem->Serialize(ar);
		}
	}
}

//释放对象
VOID CChatHistory::Release()
{
	//关闭记录
	ASSERT(m_pIChatHistoryManager);
	if( m_pIChatHistoryManager )
		m_pIChatHistoryManager->CloseChatHistory(GetChatID());
}

//接口查询
VOID * CChatHistory::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IChatHistory,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IChatHistory,Guid,dwQueryVer);
	return NULL;
}

//重置记录
VOID __cdecl CChatHistory::ResetChatHistory()
{
	//设置变量
	m_dwChatID = 0;
	m_pIChatHistoryManager = NULL;

	//删除数组
	m_ChatMsgItemStorge.Append(m_ChatMsgItemActive);
	m_ChatMsgItemActive.RemoveAll();
}

//枚举消息
IChatMsgItem* __cdecl CChatHistory::EnumChatMsgItem(INT_PTR nEnumIndex)
{
	if( nEnumIndex < 0 || nEnumIndex >= m_ChatMsgItemActive.GetCount() )
		return NULL;

	return m_ChatMsgItemActive[nEnumIndex];
}

//添加消息
bool __cdecl CChatHistory::AddMsgItem(LPCTSTR lpszSendUser,LPCTSTR lpszMsg,COLORREF crColor,bool bMySelfString)
{
	//激活项目
	CChatMsgItem *pChatMsgItem = ActiveChatMsgItem();
	ASSERT(pChatMsgItem);
	if( !pChatMsgItem ) return false;

	//设置变量
	GetLocalTime(&pChatMsgItem->m_Time);
	pChatMsgItem->m_strSendUser = lpszSendUser;
	pChatMsgItem->m_strMsg = lpszMsg;
	pChatMsgItem->m_crChatColor = crColor;
	pChatMsgItem->m_bSelfMsg = (bMySelfString?TRUE:FALSE);

	return true;
}

//清除记录
bool __cdecl CChatHistory::ClearHistory()
{
	//删除数组
	m_ChatMsgItemStorge.Append(m_ChatMsgItemActive);
	m_ChatMsgItemActive.RemoveAll();

	return true;
}

//保存记录
bool __cdecl CChatHistory::Save()
{
	ASSERT(m_pIChatHistoryManager);
	if( !m_pIChatHistoryManager ) return false;

	return m_pIChatHistoryManager->SaveChatHistory(GetChatID());
}

//激活消息项
CChatMsgItem* CChatHistory::ActiveChatMsgItem()
{
	//变量定义
	CChatMsgItem *pChatMsgItem = NULL;

	//查找
	if( m_ChatMsgItemStorge.GetCount() > 0 )
	{
		INT_PTR nIndex = m_ChatMsgItemStorge.GetCount()-1;

		pChatMsgItem = m_ChatMsgItemStorge[nIndex];
		m_ChatMsgItemStorge.RemoveAt(nIndex);
		m_ChatMsgItemActive.Add(pChatMsgItem);

		pChatMsgItem->ResetChatMsg();
		return pChatMsgItem;
	}

	//创建
	try{ pChatMsgItem = new CChatMsgItem; }
	catch(...) { ASSERT(FALSE); }
	if( !pChatMsgItem ) return NULL;

	m_ChatMsgItemActive.Add(pChatMsgItem);

	return pChatMsgItem;
}

//////////////////////////////////////////////////////////////////////////
//消息管理类

//构造函数
CChatHistoryManager::CChatHistoryManager()
{
	//初始化
	m_pIMyselfUserItem = NULL;

	m_MapChatHistory.InitHashTable(11);
}

//析构函数
CChatHistoryManager::~CChatHistoryManager()
{
	//删除记录
	m_ChatHistoryStorge.Append(m_ChatHistoryActive);
	m_ChatHistoryActive.RemoveAll();

	//删除映射
	m_MapChatHistory.RemoveAll();
}

//接口查询
VOID * CChatHistoryManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IChatHistoryManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IChatHistoryManager,Guid,dwQueryVer);
	return NULL;
}

//初始化
VOID CChatHistoryManager::SetMyselftUserItem(IClientUserItem *pIMyselfUserItem)
{
	m_pIMyselfUserItem = pIMyselfUserItem;
}

//激活记录
IChatHistory* __cdecl CChatHistoryManager::ActiveChatHistory(DWORD dwChatID)
{
	//效验
	ASSERT(m_pIMyselfUserItem);
	if( !m_pIMyselfUserItem ) return NULL;

	//变量定义
	CChatHistory *pChatHistory = NULL;

	//搜索现有
	if( m_MapChatHistory.Lookup(dwChatID,pChatHistory) )
		return pChatHistory;
	
	//查找库存
	if( m_ChatHistoryStorge.GetCount() > 0 )
	{
		INT_PTR nIndex = m_ChatHistoryStorge.GetCount()-1;

		pChatHistory = m_ChatHistoryStorge[nIndex];
		m_ChatHistoryStorge.RemoveAt(nIndex);
	}
	else
	{
		//创建
		try{ pChatHistory = new CChatHistory; }
		catch(...) { ASSERT(FALSE); }
		if( !pChatHistory ) return NULL;
	}

	//设置变量
	pChatHistory->m_dwChatID = dwChatID;
	pChatHistory->m_pIChatHistoryManager = this;

	//读取路径
	TCHAR szWorkDir[MAX_PATH] = {0}, szPath[MAX_PATH] = {0};
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));
	_sntprintf( szPath,CountArray(szPath),TEXT("%s\\Users\\%ld\\%ld.whc"),szWorkDir,
		m_pIMyselfUserItem->GetGameID(),dwChatID );

	//加载文件
	try
	{
		CFile file;
		if( file.Open(szPath,CFile::modeRead) )
		{
			//读取记录
			CArchive ar(&file, CArchive::load);

			pChatHistory->Serialize(ar);

			//关闭
			ar.Close();
		}
	}catch(...)
	{
		ASSERT(FALSE);
		pChatHistory->ResetChatHistory();
		m_ChatHistoryStorge.Add(pChatHistory);
		return NULL;
	}

	//设置映射
	m_MapChatHistory[dwChatID] = pChatHistory;

	//添加
	m_ChatHistoryActive.Add(pChatHistory);
	
	return pChatHistory;
}

//关闭记录
bool __cdecl CChatHistoryManager::CloseChatHistory(DWORD dwChatID)
{
	//变量定义
	CChatHistory *pChatHistory = NULL;

	//查找
	m_MapChatHistory.Lookup(dwChatID,pChatHistory);
	ASSERT(pChatHistory);
	if( !pChatHistory) return false;

	m_MapChatHistory.RemoveKey(dwChatID);

	//删除
	for( INT_PTR i = 0; i < m_ChatHistoryActive.GetCount(); i++ )
	{
		CChatHistory *pTmp = m_ChatHistoryActive[i];
		if( pTmp == pChatHistory )
		{
			m_ChatHistoryActive.RemoveAt(i);
			pChatHistory->ResetChatHistory();
			break;
		}
	}

	//添加到库存
	m_ChatHistoryStorge.Add(pChatHistory);

	return true;
}

//保存记录
bool __cdecl CChatHistoryManager::SaveChatHistory(DWORD dwChatID)
{
	//效验
	ASSERT(m_pIMyselfUserItem);
	if( !m_pIMyselfUserItem ) return false;

	//变量定义
	CChatHistory *pChatHistory = NULL;

	//查找
	m_MapChatHistory.Lookup(dwChatID,pChatHistory);
	ASSERT(pChatHistory);
	if( !pChatHistory ) return false;

	//读取路径
	TCHAR szWorkDir[MAX_PATH] = {0}, szPath[MAX_PATH] = {0};
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));
	_sntprintf( szPath,CountArray(szPath),TEXT("%s\\Users\\%ld\\%ld.whc"),szWorkDir,
		m_pIMyselfUserItem->GetGameID(),dwChatID );
	
	//加载文件
	try
	{
		CFile file;
		CFileException ex;
		if( !file.Open(szPath,CFile::modeWrite|CFile::modeCreate,&ex) )
		{
			//文件夹未创建
			if( ex.m_cause == ERROR_PATH_NOT_FOUND )
			{
				TCHAR szDir[MAX_PATH] = {0};
				_sntprintf(szDir,CountArray(szDir),TEXT("%s\\Users"),szWorkDir);
				if( CreateDirectory(szDir,NULL) || GetLastError() == ERROR_ALREADY_EXISTS )
				{
					_sntprintf(szDir,CountArray(szDir),TEXT("%s\\Users\\%ld"),szWorkDir,
						m_pIMyselfUserItem->GetGameID());
					CreateDirectory(szDir,NULL);
				}

				//重新打开
				file.Open(szPath,CFile::modeWrite|CFile::modeCreate);
			}
		}
		
		if( file.m_hFile != CFile::hFileNull )
		{
			//保存记录
			CArchive ar(&file, CArchive::store);
			
			pChatHistory->Serialize(ar);

			//关闭
			ar.Close();
		}
	}catch(...)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////