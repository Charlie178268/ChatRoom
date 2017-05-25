#pragma once
#include "afxsock.h"
//�Զ�����Ϣ�������WM_USER
#define SOCKET_EVENT WM_USER + 1001  
//ö����Ϣ����
enum { ACCEPT = 0, SEND = 1, RETURN = 2, CLOSE = 3 };

class CMySocket :
	public CSocket
{
public:
	CMySocket();
	~CMySocket();
	void AttachCWnd(CWnd *pWnd);//��Socket����������--socket�б����˴��ڵ�ָ��
	CWnd *pWnd;//�����������
	ULONG	m_Total;//������Ϣ��������
	CString m_Player;//�����û���
	//����CSocket�����������
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

