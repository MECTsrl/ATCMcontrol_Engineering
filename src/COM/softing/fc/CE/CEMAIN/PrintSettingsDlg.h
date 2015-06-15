
#ifndef __PRINTSETTINGSDLG_H_
#define __PRINTSETTINGSDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Resource.h"
#include "PrintSettings.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CPrintSettingsDlg dialog

class CPrintSettingsDlg : public CDialog
{
// Construction
public:
	CPrintSettingsDlg(CPrintSettings* pPrintSettings, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintSettingsDlg)
	enum { IDD = IDD_PRINT_SETTINGS };
	BOOL	m_bCoverPage;
	BOOL	m_bContents;
	BOOL	m_bCrossRef;
	BOOL	m_bFileView;
	BOOL	m_bClassView;
	BOOL	m_bInstView;
	BOOL	m_bHWView;
	BOOL	m_bGrHeader;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintSettingsDlg)
	afx_msg void OnCovpageBtn();
	afx_msg void OnFooterBtn();
	afx_msg void OnHeaderBtn();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void CopyDataToMembers();
    void CopyDataFromMembers();


public:
    CPrintSettings* m_pPrintSettings;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __PRINTSETTINGSDLG_H_

