#include "StdAfx.h"
#include "..\..\服务器组件\游戏服务\GameServiceHead.h"
#include ".\sensitivewordsfilter.h"
#include <locale>


int CToken::isChinese(character *_char)
{        
	if((unsigned short)_char->val_word.high > 127        
		&& (unsigned short)_char->val_word.low > 127)        
		return CH;        
	return EN;        
}        

int CToken::isChinese(LPCTSTR buf,int pos,int size)        
{        
	if(pos+1 > size)        
		return NULL;        
	//  	if((unsigned int)buf[pos]>=MAX_CH)
	//  		return NULL;
	if((unsigned int)buf[pos] > 127 && (unsigned int)buf[pos+1] > 127)        
		return CH;  
	if (buf[pos]<0 && (unsigned int)buf[pos+1] < 127)
		return CH;
	return EN;        
}        

//////////////////////////////////////////////////////////////////////////
CToken *tokenArray::insertToken(unsigned short code,character &_char)        
{        
	std::vector<CToken*>::iterator it = alltokens.begin();        
	std::vector<CToken*>::iterator end = alltokens.end();        
	for( ; it != end; ++it)        
		if( (*it)->getCode() == code)        
			break;        
	if(it != alltokens.end())        
		return *it;        
	else        
	{        
		CToken *tok = new CToken(code,_char);        
		alltokens.push_back(tok);
		nNewCount++;
		return tok;        
	}   

	//实际应用中，插入后要对数组排序    
}        

CToken *tokenArray::getToken(unsigned short code)        
{      
	//实际应用中使用二分查找   
	std::vector<CToken*>::iterator it = alltokens.begin();        
	std::vector<CToken*>::iterator end = alltokens.end();        
	for( ; it != end; ++it)        
		if( (*it)->getCode() == code)        
			return *it;        
	return NULL;        
}        

tokenArray* CToken::getChild(character &_char)        
{        
	if(isChinese(&_char))        
		return &word_children;        
	else        
		return &byte_children;        
}           

CToken* CToken::addChild(unsigned short code,character &_char)        
{        
	if(isChinese(&_char))        
		return word_children.insertToken(code,_char);        
	else        
		return byte_children.insertToken(code,_char);        
}        


//////////////////////////////////////////////////////////////////////////
void CFilterTree::BuildWord(LPCTSTR msg)        
{
}        
    
//过滤脏词
CString CFilterTree::Filtrate(LPCTSTR msg)        
{
	//变量定义
	CString strChat=TEXT("");

	//参数验证
	ASSERT(msg!=NULL);
	if(msg==NULL)return strChat; 

	//获取长度
	int size = (int)lstrlen(msg);        
	if(size <= 0)return strChat;

	//过滤脏词
	strChat=msg;
	for (INT_PTR i=0;i<m_ForbidArray.GetCount();i++)
	{
		strChat.Replace(m_ForbidArray[i],TEXT("*"));
	}

	//
	TCHAR *pLimit[] = { TEXT("25qp"),TEXT("25ＱＰ"),TEXT("２５ＱＰ"),TEXT("25qＰ"),TEXT("25Ｑp")};
	for (INT_PTR i=0;i<CountArray(pLimit);i++)
	{
		strChat.Replace(pLimit[i],TEXT("foxuc"));
	}
	   
	return strChat;          
}

//创建过滤表
void CFilterTree::BuildTree()        
{        
	INT_PTR nCount=m_ForbidArray.GetCount();
	for(INT_PTR i = 0; i < nCount; ++i)        
	{       
		BuildWord(m_ForbidArray[i]);           
	}        
}

//添加禁词
bool CSensitiveWordsFilter::AddSensitiveWords(LPCTSTR pSensitiveWords)
{
	//获取长度
	int nlen = lstrlen(pSensitiveWords);

	//分配空间
	LPTSTR pWords=new TCHAR[nlen+1];
	ZeroMemory(pWords, sizeof(TCHAR)*(nlen+1));

	//拷贝数据
	lstrcpyn(pWords,pSensitiveWords,nlen+1);
	
	//添加记录
	m_FilterTree.m_ForbidArray.Add(pWords);

	return true;
}

//添加完成
void CSensitiveWordsFilter::FinishAdd()
{
	m_FilterTree.BuildTree();
}

//过滤敏感词
void CSensitiveWordsFilter::Filtrate(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen)
{
	int nLen=0;
	CString lpOut=Filtrate(pMsg,nLen); 
	lstrcpyn(pszFiltered,lpOut,nMaxLen);

	return ;
}

//过滤敏感词
CString  CSensitiveWordsFilter::Filtrate(LPCTSTR pMsg, int &nLenght)
{
	CString ret= m_FilterTree.Filtrate(pMsg);
	nLenght=(int)ret.GetLength();
    return ret;
}

CFilterTree::~CFilterTree()
{   
	Reset();
}

//重置
void CFilterTree::Reset()
{
	for (int i=0; i<m_ForbidArray.GetCount(); i++)
	{
		SafeDeleteArray(m_ForbidArray.GetAt(i));
	}
	m_ForbidArray.RemoveAll();
}

//初始化
void CSensitiveWordsFilter::ResetSensitiveWordArray()
{
	m_FilterTree.Reset();
}