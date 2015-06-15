#ifndef _DLGPROJECTINFOUPLOAD_H_
#define _DLGPROJECTINFOUPLOAD_H_

#include "AddonBaseResource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProjectInfoUpload dialog

class CDlgProjectInfoUpload : public CDialog
{
// Construction
public:
	CDlgProjectInfoUpload(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProjectInfoUpload)
	enum { IDD = IDD_PROJECT_INFO_UPLOAD };
	CString	m_strCreated;
	CString	m_strModified;
	CString	m_strName;
	CString	m_strOwner;
	CString	m_strText1;
	CString	m_strText2;
	CString	m_strText3;
	CString	m_strVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProjectInfoUpload)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProjectInfoUpload)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
