#if !defined(AFX_INSCLDLG_H__4E49A732_31FE_11D4_96B6_00E02939A881__INCLUDED_)
#define AFX_INSCLDLG_H__4E49A732_31FE_11D4_96B6_00E02939A881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// InsClDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsClDlg dialog

class CInsClDlg : public CDialog
{
// Construction
public:
	CBitmap m_Prev[6];

	CInsClDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	CStringList *m_pTypeList;
	//{{AFX_DATA(CInsClDlg)
	enum { IDD = IDD_GR_PROP_COIL };
	CString	m_strName;
	CString	m_strType;
	BOOL	m_bSetDefaultSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsClDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_CL_NAME); }
	CComboBox& TypeBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_CL_TYPE); }

	// Generated message map functions
	//{{AFX_MSG(CInsClDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeClType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_INSCLDLG_H__4E49A732_31FE_11D4_96B6_00E02939A881__INCLUDED_)
