
// ClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "MySocket.h"
#include "LogDLG.h"
// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	LRESULT OnSocket(WPARAM wParam, LPARAM lParam);//Socket响应函数
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_showMsg;
	CString m_sendMsg;
	BOOL m_chatOneByOne;
	CMySocket	m_socket;
	CBitmap m_bmBg;
	CBrush  m_brBg;

	//用户名列表
	CString pName[100];
	//用户名列表项
	int curNum;
	CImageList	m_imageList;//头像列表
	int			m_curIndex;//头像索引
	CString m_username;//好友名
	afx_msg void OnBnClickedSendbtn();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
