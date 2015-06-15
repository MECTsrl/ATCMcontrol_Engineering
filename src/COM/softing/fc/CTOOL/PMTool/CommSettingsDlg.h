#if !defined(AFX_COMMSETTINGSDLG_H__F26D1965_F278_486B_96A6_52D477152EC5__INCLUDED_)
#define AFX_COMMSETTINGSDLG_H__F26D1965_F278_486B_96A6_52D477152EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommSettingsDlg.h : header file
//
#include "MainPropertySheet.h"

/////////////////////////////////////////////////////////////////////////////
// CCommSettingsDlg dialog

class CCommSettingsDlg : public CDialog
{
// Construction
public:
	CCommSettingsDlg(CMainPropertySheet* pMainSheet, CWnd* pParent = NULL);   // standard constructor
    ~CCommSettingsDlg();

    void Initialize(    const CStringArray& crastrTargetTypes,
                        const CString& crstrTargetType, 
                        const CString& crstrFirmwareVersion,
                        const CString& crstrCommChannel);

    void GetData(   CString& rstrTargetType,
                    CString& rstrFirmwareVersion,
                    CString& rstrCommChannel);

// Dialog Data
	//{{AFX_DATA(CCommSettingsDlg)
	enum { IDD = IDD_COMM_SETTINGS };
	CComboBox	m_tTargetTypeCombo;
	CComboBox	m_tFirmwareCombo;
	CComboBox	m_tCommChannelCombo;
	CString	    m_strConnPar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommSettingsDlg)
	afx_msg void OnSelchangeTargetTypeCombo();
	afx_msg void OnSelchangeTargetFirmwareCombo();
	afx_msg void OnSelchangeCommChannelCombo();
	afx_msg void OnEditchangeConnParCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeConnparEdit();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void FillTargetTypeCombo();
    void FillFirmwareCombo();
    void FillCommChannelCombo();

    BOOL GetFirmwareData(const CString& crstrFirmwareVersion, CFirmwareData& rtFWData);

protected:
    CMainPropertySheet*     m_pMainSheet;
    CStringArray            m_astrTargetTypes;
    CString                 m_strTargetType;
    CString                 m_strFirmwareVersion;
    CString                 m_strCommChannel;
    TFirmwareArr*           m_pFWArr;
    BOOL                    m_bConnEdited;  // connect par edited?
    BOOL                    m_bInit;        // initializing?
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMSETTINGSDLG_H__F26D1965_F278_486B_96A6_52D477152EC5__INCLUDED_)
