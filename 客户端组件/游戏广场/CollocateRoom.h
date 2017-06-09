#pragma once


// CCollocateRoom dialog

class CCollocateRoom : public CSkinDialog
{
	DECLARE_DYNAMIC(CCollocateRoom)

public:
	CCollocateRoom();
	virtual ~CCollocateRoom();

	//配置变量
protected:
	CParameterGame *				m_pParameterGame;					//游戏配置
	CParameterServer *				m_pParameterServer;					//房间配置

	//控件变量
protected:
	CSkinButton						m_btOk;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

	//胜率限制
public:
	WORD							m_wMinWinRate;						//最低胜率
	bool							m_bLimitWinRate;					//限制胜率

	//逃率限制
public:
	WORD							m_wMaxFleeRate;						//最高逃跑
	bool							m_bLimitFleeRate;					//限制断线

	//积分限制
public:
	LONG							m_lMaxGameScore;					//最高分数 
	LONG							m_lMinGameScore;					//最低分数
	bool							m_bLimitGameScore;					//限制分数

	//其他配置
public:
	bool							m_bLimitDetest;						//限制玩家
	bool							m_bLimitSameIP;						//效验地址

	//携带密码
public:
	bool							m_bTakePassword;					//携带密码
	TCHAR							m_szPassword[LEN_PASSWORD];			//桌子密码

	//编辑控件
protected:
	CSkinEdit						m_edWinRate;						//用户胜率
	CSkinEdit						m_edFleeRate;						//用户逃率
	CSkinEdit						m_edScoreMin;						//最低积分
	CSkinEdit						m_edScoreMax;						//最高积分
	CSkinEdit						m_edPassword;						//桌子密码

	//功能函数
public:
	//配置参数
	bool InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer);

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
