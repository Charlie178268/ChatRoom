#pragma once
#include "afxsock.h"
//自定义消息必须大于WM_USER
#define SOCKET_EVENT WM_USER + 1001  
//枚举消息类型
enum { ACCEPT = 0, SEND = 1, RETURN = 2, CLOSE = 3 };

class CMySocket :
	public CSocket
{
public:
	CMySocket();
	~CMySocket();
	void AttachCWnd(CWnd *pWnd);//将Socket与操作窗体绑定--socket中保存了窗口的指针
	CWnd *pWnd;//保存关联窗口
	ULONG	m_Total;//保存消息发送条数
	CString m_Player;//保存用户名
	//重载CSocket类的三个函数
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

