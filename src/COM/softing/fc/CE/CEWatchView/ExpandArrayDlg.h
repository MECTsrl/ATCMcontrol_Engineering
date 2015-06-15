#if !defined(AFX_EXPANDARRAYDLG_H__97356119_45EB_449D_8C35_4779FAED9E7B__INCLUDED_)
#define AFX_EXPANDARRAYDLG_H__97356119_45EB_449D_8C35_4779FAED9E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpandArrayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpandArrayDlg dialog

class CExpandArrayDlg : public CDialog
{
// Construction
public:
	CExpandArrayDlg(int iOffset = 0, int iMaxCount = 100, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExpandArrayDlg)
	enum { IDD = IDD_ARRAYEXPAND };
	int		m_iCount;
	int		m_iOffset;
	//}}AFX_DATA

	int	m_iMaxCount;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpandArrayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpandArrayDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPANDARRAYDLG_H__97356119_45EB_449D_8C35_4779FAED9E7B__INCLUDED_)
