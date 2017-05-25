#include "stdafx.h"
#include "MySocket.h"


CMySocket::CMySocket()
{
	pWnd = nullptr;
	m_msgTotal = 0;
}


CMySocket::~CMySocket()
{
}


void CMySocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pWnd) {
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, CLOSE);
	}
	CSocket::OnClose(nErrorCode);
}


void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_msgTotal++;
	if (pWnd) {
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, RETURN);
	}
	CSocket::OnReceive(nErrorCode);
}

void CMySocket::AttachCWnd(CWnd* cwnd) {
	pWnd = cwnd;
}
