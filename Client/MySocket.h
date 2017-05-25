#pragma once
#define SOCKET_EVENT WM_USER+1001
enum {ACCEPT=0, SEND=1, RETURN=2, CLOSE=3};//定义消息类型枚举常量
class CMySocket :
	public CSocket
{
public:
	CMySocket();
	~CMySocket();

	void AttachCWnd(CWnd* cwnd);
	CWnd *pWnd;//保存关联窗口
	ULONG m_msgTotal;//消息发送条数
	CString m_player;//保存好友名
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

