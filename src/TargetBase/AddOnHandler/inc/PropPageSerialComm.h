#ifndef _PROPPAGESERIALCOMM_H_
#define _PROPPAGESERIALCOMM_H_

#include "AddonBaseResource.h"
#include "BaseCommPropertyPage.h"

//------------------------------------------------------------------*
/**
 *  class CPropPageSerialComm
 *
 *  - property page for serial communications
 *  - data: COM-port, Baud-Rate, parity, data bits, stop bits
 *  - parameters are encoded in a single string (m_strSerialPar)
 *  - use SetConnectParameter() to initialize data in form of a string
 *  - use GetConnectParameter() to retrieve data in form of a string
 *
 *  @see    CBaseCommPropertyPage
 */
class CPropPageSerialComm : public CBaseCommPropertyPage
{
//	DECLARE_DYNCREATE(CPropPageSerialComm)

// Construction
public:
	CPropPageSerialComm(CWnd* pParent = NULL);
	~CPropPageSerialComm();

// Dialog Data
	//{{AFX_DATA(CPropPageSerialComm)
	enum { IDD = IDD_PROPPAGE_COMMRS232 };
	CComboBox	m_tStopBitsCombo;
	CComboBox	m_tParityCombo;
	CComboBox	m_tDataBitsCombo;
	CComboBox	m_tSerialPortCombo;
	CComboBox	m_tBaudRateCombo;
	//}}AFX_DATA

    virtual void SetConnectParameter(const CString& crstrParameter);
    virtual CString GetConnectParameter();

    virtual CString GetCommChannel();
    virtual CString GetCommChannelDescription();

    virtual void UpdatePageData();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageSerialComm)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageSerialComm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void ReadTargetData();
    void WriteTargetData();

protected:
    CString             m_strSerialPar;     // serial parameter string
    BOOL                m_bNewDlg;          // create new dialog

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
