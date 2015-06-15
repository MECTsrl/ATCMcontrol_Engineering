#if !defined(AFX_INSDIVDLG_H__C3B21245_5126_11D2_810B_444553540000__INCLUDED_)
#define AFX_INSDIVDLG_H__C3B21245_5126_11D2_810B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InsDivDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsDivDlg dialog

class CInsDivDlg : public CDialog
{
// Construction
public:
	CInsDivDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	//{{AFX_DATA(CInsDivDlg)
	enum { IDD = IDD_GR_PROP_DIV };
	int		m_iSelection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsDivDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
	//{{AFX_MSG(CInsDivDlg)
	virtual BOOL OnInitDialog();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_INSDIVDLG_H__C3B21245_5126_11D2_810B_444553540000__INCLUDED_)
