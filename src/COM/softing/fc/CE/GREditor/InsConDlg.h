
#if !defined(AFX_INSCONDLG_H__B5E83E02_FC4E_11D1_8E7B_444553540000__INCLUDED_)
#define AFX_INSCONDLG_H__B5E83E02_FC4E_11D1_8E7B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InsConDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsConDlg dialog

class CInsConDlg : public CDialog
{
// Construction
public:
	CBitmap m_Prev[2];
	CInsConDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	//{{AFX_DATA(CInsConDlg)
	enum { IDD = IDD_GR_PROP_CON };
	CString	m_strName;
	int		m_iConPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsConDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_CON_NAME); }

	// Generated message map functions
	//{{AFX_MSG(CInsConDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnConChanged();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_INSCONDLG_H__B5E83E02_FC4E_11D1_8E7B_444553540000__INCLUDED_)
