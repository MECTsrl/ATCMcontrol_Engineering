#ifndef __PRINTFOOTERDATADLG_H_
#define __PRINTFOOTERDATADLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Resource.h"

//----  Forward Class Definitions:   -------------------------------*
class CPrintFooter;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CPrintFooterDataDlg dialog

class CPrintFooterDataDlg : public CDialog
{
// Construction
public:
	CPrintFooterDataDlg(CPrintFooter* pPrintFooter, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintFooterDataDlg)
	enum { IDD = IDD_PRINT_FOOTER };
	CString	m_strRevNo1;
	CString	m_strRevNo2;
	CString	m_strRevNo3;
	CString	m_strChangeDesc1;
	CString	m_strChangeDesc2;
	CString	m_strChangeDesc3;
	CString	m_strChangeName1;
	CString	m_strChangeName2;
	CString	m_strChangeName3;
	CString	m_strChangeDate1;
	CString	m_strChangeDate2;
	CString	m_strChangeDate3;
	CString	m_strDate;
	CString	m_strDrawn;
	CString	m_strApproved;
	CString	m_strReleased;
	CString	m_strDateTime;
	CString	m_strProjPath;
	CString	m_strObjName;
	CString	m_strPageCount;
	CString	m_strInfo1;
	CString	m_strInfo2;
	CString	m_strInfo3;
	CString	m_strInfo4;
	CString	m_strBitmap;
	//}}AFX_DATA

    // captions
    CString m_strRevNoCap;
    CString m_strChangeDescCap;
    CString m_strChangeDateCap;
    CString m_strChangeNameCap;
    CString m_strDateCap;
    CString m_strDrawnCap;
    CString m_strApprovedCap;
    CString m_strReleasedCap;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintFooterDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    void CopyDataToMembers();
    void CopyDataFromMembers();

    void SetButtonTexts();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintFooterDataDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFooterApprBtn();
	afx_msg void OnFooterCbitmapSelBtn();
	afx_msg void OnFooterChdateBtn();
	afx_msg void OnFooterChdescrBtn();
	afx_msg void OnFooterChnameBtn();
	afx_msg void OnFooterDateBtn();
	afx_msg void OnFooterDrawnBtn();
	afx_msg void OnFooterRelBtn();
	afx_msg void OnFooterRevnoBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CPrintFooter*   m_pPrintFooter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __PRINTFOOTERDATADLG_H_
