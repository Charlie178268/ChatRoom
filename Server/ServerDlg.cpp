
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "wchar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg �Ի���



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, m_showMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_StartBtn, m_startBtn);
	DDX_Control(pDX, IDC_LISTUSER, m_list);
	DDX_Text(pDX, IDC_EDITSHOWMSG, m_showMsg);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_StartBtn, &CServerDlg::OnBnClickedStartbtn)
	ON_MESSAGE(SOCKET_EVENT, OnSocket)
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//�޸����Ի���ı���
	m_brBg.DeleteObject();
	m_bmBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_bg);
	m_brBg.CreatePatternBrush(&m_bmBg);

	m_startBtn.LoadBitmaps(IDB_normalBtn, IDB_pushedBtn, NULL, IDB_stopBtn);
	m_startBtn.SizeToContent();//��ť����Ӧλͼ�Ĵ�С

    //��ʼ���б�ؼ����б�ͷ
	m_list.InsertColumn(0, _T("�û���"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("��Ϣ��"), LVCFMT_LEFT, 130);
	m_list.InsertColumn(3, _T("ͷ��"), LVCFMT_LEFT, 60);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brBg;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//��Ӧ�ͻ��˵���Ϣ
LRESULT CServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CMySocket *sock = (CMySocket*)wParam;
	CMySocket *c;

	SOCKADDR_IN sockAddr;
	int nSize = sizeof(sockAddr);
	BOOL res;

	switch (lParam)
	{
		//�µ�������Ϣ
	case ACCEPT:
		c = new CMySocket;
		c->AttachCWnd(this);

		res = sock->Accept(*c, (SOCKADDR *)&sockAddr, &nSize);
		if (res == FALSE)
		{
			MessageBox(_T("Accept Error!"));
		}
		break;
		//�ر�������Ϣ
	case CLOSE:
		ClosePlayer(sock);
		break;
		//�յ�������Ϣ
	case RETURN:
		ParserPkt(sock);
		break;
	}
	return 1;
}
//�û��˳���Ⱥ���˳���Ϣ
void CServerDlg::ClosePlayer(CMySocket *from)
{
	int i, msg_len;
	TCHAR out_msg[200];
	msg_len = sprintf((char*)out_msg, "%s �˳�������\r\n", from->m_Player) + 1;
	TCHAR nbuf[100];

	m_csList.Lock();
	for (i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemData(i) == (DWORD)from)
		{
			delete from;
			nbuf[0] = 0x41;//��һ���ֽڱ�ʾ���ͣ����룬�˳���˽��Ⱥ��
			m_list.GetItemText(i, 0, nbuf + 1, 100);

			m_list.DeleteItem(i);
			break;
		}
	}
	//Ⱥ���˳���Ϣ
	for (i = 0; i < m_list.GetItemCount(); i++)
	{
		CMySocket *s;
		s = (CMySocket*)m_list.GetItemData(i);

		s->Send(nbuf, 100);
	}
	m_csList.Unlock();

	Append(out_msg);
}
//׷����Ϣ��
void CServerDlg::Append(TCHAR *msg)
{
	//��ȡ��Ϣ�������е���Ϣ
	m_showMsg += msg;
	UpdateData(FALSE);
}
//�����յ�������,Ȼ��嵽�б����ʾ�����Ϣ��Ⱥ�������û���˽��
void CServerDlg::ParserPkt(CMySocket *from)
{
	char SendBuff[4096];		//���ͻ���
	char ShowBuff[4096];		//��ʾ����
	char nbuf[100];				//��ʱ������

								//��ʼ����������
	memset(SendBuff, 0, 4096);
	memset(ShowBuff, 0, 4096);
	memset(nbuf, 0, 100);

	int len;					//��¼���ͳ���
	int item;					//�б����
	TCHAR pic[2];				//ͼ�����		
	CMySocket *s1;				//����һ����Ϣ��Socket		
	CMySocket *s;				//�����û�������Ϣ��Socket


								//��ȡ����
	len = from->Receive(SendBuff, 4096);

	//0---���ӹرգ�-1---���ʹ���
	if (len < 1)
	{
		AfxMessageBox(_T("��Ϣ����--������ϢError"));
		return;
	}

	//0x11---�����������û�����������
	if (SendBuff[0] == 0x11)
	{
		CString ipaddr;				//IP�ַ���	
		UINT port;					//�˿ں�

		//ȡ������׽������ӵĶԷ���IP��ַ���˿ں�
		from->GetPeerName(ipaddr, port);
		//��SOCKETͨ�ŵ��û��ĳ�ν
		from->m_Player = SendBuff + 2;//���Ƶļ�¼����Ϊ��ʱֹͣ

		//ͷ����Ŵ���
		pic[0] = SendBuff[1] + 0x30;	//�ҵ��ַ���1����λ�� '1'��asciiΪ0x31
		pic[1] = '\0';

		//���б��в���һ��
		m_csList.Lock();

		//�б��в���һ�У����ز�����к�
		item = m_list.InsertItem(0, SendBuff + 2);
		//�����SOCKETָ�뵽���еĸ���������
		m_list.SetItemData(item, (DWORD)from);
		//���õ�2�У�IP��ַ��
		m_list.SetItemText(item, 1, ipaddr);
		//���õ�4�У��û�ͷ��
		m_list.SetItemText(item, 3, pic);

		m_csList.Unlock();

		//s1�б����¼����û���socket
		s1 = (CMySocket*)m_list.GetItemData(item);

		//֪ͨ�����û������û�����
		m_csList.Lock();

		for (item = 0; item < m_list.GetItemCount(); item++)
		{
			s = (CMySocket*)m_list.GetItemData(item);
			len = sprintf(ShowBuff, " %s ��������\r\n", from->m_Player);

			Sleep(200);

			//����ͼ��ź��û���
			if (s != from)
			{
				//����ָ��Ϊ0x11������
				s->Send(SendBuff, len + 1);

				//�����û������ѵ�¼�û���Ϣ

				//��ȡͼ���
				m_list.GetItemText(item, 3, &pic[0], 2);
				//0x31---�ѵ�¼�û���Ϣ��
				nbuf[0] = 0x31;
				nbuf[1] = pic[0];
				//��ȡ�û���
				m_list.GetItemText(item, 0, nbuf + 2, 100);
				len = strlen(nbuf);
				s1->Send(nbuf, len + 1);		//�����û���+ͷ���
			}
			else
			{
				//���û���Ϣ��1
				char tot[10];

				sprintf(tot, "%u", from->m_Total);
				//���õ�2�У���Ϣ����
				m_list.SetItemText(item, 2, tot);
			}
		}

		m_csList.Unlock();
	}
	//˽����Ϣ---��ͬ����Ϣ��ʽ
	else if (SendBuff[0] == 0x51)	//�����û���ת����
	{
		TCHAR pName[100], bName[100];
		memset(pName, 0, 100);
		memset(bName, 0, 100);

		//����˽�ĵ��û�������
		strcpy(pName, SendBuff + 1);
		for (item = 0; item < m_list.GetItemCount(); item++)
		{
			m_list.GetItemText(item, 0, bName, 100);

			if (0 == strcmp(pName, bName))
			{
				s = (CMySocket*)m_list.GetItemData(item);
				//ͬ��ֱ�ӷ������ı�
				s->Send(SendBuff + 99, len);
			}
		}
	}
	//������Ϣ
	else	//�������û�ת����Ϣ	
	{
		m_csList.Lock();

		for (item = 0; item < m_list.GetItemCount(); item++)
		{
			s = (CMySocket*)m_list.GetItemData(item);
			s->Send(SendBuff, len);

			if (s == from)
			{
				TCHAR tot[10];
				sprintf(tot, "%u", from->m_Total);
				m_list.SetItemText(item, 2, tot);				//���õ�2�У���Ϣ����
			}
		}
		memcpy(ShowBuff, SendBuff + 1, 4096);

		m_csList.Unlock();
	}

	//����������������Ϣ��д������2�ࣺ�û����롢˽����Ϣ������¼��������Ϣ
	Append(ShowBuff);
}

//������������ť������Socket
void CServerDlg::OnBnClickedStartbtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_socket.AttachCWnd(this);
	BOOL isTrue = m_socket.Create(0x8123, SOCK_STREAM);
	if (isTrue)
	{
		m_socket.Listen();
		AfxMessageBox("�����������ɹ���");
		GetDlgItem(IDC_StartBtn)->EnableWindow(FALSE);
		return;
	}
	AfxMessageBox("������˼�����˵�����...");
}
