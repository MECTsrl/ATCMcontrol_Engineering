#ifndef _PROPPAGETARGETGENERAL_H_
#define _PROPPAGETARGETGENERAL_H_

#include "AddonBaseResource.h"
#include "BasePropertyPage.h"
#include "CEProjInfo.h"

class CBaseAddonParameter;

//------------------------------------------------------------------*
/**
 *  class CPropPageTargetGeneral
 *
 *  - property page for general target data
 *  - data: name, OPC-Server, assigned resource, firmware version, 
 *    communication channel
 *  - derived from CBasePropertyPage
 *
 *  @see    CTargetPropertySheet, CBasePropertyPage
 */
class CPropPageTargetGeneral : public CBasePropertyPage
{
// Construction
public:
	CPropPageTargetGeneral(CWnd* pParent = NULL);   // standard constructor

    virtual void Init(ICEProjInfo* pProjInfo, CBaseAddonParameter* pAddonPar, const CStringArray& crastrResources, BOOL bNewDlg);

    virtual BOOL Validate();

// Dialog Data
	//{{AFX_DATA(CPropPageTargetGeneral)
	enum { IDD = IDD_PROPPAGE_TARGET_GENERAL };
	CString	m_strName;
	CString	m_strOPCServer;
	CString	m_strLastModDate;
	CString	m_strCreateUser;
	CString	m_strLastModUser;
	CString	m_strCreateDate;
	CComboBox	m_tResourceCombo;
	CComboBox	m_tFirmwareCombo;
	CComboBox	m_tCommChannelCombo;
	//}}AFX_DATA

    CString     m_strResource;      // current resource
    CString     m_strCommChannel;   // current communication channel
    UINT        m_uiFirmwareIndex;  // current firmware index


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageTargetGeneral)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageTargetGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePropgenCommChannelCb();
	afx_msg void OnSelchangePropgenFirmware();
	afx_msg void OnChangePropgenName();
	afx_msg void OnSelchangePropgenResources();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void FillCombos();
    void FillResourceCombo();
    void FillFirmwareCombo();
    void FillCommChannelCombo();

    BOOL CheckDialogData();

    void CheckCommChannels();
    
protected:
    CComPtr<ICEProjInfo>    m_pProjInfo;        // project info interface pointer 
                                                // used to check identifier
    CBaseAddonParameter*    m_pAddonPar;        // target addon parameter
    CStringArray            m_astrResources;    // resource names
    BOOL                    m_bNewDlg;          // create new dialog
    BOOL                    m_bNameChanged;     // name changed
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif