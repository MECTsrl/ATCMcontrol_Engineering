// TestDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__25E1BEB8_F68A_11D4_B8EE_002018641833__INCLUDED_)
#define AFX_TESTDLG_H__25E1BEB8_F68A_11D4_B8EE_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEProjInfo.h"
#include "CEContainer.h"

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor
    ~CTestDlg();

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CComboBox	m_CtrlMethod;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnInvoke();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	ICEProjInfo * m_pICEProjInfo;
    //ICEContainer * m_pICEContainer;
    IUnknown * m_pIUnkCEContainer;

private:
	void InvokeContainerIF();
	void InVoke_fileChangedHint();
    void InVoke_getContainerIF();
    void InVoke_getContainerIFforFile();
    void DispMessage(DWORD dwErrCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__25E1BEB8_F68A_11D4_B8EE_002018641833__INCLUDED_)
