#ifndef _CHANGEPASSWORDDLG_H_
#define _CHANGEPASSWORDDLG_H_

#include "AddonBaseResource.h"

class CChangePasswordDlg : public CDialog
{
// Construction
public:
	CChangePasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangePasswordDlg)
	enum { IDD = IDD_CHANGE_PASSWORD };
	CString	m_strPassword1;
	CString	m_strPassword2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangePasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangePasswordDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
