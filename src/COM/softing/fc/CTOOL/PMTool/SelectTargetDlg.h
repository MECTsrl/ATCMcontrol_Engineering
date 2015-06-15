#if !defined(AFX_SELECTTARGETDLG_H__DFFF22C3_C35B_4FC0_AFFC_4C1EAF9CCECB__INCLUDED_)
#define AFX_SELECTTARGETDLG_H__DFFF22C3_C35B_4FC0_AFFC_4C1EAF9CCECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectTargetDlg.h : header file
//

#include "resource.h"
#include "PMPropPage.h"


/////////////////////////////////////////////////////////////////////////////
// CSelectTargetDlg dialog

class CSelectTargetDlg : public CDialog
{
// Construction
public:
	CSelectTargetDlg(CWnd* pParent = NULL);   // standard constructor

    void Initialize(TTIList* pTargetInfoList);

    TTargetInfo* GetSelectedTarget();

// Dialog Data
	//{{AFX_DATA(CSelectTargetDlg)
	enum { IDD = IDD_SELECT_TARGET };
	CListCtrl	m_tTargetListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectTargetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectTargetDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
    void FillTargetListControl(TTIList* pTargetInfoList);

protected:
    TTIList*        m_pTargetInfoList;
    TTargetInfo*    m_pSelTarget;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTTARGETDLG_H__DFFF22C3_C35B_4FC0_AFFC_4C1EAF9CCECB__INCLUDED_)
