// PMToolDlg.h : header file
//

#if !defined(AFX_PMTOOLDLG_H__AD8C2C4C_C060_4D09_8598_0D62A403C0B2__INCLUDED_)
#define AFX_PMTOOLDLG_H__AD8C2C4C_C060_4D09_8598_0D62A403C0B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPMToolDlg dialog

class CPMToolDlg : public CDialog
{
// Construction
public:
	CPMToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPMToolDlg)
	enum { IDD = IDD_PMTOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPMToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPMToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMTOOLDLG_H__AD8C2C4C_C060_4D09_8598_0D62A403C0B2__INCLUDED_)
