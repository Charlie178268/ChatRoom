
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_showMsg(_T(""))
	, m_sendMsg(_T(""))
	, m_chatOneByOne(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_showMsg);
	DDX_Text(pDX, IDC_EDIT2, m_sendMsg);
	DDX_Check(pDX, IDC_CHECK1, m_chatOneByOne);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(SOCKET_EVENT, OnSocket)
	ON_BN_CLICKED(IDC_SendBtn, &CClientDlg::OnBnClickedSendbtn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CClientDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
tryagain:
	//先弹出登录对话框
	CLogDLG ld;
	if (ld.DoModal() != IDOK) {//没有按确认建
							   //退出程序
		PostQuitMessage(0);
		return TRUE;
	}
	//输入信息校验
	if (strlen(ld.m_ipAddr) == 0) {
		MessageBox(_T("请输入IP地址！"));
		goto tryagain;
	}

	if (ld.m_username.IsEmpty()) {
		MessageBox(_T("请输入用户名！"));
		goto tryagain;
	}

	m_username = ld.m_username;
	char im = ld.m_imgNum + 1;

	CString	msg;
	DWORD	err;

	//把SOCKET与对话框联系起来，SOCKET有消息就通知本对话框
	m_socket.AttachCWnd(this);

	if (m_socket.Create() == FALSE)			//自动完成SOCKET的初始化、设置
	{
		err = GetLastError();
		msg.Format(_T("创建Socket失败!\r\n错误代码:%d"), err);//sprintf相同
		goto msgbox;
	}


	//设置对话框的标题     
	SetWindowText(_T("正在连接到服务器..."));

	//连接到服务器计算机，端口为0x8123的程序
	if (m_socket.Connect((LPCTSTR)ld.m_ipAddr, 0x8123) == FALSE)
	{
		//如果连接成功，对方创建新的SOCKET，新的端口号，就与新的SOCKET，新的端口号通信。
		err = GetLastError();
		msg.Format(_T("连接服务器失败! \r\n错误代码:%d"), err);

	msgbox:
		MessageBox(msg);
		PostQuitMessage(0);           //退出
		return TRUE;
	}

	char	pkt[200];
	sprintf_s(pkt, "%s 已连接到服务器!", m_username);

	//再次设置对话框的标题
	SetWindowText(pkt);

	Sleep(1000);

	//构造协议信息

	//登入聊天室,给服务器发送用户登录信息
	pkt[0] = 0x11;
	pkt[1] = im;//头像号码
	strncpy(pkt + 2, m_username, 98);      

	 //发送
	int l = strlen(pkt) + 1;
	if (m_socket.Send(pkt, l) == FALSE)
	{
		MessageBox(_T("发送数据错误!"));
	}

	//设置对话框背景
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_bg);
	m_brBg.CreatePatternBrush(&m_bmBg);
	//载入图标头像
	HICON myIcon[6];
	int i;
	for (i = 0; i<6; i++)
	{
		myIcon[i] = AfxGetApp()->LoadIcon(IDI_ICON1 + i);
	}

	//创建图像列表
	m_imageList.Create(32, 32, ILC_COLOR32, 6, 6);
	for (i = 0; i < 6; i++)
	{
		m_imageList.Add(myIcon[i]);
	}

	m_list.SetImageList(&m_imageList, LVSIL_SMALL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Socket消息响应函数
LRESULT CClientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	char	pkt[4096];
	memset(pkt, 0, 4096);

	LVFINDINFO   info;
	LVITEM lvitem;

	switch (lParam)
	{
	case RETURN:
		m_socket.Receive(pkt, 4096);

		switch (pkt[0])
		{
		case 0x11:
			//连接信息
			pName[curNum] = pkt + 2;
			curNum++;
			m_showMsg += pkt + 2;
			m_showMsg += " 进入聊室。\r\n";

			lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvitem.iItem = curNum;
			lvitem.pszText = pkt + 2;
			lvitem.iImage = pkt[1] - 1;
			lvitem.iSubItem = 0;

			m_list.InsertItem(&lvitem);

			break;
			//已加入用户信息
		case 0x31:
			pName[curNum] = pkt + 2;
			curNum++;

			lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvitem.iItem = curNum;
			lvitem.pszText = pkt + 2;
			lvitem.iImage = pkt[1] - 0x31;
			lvitem.iSubItem = 0;

			m_list.InsertItem(&lvitem);
			break;
		//退出
		case 0x41:
			//pkt + 1保存的是用户名
			m_showMsg += pkt + 1;
			m_showMsg += " 退出聊室\r\n";

			info.flags = LVFI_PARTIAL | LVFI_STRING;
			info.psz = pkt + 1;
			int item;
			item = m_list.FindItem(&info);

			if (item != -1)
			{
				m_list.DeleteItem(item);
			}
			break;
		default:
			//对于没有任何命令的消息，直接显示在消息框中
			m_showMsg += pkt + 1;
		}

		UpdateData(false);
		break;

	case CLOSE:
		MessageBox("服务器已关闭!");
		break;
	}
	return 1;
}

void CClientDlg::OnBnClickedSendbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	char pkt[4096];
	memset(pkt, 0, sizeof(pkt));

	int			len;

	if (m_chatOneByOne)
	{
		//私聊信息
		pkt[0] = 0x51;
		strcpy(pkt + 1, pName[m_curIndex]);
		len = sprintf(pkt + 100, "私聊：%s：%s\r\n", m_username, m_sendMsg);
		m_socket.Send(pkt, len + 100);

		sprintf(pkt + 100, "私聊：对%s说：%s\r\n", pName[m_curIndex], m_sendMsg);
		m_showMsg += pkt + 100;
		m_sendMsg.Empty();
	}
	else
	{
		//群发信息
		pkt[0] = 0x21;										//填入功能码0x21
		len = sprintf(pkt + 1, "%s说: %s\r\n", m_username, m_sendMsg);	//len是长度
		m_socket.Send(pkt, len + 3);							//字符串以零结尾。多发两个零

		m_sendMsg.Empty();
	}
	UpdateData(FALSE);
}


void CClientDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_curIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (m_curIndex == -1)
	{
		AfxMessageBox("还没有选择私聊的用户...");
		return;
	}

	m_chatOneByOne = true;
	UpdateData(false);

	*pResult = 0;
}
