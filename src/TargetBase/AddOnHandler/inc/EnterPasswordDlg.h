#ifndef _ENTERPASSWORDDLG_H_
#define _ENTERPASSWORDDLG_H_

#include "AddonBaseResource.h"

class CEnterPasswordDlg : public CDialog
{
// Construction
public:
	CEnterPasswordDlg(const CString& crstrComparePwd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnterPasswordDlg)
	enum { IDD = IDD_ENTER_PASSWORD };
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnterPasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnterPasswordDlg)
	virtual void OnOK();
	afx_msg void OnChangePasswd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    BOOL    m_bPasswdChanged;

protected:
    CString m_strComparePwd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
