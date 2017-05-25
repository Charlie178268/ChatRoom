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


// CLogDLG ��Ϣ�������


void CLogDLG::OnCbnSelchangePiccombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL CLogDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���þ�̬���ֵĴ�С
	//����Static����  
	CFont m_Font;
	m_Font.CreatePointFont(600, "Arial", NULL);
	m_static1.SetFont(&m_Font, true);
	m_static1.SetWindowText("IP:");
	m_static2.SetFont(&m_Font, true);
	m_static2.SetWindowText("�û���:");
	m_static3.SetFont(&m_Font, true);
	m_static3.SetWindowText("ͷ��:");
	//m_static1
	//���öԻ��򱳾�
	m_bmBg.DeleteObject();
	m_brBg.DeleteObject();
	m_bmBg.LoadBitmap(IDB_logtext);
	m_brBg.CreatePatternBrush(&m_bmBg);
	m_Ip.SetWindowText("127.0.0.1");
	m_loginBtn.LoadBitmaps(IDB_normalLogBtn, IDB_pushedLogBtn);
	m_loginBtn.SizeToContent();

	//��ʼ��ͷ���б�
	m_imageList.Create(32, 32, ILC_COLOR16, 6, 6);
	for (int i = 1; i <= 6; i++) {
		m_imageList.Add(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(137 + i)));
		//��Դ������Ŷ����,�������ַ���.����Ҫ������ת�����ַ���ָ��,Ȼ���ٴ��ݸ�LoadResource֮��ĺ���,�����ż�������Դ.
	}
	//��ͷ����Ͽ����ͷ���б�
	m_btmSelect.SetImageList(&m_imageList);
	//��ͼ����뵽��Ͽ���
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

		//Ϊ��Ͽ��������Ŀ
		nItem = m_btmSelect.InsertItem(&cbi);
		ASSERT(nItem == i);
	}

	//���õ�ǰ��ѡ��
	m_btmSelect.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CLogDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CLogDLG::OnBnClickedLogbtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_imgNum = m_btmSelect.GetCurSel();
	m_Ip.GetWindowText(m_ipAddr, 16);

	OnOK();//�رնԻ���
}

