#if !defined(AFX_PROPPAGEBACKUP_H__9A55359F_9D74_440A_8F37_2BF4507C86AD__INCLUDED_)
#define AFX_PROPPAGEBACKUP_H__9A55359F_9D74_440A_8F37_2BF4507C86AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageBackup.h : header file
//

#include "PMPropPage.h"

class CMainPropertySheet;

/////////////////////////////////////////////////////////////////////////////
// CPropPageBackup dialog

class CPropPageBackup : public CPMPropPage
{
	DECLARE_DYNCREATE(CPropPageBackup)

// Construction
public:
	CPropPageBackup(CMainPropertySheet* pMainSheet = NULL, const CString& crstrProjectPath = _T(""));
	~CPropPageBackup();

    virtual CString GetProjectPath();

    virtual void InitializeFromRegistry(BOOL bGeneral = FALSE);
    virtual void SaveToRegistry(BOOL bGeneral = FALSE);
    
    BOOL BackupProject();

// Dialog Data
	//{{AFX_DATA(CPropPageBackup)
	enum { IDD = IDD_BACKUP };
	CComboBox	m_tTargetCombo;
	CString	m_strBackupPath;
	CString	m_strProjectPath;
	BOOL	m_bOPCServerConfig;
	int		m_iDestination;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageBackup)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageBackup)
	afx_msg void OnBackupBtn();
	afx_msg void OnBackupPathBtn();
	afx_msg void OnControlRadio();
	afx_msg void OnFileSystemRadio();
	afx_msg void OnProjectPathBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeAddressEdit();
	afx_msg void OnChangeBackupPathEdit();
	afx_msg void OnChangeProjectPathEdit();
	afx_msg void OnOpcserverConfigCheck();
	afx_msg void OnChangePasswordEdit();
	afx_msg void OnMaxtextPasswordEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void SetDestinationMode();
    BOOL CheckData();

    void FillTargetCombo(const CString& crstrOldTargetString);
    CString GetCurrentTargetString();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEBACKUP_H__9A55359F_9D74_440A_8F37_2BF4507C86AD__INCLUDED_)
