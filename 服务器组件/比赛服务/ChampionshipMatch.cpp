#include "StdAfx.h"
#include ".\championshipmatch.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WORD CChampionshipMatch::m_wChairCount;

CChampionshipMatch::CChampionshipMatch(tagMatchRule *pMatchRule, DWORD dwMatchNO,IChampionshipMatchSink *pSink)
:m_pMatchRule(pMatchRule),m_dwMatchNO(dwMatchNO),m_pMatchSink(pSink)
{
	m_pAssignTable=NULL;
	m_wTableCount=0;
	m_dwCurBase=0;
	m_dwTimeCount=0;
	m_bStarted=false;
	m_bGameEnd=false;
	m_WaitStartTable.RemoveAll();
	m_enMatchStatus=MS_SIGNUPING;
	m_wPlayingTableCount=0;
	m_wCurRountSec=0;
	m_wFinishTable=0;
	m_MaybeOutUser.RemoveAll();
	m_dwCurBase=m_pMatchRule->dwInitalBase;
	m_QuitUserMap.RemoveAll();
	m_bIsCanStandUp=true;
	m_UserSeatMap.RemoveAll();
	m_CanReadyUserMap.RemoveAll();
	m_bWillOver=false;
	m_bAssignTable=false;
	m_wAndroidUserCount=0;
	m_dwWaitTime=0;
	m_bException=false;
}

CChampionshipMatch::~CChampionshipMatch(void)
{
	SafeDeleteArray(m_pAssignTable);
	m_pAssignTable=NULL;
	m_wTableCount=0;
	m_dwCurBase=0;
	m_dwTimeCount=0;
	m_bStarted=false;
	m_bGameEnd=false;
	m_WaitStartTable.RemoveAll();
	m_enMatchStatus=MS_NULL_STATUS;
	m_wPlayingTableCount=0;
	m_wCurRountSec=0;
	m_wFinishTable=0;
	m_MaybeOutUser.RemoveAll();
	m_dwCurBase=m_pMatchRule->dwInitalBase;
	m_QuitUserMap.RemoveAll();
	m_bIsCanStandUp=true;
	m_UserSeatMap.RemoveAll();
	m_CanReadyUserMap.RemoveAll();
	m_bWillOver=false;
	m_bAssignTable=false;
	m_wAndroidUserCount=0;
	m_dwWaitTime=0;
	m_bException=false;	
}

//接口查询
VOID* CChampionshipMatch::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameEventSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameEventSink,Guid,dwQueryVer);
	return NULL;
}

//设置比赛规则
bool CChampionshipMatch::SetMatchRule(tagMatchRule *pRule)
{
	m_pMatchRule=pRule;
	m_dwCurBase=pRule->dwInitalBase;
	return true;
}

//用户报名
bool CChampionshipMatch::OnUserSignUp(IServerUserItem * pUserItem)
{
	//用户判断
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//查找用户
	IServerUserItem * pIServerUserExist=NULL;
	m_OnMatchUserMap.Lookup(pUserItem->GetUserID(),pIServerUserExist);

	//用户判断
	ASSERT(pIServerUserExist==NULL);
	if (pIServerUserExist!=NULL) return false;

	//人数效验
	ASSERT(m_OnMatchUserMap.GetCount()<m_pMatchRule->wUserCount);
	if (m_OnMatchUserMap.GetCount()>=m_pMatchRule->wUserCount) return false;

	//机器判断
	if (pUserItem->IsAndroidUser()==true)
	{
		if (m_wAndroidUserCount>=m_pMatchRule->wAndroidUserCount)
		{
			return false;
		}
		else
		{
			m_wAndroidUserCount++;
		}
	}

	//设置成绩
	tagUserInfo *pUserScore=pUserItem->GetUserInfo();
	pUserScore->dwWinCount=0L;
	pUserScore->dwLostCount=0L;
	pUserScore->dwFleeCount=0L;
	pUserScore->dwDrawCount=0L;
	pUserScore->lScore=(SCORE)m_pMatchRule->dwInitalScore;

	//发送成绩
	m_pMatchSink->SendGroupUserScore(pUserItem,NULL);

	//设置用户
	m_OnMatchUserMap[pUserItem->GetUserID()]=pUserItem;

	//报名人满
	if (m_OnMatchUserMap.GetCount()>=m_pMatchRule->wUserCount)
	{
		//安排座位
		DWORD dwUserID=0L;
		IServerUserItem *pIServerUserItem=NULL;
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		while (pos!=NULL)
		{
			//获取用户
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID,pIServerUserItem);

			//效验结果
			ASSERT(dwUserID!=0L);
			ASSERT(pIServerUserItem!=NULL);

			//变量定义
			WORD wTableID=INVALID_TABLE;
			WORD wChairID=INVALID_CHAIR;
			ITableFramEx * pTableEx=GetEmptyChair(wTableID,wChairID);

			//坐下处理
			if ((pTableEx!=NULL)&&(pIServerUserItem!=NULL))
			{
				if (pTableEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{
					ASSERT(FALSE);
				}
			}
			else
			{
				//错误判断
				if (pTableEx==NULL)
				{
					CTraceService::TraceString(TEXT("pTableEx为空"),TraceLevel_Exception);
				}
				if (pIServerUserItem==NULL)
				{
					CTraceService::TraceString(TEXT("pIServerUserItem为空"),TraceLevel_Exception);
				}
			}
		}

		//设置状态
		m_bIsCanStandUp=false;
		m_enMatchStatus=MS_FIRST_ROUND;		

		//通知开始
		m_dwTimeCount=(DWORD)time(NULL);
		m_tMatchStart=CTime::GetCurrentTime();

		//事件通知
		ASSERT(m_pMatchSink!=NULL);
		if (m_pMatchSink!=NULL) m_pMatchSink->OnMatchStart(this);
	}
	else
	{
		BYTE cbMatchStatus=MS_SIGNUP;
		m_pMatchSink->SendData(pUserItem, MDM_GR_MATCH, SUB_GR_MATCH_STATUS, &cbMatchStatus, sizeof(cbMatchStatus));
	}

	return true;
}

//用户退赛
BOOL CChampionshipMatch::OnUserQuitMatch(IServerUserItem *pUserItem,bool bByUser)
{
	//用户判断
	ASSERT(pUserItem!=NULL);
	if (pUserItem==NULL) return false;

	//删除用户
	BOOL bSuccess=m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
		
	if (m_enMatchStatus==MS_FIRST_ROUND)
	{
		m_UserSeatMap.RemoveKey(pUserItem);	
		INT_PTR nCount=m_OnMatchUserMap.GetCount();
		if (nCount<=(INT_PTR)m_pMatchRule->wRemainUserCount)
		{
			if(nCount<(INT_PTR)m_pMatchRule->dwSecRoundUserCount)
			{
				//构造数据包
				CMD_CM_SystemMessage Message;
				Message.wType=SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME;
				lstrcpyn(Message.szString,TEXT("抱歉，由于过多用户强行退出比赛，该场比赛提前结束。欢迎您继续报名参加比赛！"),CountArray(Message.szString));
				Message.wLength=CountStringBuffer(Message.szString);

				//发送数据
				WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
				m_pMatchSink->SendGroupData(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&Message,wSendSize, this);

				MatchOver();
				return TRUE;
			}
		
			m_enMatchStatus=MS_STOP_OUT;
			m_WaitStartTable.RemoveAll();
			
			TCHAR szMsg[256]=TEXT("");
			_sntprintf(szMsg,CountArray(szMsg),TEXT("已经达到淘汰人数，%d秒后即将结束初赛排名，确定晋级玩家！"),m_pMatchRule->wWaitTime);
			m_pMatchSink->SetGameTimer(IDI_FINISH_FIRST_ROUND,m_pMatchRule->wWaitTime*1000,1,NULL,this);
			if(m_dwWaitTime!=0)
				m_dwWaitTime=(DWORD)time(NULL);

			for(WORD wLoop=0;wLoop<m_wTableCount;wLoop++)
			{
				if(pUserItem->GetTableID()==m_pAssignTable[wLoop].pTableFrame->GetTableID())
					continue;
				if(m_pAssignTable[wLoop].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
					m_wPlayingTableCount++;
			}
			POSITION pos=m_OnMatchUserMap.GetStartPosition();
			IServerUserItem *pUser=NULL;
			DWORD dwUserID;
			while(pos!=NULL)
			{
				m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUser);
				m_pMatchSink->SendGameMessage(pUser,szMsg,SMT_CHAT|SMT_TABLE_ROLL);
			}	
		}
		else
		{
			m_wFinishTable=0;
			for(WORD wLoop=0;wLoop<m_wTableCount;wLoop++)
			{
				ITableFramEx TableFrameEx=m_pAssignTable[wLoop];
				if(TableFrameEx.pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) continue;
					
				if(TableFrameEx.cbGameCount==m_pMatchRule->cbFirGameCount || TableFrameEx.cbOnSeatUserCount<m_wChairCount)
					m_wFinishTable++;
			}
		}
	}
	else if(m_enMatchStatus==MS_STOP_OUT)
	{
		m_UserSeatMap.RemoveKey(pUserItem);	
		INT_PTR nCount=m_OnMatchUserMap.GetCount();
		if(nCount<(INT_PTR)m_pMatchRule->dwSecRoundUserCount)
		{
			//构造数据包
			CMD_CM_SystemMessage Message;
			Message.wType=SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME;
			lstrcpyn(Message.szString,TEXT("抱歉，由于过多用户强行退出比赛，该场比赛提前结束。欢迎您继续报名参加比赛！"),CountArray(Message.szString));
			Message.wLength=CountStringBuffer(Message.szString);

			//发送数据
			WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
			m_pMatchSink->SendGroupData(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&Message,wSendSize, this);

			MatchOver();
			return TRUE;
		}
	}
	else if(m_enMatchStatus==MS_SECOND_ROUND || m_enMatchStatus==MS_START_NEXT_ROUND)
	{
		DWORD dwSeatID;
		if(m_UserSeatMap.Lookup(pUserItem ,dwSeatID))
		{
			m_UserSeatMap.RemoveKey(pUserItem);	
			for(BYTE i=0; i<m_wTableCount; i++)
			{
				if(m_pAssignTable[i].pTableFrame->GetTableID()==LOWORD(dwSeatID))
				{ 
					IAndroidUserItem *pAndroidUserItem=m_pMatchSink->GetFreeAndroidUserItem();

					if(pAndroidUserItem==NULL || m_pAssignTable[i].cbGameCount>=m_pMatchRule->cbSecGameCount)
					{
						if(m_pAssignTable[i].cbGameCount<m_pMatchRule->cbSecGameCount)
						{
							m_pAssignTable[i].cbGameCount=m_pMatchRule->cbSecGameCount;
							if(bByUser) OnTableFinishRound(&m_pAssignTable[i]);
						}
					}
					else
					{
						if(m_pMatchSink->SetReplaceAndroidItem(pAndroidUserItem->GetMeUserItem()))
						{
							m_OnMatchUserMap[pAndroidUserItem->GetUserID()]=pAndroidUserItem->GetMeUserItem();
							tagUserInfo *pAndoridScore=pAndroidUserItem->GetMeUserItem()->GetUserInfo();
							tagUserInfo *pUserScore=pUserItem->GetUserInfo();

							pAndoridScore->lScore=pUserScore->lScore;
							pAndoridScore->dwWinCount=pUserScore->dwWinCount;
							pAndoridScore->dwLostCount=pUserScore->dwLostCount;
							pAndoridScore->dwFleeCount=pUserScore->dwFleeCount;
							pAndoridScore->dwDrawCount=pUserScore->dwDrawCount;

							m_pMatchSink->SendGroupUserScore(pAndroidUserItem->GetMeUserItem(),this);
							try
							{
								m_pAssignTable[i].pTableFrame->PerformSitDownAction(HIWORD(dwSeatID),pAndroidUserItem->GetMeUserItem(),TEXT(""));
							}
							catch(LPCTSTR pszString)
							{
								CString str;
								str.Format(TEXT("%s,%s,%d"),pszString,__FUNCTION__,__LINE__);
								CTraceService::TraceString(str,TraceLevel_Exception);
							}
						}
						else
						{
							if(m_pAssignTable[i].cbGameCount<m_pMatchRule->cbSecGameCount)
							{
								m_pAssignTable[i].cbGameCount=m_pMatchRule->cbSecGameCount;
								if(bByUser) OnTableFinishRound(&m_pAssignTable[i]);
							}
						}
					}
					break;;
				}
			}
		}
		for (INT_PTR i=0;i<m_MaybeOutUser.GetCount();i++)
		{
			IServerUserItem *pOutUserItem=m_MaybeOutUser.GetAt(i);
			if(pOutUserItem!=NULL && pOutUserItem->GetUserID()==pUserItem->GetUserID())
			{
				m_MaybeOutUser.RemoveAt(i);
				bSuccess=true;
				break;
			}
		}
		for (INT_PTR i=0;i<m_OutUserArray.GetCount();i++)
		{
			tagMatchScore *pOutUser=m_OutUserArray.GetAt(i);
			if(pOutUser!=NULL && pOutUser->pUserItem!=NULL && pOutUser->pUserItem->GetUserID()==pUserItem->GetUserID())
			{
				SafeDelete(pOutUser);
				m_OutUserArray.RemoveAt(i);
				bSuccess=true;
				break;
			}
		}		
	}

	if (bSuccess)
	{
		if (pUserItem->IsAndroidUser())
		{
			ASSERT(m_wAndroidUserCount>0);
			if (m_wAndroidUserCount>0) m_wAndroidUserCount--;
		}

		BYTE cbUserStatus=MS_NULL;
		m_pMatchSink->SendData(pUserItem, MDM_GR_MATCH, SUB_GR_MATCH_STATUS, &cbUserStatus, sizeof(cbUserStatus));
		if(m_enMatchStatus!=MS_SIGNUP)
		{
			m_pMatchSink->WriteUserAward(pUserItem,-1,m_dwMatchNO,TEXT(""),this);
		}
	}
	return bSuccess;
}

//设置回调接口
bool CChampionshipMatch::SetMatchSink(IChampionshipMatchSink *pSink)
{
	return true;
}

//分配桌子
void CChampionshipMatch::SetMatchTable(ITableFrame *pTable[], WORD wTableCount)
{
	if(pTable==NULL || wTableCount==0)
	{
		return ;
	}
	m_pAssignTable=new ITableFramEx[wTableCount];
	ZeroMemory(m_pAssignTable,sizeof(ITableFramEx)*wTableCount);
	for (WORD i=0;i<wTableCount;i++)
	{
		m_pAssignTable[i].pTableFrame=pTable[i];
		m_pAssignTable[i].cbGameCount=0;
		m_pAssignTable[i].dwBase=m_pMatchRule->dwInitalBase;
	}
	m_wTableCount=wTableCount;
}

//获取空座位
ITableFramEx* CChampionshipMatch::GetEmptyChair(WORD &wTableID, WORD &wChairID)
{
	for (WORD i=0;i<m_wTableCount;i++)
	{
		ITableFramEx & pTable=m_pAssignTable[i];
		if(pTable.pTableFrame->IsGameStarted()) continue;

		for(WORD j=0;j<m_wChairCount;j++)
		{
			if(pTable.pTableFrame->GetTableUserItem(j)==NULL)
			{
				wTableID=i;
				wChairID=j;
				return &m_pAssignTable[i];
			}
		}
	}

	return NULL;
}
//开始比赛
bool CChampionshipMatch::StartMatch(ITableFramEx *pTableFrame)
{
	if(pTableFrame==NULL)
	{
		for (WORD i=0;i<m_wTableCount;i++)
		{
			if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
				continue;
			for (WORD j=0;j<m_wChairCount;j++)
			{
				IServerUserItem *pUserItem=m_pAssignTable[i].pTableFrame->GetTableUserItem(j);
				if(pUserItem!=NULL)
					pUserItem->SetUserStatus(US_READY,pUserItem->GetTableID(),pUserItem->GetChairID());
				else
				{
					try
					{
						pUserItem=GetSeatUserItem(MAKELONG((m_pAssignTable[i].pTableFrame->GetTableID()), j));
					}
					catch(...)
					{
						CString str;
						str.Format(TEXT("%d,%d"), m_enMatchStatus,__LINE__);
						CTraceService::TraceString(str,TraceLevel_Info);
					}
					if(pUserItem!=NULL)
					{
						try
						{
							m_pAssignTable[i].pTableFrame->PerformSitDownAction(j,pUserItem);
						}
						catch(LPCTSTR pszString)
						{
							CString str;
							str.Format(TEXT("%s,%s,%d"),pszString,__FUNCTION__,__LINE__);
							CTraceService::TraceString(str,TraceLevel_Exception);
						}

						pUserItem->SetUserStatus(US_READY,pUserItem->GetTableID(),pUserItem->GetChairID());
					}					
				}
			}
			m_WaitStartTable.Add(&m_pAssignTable[i]);
		}
		m_bIsCanStandUp=true;
	}
	else
	{
		if(m_enMatchStatus==MS_SECOND_ROUND)
		{
			if(pTableFrame->cbGameCount==m_pMatchRule->cbSecGameCount)
			{
				for (INT_PTR i=0;i<m_WaitStartTable.GetCount();i++)
				{
					ITableFramEx *pTable=m_WaitStartTable.GetAt(i);
					if(pTable->pTableFrame->GetTableID()==pTableFrame->pTableFrame->GetTableID())
					{
						m_WaitStartTable.RemoveAt(i);
						i--;
					}
				}
				return true;
			}
		}

		if(pTableFrame->pTableFrame->GetGameStatus()<GAME_STATUS_PLAY)
		{
			POSITION pos=m_QuitUserMap.GetStartPosition();
			IServerUserItem *pUser=NULL;
			DWORD dwUserID=0;
			while(pos!=NULL)
			{
				m_QuitUserMap.GetNextAssoc(pos, dwUserID, pUser);
				if(pUser->GetTableID()==pTableFrame->pTableFrame->GetTableID() && pUser->GetUserStatus()<US_READY)
				{									
					m_pMatchSink->SendData(pUser,MDM_GF_FRAME, SUB_GF_USER_READY, NULL,0);
					m_CanReadyUserMap[pUser->GetUserID()]=pUser;
				}				
			}
			
			for (WORD j=0;j<m_wChairCount;j++)
			{
				IServerUserItem *pUserItem=pTableFrame->pTableFrame->GetTableUserItem(j);
				
				if(pUserItem!=NULL)
				{
					pUserItem->SetUserStatus(US_READY,pUserItem->GetTableID(),pUserItem->GetChairID());
				}
				else 
				{
					try
					{
						pUserItem=GetSeatUserItem(MAKELONG((pTableFrame->pTableFrame->GetTableID()), j));
					}
					catch(...)
					{
						CString str;
						str.Format(TEXT("%d,%d"), m_enMatchStatus,__LINE__);
						CTraceService::TraceString(str,TraceLevel_Info);
					}
					
					if(pUserItem!=NULL)
					{
						try
						{
							pTableFrame->pTableFrame->PerformSitDownAction(j,pUserItem);
						}
						catch(LPCTSTR pszString)
						{
							CString str;
							str.Format(TEXT("%s,%s,%d"),pszString,__FUNCTION__,__LINE__);
							CTraceService::TraceString(str,TraceLevel_Exception);
						}
						pUserItem->SetUserStatus(US_READY,pUserItem->GetTableID(),pUserItem->GetChairID());
					}
					else
					{
						if(m_enMatchStatus==MS_SECOND_ROUND)
						{
							pTableFrame->cbStartTimes++;
							IAndroidUserItem *pAndroidUserItem=m_pMatchSink->GetFreeAndroidUserItem();
							if(pAndroidUserItem!=NULL)
								pUserItem=pAndroidUserItem->GetMeUserItem();


							if(pUserItem==NULL || pTableFrame->cbStartTimes>=3 || pUserItem->GetUserID()==0)
							{
								if(pTableFrame->cbGameCount<m_pMatchRule->cbSecGameCount)
								{
									pTableFrame->cbGameCount=m_pMatchRule->cbSecGameCount;
									OnTableFinishRound(pTableFrame);
								}
							}
							else
							{
								ASSERT(pUserItem->GetTableID()==INVALID_TABLE);
								
								try
								{
									pTableFrame->pTableFrame->PerformSitDownAction(j,pUserItem);
									pUserItem->SetUserStatus(US_READY,pUserItem->GetTableID(),pUserItem->GetChairID());
								}
								catch(LPCTSTR pszString)
								{
									CString str;
									str.Format(TEXT("%s,%s,%d"),pszString,__FUNCTION__,__LINE__);
									CTraceService::TraceString(str,TraceLevel_Exception);
								}
							}
						}
					}

				}
			}
		}
		else
		{
			for (INT_PTR i=0;i<m_WaitStartTable.GetCount();i++)
			{
				ITableFramEx *pTable=m_WaitStartTable.GetAt(i);
				if(pTable->pTableFrame->GetTableID()==pTableFrame->pTableFrame->GetTableID())
				{
					m_WaitStartTable.RemoveAt(i);
					i--;
				}
			}
		}
	}
	
	return true;
}

//游戏开始
bool CChampionshipMatch::OnEventGameStart(ITableFrame *pITableFrame, WORD wChairCount)
{

	pITableFrame->SetCellScore(m_dwCurBase);

	TCHAR szMsg[256]=TEXT("\0");
	ASSERT(m_enMatchStatus!=MS_START_NEXT_ROUND);
	if(m_enMatchStatus==MS_SECOND_ROUND)
	{
		ITableFramEx &table=m_pAssignTable[0];
		for(WORD i=0; i<m_wTableCount; i++)
		{		
			ITableFramEx &table=m_pAssignTable[i];
			if(table.pTableFrame==pITableFrame)
			{
				table.cbStartTimes=0;
				_sntprintf(szMsg, CountArray(szMsg),TEXT("淘汰赛第%d轮（共%d轮）第%d局开始（共%d局)。"),m_wCurRountSec,
					m_pMatchRule->cbSecRoundCount,table.cbGameCount+1,m_pMatchRule->cbSecGameCount);

				//构造数据包
				CMD_CM_SystemMessage Message;
				Message.wType=SMT_CHAT;

				lstrcpyn(Message.szString,szMsg,CountArray(Message.szString));
				Message.wLength=CountStringBuffer(Message.szString);

				//发送数据
				WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
				pITableFrame->SendTableData(INVALID_CHAIR,SUB_GF_SYSTEM_MESSAGE, (void*)&Message,wSendSize,MDM_GF_FRAME);


				//构造数据
				CMD_GR_Match_Info MatchInfo;
				ZeroMemory(&MatchInfo,sizeof(MatchInfo));
				_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("积分方式：积分"));
				_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("游戏基数：%d"), m_dwCurBase);
				_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("局制名称：%d局积分制"),m_pMatchRule->cbSecGameCount);
				_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("%d局积分最高者获胜"), m_pMatchRule->cbSecGameCount);

				for(WORD i=0; i<wChairCount; i++)
				{
					//获取用户
					IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(i);
					if(pITableFrame==NULL) continue;

					//变量定义
					tagUserInfo *pUserInfo=pIServerUserItem->GetUserInfo();
					MatchInfo.wGameCount=table.cbGameCount+1;

					//发送数据
					pITableFrame->SendTableData(i, SUB_GR_MATCH_INFO, &MatchInfo, sizeof(MatchInfo), MDM_GF_FRAME);
				}
				break;
			}
		}

	}
	else if(m_enMatchStatus==MS_FIRST_ROUND)
	{
		//构造数据
		CMD_GR_Match_Info MatchInfo;
		ZeroMemory(&MatchInfo,sizeof(MatchInfo));
		_sntprintf(MatchInfo.szTitle[0], CountArray(MatchInfo.szTitle[0]),TEXT("积分方式：底分×基数"));
		_sntprintf(MatchInfo.szTitle[1], CountArray(MatchInfo.szTitle[1]),TEXT("游戏基数：%d"), m_dwCurBase);
		if(m_pMatchRule->enFirstRule==FirstRoundRule_Score)
		{
			_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("局制名称：打立出局"));
			_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("低于 %d 分将被淘汰"), (int)(m_dwCurBase*m_pMatchRule->cbLeastScore/100));
		}
		else
		{
			_sntprintf(MatchInfo.szTitle[2], CountArray(MatchInfo.szTitle[2]),TEXT("局制名称：定时排名"));
			CTimeSpan span=CTime::GetCurrentTime()-m_tMatchStart;
			_sntprintf(MatchInfo.szTitle[3], CountArray(MatchInfo.szTitle[3]),TEXT("大约 %d 分钟后排名"), 
				(m_pMatchRule->dwFirstRoundTime-span.GetTotalSeconds()-m_pMatchRule->dwAheadStatistics)/60+1);
		}
		

		for(WORD i=0; i<wChairCount; i++)
		{
			//获取用户
			IServerUserItem *pIServerUserItem=pITableFrame->GetTableUserItem(i);
			if(pITableFrame==NULL) continue;

			//变量定义
			tagUserInfo *pUserInfo=pIServerUserItem->GetUserInfo();
			MatchInfo.wGameCount=(WORD)(pUserInfo->dwWinCount+pUserInfo->dwLostCount+pUserInfo->dwFleeCount+pUserInfo->dwDrawCount+1);

			//发送数据
			pITableFrame->SendTableData(i, SUB_GR_MATCH_INFO, &MatchInfo, sizeof(MatchInfo), MDM_GF_FRAME);
		}
	}

	POSITION pos=m_QuitUserMap.GetStartPosition();
	DWORD dwUserID=0;
	IServerUserItem *pUserItem=NULL;
	while(pos!=NULL)
	{
		m_QuitUserMap.GetNextAssoc(pos,dwUserID, pUserItem);
		if(pUserItem->GetTableID()==pITableFrame->GetTableID())
		{
			pITableFrame->PerformStandUpAction(pUserItem);
			if(pUserItem->GetTableID()!=INVALID_TABLE)
			{
				CString strTace;
				strTace.Format(TEXT("起立失败（%d）"),__LINE__);
				CTraceService::TraceString(strTace,TraceLevel_Exception);
			}
			m_UserSeatMap.RemoveKey(pUserItem);
			pUserItem->SetUserStatus(US_OFFLINE,pITableFrame->GetTableID(),pUserItem->GetChairID());
			m_pMatchSink->SendGroupUserStatus(pUserItem,this);
		}
	}
	
	for (INT_PTR i=0;i<m_WaitStartTable.GetCount();i++)
	{
		ITableFramEx *pTable=m_WaitStartTable.GetAt(i);
		if(pTable->pTableFrame->GetTableID()==pITableFrame->GetTableID())
		{
			m_WaitStartTable.RemoveAt(i);
			i--;
		}
	}
	return true;
}
//游戏结束
bool CChampionshipMatch::OnEventGameEnd(ITableFrame *pITableFrame,WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	m_bGameEnd=true;
	ITableFramEx *pTable=NULL;
	WORD wTableIndex=0;
	for (WORD i=0;i<m_wTableCount;i++)
	{
		pTable=&m_pAssignTable[i];
		if(pTable->pTableFrame->GetTableID()==pITableFrame->GetTableID())
		{
			wTableIndex=i;
			break;
		}
	}

	pTable->cbGameCount++;
	if(m_enMatchStatus==MS_STOP_OUT)
	{
		if(m_OnMatchUserMap.GetCount()<(INT_PTR)m_pMatchRule->dwSecRoundUserCount)
		{
			MatchOver();
			return TRUE;
		}
		m_wPlayingTableCount--;
		LPCTSTR szMsg=TEXT("已经达到淘汰人数，等待其他玩家结束游戏后确定晋级人数!");
		for(WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pUser=pTable->pTableFrame->GetTableUserItem(j);
			if(pUser!=NULL)
				m_pMatchSink->SendGameMessage(pUser, szMsg,SMT_CHAT|SMT_TABLE_ROLL);
		}
		SendWaitTip(pITableFrame);
	}
	else if(m_enMatchStatus==MS_FIRST_ROUND)
	{
		if(m_pMatchRule->enFirstRule==FirstRoundRule_Score)
			FirstRoundRuleScoreGameEnd(pTable,wTableIndex);
		else if(m_pMatchRule->enFirstRule==FirstRoundRule_Time)
			FirstRoundRuleTimeGameEnd(pTable);
	}
	else if(m_enMatchStatus==MS_SECOND_ROUND)
	{
		OnTableFinishRound(pTable);
	}
	else if(m_enMatchStatus==MS_START_NEXT_ROUND)
	{
		ASSERT(false);
	}

	tagMatchScore *pScore = new tagMatchScore[m_OnMatchUserMap.GetCount()];
	ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
	WORD wCount=SortMapUser(pScore);
	for (WORD i=0;i<wCount;i++)
	{
		m_pMatchSink->SendGroupUserScore(pScore[i].pUserItem,this);	
	}
	SafeDeleteArray(pScore);

	return true;
}
//定时器
bool CChampionshipMatch::OnTimeMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//状态校验
	if(m_enMatchStatus==MS_NULL_STATUS) return true;

	switch(dwTimerID)
	{
	case IDI_CHECK_MATCH_GROUP:
		{
			if(m_pMatchRule->enFirstRule==FirstRoundRule_Score)
			{
				bool bCanStart=true;
				DWORD dwTimeCount=(DWORD)time(NULL)-m_dwTimeCount;
				if(m_enMatchStatus==MS_FIRST_ROUND && dwTimeCount>m_pMatchRule->dwGrowthTime)
				{
					m_dwCurBase=m_dwCurBase*(100+m_pMatchRule->cbGrowthRange)/100;
					m_dwTimeCount=(DWORD)time(NULL);
				}		

				if(m_enMatchStatus==MS_STOP_OUT && !m_bGameEnd)
				{
					bool bPlaying=false;
					for (WORD i=0;i<m_wTableCount;i++)
					{
						if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
							bPlaying=true;
					}
					if(!bPlaying)
						SelectPromotionUser();
				}

				if(m_enMatchStatus==MS_FIRST_ROUND && m_bGameEnd==false)
				{
					WORD wReadyUserCount=0;
					for(WORD i=0; i<m_wTableCount; i++)
					{
						WORD wTableReadyCount=GetSitUserCount(m_pAssignTable[i].pTableFrame);
						if(wTableReadyCount!=m_wChairCount)
							wReadyUserCount+=wTableReadyCount;
					}
					
			 		for(WORD i=0; i<m_wTableCount; i++)
					{
						if(wReadyUserCount<m_wChairCount) break;

						WORD wTableIUserCount=GetSitUserCount(m_pAssignTable[i].pTableFrame);
						if(wTableIUserCount==3) continue;

						WORD wChairID=GetNullChairID(m_pAssignTable[i].pTableFrame);

						for(WORD j=i+1; j<m_wTableCount; j++)
						{							
							if(wChairID==INVALID_CHAIR) break;

							WORD wUserCount=GetSitUserCount(m_pAssignTable[j].pTableFrame);
							if(wUserCount==3 || wUserCount==0) continue;

							for(WORD k=0; k<m_wChairCount; k++)
							{
								IServerUserItem *pUserItem=m_pAssignTable[j].pTableFrame->GetTableUserItem(k);

								if(pUserItem==NULL || pUserItem->GetUserStatus()>=US_PLAYING) continue;

								if(!m_pAssignTable[j].pTableFrame->PerformStandUpAction(pUserItem))
								{
									CString strTace;
									strTace.Format(TEXT("起立失败（%d）"),__LINE__);
									CTraceService::TraceString(strTace,TraceLevel_Exception);
								}

								m_UserSeatMap.RemoveKey(pUserItem);
								try
								{
									m_pAssignTable[i].pTableFrame->PerformSitDownAction(wChairID,pUserItem,TEXT(""));
								}
								catch(LPCTSTR pszString)
								{
									CString str;
									str.Format(TEXT("%s,%s,%d"),pszString,__FUNCTION__,__LINE__);
									CTraceService::TraceString(str,TraceLevel_Exception);
								}
								wReadyUserCount--;

								wChairID=GetNullChairID(m_pAssignTable[i].pTableFrame);

								if(wChairID==INVALID_CHAIR) break;
							}
							if(wChairID==INVALID_CHAIR) 
							{
								wReadyUserCount-=wTableIUserCount;
								if(m_pAssignTable[i].cbOnSeatUserCount==m_wChairCount)
									m_WaitStartTable.Add(&m_pAssignTable[i]); 
								break; 
							}
						}
					}					
				}

				if(m_WaitStartTable.GetCount()>0 && bCanStart && !m_bGameEnd)
				{

					for (INT_PTR n=0;n<m_WaitStartTable.GetCount();n++)
					{
						ITableFramEx *pTable=m_WaitStartTable.GetAt(n);
						if(pTable->pTableFrame->GetGameStatus()==GAME_STATUS_FREE)
							StartMatch(pTable);
						else 
						{
							m_WaitStartTable.RemoveAt(n);
							n--;
						}
					}
					
				}			
			}
			else if(m_pMatchRule->enFirstRule==FirstRoundRule_Time)
			{			
				if((m_wPlayingTableCount==0 || m_wPlayingTableCount>m_wTableCount) && !m_bGameEnd)
				{
					SelectPromotionUser();
				}				
			}			

		}
		m_bGameEnd=false;
		return true;
	case IDI_STAR_NEXT_ROUND:
		{
			if(m_enMatchStatus==MS_SIGNUP || m_enMatchStatus==MS_SECOND_ROUND || m_enMatchStatus==MS_STOP_OUT || m_enMatchStatus==MS_NULL_STATUS)
				return true;
			try
			{
				tagMatchScore *pScore=new tagMatchScore[m_OnMatchUserMap.GetCount()];
				ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
				WORD wCount=SortMapUser(pScore);

				if (m_enMatchStatus==MS_FIRST_ROUND)
				{
					if(m_wFinishTable>1)
					{
						try
						{
							RearrangeUserSeat(pScore,wCount);
						}
						catch(...)
						{
							CString str;
							str.Format(TEXT("RearrangeUserSeat:%d"),__LINE__);
							CTraceService::TraceString(str,TraceLevel_Exception);
							SafeDeleteArray(pScore);
						}
					}
					else
					{
						SafeDeleteArray(pScore);
						return true;
					}
				}
				else if(m_enMatchStatus==MS_START_NEXT_ROUND)
				{
					if(wCount<m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec-1])
					{
						if(m_wCurRountSec==m_pMatchRule->cbSecGameCount)
						{
							for(WORD i=0; i<wCount; i++)
							{
								m_pMatchSink->WriteUserAward(pScore[i].pUserItem, i,m_dwMatchNO,NULL,this);
								m_OnMatchUserMap.RemoveKey(pScore[i].pUserItem->GetUserID());
								m_UserSeatMap.RemoveKey(pScore[i].pUserItem);
							}						
						}
						else
						{
							LPCTSTR szMsg=TEXT("抱歉，由于过多用户强行退出比赛，该场比赛提前结束。欢迎您继续报名参加比赛！");
							for(WORD i=0; i<wCount; i++)
							{
								if(pScore[i].pUserItem!=NULL)
								{
									m_pMatchSink->OnUserOut(pScore[i].pUserItem, szMsg, (WORD)lstrlen(szMsg), -1,this);
									m_OnMatchUserMap.RemoveKey(pScore[i].pUserItem->GetUserID());
									m_UserSeatMap.RemoveKey(pScore[i].pUserItem);
								}
							}
						}

						SafeDeleteArray(pScore);

						MatchOver();

						return true;
					}
					m_WaitStartTable.RemoveAll();
					try
					{
						RearrangeUserSeat(pScore,m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec-1]);
					}
					catch(...)
					{
						CString str;
						str.Format(TEXT("RearrangeUserSeat:%d"),__LINE__);
						CTraceService::TraceString(str,TraceLevel_Exception);
					}
					for (WORD i=0;i<m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec-1];i++)
					{
						CString str;
						str.Format(TEXT("淘汰赛第%d轮开始，您晋级后的积分为%d,处于第%d名"),m_wCurRountSec,(int)(pScore[i].lScore*0.3), i+1);
						m_pMatchSink->SendGameMessage(pScore[i].pUserItem, str, SMT_CHAT|SMT_TABLE_ROLL);
					}
					m_enMatchStatus=MS_SECOND_ROUND;
				}

				SafeDeleteArray(pScore);
				m_wFinishTable=0;
				m_MaybeOutUser.RemoveAll();
				m_QuitUserMap.RemoveAll();
				return true;
			}
			catch(...)
			{
				CString str;
				str.Format(TEXT("比赛人数:%d,%d,m_enMatchStatus=%d,m_wCurRountSec=%d"),
					m_OnMatchUserMap.GetCount(),m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec-1],m_enMatchStatus,m_wCurRountSec);
				CTraceService::TraceString(str,TraceLevel_Exception);
			}
			break;
		}
	case IDI_NORMAL_TIMER:
		{
			if(m_pMatchRule->enFirstRule==FirstRoundRule_Time)
			{
				if(m_tMatchStart.GetYear()==1970)
					return true;

				CTimeSpan span=CTime::GetCurrentTime()-m_tMatchStart;
				if(m_pMatchRule->dwFirstRoundTime-span.GetTotalSeconds()<=m_pMatchRule->dwAheadStatistics && !m_bWillOver)
				{
					m_bWillOver=true;
					m_wPlayingTableCount=0;
					m_WaitStartTable.RemoveAll();
					for(int i=0; i<m_wTableCount; i++)
					{
						if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY || m_pAssignTable[i].pTableFrame->IsGameStarted())
							m_wPlayingTableCount++;
					}
					//构造数据包
					CMD_CM_SystemMessage Message;
					Message.wType=SMT_TABLE_ROLL;
					lstrcpyn(Message.szString,TEXT("本轮比赛即将结束，等待所有玩家结束游戏后确定晋级名单"),CountArray(Message.szString));
					Message.wLength=CountStringBuffer(Message.szString);

					//发送数据
					WORD wSendSize=sizeof(Message)-sizeof(Message.szString)+Message.wLength*sizeof(TCHAR);
					return m_pMatchSink->SendGroupData(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&Message,wSendSize, this);

					if(m_wPlayingTableCount==0)
						SelectPromotionUser();
				}
				
			}
			break;
		}
	case IDI_FINISH_FIRST_ROUND:
		{
			if(m_enMatchStatus!=MS_STOP_OUT)
				return true;
			for(WORD i=0; i<m_wTableCount;i++)
			{
				if(m_pAssignTable[i].pTableFrame->IsGameStarted())
					m_pAssignTable[i].pTableFrame->DismissGame();
			}
		}
	}
	return true;
}

void CChampionshipMatch::SelectPromotionUser()
{
	m_WaitStartTable.RemoveAll();

	INT_PTR nCount=m_OnMatchUserMap.GetCount();
	if(nCount==0 || nCount<(INT_PTR)m_pMatchRule->dwSecRoundUserCount)
	{
		MatchOver();
		return;
	}

	tagMatchScore *pScore=new tagMatchScore[nCount];
	ZeroMemory(pScore,sizeof(tagMatchScore)*nCount);
	
	SortMapUser(pScore);
	CString str;
	INT_PTR nOutUserCount=nCount-m_pMatchRule->dwSecRoundUserCount;
	INT_PTR nPromotCount=0;
	for (INT_PTR i=0;i<nCount;i++)
	{
		if(i>=(INT_PTR)m_pMatchRule->dwSecRoundUserCount)
		{
			str.Format(TEXT("抱歉，您未能成功晋级，在本次比赛中您获得第%d名"), i+1);
			m_OnMatchUserMap.RemoveKey(pScore[i].dwUserID);
			m_UserSeatMap.RemoveKey(pScore[i].pUserItem);
			m_pMatchSink->WriteUserAward(pScore[i].pUserItem, (WORD)i, m_dwMatchNO, str,this);
			m_pMatchSink->SendData(pScore[i].pUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
		}
		else
		{
			str=TEXT("恭喜您晋级成功，请稍候，决赛马上开始！");
			m_pMatchSink->SendGameMessage(pScore[i].pUserItem, str, SMT_CHAT|SMT_TABLE_ROLL);
		}
	}
	try
	{
		RearrangeUserSeat(pScore,m_pMatchRule->dwSecRoundUserCount);
	}
	catch(...)
	{
		CString str;
		str.Format(TEXT("RearrangeUserSeat:%d"),__LINE__);
		CTraceService::TraceString(str,TraceLevel_Exception);
	}
	m_enMatchStatus=MS_SECOND_ROUND;
	m_wCurRountSec=1;	
    m_dwCurBase=m_pMatchRule->dwInitalBase/10;
	m_wFinishTable=0;
	SafeDeleteArray(pScore);
}

//将玩家排序
WORD CChampionshipMatch::SortMapUser(tagMatchScore score[])
{
	INT_PTR nCount=m_OnMatchUserMap.GetCount();
	POSITION pos=m_OnMatchUserMap.GetStartPosition();
	nCount=0;
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos,score[nCount].dwUserID, score[nCount].pUserItem);
		score[nCount].lScore=score[nCount].pUserItem->GetUserScore();
		nCount++;
	}	
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return (WORD)nCount;
}

//玩家排序
WORD CChampionshipMatch::SortUserItemArray(tagMatchScore score[], CMatchUserItemArray &MatchUserItemArray)
{
	INT_PTR nCount=MatchUserItemArray.GetCount();

	for(INT_PTR i=0;i<nCount;i++)
	{
		score[i].pUserItem=MatchUserItemArray.GetAt(i);
		score[i].lScore=score[i].pUserItem->GetUserScore();
	}	
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i].lScore > score[mid].lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return (WORD)nCount;
}

//将所有玩家起立
void CChampionshipMatch::RearrangeUserSeat(tagMatchScore score[], INT_PTR nCount)
{
	m_WaitStartTable.RemoveAll();

	//用户起立
	for (WORD i=0;i<m_wTableCount;i++)
	{
		//获取桌子
		ASSERT(m_pAssignTable[i].pTableFrame!=NULL);
		ITableFrame * pITableFrame=m_pAssignTable[i].pTableFrame;

		//状态判断
		if (pITableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
		{
			continue;
		}

		//用户处理
		for (WORD j=0;j<m_wChairCount;j++)
		{
			//变量定义
			WORD wTableID=pITableFrame->GetTableID();
			IServerUserItem * pIServerUserItem=pITableFrame->GetTableUserItem(j);

			//查找座位
			if (pIServerUserItem==NULL)
			{
				pIServerUserItem=GetSeatUserItem(MAKELONG(wTableID,j));
			}

			//用户起立
			if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetTableID()!=INVALID_TABLE))
			{
				//用户效验
				ASSERT(pITableFrame->GetTableUserItem(pIServerUserItem->GetChairID())==pIServerUserItem);
				if (pITableFrame->GetTableUserItem(pIServerUserItem->GetChairID())!=pIServerUserItem)
				{
					CTraceService::TraceString(TEXT("RearrangeUserSeat 用户对象所坐的位置与桌子对象绑定信息不一致"),TraceLevel_Exception);
					continue;
				}

				//执行起立
				pITableFrame->PerformStandUpAction(pIServerUserItem);
			}

			//删除索引
			m_UserSeatMap.RemoveKey(pIServerUserItem);
		}

		//设置变量
		m_pAssignTable[i].cbGameCount=0;
		ZeroMemory(m_pAssignTable[i].bOffline,sizeof(m_pAssignTable[i].bOffline));
	}

	for (WORD i=0;i<nCount;i++)
	{
		//变量定义
		IServerUserItem * pIServerUserItem=score[i].pUserItem;

		//参数判断 
		if (score[i].pUserItem==NULL)
		{
			CTraceService::TraceString(TEXT("不可能出现 pUserItem==NULL"),TraceLevel_Exception);
			continue;
		}

		//状态判断
		if (score[i].pUserItem->GetUserStatus()>=US_PLAYING)
		{
			continue;
		}

		//旁观起立
		if (score[i].pUserItem->GetUserStatus()==US_LOOKON || pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			for (WORD j=0;j<m_wTableCount;j++)
			{
				if (m_pAssignTable[j].pTableFrame->GetTableID()==pIServerUserItem->GetTableID())
				{
					m_pAssignTable[j].pTableFrame->PerformStandUpAction(pIServerUserItem);
					break;
				}
			}
		}

		//变量定义
		tagUserInfo * pUserScore=pIServerUserItem->GetUserInfo();

		//调整积分
		if(m_enMatchStatus==MS_STOP_OUT)
		{
			pUserScore->lScore=(SCORE)sqrt(FLOAT(pUserScore->lScore))*10L/3L;
		}
		else if(m_enMatchStatus==MS_START_NEXT_ROUND)
		{
			pUserScore->lScore=(SCORE)(pUserScore->lScore*0.3);
		}

		//发送积分
		m_pMatchSink->SendGroupUserScore(pIServerUserItem,this);

		//获取桌子
		WORD wChairID=INVALID_TABLE;
		WORD wTableID=INVALID_CHAIR;
		ITableFramEx * pITableFramEx=GetEmptyChair(wTableID,wChairID);

		//错误判断
		if ((pITableFramEx==NULL)||(pITableFramEx->pTableFrame->IsGameStarted()))
		{
			CTraceService::TraceString(TEXT("不可能出现 Table==NULL 或 Table->pTableFrame->IsGameStarted()"),TraceLevel_Exception);
			continue;
		}

		//坐下位置
		if (pITableFramEx->pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
		{
			CTraceService::TraceString(TEXT("不可能出现 Table==NULL 或 Table->pTableFrame->IsGameStarted()1"),TraceLevel_Exception);
			continue;
		}

		//等待判断
		if (pITableFramEx->cbOnSeatUserCount>=m_wChairCount)
		{
			//查找现有
			INT_PTR nIndex;
			for (nIndex=0;nIndex<m_WaitStartTable.GetCount();nIndex++)
			{
				if (m_WaitStartTable[nIndex]==pITableFramEx)
				{
					CTraceService::TraceString(TEXT("不可能出现 m_WaitStartTable[k]==pITableFramEx"),TraceLevel_Exception);
					break;
				}
			}

			//插入数组
			if (nIndex>=m_WaitStartTable.GetCount())
			{
				m_WaitStartTable.Add(pITableFramEx);
			}
		}		
	}

	return;
}

//是否可以离开座位 
bool CChampionshipMatch::IsCanStanUp(IServerUserItem *pUserItem)
{
	if(m_enMatchStatus==MS_FIRST_ROUND || m_enMatchStatus==MS_STOP_OUT || m_enMatchStatus==MS_SIGNUP)
	{
		if(!m_bIsCanStandUp)
		{
			if(pUserItem->GetUserStatus()<US_READY)
			{
				m_pMatchSink->SendData(pUserItem,MDM_GF_FRAME, SUB_GF_USER_READY, NULL,0);
				m_CanReadyUserMap[pUserItem->GetUserID()]=pUserItem;
			}
			return false;
		}
		else
		{

		}
	}
	else if(m_enMatchStatus==MS_SECOND_ROUND|| m_enMatchStatus==MS_START_NEXT_ROUND)
	{
		WORD wTableID=pUserItem->GetTableID();
		if(wTableID==INVALID_TABLE)
		{
			ASSERT(false);
			return true;
		}
		for (WORD i=0;i<m_wTableCount;i++)
		{
			if(m_pAssignTable[i].pTableFrame->GetTableID()==wTableID)
			{
				if (m_pAssignTable[i].cbGameCount>=m_pMatchRule->cbSecGameCount)
				{

				}
				else 
				{
					return false;
				}
			}
		}
 	}
	return false;
}
//玩家断线或重连 
void CChampionshipMatch::SetUserOffline(WORD wTableID,IServerUserItem *pUserItem, bool bOffline)
{
	if(bOffline)
	{
		m_QuitUserMap[pUserItem->GetUserID()]=pUserItem;
		WORD wChairID=pUserItem->GetChairID();
		if(pUserItem->GetUserStatus()<US_READY)
			pUserItem->SetUserStatus(US_READY, wTableID,wChairID);
		if(wChairID!=INVALID_CHAIR)
		{
			for (WORD i=0;i<m_wTableCount;i++)
			{
				if(m_pAssignTable[i].pTableFrame->GetTableID()==wTableID)
				{
					m_pAssignTable[i].bOffline[wChairID]=true;
				}
			}
		}
	}
	else
	{
		m_QuitUserMap.RemoveKey(pUserItem->GetUserID());
		WORD wChairID=pUserItem->GetChairID();
		if(wChairID!=INVALID_CHAIR)
		{
			for (WORD i=0;i<m_wTableCount;i++)
			{
				if(m_pAssignTable[i].pTableFrame->GetTableID()==wTableID)
				{
					m_pAssignTable[i].bOffline[wChairID]=false;
				}
			}
		}
	}
}

//将玩家排序
WORD CChampionshipMatch::SortOutUserArray(tagMatchScore *score[])
{
	INT_PTR nCount=m_OutUserArray.GetCount();

	for(INT_PTR i=0;i<nCount;i++)
	{
		score[i]=m_OutUserArray.GetAt(i);
	}	
	for(INT_PTR i = 1;i < nCount;i++)
	{
		INT_PTR left = 0,right = i - 1;
		tagMatchScore  *Temp = score[i];
		while(left <= right)
		{
			INT_PTR mid = (left + right)/2;
			if(score[i]->lScore > score[mid]->lScore)right = mid - 1;
			else left = mid + 1;
		}
		for(INT_PTR j = i - 1;j >= left;j--)
			score[j+1] = score[j];
		score[left] = Temp;
	}
	return (WORD)nCount;
}

//获取名次
WORD CChampionshipMatch::GetUserRank(IServerUserItem *pUserItem, ITableFrame *pITableFrame)
{
	//参数效验
	ASSERT(pUserItem!=NULL);
	if(pUserItem==NULL) return INVALID_WORD;
	//定义变量
	SCORE lUserScore=pUserItem->GetUserScore();
	WORD wRank=0;
	DWORD dwUserID=0;
	IServerUserItem *pLoopUserItem=NULL;

	if(pITableFrame==NULL)
	{
		//遍历容器
		POSITION pos=m_OnMatchUserMap.GetStartPosition();
		while(pos!=NULL)
		{
			m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pLoopUserItem);
			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
				wRank++;
		}
	}
	else
	{
		ASSERT(pUserItem->GetTableID()==pITableFrame->GetTableID());
		if(pUserItem->GetTableID()!=pITableFrame->GetTableID()) return INVALID_WORD;

		for(WORD i=0; i<m_wChairCount; i++)
		{
			pLoopUserItem=pITableFrame->GetTableUserItem(i);
			if(pLoopUserItem==NULL)continue;

			if(pLoopUserItem!=pUserItem && pLoopUserItem->GetUserScore()>lUserScore)
				wRank++;
		}
	}

	return wRank;
}

//完成一轮
bool CChampionshipMatch::OnTableFinishRound(ITableFramEx *pITableFrameEx)
{
	if(pITableFrameEx->cbGameCount>=m_pMatchRule->cbSecGameCount)
	{
		tagMatchScore *pMatchScore= new tagMatchScore[m_wChairCount];
		ZeroMemory(pMatchScore,sizeof(tagMatchScore)*m_wChairCount);
 		BYTE cbSomeOneStandUp=0;
		WORD wMaxInx=0;
		WORD wMinInx=0;
		for (WORD i=0;i<m_wChairCount;i++)
		{
			pMatchScore[i].pUserItem=pITableFrameEx->pTableFrame->GetTableUserItem(i);
			if(pMatchScore[i].pUserItem!=NULL)
			{
				pMatchScore[i].lScore=pMatchScore[i].pUserItem->GetUserScore();
			}
			else
			{
				IServerUserItem *pSeatUser=NULL;
				try
				{
					pSeatUser=GetSeatUserItem(MAKELONG((pITableFrameEx->pTableFrame->GetTableID()),i));
				}
				catch(...)
				{
					CString str;
					str.Format(TEXT("%d,%d"), m_enMatchStatus,__LINE__);
					CTraceService::TraceString(str,TraceLevel_Info);
				}
				if(pSeatUser!=NULL)
				{
					pMatchScore[i].pUserItem=pSeatUser;
					pMatchScore[i].lScore=pSeatUser->GetUserScore();
				}
				else 
				{
					cbSomeOneStandUp++;
				}
			}
			if(i>0)
			{
				if(pMatchScore[wMaxInx].lScore<pMatchScore[i].lScore)
					wMaxInx=i;
				if(pMatchScore[wMinInx].lScore>pMatchScore[i].lScore)
					wMinInx=i;
			}
		}

		if(m_wCurRountSec!=m_pMatchRule->cbSecRoundCount)
		{
			for(WORD i=0;i<m_wChairCount;i++)
			{
				if(pMatchScore[i].pUserItem==NULL) continue;

				if(i==wMinInx && cbSomeOneStandUp==0)
				{
					TCHAR szMsg[256]=TEXT("");
					_sntprintf(szMsg,CountArray(szMsg),TEXT("您已完成本轮比赛，处于本桌第 %d 名。很遗憾，您被淘汰出局。请等待其他桌结束后重新排名确定所有淘汰选手名字。"),m_wChairCount);

					tagMatchScore *pOutUser=new tagMatchScore;
					ZeroMemory(pOutUser,sizeof(tagMatchScore));
					pOutUser->pUserItem=pMatchScore[wMinInx].pUserItem;
					pOutUser->lScore=pMatchScore[wMinInx].lScore;
					pOutUser->dwUserID=pMatchScore[wMinInx].pUserItem->GetUserID();
					m_OutUserArray.Add(pOutUser);
					m_QuitUserMap.RemoveKey(pMatchScore[wMinInx].pUserItem->GetUserID());
					m_OnMatchUserMap.RemoveKey(pMatchScore[wMinInx].pUserItem->GetUserID());
					m_UserSeatMap.RemoveKey(pMatchScore[wMinInx].pUserItem);

					m_pMatchSink->SendGameMessage(pMatchScore[wMinInx].pUserItem,szMsg,SMT_TABLE_ROLL|SMT_CHAT|SMT_EJECT);
				}
				else if(i==wMaxInx)
				{
					m_pMatchSink->SendData(pMatchScore[wMaxInx].pUserItem, MDM_GF_FRAME, SUB_GF_USER_READY, NULL,0);
				}
				else
				{										
					m_MaybeOutUser.Add(pMatchScore[i].pUserItem);							
					m_QuitUserMap.RemoveKey(pMatchScore[i].pUserItem->GetUserID());
					m_OnMatchUserMap.RemoveKey(pMatchScore[i].pUserItem->GetUserID());
					m_UserSeatMap.RemoveKey(pMatchScore[i].pUserItem);

					m_pMatchSink->SendData(pMatchScore[i].pUserItem, MDM_GF_FRAME, SUB_GF_USER_READY, NULL,0);
				}				
			}
			SendWaitTip(pITableFrameEx->pTableFrame);
		}
		m_wFinishTable++;
		if(m_wFinishTable==m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec-1]/m_wChairCount)
		{
			if(m_wCurRountSec==m_pMatchRule->cbSecRoundCount)
			{
				//比赛结束
				DWORD dwUserID=0;
				IServerUserItem *pUserItem=NULL;
				tagMatchScore *pScore = new tagMatchScore[m_OnMatchUserMap.GetCount()];
				ZeroMemory(pScore,sizeof(tagMatchScore)*m_OnMatchUserMap.GetCount());
				WORD wCount=SortMapUser(pScore);
				for (WORD i=0;i<wCount;i++)
				{
					m_pMatchSink->WriteUserAward(pScore[i].pUserItem, i,m_dwMatchNO,NULL,this);
					m_UserSeatMap.RemoveKey(pScore[i].pUserItem);
					m_OnMatchUserMap.RemoveKey(pScore[i].pUserItem->GetUserID());
				}
				
				SafeDeleteArray(pScore);
				MatchOver();
			}
			else //下一轮
			{
				INT_PTR wCount=m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec]-m_OnMatchUserMap.GetCount();
				INT_PTR nMaybeCount=m_MaybeOutUser.GetCount();
				if(wCount>nMaybeCount)
				{
					//ASSERT(false);
					SafeDeleteArray(pMatchScore);
					MatchOver();
					return false;
				}
				tagMatchScore *pScore=new tagMatchScore[nMaybeCount];
				ZeroMemory(pScore,sizeof(tagMatchScore)*nMaybeCount);
				SortUserItemArray(pScore, m_MaybeOutUser);
				for (INT_PTR i=0;i<nMaybeCount;i++)
				{
					if(i<wCount)
					{
						m_OnMatchUserMap[pScore[i].pUserItem->GetUserID()]=pScore[i].pUserItem;
						m_QuitUserMap.RemoveKey(pScore[i].pUserItem->GetUserID());
					}
					else
					{
						TCHAR szMsg[128]=TEXT("");
						_sntprintf(szMsg, CountArray(szMsg),TEXT("很遗憾，您未能成功晋级！在本次比赛中获得第%d名。"), m_pMatchRule->arrSecRoundUserCount[m_wCurRountSec]+i+1);
						m_pMatchSink->SendGameMessage(pScore[i].pUserItem, szMsg, SMT_CHAT|SMT_TABLE_ROLL|SMT_CLOSE_GAME|SMT_EJECT);
						m_pMatchSink->SendData(pMatchScore[wMaxInx].pUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
						tagMatchScore *pOutUser=new tagMatchScore;
						ZeroMemory(pOutUser,sizeof(tagMatchScore));
						pOutUser->pUserItem=pScore[i].pUserItem;
						pOutUser->lScore=pScore[i].lScore;
						pOutUser->dwUserID=pScore[i].pUserItem->GetUserID();
						m_OutUserArray.Add(pOutUser);
					}
				}
				
				m_wCurRountSec++;
				m_enMatchStatus=MS_START_NEXT_ROUND;
				INT_PTR nOutUserCount=m_OutUserArray.GetCount();
				wCount=m_OnMatchUserMap.GetCount();
				tagMatchScore **ppOutUser=new tagMatchScore*[nOutUserCount];
				
				SortOutUserArray(ppOutUser);
				CString str;
				for (INT_PTR i=0;i<nOutUserCount;i++)
				{
					str.Format(TEXT("比赛已结束，恭喜您获得第%d名。谢谢您的参与。您可以安全关闭比赛客户端，继续报名参加其他比赛。"),
						wCount+i+1);
					if(ppOutUser[i]->pUserItem!=NULL)
					{
						m_pMatchSink->WriteUserAward(ppOutUser[i]->pUserItem,(WORD)(wCount+i),m_dwMatchNO, str,this);
						m_UserSeatMap.RemoveKey(ppOutUser[i]->pUserItem);
						m_pMatchSink->SendData(ppOutUser[i]->pUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,NULL,0);
					}
				}
				for (INT_PTR i=0;i<m_OutUserArray.GetCount();i++)
				{
					tagMatchScore *pOutUser=m_OutUserArray.GetAt(i);
					SafeDelete(m_OutUserArray.GetAt(i));
				}
				m_OutUserArray.RemoveAll();
				m_MaybeOutUser.RemoveAll();
				SafeDeleteArray(ppOutUser);
				SafeDeleteArray(pScore);
				
			}
			m_wFinishTable=0;
		}

		SafeDeleteArray(pMatchScore);
	}
	else
	{
		m_WaitStartTable.Add(pITableFrameEx);
	}

	return true;
}

//获取用户
IServerUserItem * CChampionshipMatch::GetSeatUserItem(DWORD dwSeatID)
{
	//定义变量
	IServerUserItem *pIServerUserItem=NULL;
	DWORD dwUserSeat=0;
	POSITION pos=m_UserSeatMap.GetStartPosition();

	//遍历容器
	while(pos!=NULL)
	{
		m_UserSeatMap.GetNextAssoc(pos, pIServerUserItem,dwUserSeat);
		if(dwUserSeat==dwSeatID)
		{
			ASSERT(pIServerUserItem!=NULL);
			if (pIServerUserItem==NULL) return NULL;

			IServerUserItem *pUserTemp=NULL;
            if (m_OnMatchUserMap.Lookup(pIServerUserItem->GetUserID(),pUserTemp)==FALSE)
			{
				CString str;
				str.Format(TEXT("获取座位上用户出错，用户(%s,%d桌%d椅,用户状态:%d)已经不在本场比赛中%d桌%d椅"),pIServerUserItem->GetNickName(),LOWORD(dwSeatID),HIWORD(dwSeatID),
					pIServerUserItem->GetUserStatus(),pIServerUserItem->GetTableID(),pIServerUserItem->GetChairID());
				CTraceService::TraceString(str,TraceLevel_Exception);
				m_UserSeatMap.RemoveKey(pIServerUserItem);
				WORD wTableID=pIServerUserItem->GetTableID();
				if(wTableID!=NULL)
				{
					for(WORD i=0; i<m_wTableCount; i++)
					{
						if(wTableID==m_pAssignTable[i].pTableFrame->GetTableID())
						{
							m_pAssignTable[i].pTableFrame->PerformStandUpAction(pIServerUserItem);
							break;
						}
					}
				}
				throw 0;
			}
			return pIServerUserItem;
		}
	}

	return NULL;
}

//用户坐下
bool CChampionshipMatch::OnActionUserSitDown(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!bLookonUser)
	{
		WORD i;
		for(i=0; i<m_wTableCount; i++)
		{
			if(m_pAssignTable[i].pTableFrame->GetTableID()==wTableID)
			{
				m_pAssignTable[i].cbOnSeatUserCount++;
				break;
			}
		}
		if(i>=m_wTableCount)
		{
			CTraceService::TraceString(TEXT("用户坐下的桌子不在本场比赛中。"),TraceLevel_Exception);
			return false;
		}
		IServerUserItem *pUserItem=NULL;
		if(!m_OnMatchUserMap.Lookup(pIServerUserItem->GetUserID(),pUserItem))
		{
			CString str;
			str.Format(TEXT("发生严重错误，[%s]不是本场比赛的玩家坐在本场比赛的桌子中,桌子号：%d(%d)，椅子号：%d(%d),MatcStatus:%d,MatchRound:%d"),
				pIServerUserItem->GetNickName(),pIServerUserItem->GetTableID(),wTableID,pIServerUserItem->GetChairID(),wChairID,m_enMatchStatus,m_wCurRountSec);
			throw str;
		}
		m_UserSeatMap[pIServerUserItem]=MAKELONG(wTableID,wChairID);
	}
	return true;
}

//用户起来
bool CChampionshipMatch::OnActionUserStandUp(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!bLookonUser)
	{
		for(WORD i=0; i<m_wTableCount; i++)
		{
			if(m_pAssignTable[i].pTableFrame->GetTableID()==wTableID)
			{
				m_pAssignTable[i].cbOnSeatUserCount--;
				break;
			}
		}
		if(pIServerUserItem!=NULL)
		{
			IServerUserItem *pUserTemp=NULL;
			if(!m_OnMatchUserMap.Lookup(pIServerUserItem->GetUserID(),pUserTemp))
				m_UserSeatMap.RemoveKey(pIServerUserItem);
		}
	}
	return true;
}

//用户同意
bool CChampionshipMatch::OnActionUserOnReady(WORD wTableID, WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//参数效验
	ASSERT(pIServerUserItem!=NULL);
	if(pIServerUserItem==NULL) return true;

	//变量定义
	IServerUserItem * pIUserItem=NULL;

	if(m_enMatchStatus==MS_STOP_OUT)
		return false;
	else if(m_enMatchStatus==MS_SECOND_ROUND)
	{
		for(WORD i=0; i<m_wTableCount; i++)
		{
			if(m_pAssignTable[i].pTableFrame->GetTableID()==wTableID)
			{
				if(m_pAssignTable[i].cbGameCount>=m_pMatchRule->cbSecGameCount)
					return false;
			}
		}
	}
	if(!m_OnMatchUserMap.Lookup(pIServerUserItem->GetUserID(),pIUserItem))
		return false;

	if(m_CanReadyUserMap.Lookup(pIServerUserItem->GetUserID(),pIUserItem))
	{
		m_CanReadyUserMap.RemoveKey(pIServerUserItem->GetUserID());
		return true;
	}
	return false;
}

//比赛结束
void CChampionshipMatch::MatchOver()
{
	for (WORD i=0; i<m_wTableCount; i++)
	{
		if(m_pAssignTable[i].pTableFrame!=NULL && m_pAssignTable[i].pTableFrame->IsGameStarted())
			m_pAssignTable[i].pTableFrame->DismissGame();
	}

	//删除用户
	POSITION pos=m_QuitUserMap.GetStartPosition();
	IServerUserItem *pLastUserItem=NULL;
	DWORD dwUserID;	
	while(pos!=NULL)
	{
		m_QuitUserMap.GetNextAssoc(pos, dwUserID, pLastUserItem);
		if(pLastUserItem!=NULL)
			m_pMatchSink->OnUserOut(pLastUserItem,NULL, 0,-1,this);
	}
	m_QuitUserMap.RemoveAll();

	pos=m_OnMatchUserMap.GetStartPosition();
	while(pos!=NULL)
	{
		m_OnMatchUserMap.GetNextAssoc(pos, dwUserID, pLastUserItem);
		if(pLastUserItem!=NULL)
			m_pMatchSink->OnUserOut(pLastUserItem,NULL, 0,-1,this);
	}
	m_OnMatchUserMap.RemoveAll();

	for(INT_PTR i=0; i<m_MaybeOutUser.GetCount(); i++)
	{
		if(m_MaybeOutUser.GetAt(i)!=NULL)
			m_pMatchSink->OnUserOut(m_MaybeOutUser.GetAt(i), NULL, 0,-1,this);
	}
	m_MaybeOutUser.RemoveAll();

	for (INT_PTR i=0;i<m_OutUserArray.GetCount();i++)
	{
		tagMatchScore *pOutUser=m_OutUserArray.GetAt(i);
		if(pOutUser!=NULL && pOutUser->pUserItem!=NULL)
		{
			m_pMatchSink->OnUserOut(pOutUser->pUserItem, NULL, 0,-1,this);
			SafeDelete(pOutUser);
		}
	}		
	m_OutUserArray.RemoveAll();
	m_UserSeatMap.RemoveAll();
	
	if(m_enMatchStatus!=MS_NULL_STATUS)
	{
		m_enMatchStatus=MS_NULL_STATUS;
		m_pMatchSink->OnEventMatchOver(this);
	}

}

//定时排名游戏结束
void CChampionshipMatch::FirstRoundRuleTimeGameEnd(ITableFramEx *pITableFrameEx)
{
	if(!m_bWillOver)
	{
		if(pITableFrameEx->cbGameCount>=m_pMatchRule->cbFirGameCount)
		{
			
		}
		else
		{
			m_WaitStartTable.Add(pITableFrameEx);
		}

		for (WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pUserItem=pITableFrameEx->pTableFrame->GetTableUserItem(j);
			if(pUserItem!=NULL)
			{
				LPCTSTR lpStr=TEXT("本局已结束，请稍候，系统配桌中...");
				m_pMatchSink->SendGameMessage(pUserItem,lpStr, SMT_CHAT|SMT_TABLE_ROLL);
			}
		}
	}
	else
	{
		m_wPlayingTableCount--;
	}
}

//打立出局游戏结束
void CChampionshipMatch::FirstRoundRuleScoreGameEnd(ITableFramEx *pITableFrameEx,WORD wTableIndex)
{
	bool bOut=false;
	for (WORD j=0;j<m_wChairCount;j++)
	{
		IServerUserItem *pUserItem=pITableFrameEx->pTableFrame->GetTableUserItem(j);

		if(pUserItem!=NULL)
		{
			const tagUserInfo *pScore =pUserItem->GetUserInfo();
			
			if(m_enMatchStatus==MS_FIRST_ROUND)
			{
				bool bCanOut=false;
				if(pITableFrameEx->bOffline[j])
				{
					bCanOut=true;
				}
				if (pScore->lScore<m_dwCurBase*m_pMatchRule->cbLeastScore/100|| bCanOut)
				{
					TCHAR szReason[256]=TEXT("");
					_sntprintf(szReason, CountArray(szReason), TEXT("很遗憾，由于您的当前积分不足基数的%d%%，您被淘汰出局了。在本次比赛中获得第%d名。"),
						m_pMatchRule->cbLeastScore,m_OnMatchUserMap.GetCount());
					m_OnMatchUserMap.RemoveKey(pUserItem->GetUserID());
					m_UserSeatMap.RemoveKey(pUserItem); 
					m_pMatchSink->WriteUserAward(pUserItem, (WORD)m_OnMatchUserMap.GetCount(), m_dwMatchNO, szReason,this);
					pITableFrameEx->pTableFrame->PerformStandUpAction(pUserItem);
					if(pUserItem->GetTableID()!=INVALID_TABLE)
					{
						CString strTace;
						strTace.Format(TEXT("起立失败（%d）"),__LINE__);
						CTraceService::TraceString(strTace,TraceLevel_Exception);
					}
					bOut=true;
					pITableFrameEx->cbGameCount=0;
					INT_PTR nCount=m_OnMatchUserMap.GetCount();
					if(nCount<=m_pMatchRule->wRemainUserCount && m_enMatchStatus!=MS_STOP_OUT)
					{
						m_enMatchStatus=MS_STOP_OUT;
						m_WaitStartTable.RemoveAll();
						if(nCount<(INT_PTR)m_pMatchRule->dwSecRoundUserCount)
						{							
							MatchOver();
							return ;
						}
						TCHAR szMsg[256]=TEXT("");
						_sntprintf(szMsg,CountArray(szMsg),TEXT("已经达到淘汰人数，%d秒后即将结束初赛排名，确定晋级玩家！"),m_pMatchRule->wWaitTime);
						m_pMatchSink->SetGameTimer(IDI_FINISH_FIRST_ROUND,m_pMatchRule->wWaitTime*1000,1,NULL,this);
						if(m_dwWaitTime!=0)
							m_dwWaitTime=(DWORD)time(NULL);

						for(WORD wLoop=0;wLoop<m_wTableCount;wLoop++)
						{
							if(m_pAssignTable[wLoop].pTableFrame->GetTableID()==pITableFrameEx->pTableFrame->GetTableID())
								continue;
							if(m_pAssignTable[wLoop].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
								m_wPlayingTableCount++;
						}

						SendWaitTip(pITableFrameEx->pTableFrame);						

						POSITION pos=m_OnMatchUserMap.GetStartPosition();
						IServerUserItem *pUser=NULL;
						DWORD dwUserID;
						while(pos!=NULL)
						{
							m_OnMatchUserMap.GetNextAssoc(pos,dwUserID, pUser);
							m_pMatchSink->SendGameMessage(pUser,szMsg,SMT_CHAT|SMT_TABLE_ROLL);
						}	
					}								
				}
				else
				{
					LPCTSTR lpStr=TEXT("本局已结束，请稍候，系统配桌中...");
					m_pMatchSink->SendGameMessage(pUserItem,lpStr, SMT_CHAT|SMT_TABLE_ROLL);
				}
			}
		}
	}

	if(bOut)
	{
		m_wFinishTable++;
	}
	else if(pITableFrameEx->cbGameCount>=m_pMatchRule->cbFirGameCount)
	{
		ASSERT(m_enMatchStatus==MS_FIRST_ROUND);
		m_wFinishTable++;
	}	
	else if(m_enMatchStatus==MS_FIRST_ROUND) 
	{
		m_WaitStartTable.Add(pITableFrameEx);
	}
}

//用户数目
WORD CChampionshipMatch::GetSitUserCount(ITableFrame *pTableFrame)
{
	//参数效验
	ASSERT(pTableFrame!=NULL);
	if(pTableFrame==NULL) return 0;

	if(pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) return m_wChairCount;

	//变量定义
	WORD wUserCount=0;

	//数目统计
	for (WORD i=0;i<pTableFrame->GetChairCount();i++)
	{
		IServerUserItem *pUserItem=pTableFrame->GetTableUserItem(i);
		if (pUserItem!=NULL && pUserItem->GetUserStatus()<US_PLAYING) wUserCount++;
	}

	return wUserCount;
}

//获取空椅
WORD CChampionshipMatch::GetNullChairID(ITableFrame *pTableFrame)
{
	//效验参数
	ASSERT(pTableFrame!=NULL);
	if(pTableFrame==NULL) return INVALID_CHAIR;

	for(WORD i=0; i<pTableFrame->GetChairCount(); i++)
	{
		if(pTableFrame->GetTableUserItem(i)==NULL)
			return i;
	}
	return INVALID_CHAIR;
}

//发送提示
VOID CChampionshipMatch::SendWaitTip(ITableFrame *pTableFrame)
{
	//状态校验
	if(m_enMatchStatus==MS_NULL_STATUS || m_enMatchStatus==MS_SIGNUPING || m_enMatchStatus==MS_FIRST_ROUND || m_enMatchStatus==MS_START_NEXT_ROUND)
		return;

	//变量定义
	WORD wPlaying=0;
	for(WORD i=0;i<m_wTableCount;i++)
	{
 		if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY)
			wPlaying++;
		else if(m_enMatchStatus==MS_SECOND_ROUND && m_pAssignTable[i].cbGameCount<m_pMatchRule->cbSecGameCount && m_pAssignTable[i].cbGameCount>0)
			wPlaying++;
	}
	
	//变量定义
	CMD_GR_Match_Wait_Tip WaitTip;
	ZeroMemory(&WaitTip,sizeof(WaitTip));
	WaitTip.wPlayingTable=wPlaying;
	WaitTip.wUserCount=(WORD)(WORD)(m_OnMatchUserMap.GetCount()+m_MaybeOutUser.GetCount()+m_OutUserArray.GetCount());
	lstrcpyn(WaitTip.szMatchName, m_pMatchRule->szMatchName,LEN_SERVER);

	if(m_enMatchStatus==MS_STOP_OUT || m_enMatchStatus==MS_SECOND_ROUND)
	{
		for(WORD i=0;i<m_wChairCount;i++)
		{
			IServerUserItem *pTableUserItem=pTableFrame->GetTableUserItem(i);
			if(pTableUserItem==NULL) continue;
			WaitTip.wCurTableRank=GetUserRank(pTableUserItem,pTableFrame);
			if(WaitTip.wCurTableRank==INVALID_WORD) continue;

			WaitTip.wRank=GetUserRank(pTableUserItem)+1;
			WaitTip.wCurTableRank+=1;
			if(m_enMatchStatus==MS_SECOND_ROUND && WaitTip.wCurTableRank==m_wChairCount) continue;
			WaitTip.lScore=pTableUserItem->GetUserScore();

			m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
		}
	}

	for(WORD i=0;i<m_wTableCount;i++)
	{
		if(pTableFrame->GetTableID()==m_pAssignTable[i].pTableFrame->GetTableID())continue;
		if(m_pAssignTable[i].pTableFrame->GetGameStatus()>=GAME_STATUS_PLAY) continue; 
		if(m_pAssignTable[i].cbOnSeatUserCount==0) continue;
		
		for(WORD j=0;j<m_wChairCount;j++)
		{
			IServerUserItem *pTableUserItem=m_pAssignTable[i].pTableFrame->GetTableUserItem(j);
			if(pTableUserItem==NULL) continue;
			WaitTip.wCurTableRank=GetUserRank(pTableUserItem,m_pAssignTable[i].pTableFrame);
			if(WaitTip.wCurTableRank==INVALID_WORD) continue;

			WaitTip.wRank=GetUserRank(pTableUserItem)+1;
			WaitTip.wCurTableRank+=1;
			if(m_enMatchStatus==MS_SECOND_ROUND && WaitTip.wCurTableRank==m_wChairCount) continue;
			WaitTip.lScore=pTableUserItem->GetUserScore();

			m_pMatchSink->SendData(pTableUserItem,MDM_GF_FRAME,SUB_GR_MATCH_WAIT_TIP,&WaitTip,sizeof(WaitTip));
		}
	}
}