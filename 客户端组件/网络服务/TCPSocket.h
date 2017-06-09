#ifndef TCP_SOCKET_HEAD_FILE
#define TCP_SOCKET_HEAD_FILE

#pragma once

#include "WinSock2.h"
#include "WHSocketHead.h"

//////////////////////////////////////////////////////////////////////////

//TCP 网络连接
class CTCPSocket : public CWnd, public ITCPSocket
{
	//辅助变量
protected:
	WORD							m_wSocketID;						//网络标识
	BYTE							m_cbSocketStatus;					//网络状态

	//内核变量
protected:
	SOCKET							m_hSocket;							//连接句柄
	ITCPSocketSink *				m_pITCPSocketSink;					//回调接口

	//代理信息
protected:
	BYTE							m_ProxyServerType;					//代理类型
	tagProxyServer					m_ProxyServerInfo;					//代理信息

	//接收变量
protected:
	WORD							m_wRecvSize;						//接收长度
	BYTE							m_cbRecvBuf[SOCKET_TCP_BUFFER*10];	//接收缓冲

	//缓冲变量
protected:
	bool							m_bNeedBuffer;						//缓冲状态
	DWORD							m_dwBufferData;						//缓冲数据
	DWORD							m_dwBufferSize;						//缓冲大小
	LPBYTE							m_pcbDataBuffer;					//缓冲数据

	//加密数据
protected:
	BYTE							m_cbSendRound;						//字节映射
	BYTE							m_cbRecvRound;						//字节映射
	DWORD							m_dwSendXorKey;						//发送密钥
	DWORD							m_dwRecvXorKey;						//接收密钥

	//计数变量
protected:
	DWORD							m_dwSendTickCount;					//发送时间
	DWORD							m_dwRecvTickCount;					//接收时间
	DWORD							m_dwSendPacketCount;				//发送计数
	DWORD							m_dwRecvPacketCount;				//接受计数

	//函数定义
public:
	//构造函数
	CTCPSocket();
	//析构函数
	virtual ~CTCPSocket();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//标识接口
public:
	//获取标识
	virtual WORD GetSocketID() { return m_wSocketID;}
	//设置标识
	virtual VOID SetSocketID(WORD wSocketID) { m_wSocketID=wSocketID; }

	//配置接口
public:
	//设置接口
	virtual bool SetTCPSocketSink(IUnknownEx * pIUnknownEx);
	//获取接口
	virtual VOID * GetTCPSocketSink(const IID & Guid, DWORD dwQueryVer);

	//信息接口
public:
	//获取状态
	virtual BYTE GetSocketStatus() { return m_cbSocketStatus; }
	//发送间隔
	virtual DWORD GetLastSendTick() { return m_dwSendTickCount; }
	//接收间隔
	virtual DWORD GetLastRecvTick() { return m_dwRecvTickCount; }
	//发送数目
	virtual DWORD GetSendPacketCount() { return m_dwSendPacketCount; }
	//接收数目
	virtual DWORD GetRecvPacketCount() { return m_dwRecvPacketCount; }

	//代理接口
public:
	//代理测试
	virtual BYTE ProxyServerTesting();
	//设置代理
	virtual bool SetProxyServerInfo(BYTE ProxyServerType, const tagProxyServer & ProxyServer);

	//操作接口
public:
	//关闭连接
	virtual VOID CloseSocket();
	//连接操作
	virtual BYTE Connect(DWORD dwServerIP, WORD wPort);
	//连接操作
	virtual BYTE Connect(LPCTSTR pszServerIP, WORD wPort);
	//发送函数
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual WORD SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//辅助函数
protected:
	//连接代理
	DWORD ConnectProxyServer();
	//地址解释
	DWORD TranslateAddr(LPCTSTR pszServerAddr);

	//辅助函数
protected:
	//关闭连接
	VOID CloseSocket(BYTE cbShutReason);
	//缓冲数据
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);
	//发送数据
	DWORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);

	//加密函数
protected:
	//解密数据
	WORD CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize);
	//加密数据
	WORD EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize);

	//内联函数
private:
	//字节映射
	inline WORD SeedRandMap(WORD wSeed);
	//发送映射
	inline BYTE MapSendByte(BYTE cbData);
	//接收映射
	inline BYTE MapRecvByte(BYTE cbData);

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//网络消息
	LRESULT	OnSocketNotify(WPARAM wParam, LPARAM lParam);

	//消息函数
protected:
	//网络读取
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//网络发送
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//网络关闭
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//网络连接
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif