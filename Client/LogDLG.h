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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CBitmap m_bmBg;
	CBrush m_brBg;

	DECLARE_MESSAGE_MAP()
public:
	// ip��ַ����
	CIPAddressCtrl m_Ip;
	// �û�������
	CString m_username;

	afx_msg void OnCbnSelchangePiccombo();
	// ��¼��ť
	CBitmapButton m_loginBtn;
	//����ip��ַ
	char m_ipAddr[16];
	//����ͼ��
	CImageList m_imageList;
	//����ͼ�����
	int m_imgNum;
	virtual BOOL OnInitDialog();
	// ѡ��ͷ���Ҫ��Extended ComboBox
	CComboBoxEx m_btmSelect;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedLogbtn();
	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3;
};

