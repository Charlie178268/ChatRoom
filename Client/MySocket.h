#pragma once
#define SOCKET_EVENT WM_USER+1001
enum {ACCEPT=0, SEND=1, RETURN=2, CLOSE=3};//������Ϣ����ö�ٳ���
class CMySocket :
	public CSocket
{
public:
	CMySocket();
	~CMySocket();

	void AttachCWnd(CWnd* cwnd);
	CWnd *pWnd;//�����������
	ULONG m_msgTotal;//��Ϣ��������
	CString m_player;//���������
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

