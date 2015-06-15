#if !defined(AFX_FCSTYLEDLG_H__5DEFCA96_CA43_11D2_9FFA_00A024363DFC__INCLUDED_)
#define AFX_FCSTYLEDLG_H__5DEFCA96_CA43_11D2_9FFA_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcStyleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcDlgStyle dialog
class CFcHtmlEdView;

class CFcDlgStyle : public CDialog
{
// Construction
public:
	CFcDlgStyle(CFcHtmlEdView* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFcDlgStyle)
	enum { IDD = IDD_STYLE };
	CListBox	m_StyleList;
	int		m_Align;
	CString	m_Style;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcDlgStyle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFcDlgStyle)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private :
	CFcHtmlEdView* m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCSTYLEDLG_H__5DEFCA96_CA43_11D2_9FFA_00A024363DFC__INCLUDED_)
