#if !defined(AFX_SETTINGSDIALOGS_H__77501171_C64B_44AC_B437_40223925A525__INCLUDED_)
#define AFX_SETTINGSDIALOGS_H__77501171_C64B_44AC_B437_40223925A525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Settings.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CLibrarySettingsDialog dialog

class CLibrarySettingsDialog : public CDialog
{
// Construction
public:
	CLibrarySettingsDialog(CWnd* pParent = NULL);   // standard constructor
    ~CLibrarySettingsDialog();

// Dialog Data
	//{{AFX_DATA(CLibrarySettingsDialog)
	enum { IDD = IDD_SETTINGS_LIBRARY };
	CComboBox	m_cbTarget;
	CString	m_strLibName;
	CString	m_strLibPath;
	BOOL	m_bStandardLibPath;
	int		m_iTargetType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibrarySettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    class CTargetDisplayInfo
    {
    public:
        CString strTargetType;
        CString strTargetName;
    };

	// Generated message map functions
	//{{AFX_MSG(CLibrarySettingsDialog)
	virtual void OnOK();
	afx_msg void OnNewBtLibraryPath();
	afx_msg void OnLibStandardPath();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void GetAllTargets();

private:
    CString m_strPathValidated;
    BOOL    m_validate;

    CString m_strTarget;
    CArray<CTargetDisplayInfo, CTargetDisplayInfo&> m_arrTargetDisplayInfo;
};



/////////////////////////////////////////////////////////////////////////////
// CPasswordSettingsDialog dialog

class CPasswordSettingsDialog : public CDialog
{
// Construction
public:
	CPasswordSettingsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasswordSettingsDialog)
	enum { IDD = IDD_SETTINGS_PASSWORD };
	CString	m_strFullPwd;
	CString	m_strReadPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordSettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InitValues();

	// Generated message map functions
	//{{AFX_MSG(CPasswordSettingsDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




/////////////////////////////////////////////////////////////////////////////
// CEngineeringSettingsDialog dialog

class CEngineeringSettingsDialog : public CDialog
{
// Construction
public:
	CEngineeringSettingsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEngineeringSettingsDialog)
	enum { IDD = IDD_SETTINGS_ENGINEERING };
	BOOL	m_bXRefDisabled;
	BOOL	m_bConsoleInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEngineeringSettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InitValues();

	// Generated message map functions
	//{{AFX_MSG(CEngineeringSettingsDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};




#endif // !defined(AFX_SETTINGSDIALOGS_H__77501171_C64B_44AC_B437_40223925A525__INCLUDED_)

