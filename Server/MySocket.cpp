#include "stdafx.h"
#include "MySocket.h"


CMySocket::CMySocket()
{
	pWnd = NULL;
	m_Total = 0;
}


CMySocket::~CMySocket()
{
}

void CMySocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pWnd) {
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, ACCEPT);
	}

	CSocket::OnAccept(nErrorCode);
}


void CMySocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pWnd)
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, CLOSE);
		//�ú�����ָ������Ϣ���͵�һ���������ڡ��˺���Ϊָ���Ĵ��ڵ��ô��ڳ���ֱ�����ڳ���������Ϣ�ٷ���
	CSocket::OnClose(nErrorCode);
}


void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_Total++;
	if (pWnd)
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, RETURN);

	CSocket::OnReceive(nErrorCode);
}

void CMySocket::AttachCWnd(CWnd *pW)
{
	pWnd = pW;
}
