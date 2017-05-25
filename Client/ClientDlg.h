
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MySocket.h"
#include "LogDLG.h"
// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnSocket(WPARAM wParam, LPARAM lParam);//Socket��Ӧ����
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_showMsg;
	CString m_sendMsg;
	BOOL m_chatOneByOne;
	CMySocket	m_socket;
	CBitmap m_bmBg;
	CBrush  m_brBg;

	//�û����б�
	CString pName[100];
	//�û����б���
	int curNum;
	CImageList	m_imageList;//ͷ���б�
	int			m_curIndex;//ͷ������
	CString m_username;//������
	afx_msg void OnBnClickedSendbtn();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
