
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "wchar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg 对话框



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


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//修改主对话框的背景
	m_brBg.DeleteObject();
	m_bmBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_bg);
	m_brBg.CreatePatternBrush(&m_bmBg);

	m_startBtn.LoadBitmaps(IDB_normalBtn, IDB_pushedBtn, NULL, IDB_stopBtn);
	m_startBtn.SizeToContent();//按钮自适应位图的大小

    //初始化列表控件的列标头
	m_list.InsertColumn(0, _T("用户名"), LVCFMT_LEFT, 120);
	m_list.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("消息数"), LVCFMT_LEFT, 130);
	m_list.InsertColumn(3, _T("头像"), LVCFMT_LEFT, 60);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brBg;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//响应客户端的信息
LRESULT CServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	CMySocket *sock = (CMySocket*)wParam;
	CMySocket *c;

	SOCKADDR_IN sockAddr;
	int nSize = sizeof(sockAddr);
	BOOL res;

	switch (lParam)
	{
		//新的连接消息
	case ACCEPT:
		c = new CMySocket;
		c->AttachCWnd(this);

		res = sock->Accept(*c, (SOCKADDR *)&sockAddr, &nSize);
		if (res == FALSE)
		{
			MessageBox(_T("Accept Error!"));
		}
		break;
		//关闭连接消息
	case CLOSE:
		ClosePlayer(sock);
		break;
		//收到数据消息
	case RETURN:
		ParserPkt(sock);
		break;
	}
	return 1;
}
//用户退出，群发退出消息
void CServerDlg::ClosePlayer(CMySocket *from)
{
	int i, msg_len;
	TCHAR out_msg[200];
	msg_len = sprintf((char*)out_msg, "%s 退出聊天室\r\n", from->m_Player) + 1;
	TCHAR nbuf[100];

	m_csList.Lock();
	for (i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemData(i) == (DWORD)from)
		{
			delete from;
			nbuf[0] = 0x41;//第一个字节表示类型，加入，退出，私聊群发
			m_list.GetItemText(i, 0, nbuf + 1, 100);

			m_list.DeleteItem(i);
			break;
		}
	}
	//群发退出消息
	for (i = 0; i < m_list.GetItemCount(); i++)
	{
		CMySocket *s;
		s = (CMySocket*)m_list.GetItemData(i);

		s->Send(nbuf, 100);
	}
	m_csList.Unlock();

	Append(out_msg);
}
//追加消息体
void CServerDlg::Append(TCHAR *msg)
{
	//读取消息框中所有的消息
	m_showMsg += msg;
	UpdateData(FALSE);
}
//解析收到的数据,然后插到列表项，显示相关信息，群发所有用户和私聊
void CServerDlg::ParserPkt(CMySocket *from)
{
	char SendBuff[4096];		//发送缓冲
	char ShowBuff[4096];		//显示缓冲
	char nbuf[100];				//临时缓冲区

								//初始化各缓冲区
	memset(SendBuff, 0, 4096);
	memset(ShowBuff, 0, 4096);
	memset(nbuf, 0, 100);

	int len;					//记录发送长度
	int item;					//列表序号
	TCHAR pic[2];				//图像序号		
	CMySocket *s1;				//发送一般消息的Socket		
	CMySocket *s;				//发送用户进入信息的Socket


								//读取数据
	len = from->Receive(SendBuff, 4096);

	//0---连接关闭，-1---发送错误
	if (len < 1)
	{
		AfxMessageBox(_T("消息解析--接收消息Error"));
		return;
	}

	//0x11---服务器接收用户进入聊天室
	if (SendBuff[0] == 0x11)
	{
		CString ipaddr;				//IP字符串	
		UINT port;					//端口号

		//取得与该套接字链接的对方的IP地址、端口号
		from->GetPeerName(ipaddr, port);
		//与SOCKET通信的用户的称谓
		from->m_Player = SendBuff + 2;//名称的记录会在为空时停止

		//头像序号处理
		pic[0] = SendBuff[1] + 0x30;	//找到字符‘1’的位置 '1'的ascii为0x31
		pic[1] = '\0';

		//向列表中插入一项
		m_csList.Lock();

		//列表中插入一行，返回插入的行号
		item = m_list.InsertItem(0, SendBuff + 2);
		//保存该SOCKET指针到该行的附加数据域
		m_list.SetItemData(item, (DWORD)from);
		//设置第2列，IP地址列
		m_list.SetItemText(item, 1, ipaddr);
		//设置第4列，用户头像
		m_list.SetItemText(item, 3, pic);

		m_csList.Unlock();

		//s1中保存新加入用户的socket
		s1 = (CMySocket*)m_list.GetItemData(item);

		//通知所有用户有新用户加入
		m_csList.Lock();

		for (item = 0; item < m_list.GetItemCount(); item++)
		{
			s = (CMySocket*)m_list.GetItemData(item);
			len = sprintf(ShowBuff, " %s 进入聊室\r\n", from->m_Player);

			Sleep(200);

			//发送图标号和用户名
			if (s != from)
			{
				//发送指令为0x11的命令
				s->Send(SendBuff, len + 1);

				//向新用户发送已登录用户信息

				//获取图标号
				m_list.GetItemText(item, 3, &pic[0], 2);
				//0x31---已登录用户信息，
				nbuf[0] = 0x31;
				nbuf[1] = pic[0];
				//获取用户名
				m_list.GetItemText(item, 0, nbuf + 2, 100);
				len = strlen(nbuf);
				s1->Send(nbuf, len + 1);		//发送用户名+头像号
			}
			else
			{
				//新用户消息加1
				char tot[10];

				sprintf(tot, "%u", from->m_Total);
				//设置第2列，消息数列
				m_list.SetItemText(item, 2, tot);
			}
		}

		m_csList.Unlock();
	}
	//私聊信息---不同的消息格式
	else if (SendBuff[0] == 0x51)	//根据用户名转发了
	{
		TCHAR pName[100], bName[100];
		memset(pName, 0, 100);
		memset(bName, 0, 100);

		//按照私聊的用户名查找
		strcpy(pName, SendBuff + 1);
		for (item = 0; item < m_list.GetItemCount(); item++)
		{
			m_list.GetItemText(item, 0, bName, 100);

			if (0 == strcmp(pName, bName))
			{
				s = (CMySocket*)m_list.GetItemData(item);
				//同样直接发送了文本
				s->Send(SendBuff + 99, len);
			}
		}
	}
	//公聊信息
	else	//向所有用户转发消息	
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
				m_list.SetItemText(item, 2, tot);				//设置第2列，消息数列
			}
		}
		memcpy(ShowBuff, SendBuff + 1, 4096);

		m_csList.Unlock();
	}

	//无论怎样都会有信息被写，包括2类：用户进入、私聊信息、不记录公聊天信息
	Append(ShowBuff);
}

//开启服务器按钮，创建Socket
void CServerDlg::OnBnClickedStartbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	m_socket.AttachCWnd(this);
	BOOL isTrue = m_socket.Create(0x8123, SOCK_STREAM);
	if (isTrue)
	{
		m_socket.Listen();
		AfxMessageBox("开启服务器成功！");
		GetDlgItem(IDC_StartBtn)->EnableWindow(FALSE);
		return;
	}
	AfxMessageBox("不好意思，出了点问题...");
}
