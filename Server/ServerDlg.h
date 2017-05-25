
// ServerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MySocket.h"
#include <afxmt.h>			//引入临界区的支持

class CServerDlg : public CDialogEx
{
public:
	CServerDlg(CWnd* pParent = NULL);	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CBitmap m_bmBg;//背景的图
	CBrush  m_brBg;//背景的画刷
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBitmapButton m_startBtn;
	CListCtrl m_list;
	CMySocket m_socket;
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);//Socket的消息响应
														   //解析消息
	void ParserPkt(CMySocket *from);
	//向聊天室追加动态信息
	void Append(TCHAR *msg);

	void ClosePlayer(CMySocket *from);
	CString m_showMsg;
	CCriticalSection m_csList;
	afx_msg void OnBnClickedStartbtn();
};
