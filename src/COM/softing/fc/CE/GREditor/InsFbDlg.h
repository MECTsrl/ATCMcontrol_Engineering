#if !defined(AFX_INSFBDLG_H__7EFE4873_5DA7_11D2_9A33_0000C0D26FBC__INCLUDED_)
#define AFX_INSFBDLG_H__7EFE4873_5DA7_11D2_9A33_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsFbDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsFbDlg dialog

class CInsFbDlg : public CDialog
{
   bool m_bEnableInst;
// Construction
public:
	CInsFbDlg(CWnd* pParent = NULL);   // standard constructor
   void EnableInstance(bool bEnable){m_bEnableInst = bEnable;}
// Dialog Data
// pointer to be used for filling in the list boxes.
// the pointer are given valid values in the view.	
	//{{AFX_DATA(CInsFbDlg)
	enum { IDD = IDD_GR_PROP_FB };
	CString	m_strName;
	BOOL	   m_bSetDefaultSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsFbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsFbDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_INSFBDLG_H__7EFE4873_5DA7_11D2_9A33_0000C0D26FBC__INCLUDED_)
