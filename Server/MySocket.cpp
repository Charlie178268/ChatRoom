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
	// TODO: 在此添加专用代码和/或调用基类
	if (pWnd) {
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, ACCEPT);
	}

	CSocket::OnAccept(nErrorCode);
}


void CMySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pWnd)
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, CLOSE);
		//该函数将指定的消息发送到一个或多个窗口。此函数为指定的窗口调用窗口程序，直到窗口程序处理完消息再返回
	CSocket::OnClose(nErrorCode);
}


void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_Total++;
	if (pWnd)
		pWnd->SendMessage(SOCKET_EVENT, (WPARAM)this, RETURN);

	CSocket::OnReceive(nErrorCode);
}

void CMySocket::AttachCWnd(CWnd *pW)
{
	pWnd = pW;
}
