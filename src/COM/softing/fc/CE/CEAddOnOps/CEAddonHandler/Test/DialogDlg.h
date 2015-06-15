// DialogDlg.h : header file
//

#if !defined(AFX_DIALOGDLG_H__EA38A35B_E717_11D4_B8D2_002018641833__INCLUDED_)
#define AFX_DIALOGDLG_H__EA38A35B_E717_11D4_B8D2_002018641833__INCLUDED_

#include "AddOnOps_Test.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDialogDlg dialog

class CDialogDlg : public CDialog
{
// Construction
public:
	CDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDlg)
	enum { IDD = IDD_DIALOG_DIALOG };
	CComboBox	m_CtrlDispId;
	CButton	m_Disconnect;
	CButton	m_Connect;
	CString	m_strAdditionalInfo;
	CString	m_strId;
	CString	m_strSourceFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnInvoke();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	AddOnOps_Test test;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDLG_H__EA38A35B_E717_11D4_B8D2_002018641833__INCLUDED_)
