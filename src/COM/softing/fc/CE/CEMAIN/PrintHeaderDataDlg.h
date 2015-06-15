#ifndef __PRINTHEADERDATADLG_H_
#define __PRINTHEADERDATADLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Resource.h"

//----  Forward Class Definitions:   -------------------------------*
class CPrintHeader;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CPrintHeaderDataDlg dialog

class CPrintHeaderDataDlg : public CDialog
{
// Construction
public:
	CPrintHeaderDataDlg(CPrintHeader* pPrintHeader, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintHeaderDataDlg)
	enum { IDD = IDD_PRINT_HEADER };
	CString	m_strBitmapLeft;
	CString	m_strBitmapRight;
	CString	m_strTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintHeaderDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintHeaderDataDlg)
	afx_msg void OnHeaderLbitmapSelBtn();
	afx_msg void OnHeaderRbitmapSelBtn();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void CopyDataToMembers();
    void CopyDataFromMembers();

public:
    CPrintHeader*   m_pPrintHeader;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __PRINTHEADERDATADLG_H_
