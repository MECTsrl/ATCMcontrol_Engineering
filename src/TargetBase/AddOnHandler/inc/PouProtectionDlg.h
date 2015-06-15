#ifndef _POUPROTECTIONDLG_H_
#define _POUPROTECTIONDLG_H_

#include "AddonBaseResource.h"
#include "SBLLexTree.h"
#include "CEProjInfo.h"

class CGHFile;

/////////////////////////////////////////////////////////////////////////////
// CPouProtectionDlg dialog

class CPouProtectionDlg : public CDialog
{
// Construction
public:
	CPouProtectionDlg(CGHFile* pGHFile, ICEProjInfo* pICEProjInfo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPouProtectionDlg)
	enum { IDD = IDD_POUPROTECTION };
	CListBox	m_tProtPouListCtrl;
	CListBox	m_tUnprotPouListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPouProtectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPouProtectionDlg)
	afx_msg void OnAddAllPous();
	afx_msg void OnAddPou();
	afx_msg void OnRemoveAllPous();
	afx_msg void OnRemovePou();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkProtectedPous();
	afx_msg void OnDblclkUnprotectedPous();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


    BOOL SaveToGhFile();
    BOOL LoadFromProject();
    BOOL LoadFromGhFile();
    void SaveOldAssoc();

    BOOL LoadPousOfTypeFromProject(const CString& crstrType);

    CString GetFilePathName(const CString& crstrFileName);
    BOOL DetermineFingerprint(const CString& crstrFileName, CString& rstrFingerprint);

    void FillListControls();

    void SetReadOnlyFlags();

    void AddNewPou(const CString& crstrFileName);
    void RemoveNewPou(const CString& crstrFileName);

protected:
    CSBLLexTree<bool>       m_tProtAssoc;       // contains file names of protected pous
    CSBLLexTree<bool>       m_tUnprotAssoc;     // contains file names of unprotected pous
    CSBLLexTree<bool>       m_tProtOldAssoc;    // contains file names of protected pous at beginning
    CSBLLexTree<bool>       m_tUnprotOldAssoc;  // contains file names of unprotected pous at beginning

    CGHFile*                m_pGHFile;          // gh file
    CComPtr<ICEProjInfo>    m_pICEProjInfo;     // project info interface pointer
    CString                 m_strProjPath;      // project path
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
