#ifndef _WIZARDPAGENAMES_H_
#define _WIZARDPAGENAMES_H_

#include "ProjectWizardPage.h"
#include "AddonBaseResource.h"


//------------------------------------------------------------------*
/**
 *  class CWizardPageNames
 *
 *  - project new wizard property page for target specific data
 *  - the user selects a target from the list box of new targets
 *  - for a single target the following data is shown and can be
 *    modified: resource, target name, task name, program name, IEC language,
 *    OPC server address, firmware version, communication channel
 *  - the user can change parameters for communication channels
 *  - data for all targets is stored in CProjectWizardData
 *  - derived from CProjectWizardPage
 *
 *  @see    CProjectWizardPage
 */
class CWizardPageNames : public CProjectWizardPage 
{
public:
    CWizardPageNames(CPropertySheet *pParent, CProjectWizardData *pData);

public:
// Dialog Data
	//{{AFX_DATA(CWizardPageNames)
	enum { IDD = IDD_4CWIZ_NAMES };
	CComboBox	m_tCommChannelCombo;
	CComboBox	m_tVersionCombo;
    int         m_iActualResource;
    CString	    m_sResource;
    CString	    m_sProgram;
    int         m_iLanguage;
    CString	    m_sTask;
    CString	    m_sTarget;
    CString	    m_strOPCServer;
	//}}AFX_DATA

    CListCtrl   m_resources;
    CImageList  m_imageList;
    virtual void Init();
    void ReadResourceData();
    void WriteResourceData();
    virtual BOOL OnSetActive();
    virtual LRESULT OnWizardNext();
    virtual LRESULT OnWizardBack();
    void OnResourceChanged(NMHDR* pNMHDR, LRESULT* pResult);

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWizardPageNames)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWizardPageNames)
	afx_msg void On4cwizCommprop();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchange4cwizFirmwareVersion();
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()

protected:
    BOOL FillFirmwareVersionCombo();
    void FillCommChannelCombo();
    void UpdateControlData();

    void SetControlString();

    virtual void ShowCommunicationProperties();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
