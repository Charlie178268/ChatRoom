#pragma once
#include "afxdialogex.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "afxext.h"
class CLogDLG :
	public CDialogEx
{
	DECLARE_DYNAMIC(CLogDLG)
public:
	CLogDLG(CWnd* pParent = NULL);
	virtual ~CLogDLG();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CBitmap m_bmBg;
	CBrush m_brBg;

	DECLARE_MESSAGE_MAP()
public:
	// ip地址输入
	CIPAddressCtrl m_Ip;
	// 用户名输入
	CString m_username;

	afx_msg void OnCbnSelchangePiccombo();
	// 登录按钮
	CBitmapButton m_loginBtn;
	//保存ip地址
	char m_ipAddr[16];
	//保存图像
	CImageList m_imageList;
	//保存图像号码
	int m_imgNum;
	virtual BOOL OnInitDialog();
	// 选择头像框，要用Extended ComboBox
	CComboBoxEx m_btmSelect;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedLogbtn();
	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3;
};

