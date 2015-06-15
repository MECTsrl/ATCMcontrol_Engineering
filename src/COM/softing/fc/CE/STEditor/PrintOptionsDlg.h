#if !defined(AFX_PRINTOPTIONSDLG_H__76B28851_D05F_483A_99A2_2C54CE3FE584__INCLUDED_)
#define AFX_PRINTOPTIONSDLG_H__76B28851_D05F_483A_99A2_2C54CE3FE584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg dialog

class CPrintOptionsDlg : public CDialog
{
// Construction
public:
	CPrintOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintOptionsDlg)
	enum { IDD = IDD_PRINT_OPTIONS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintOptionsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTOPTIONSDLG_H__76B28851_D05F_483A_99A2_2C54CE3FE584__INCLUDED_)
