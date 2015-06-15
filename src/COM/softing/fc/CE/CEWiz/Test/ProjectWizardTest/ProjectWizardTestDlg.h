// ProjectWizardTestDlg.h : header file
//

#if !defined(AFX_PROJECTWIZARDTESTDLG_H__46A1F887_CB4E_11D4_B353_0008C77981F8__INCLUDED_)
#define AFX_PROJECTWIZARDTESTDLG_H__46A1F887_CB4E_11D4_B353_0008C77981F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProjectWizardTestDlg dialog

class CProjectWizardTestDlg : public CDialog
{
// Construction
public:
	CProjectWizardTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectWizardTestDlg)
	enum { IDD = IDD_PROJECTWIZARDTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectWizardTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProjectWizardTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTWIZARDTESTDLG_H__46A1F887_CB4E_11D4_B353_0008C77981F8__INCLUDED_)
