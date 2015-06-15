
#ifndef __PRINTOPTIONSDLG_H_
#define __PRINTOPTIONSDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CPrintOptionsDlg : public CDialog
{
// Construction
public:
	CPrintOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintOptionsDlg)
	enum { IDD = IDD_PRINT_OPTIONS };
	CComboBox	m_tModuleCombo;
	//}}AFX_DATA

    void SetPrintModules(CStringArray& rastrPrintModules);

    CString GetPrintModule();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    void FillCombo();

	// Generated message map functions
	//{{AFX_MSG(CPrintOptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CString m_strModule;

protected:
    CStringArray    m_astrModules;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __PRINTOPTIONSDLG_H_
