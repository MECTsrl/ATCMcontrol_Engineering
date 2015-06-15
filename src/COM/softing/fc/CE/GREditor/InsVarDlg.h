#if !defined(AFX_INSVARDLG_H__002AC3C2_0E79_11D2_B4B4_94DAFBC00000__INCLUDED_)
#define AFX_INSVARDLG_H__002AC3C2_0E79_11D2_B4B4_94DAFBC00000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InsVarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsVarDlg dialog

class CInsVarDlg : public CDialog
{
// Construction
public:
	CBitmap m_Prev[3];

	CInsVarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	CStringList *m_pTypeList;
	//{{AFX_DATA(CInsVarDlg)
	enum { IDD = IDD_GR_PROP_VAR };
	CString	m_strName;
	CString	m_strType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsVarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_VAR_NAME); }
	CComboBox& TypeBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_VAR_TYPE); }

	// Generated message map functions
	//{{AFX_MSG(CInsVarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeVarType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSVARDLG_H__002AC3C2_0E79_11D2_B4B4_94DAFBC00000__INCLUDED_)
