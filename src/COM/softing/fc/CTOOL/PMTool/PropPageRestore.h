#if !defined(AFX_PROPPAGERESTORE_H__81379C64_BFB7_47E3_A12A_099C4F6301A7__INCLUDED_)
#define AFX_PROPPAGERESTORE_H__81379C64_BFB7_47E3_A12A_099C4F6301A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageRestore.h : header file
//

#include "PMPropPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageRestore dialog

class CPropPageRestore : public CPMPropPage
{
	DECLARE_DYNCREATE(CPropPageRestore)

// Construction
public:
	CPropPageRestore(CMainPropertySheet* pMainSheet = NULL, const CString& crstrProjectPath = _T(""));
	~CPropPageRestore();

    virtual CString GetProjectPath();

    virtual void InitializeFromRegistry(BOOL bGeneral = FALSE);
    virtual void SaveToRegistry(BOOL bGeneral = FALSE);

    BOOL RestoreProject();

// Dialog Data
	//{{AFX_DATA(CPropPageRestore)
	enum { IDD = IDD_RESTORE };
	CString	m_strBackupPath;
	BOOL	m_bOPCServerConfig;
	CString	m_strProjectPath;
	int		m_iSource;
	CString	m_strAddress;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageRestore)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageRestore)
	afx_msg void OnBackupPathBtn();
	afx_msg void OnControlRadio();
	afx_msg void OnFileSystemRadio();
	afx_msg void OnProjectPathBtn();
	afx_msg void OnRestoreBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeAddressEdit();
	afx_msg void OnChangeBackupPathEdit();
	afx_msg void OnChangeProjectPathEdit();
	afx_msg void OnOpcserverConfigCheck();
	afx_msg void OnMaxtextPasswordEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void SetSourceMode();
    BOOL CheckData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGERESTORE_H__81379C64_BFB7_47E3_A12A_099C4F6301A7__INCLUDED_)
