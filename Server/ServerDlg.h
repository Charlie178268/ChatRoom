
// ServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MySocket.h"
#include <afxmt.h>			//�����ٽ�����֧��

class CServerDlg : public CDialogEx
{
public:
	CServerDlg(CWnd* pParent = NULL);	

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CBitmap m_bmBg;//������ͼ
	CBrush  m_brBg;//�����Ļ�ˢ
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBitmapButton m_startBtn;
	CListCtrl m_list;
	CMySocket m_socket;
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);//Socket����Ϣ��Ӧ
														   //������Ϣ
	void ParserPkt(CMySocket *from);
	//��������׷�Ӷ�̬��Ϣ
	void Append(TCHAR *msg);

	void ClosePlayer(CMySocket *from);
	CString m_showMsg;
	CCriticalSection m_csList;
	afx_msg void OnBnClickedStartbtn();
};
