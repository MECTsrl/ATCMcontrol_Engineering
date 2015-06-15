#if !defined(AFX_GRIDDLG_H__B5E83E01_FC4E_11D1_8E7B_444553540000__INCLUDED_)
#define AFX_GRIDDLG_H__B5E83E01_FC4E_11D1_8E7B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog

class CGridDlg : public CDialog
{
// Construction
public:
	CGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGridDlg)
	enum { IDD = IDD_GR_GRID };
	int		m_iGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDLG_H__B5E83E01_FC4E_11D1_8E7B_444553540000__INCLUDED_)
