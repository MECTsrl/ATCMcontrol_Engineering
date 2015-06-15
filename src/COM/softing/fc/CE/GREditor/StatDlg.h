#if !defined(AFX_STATDLG_H__D8525564_489B_11D3_94F0_00609762C9FA__INCLUDED_)
#define AFX_STATDLG_H__D8525564_489B_11D3_94F0_00609762C9FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ElemPtrList.h"

/////////////////////////////////////////////////////////////////////////////
// CStatDlg dialog

class CStatDlg : public CDialog
{
// Construction
public:
	CStatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatDlg)
	enum { IDD = IDD_GR_STATISTIC };
	//}}AFX_DATA
   CStatisticData data;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATDLG_H__D8525564_489B_11D3_94F0_00609762C9FA__INCLUDED_)
