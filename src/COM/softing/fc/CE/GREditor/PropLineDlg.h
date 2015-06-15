#if !defined(AFX_PROPLINEDLG_H__5A064CB3_A0EA_11D4_B15F_00E029440C3D__INCLUDED_)
#define AFX_PROPLINEDLG_H__5A064CB3_A0EA_11D4_B15F_00E029440C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropLineDlg dialog

class CPropLineDlg : public CDialog
{
// Construction
public:
	CPropLineDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropLineDlg)
	enum { IDD = IDD_GR_PROP_LINE };
	BOOL	m_bSetDefaultSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropLineDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_PROPLINEDLG_H__5A064CB3_A0EA_11D4_B15F_00E029440C3D__INCLUDED_)
