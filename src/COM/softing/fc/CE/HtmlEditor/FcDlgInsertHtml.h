#if !defined(AFX_FCDLGINSERTHTML_H__00EA70C7_D09B_11D2_9FFC_00A024363DFC__INCLUDED_)
#define AFX_FCDLGINSERTHTML_H__00EA70C7_D09B_11D2_9FFC_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcDlgInsertHtml.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcDlgInsertHtml dialog

class CFcDlgInsertHtml : public CDialog
{
// Construction
public:
	CFcDlgInsertHtml(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFcDlgInsertHtml)
	enum { IDD = IDD_INSERT_HTML };
	CString	m_HtmlText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcDlgInsertHtml)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFcDlgInsertHtml)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCDLGINSERTHTML_H__00EA70C7_D09B_11D2_9FFC_00A024363DFC__INCLUDED_)
