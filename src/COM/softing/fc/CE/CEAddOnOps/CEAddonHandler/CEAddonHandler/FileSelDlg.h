
#ifndef __FILESELDLG_H_
#define __FILESELDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "resource.h"
#include "afxtempl.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CFileSelDlg dialog

class CFileSelDlg : public CDialog
{
// Construction
public:
	CFileSelDlg(CWnd* pParent = NULL);   // standard constructor

    void Initialize(const CString& crstrTitle, const CStringArray& crastrStrings);

    void GetSelection(CStringArray& rastrSelection);

// Dialog Data
	//{{AFX_DATA(CFileSelDlg)
	enum { IDD = IDD_FILE_SELECT_DLG };
	CListBox	m_tListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileSelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
    CString         m_strTitle;         // title of dialog
    CStringArray    m_astrStrings;      // strings to be shown in list control
    CStringArray    m_astrStringsSel;   // strings that were selected

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __FILESELDLG_H_


