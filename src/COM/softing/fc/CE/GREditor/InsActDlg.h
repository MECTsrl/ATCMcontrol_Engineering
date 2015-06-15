#if !defined(AFX_INSACTDLG_H__DB6B8780_5195_11D2_B4B4_00E0291218EA__INCLUDED_)
#define AFX_INSACTDLG_H__DB6B8780_5195_11D2_B4B4_00E0291218EA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CInsActDlg dialog

class CInsActDlg : public CDialog
{
// Construction
public:
	CInsActDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	CStringList *m_pQualList;
   LPCTSTR     *pstrQual;  // only for check of not impl. timequal.
	//{{AFX_DATA(CInsActDlg)
	enum { IDD = IDD_GR_PROP_ACT };
	CButton	m_CtrlSTaction;
	CButton	m_CtrlLDaction;
	CButton	m_CtrlFBDaction;
	CString	m_strName;
	BOOL     m_bSTactWarn;
	BOOL	   m_bSetDefaultSize;
	//}}AFX_DATA
	BOOL     m_bSTaction;
	BOOL     m_bLDaction;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsActDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_ACT_NAME); }

	// Generated message map functions
	//{{AFX_MSG(CInsActDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CInsActBlkDlg dialog

class CInsActBlkDlg : public CDialog
{
// Construction
public:
	CInsActBlkDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	CStringList *m_pQualList;
   CString      strQual[QT_LAST];  // only for check of not impl. timequal.
	//{{AFX_DATA(CInsActBlkDlg)
#ifdef V120_SFC_NOTIMEQUALIFIER
	enum { IDD = IDD_GR_PROP_ACTBLK_V120 };
#else
   enum { IDD = IDD_GR_PROP_ACTBLK };
#endif
	CButton	m_CtrlSTaction;
	CButton	m_CtrlLDaction;
	CButton	m_CtrlFBDaction;
	CString	m_strName;
	CString	m_strQual;
#ifndef V120_SFC_NOTIMEQUALIFIER
	CString	m_strTime;
#endif
	BOOL     m_bSTactWarn;
	BOOL	   m_bSetDefaultSize;
	//}}AFX_DATA
	BOOL     m_bSTaction;
	BOOL     m_bLDaction;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsActBlkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_ACT_NAME); }
	CComboBox& QualBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_ACT_QUAL); }

	// Generated message map functions
	//{{AFX_MSG(CInsActBlkDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSACTDLG_H__DB6B8780_5195_11D2_B4B4_00E0291218EA__INCLUDED_)
