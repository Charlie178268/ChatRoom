#include "stdafx.h"
#include "LogDLG.h"
#include "Client.h"

IMPLEMENT_DYNAMIC(CLogDLG, CDialogEx)

CLogDLG::CLogDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOG_DIALOG, pParent)
	, m_username(_T(""))
{

}

CLogDLG::~CLogDLG()
{
}

void CLogDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IpAddress, m_Ip);
	DDX_Text(pDX, IDC_UserNameEdit, m_username);
	DDX_Control(pDX, IDC_LogBtn, m_loginBtn);
	DDX_Control(pDX, IDC_COMBOBOXEX1, m_btmSelect);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_STATIC3, m_static3);
}


BEGIN_MESSAGE_MAP(CLogDLG, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_LogBtn, &CLogDLG::OnBnClickedLogbtn)
END_MESSAGE_MAP()


// CLogDLG 消息处理程序


void CLogDLG::OnCbnSelchangePiccombo()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CLogDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置静态文字的大小
	//设置Static字体  
	CFont m_Font;
	m_Font.CreatePointFont(600, "Arial", NULL);
	m_static1.SetFont(&m_Font, true);
	m_static1.SetWindowText("IP:");
	m_static2.SetFont(&m_Font, true);
	m_static2.SetWindowText("用户名:");
	m_static3.SetFont(&m_Font, true);
	m_static3.SetWindowText("头像:");
	//m_static1
	//设置对话框背景
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_logtext);
	m_brBg.CreatePatternBrush(&m_bmBg);
	m_Ip.SetWindowText("127.0.0.1");
	m_loginBtn.LoadBitmaps(IDB_normalLogBtn, IDB_pushedLogBtn);
	m_loginBtn.SizeToContent();

	//初始化头像列表
	m_imageList.Create(32, 32, ILC_COLOR16, 6, 6);
	for (int i = 1; i <= 6; i++) {
		m_imageList.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(137 + i)));
		//资源是用序号定义的,而不是字符串.所以要把数字转换成字符串指针,然后再传递给LoadResource之类的函数,这样才加载了资源.
	}
	//将头像组合框关联头像列表
	m_btmSelect.SetImageList(&m_imageList);
	//将图像插入到组合框中
	for (int i = 0; i<m_imageList.GetImageCount(); i++)
	{
		COMBOBOXEXITEM cbi = { 0 };
		CString str;
		int nItem;

		cbi.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;

		cbi.iItem = i;
		str.Format(_T("%2d"), i + 1);
		cbi.pszText = str.GetBuffer(0);
		cbi.cchTextMax = str.GetLength();
		cbi.iImage = i;
		cbi.iSelectedImage = i;

		//为组合框添加新项目
		nItem = m_btmSelect.InsertItem(&cbi);
		ASSERT(nItem == i);
	}

	//设置当前的选择
	m_btmSelect.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CLogDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CLogDLG::OnBnClickedLogbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_imgNum = m_btmSelect.GetCurSel();
	m_Ip.GetWindowText(m_ipAddr, 16);

	OnOK();//关闭对话框
}

