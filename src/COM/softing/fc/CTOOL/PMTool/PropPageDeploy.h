#if !defined(AFX_PROPPAGEDEPLOY_H__810521DB_2749_4E50_AE68_383997A14631__INCLUDED_)
#define AFX_PROPPAGEDEPLOY_H__810521DB_2749_4E50_AE68_383997A14631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageDeploy.h : header file
//

#include <afxtempl.h>
#include "FolderDialog.h"
#include "PMPropPage.h"
#include "MainPropertySheet.h"


/////////////////////////////////////////////////////////////////////////////
// CPropPageDeploy dialog

class CPropPageDeploy : public CPMPropPage
{
	DECLARE_DYNCREATE(CPropPageDeploy)

// Construction
public:
	CPropPageDeploy(CMainPropertySheet* pMainSheet = NULL, const CString& crstrProjectPath = _T(""));
	~CPropPageDeploy();

    virtual CString GetProjectPath();

    virtual void InitializeFromRegistry(BOOL bGeneral = FALSE);
    virtual void SaveToRegistry(BOOL bGeneral = FALSE);
    
    BOOL Deploy();

// Dialog Data
	//{{AFX_DATA(CPropPageDeploy)
	enum { IDD = IDD_DEPLOY };
	CListCtrl	m_tTargetListCtrl;
	CString	m_strAddress;
	CString	m_strProjectPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageDeploy)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageDeploy)
	afx_msg void OnDeployBtn();
	afx_msg void OnProjectPathBtn();
	afx_msg void OnItemchangedTargetList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefreshListBtn();
	afx_msg void OnSetAddressBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeAddressEdit();
	afx_msg void OnChangeProjectPathEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void FillTargetListControl();
    BOOL CheckData();

	// d-188 02.02.05 SIS >>
	BOOL IsWindowsNT(void);
	BOOL IsAdmin(void);
	// d-188 02.02.05 SIS <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEDEPLOY_H__810521DB_2749_4E50_AE68_383997A14631__INCLUDED_)
