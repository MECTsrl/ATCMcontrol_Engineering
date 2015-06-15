#ifndef __COVERPAGEDATADLG_H_
#define __COVERPAGEDATADLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Resource.h"
#include "PrintLayout.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CCoverPageDataDlg dialog

class CCoverPageDataDlg : public CDialog
{
// Construction
public:
	CCoverPageDataDlg(CCoverPage* pCoverPage, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCoverPageDataDlg)
	enum { IDD = IDD_COVER_PAGE_DATA };
	CString	m_strBitmap;
	CString	m_strComment;
	CString	m_strProjectManager;
	CString	m_strProjectName;
	CString	m_strProjectNumber;
	//}}AFX_DATA

    CString m_strProjectNameCaption;
    CString m_strProjectManagerCaption;
    CString m_strProjectNumberCaption;
    CString m_strCommentCaption;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoverPageDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCoverPageDataDlg)
	afx_msg void OnCovpagPrjnocapBtn();
	afx_msg void OnCovpagBitmapSelBtn();
	afx_msg void OnCovpagCommentcapBtn();
	afx_msg void OnCovpagPrjmancapBtn();
	afx_msg void OnCovpagPrjnamcapBtn();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void CopyDataToMembers();
    void CopyDataFromMembers();

    void SetButtonTexts();

public:
    CCoverPage* m_pCoverPage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __COVERPAGEDATADLG_H_


