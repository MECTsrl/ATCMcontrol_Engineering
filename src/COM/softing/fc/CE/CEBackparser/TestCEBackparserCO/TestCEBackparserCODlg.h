// TestCEBackparserCODlg.h : header file
//

#if !defined(AFX_TESTCEBACKPARSERCODLG_H__89E29AEB_FB59_11D4_A0A1_006008736ABF__INCLUDED_)
#define AFX_TESTCEBACKPARSERCODLG_H__89E29AEB_FB59_11D4_A0A1_006008736ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEBackparserCO.h"

/////////////////////////////////////////////////////////////////////////////
// CTestCEBackparserCODlg dialog

class CTestCEBackparserCODlg : public CDialog
{
// Construction
public:
	CTestCEBackparserCODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestCEBackparserCODlg)
	enum { IDD = IDD_TESTCEBACKPARSERCO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestCEBackparserCODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestCEBackparserCODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCEBACKPARSERCODLG_H__89E29AEB_FB59_11D4_A0A1_006008736ABF__INCLUDED_)
