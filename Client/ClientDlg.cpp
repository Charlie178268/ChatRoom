
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClientDlg �Ի���



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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
tryagain:
	//�ȵ�����¼�Ի���
	CLogDLG ld;
	if (ld.DoModal() != IDOK) {//û�а�ȷ�Ͻ�
							   //�˳�����
		PostQuitMessage(0);
		return TRUE;
	}
	//������ϢУ��
	if (strlen(ld.m_ipAddr) == 0) {
		MessageBox(_T("������IP��ַ��"));
		goto tryagain;
	}

	if (ld.m_username.IsEmpty()) {
		MessageBox(_T("�������û�����"));
		goto tryagain;
	}

	m_username = ld.m_username;
	char im = ld.m_imgNum + 1;

	CString	msg;
	DWORD	err;

	//��SOCKET��Ի�����ϵ������SOCKET����Ϣ��֪ͨ���Ի���
	m_socket.AttachCWnd(this);

	if (m_socket.Create() == FALSE)			//�Զ����SOCKET�ĳ�ʼ��������
	{
		err = GetLastError();
		msg.Format(_T("����Socketʧ��!\r\n�������:%d"), err);//sprintf��ͬ
		goto msgbox;
	}


	//���öԻ���ı���     
	SetWindowText(_T("�������ӵ�������..."));

	//���ӵ���������������˿�Ϊ0x8123�ĳ���
	if (m_socket.Connect((LPCTSTR)ld.m_ipAddr, 0x8123) == FALSE)
	{
		//������ӳɹ����Է������µ�SOCKET���µĶ˿ںţ������µ�SOCKET���µĶ˿ں�ͨ�š�
		err = GetLastError();
		msg.Format(_T("���ӷ�����ʧ��! \r\n�������:%d"), err);

	msgbox:
		MessageBox(msg);
		PostQuitMessage(0);           //�˳�
		return TRUE;
	}

	char	pkt[200];
	sprintf_s(pkt, "%s �����ӵ�������!", m_username);

	//�ٴ����öԻ���ı���
	SetWindowText(pkt);

	Sleep(1000);

	//����Э����Ϣ

	//����������,�������������û���¼��Ϣ
	pkt[0] = 0x11;
	pkt[1] = im;//ͷ�����
	strncpy(pkt + 2, m_username, 98);      

	 //����
	int l = strlen(pkt) + 1;
	if (m_socket.Send(pkt, l) == FALSE)
	{
		MessageBox(_T("�������ݴ���!"));
	}

	//���öԻ��򱳾�
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_bg);
	m_brBg.CreatePatternBrush(&m_bmBg);
	//����ͼ��ͷ��
	HICON myIcon[6];
	int i;
	for (i = 0; i<6; i++)
	{
		myIcon[i] = AfxGetApp()->LoadIcon(IDI_ICON1 + i);
	}

	//����ͼ���б�
	m_imageList.Create(32, 32, ILC_COLOR32, 6, 6);
	for (i = 0; i < 6; i++)
	{
		m_imageList.Add(myIcon[i]);
	}

	m_list.SetImageList(&m_imageList, LVSIL_SMALL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Socket��Ϣ��Ӧ����
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
			//������Ϣ
			pName[curNum] = pkt + 2;
			curNum++;
			m_showMsg += pkt + 2;
			m_showMsg += " �������ҡ�\r\n";

			lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
			lvitem.iItem = curNum;
			lvitem.pszText = pkt + 2;
			lvitem.iImage = pkt[1] - 1;
			lvitem.iSubItem = 0;

			m_list.InsertItem(&lvitem);

			break;
			//�Ѽ����û���Ϣ
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
		//�˳�
		case 0x41:
			//pkt + 1��������û���
			m_showMsg += pkt + 1;
			m_showMsg += " �˳�����\r\n";

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
			//����û���κ��������Ϣ��ֱ����ʾ����Ϣ����
			m_showMsg += pkt + 1;
		}

		UpdateData(false);
		break;

	case CLOSE:
		MessageBox("�������ѹر�!");
		break;
	}
	return 1;
}

void CClientDlg::OnBnClickedSendbtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	char pkt[4096];
	memset(pkt, 0, sizeof(pkt));

	int			len;

	if (m_chatOneByOne)
	{
		//˽����Ϣ
		pkt[0] = 0x51;
		strcpy(pkt + 1, pName[m_curIndex]);
		len = sprintf(pkt + 100, "˽�ģ�%s��%s\r\n", m_username, m_sendMsg);
		m_socket.Send(pkt, len + 100);

		sprintf(pkt + 100, "˽�ģ���%s˵��%s\r\n", pName[m_curIndex], m_sendMsg);
		m_showMsg += pkt + 100;
		m_sendMsg.Empty();
	}
	else
	{
		//Ⱥ����Ϣ
		pkt[0] = 0x21;										//���빦����0x21
		len = sprintf(pkt + 1, "%s˵: %s\r\n", m_username, m_sendMsg);	//len�ǳ���
		m_socket.Send(pkt, len + 3);							//�ַ��������β���෢������

		m_sendMsg.Empty();
	}
	UpdateData(FALSE);
}


void CClientDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_curIndex = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (m_curIndex == -1)
	{
		AfxMessageBox("��û��ѡ��˽�ĵ��û�...");
		return;
	}

	m_chatOneByOne = true;
	UpdateData(false);

	*pResult = 0;
}
