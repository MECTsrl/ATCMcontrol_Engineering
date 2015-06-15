#ifndef __MULTISELDLG_H_
#define __MULTISELDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiSelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiSelDlg dialog

class CMultiSelDlg : public CDialog
{
	BOOL m_bSingleSelect;
// Construction
public:
	CMultiSelDlg(CWnd* pParent = NULL);   // standard constructor

    void Initialize(const CString& crstrTitle, const CStringArray& crastrStrings, BOOL bSingleSelect=FALSE);

    void GetSelection(CStringArray& rastrSelection);

// Dialog Data
	//{{AFX_DATA(CMultiSelDlg)
	enum { IDD = IDD_MULTI_SELECT_DLG };
	CListBox	m_tListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiSelDlg)
	virtual void OnOK();
	afx_msg void OnDblclkMultiSelList();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    CString         m_strTitle;         // title of dialog
    CStringArray    m_astrStrings;      // strings to be shown in list control
    CStringArray    m_astrStringsSel;   // strings that were selected

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __MULTISELDLG_H_

