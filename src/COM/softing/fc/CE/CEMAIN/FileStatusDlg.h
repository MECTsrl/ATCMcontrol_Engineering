#if !defined(AFX_FILESTATUSDLG_H__58288386_CEC9_4830_9AAA_68C0DC84E502__INCLUDED_)
#define AFX_FILESTATUSDLG_H__58288386_CEC9_4830_9AAA_68C0DC84E502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileStatusDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CFileStatusDlg dialog

class CFileStatusDlg : public CDialog
{
// Construction
public:
	CFileStatusDlg(CWnd* pParent = NULL);   // standard constructor

    void ShowProperties(const CString& strFileName, const CString& strPath = "");

// Dialog Data
	//{{AFX_DATA(CFileStatusDlg)
	enum { IDD = IDD_FILE_STATUS };
	CString	m_strCreated;
	CString	m_strLastModified;
	CString	m_strName;
	CString	m_strPath;
	BOOL	m_bReadOnly;
	CString	m_strSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileStatusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFilePropReadOnly();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESTATUSDLG_H__58288386_CEC9_4830_9AAA_68C0DC84E502__INCLUDED_)
