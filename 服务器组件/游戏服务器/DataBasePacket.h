#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//用户事件
#define	DBR_GR_LOGON_USERID			100									//I D 登录
#define	DBR_GR_LOGON_MOBILE			101									//手机登录
#define	DBR_GR_LOGON_ACCOUNTS		102									//帐号登录

//系统事件
#define DBR_GR_WRITE_GAME_SCORE		200									//游戏积分
#define DBR_GR_LEAVE_GAME_SERVER	201									//离开房间
#define DBR_GR_GAME_SCORE_RECORD	202									//积分记录
#define DBR_GR_MANAGE_USER_RIGHT	203									//权限管理
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//系统消息
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//敏感词

//配置事件
#define DBR_GR_LOAD_PARAMETER		300									//加载参数
#define DBR_GR_LOAD_GAME_COLUMN		301									//加载列表
#define DBR_GR_LOAD_ANDROID_USER	302									//加载机器
#define DBR_GR_LOAD_GAME_PROPERTY	303									//加载道具

//银行事件
#define DBR_GR_USER_SAVE_SCORE		400									//存入游戏币
#define DBR_GR_USER_TAKE_SCORE		401									//提取游戏币
#define DBR_GR_USER_TRANSFER_SCORE	402									//转账游戏币
#define DBR_GR_QUERY_INSURE_INFO	403									//查询银行
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    404							//查询用户

//游戏事件
#define DBR_GR_PROPERTY_REQUEST		500									//购买礼物
#define DBR_GR_GAME_FRAME_REQUEST	502									//游戏请求

//比赛事件
#define DBR_GR_MATCH_FEE			600									//比赛费用
#define DBR_GR_MATCH_START			601									//比赛开始
#define DBR_GR_MATCH_OVER			602									//比赛结束
#define DBR_GR_MATCH_REWARD			603									//比赛奖励
#define DBR_GR_MATCH_QUIT			604									//退出比赛

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//逻辑事件
#define DBO_GR_LOGON_SUCCESS		100									//登录成功
#define DBO_GR_LOGON_FAILURE		101									//登录失败

//配置事件
#define DBO_GR_GAME_PARAMETER		200									//配置信息
#define DBO_GR_GAME_COLUMN_INFO		201									//列表信息
#define DBR_GR_GAME_ANDROID_INFO	202									//机器信息
#define DBO_GR_GAME_PROPERTY_INFO	203									//道具信息

//银行命令
#define DBO_GR_USER_INSURE_INFO		300									//银行资料
#define DBO_GR_USER_INSURE_SUCCESS	301									//银行成功
#define DBO_GR_USER_INSURE_FAILURE	302									//银行失败
#define DBO_GR_USER_INSURE_USER_INFO   303								//用户资料

//游戏事件
#define DBO_GR_PROPERTY_SUCCESS		400									//道具成功
#define DBO_GR_PROPERTY_FAILURE		401									//道具失败
#define DBO_GR_GAME_FRAME_RESULT	402									//游戏结果

//比赛事件
#define DBO_GR_MATCH_EVENT_START	500									//事件范围
#define DBO_GR_MATCH_FEE_RESULT		500									//比赛费用
#define DBO_GR_MATCH_RANK			501									//比赛名次
#define DBO_GR_MATCH_QUIT_RESULT	502									//退赛结果
#define DBO_GR_MATCH_EVENT_END		599									//事件范围

//系统事件
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600									//消息结果
#define DBO_GR_SENSITIVE_WORDS		    601									//敏感词

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GR_LogonUserID
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//ID 登录
struct DBR_GR_LogonMobile
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct DBR_GR_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏积分
struct DBR_GR_WriteGameScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//进出索引

	//成绩变量
	tagVariationInfo				VariationInfo;						//提取信息
};

//离开房间
struct DBR_GR_LeaveGameServer
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwLeaveReason;						//离开原因
	DWORD							dwOnLineTimeCount;					//在线时长

	//成绩变量
	tagVariationInfo				RecordInfo;							//记录信息
	tagVariationInfo				VariationInfo;						//提取信息

	//系统信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏记录
struct DBR_GR_GameScoreRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	WORD							wUserCount;							//用户数目
	WORD							wAndroidCount;						//机器数目

	//损耗税收
	SCORE							lWasteCount;						//损耗数目
	SCORE							lRevenueCount;						//税收数目

	//统计信息
	DWORD							dwUserMemal;						//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时间

	//时间信息
	SYSTEMTIME						SystemTimeStart;					//开始时间
	SYSTEMTIME						SystemTimeConclude;					//结束时间

	//积分记录
	WORD							wRecordCount;						//记录数目
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//游戏记录
};

//存入游戏币
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账游戏币
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账游戏币
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询用户
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	DWORD							dwUserID;							//用户 I D
};

//道具请求
struct DBR_GR_PropertyRequest
{
	//购买信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;			            //使用范围 
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//用户权限
	DWORD                           dwUserRight;						//会员权限

	//系统信息
	WORD							wTableID;							//桌子号码
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//登录成功
struct DBO_GR_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团 I D
	DWORD							dwCustomID;							//自定头像
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//索引变量
	DWORD							dwInoutIndex;						//记录索引
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//手机定义
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//辅助信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szDescribeString[128];				//错误消息
};

//登录失败
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//配置信息
struct DBO_GR_GameParameter
{
	//汇率信息
	WORD							wMedalRate;							//奖牌汇率
	WORD							wRevenueRate;						//银行税收

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本
};

//列表信息
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//列表信息
};

//机器信息
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//结果代码
	WORD							wAndroidCount;						//用户数目
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//机器信息
};

//道具信息
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具信息
};

//银行资料
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来游戏币
	SCORE							lSourceInsure;						//原来游戏币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lFrozenedScore;						//冻结积分
	SCORE							lVariationScore;					//游戏币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//用户资料
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwGameID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};

//道具成功
struct DBO_GR_S_PropertySuccess
{
	//道具信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//会员权限
	DWORD                           dwUserRight;						//会员权限

	//结果信息
	SCORE							lConsumeGold;						//消费游戏币
	LONG							lSendLoveLiness;					//赠送魅力
	LONG							lRecvLoveLiness;					//接受魅力

	//会员信息
	BYTE							cbMemberOrder;						//会员等级
};

//道具失败
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//请求范围
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//比赛名次
struct DBO_GR_MatchRank
{
	BYTE							cbRank;								//比赛名次
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	LONG							lMatchScore;						//用户得分
};

//权限管理
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限
	bool							bGameRight;							//游戏权限
};

//比赛报名
struct DBR_GR_MatchFee
{
	//报名费用
	DWORD							dwUserID;							//用户ID
	DWORD							dwMatchFee;							//报名费用

	//系统信息
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwMatchID;							//比赛ID
	DWORD							dwMatchNO;							//比赛场次
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//比赛开始
struct DBR_GR_MatchStart
{
	DWORD							dwMatchID;						//比赛标识
	DWORD							dwMatchNo;						//比赛场次
	WORD							wMatchCount;					//总场次
	LONG							lInitScore;						//初始积分
};

//比赛结束
struct DBR_GR_MatchOver
{
	DWORD							dwMatchID;						//比赛标识
	DWORD							dwMatchNo;						//比赛场次
	WORD							wMatchCount;					//总场次
};

//比赛奖励
struct DBR_GR_MatchReward
{
	DWORD								dwUserID;							//用户 I D
	DWORD								dwMatchID;							//比赛 I D 
	DWORD								dwMatchNO;							//比赛场次
	WORD								wRank;								//比赛名次
	SCORE								lMatchScore;						//比赛得分
	DWORD								dwExperience;						//用户经验
	DWORD								dwGold;								//游戏币
	DWORD								dwMedal;							//用户奖牌
	DWORD								dwClientAddr;						//连接地址
};

//系统消息
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //消息ID
	BYTE							cbMessageType;						//消息类型
	BYTE                            cbAllRoom;                          //全体房间
	DWORD							dwTimeRate;						    //时间频率
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};
//////////////////////////////////////////////////////////////////////////////////

#endif