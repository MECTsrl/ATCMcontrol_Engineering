
#ifndef __PROJSAVEASDIALOG_H_
#define __PROJSAVEASDIALOG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



/////////////////////////////////////////////////////////////////////////////
// CProjectSaveAsDlg dialog

class CProjectSaveAsDlg : public CDialog
{
// Construction
public:
	CProjectSaveAsDlg(const CString& strProjectDir,
                      const CString& strOldProjectName,
                      CWnd* pParent = NULL);   // standard constructor

    CString GetProjectPath() {return m_strProjectPath;};
    CString GetProjectName() {return m_strProjectName;};

// Dialog Data
	//{{AFX_DATA(CProjectSaveAsDlg)
	enum { IDD = IDD_PROJECT_SAVE_AS };
	CEdit	m_editName;
	CEdit	m_editLocation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectSaveAsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    virtual void OnOK( );



// Implementation
protected:
    CString m_strOldProjectName;
    CString m_strProjectDir;

    CString m_strProjectPath;
    CString m_strProjectName;

	// Generated message map functions
	//{{AFX_MSG(CProjectSaveAsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnProjSaveAsBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __PROJSAVEASDIALOG_H_

