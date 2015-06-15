#if !defined(AFX_INSCMTDLG_H__FE9EEDA0_0EBA_11D2_B4B4_64E5FBC00000__INCLUDED_)
#define AFX_INSCMTDLG_H__FE9EEDA0_0EBA_11D2_B4B4_64E5FBC00000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InsCmtDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsCmtDlg dialog

class CInsCmtDlg : public CDialog
{
// Construction
public:
	CInsCmtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInsCmtDlg)
	enum { IDD = IDD_GR_PROP_CMT };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsCmtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsCmtDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_INSCMTDLG_H__FE9EEDA0_0EBA_11D2_B4B4_64E5FBC00000__INCLUDED_)
