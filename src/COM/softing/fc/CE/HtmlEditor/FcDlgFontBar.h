
#if !defined(AFX_FCDLGFONTBAR_H__9E68BAFA_DB8D_11D2_A006_00A024363DFC__INCLUDED_)
#define AFX_FCDLGFONTBAR_H__9E68BAFA_DB8D_11D2_A006_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcDlgFontBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcDlgFontBar dialog

class CFcDlgFontBar : public  CDialogBar
{
// Construction
public:
	CFcDlgFontBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFcDlgFontBar)
	enum { IDD = IDD_FONT_BAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcDlgFontBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFcDlgFontBar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCDLGFONTBAR_H__9E68BAFA_DB8D_11D2_A006_00A024363DFC__INCLUDED_)
