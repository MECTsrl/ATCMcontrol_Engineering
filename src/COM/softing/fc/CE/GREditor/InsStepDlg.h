
#if !defined(AFX_INSSTEPDLG_H__16C7F860_5077_11D2_B4B4_00E0291218EA__INCLUDED_)
#define AFX_INSSTEPDLG_H__16C7F860_5077_11D2_B4B4_00E0291218EA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InsStepDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsStepDlg dialog

class CInsStepDlg : public CDialog
{
// Construction
public:
	CInsStepDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	CStringList *m_pNameList;
	//{{AFX_DATA(CInsStepDlg)
	enum { IDD = IDD_GR_PROP_STEP };
	CString	m_strName;
   BOOL     m_bInitial;
	BOOL	m_bCopyDefaultSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsStepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CComboBox& NameBox()
		{return *(CComboBox*)GetDlgItem(IDC_GR_STEP_NAME); }

	// Generated message map functions
	//{{AFX_MSG(CInsStepDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_INSSTEPDLG_H__16C7F860_5077_11D2_B4B4_00E0291218EA__INCLUDED_)
