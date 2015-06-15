#ifndef _PROPPAGESOCKETCOMM_H_
#define _PROPPAGESOCKETCOMM_H_

#include "AddonBaseResource.h"
#include "BaseCommPropertyPage.h"


/////////////////////////////////////////////////////////////////////////////
// CPropPageSocketComm dialog

//------------------------------------------------------------------*
/**
 *  class CPropPageSocketComm
 *
 *  - property page for socket communication (TCP/IP)
 *  - data: IP address
 *  - use SetConnectParameter() to initialize data
 *  - use GetConnectParameter() to retrieve data
 *
 *  @see    CBaseCommPropertyPage
 */
class CPropPageSocketComm : public CBaseCommPropertyPage
{
//	DECLARE_DYNCREATE(CPropPageSocketComm)

// Construction
public:
	CPropPageSocketComm(CWnd* pParent = NULL);
	~CPropPageSocketComm();

// Dialog Data
	//{{AFX_DATA(CPropPageSocketComm)
	enum { IDD = IDD_PROPPAGE_COMM_SOCKET };
	CString	m_strIPAddress;
	//}}AFX_DATA

    virtual void SetConnectParameter(const CString& crstrParameter);
    virtual CString GetConnectParameter();

    virtual CString GetCommChannel();
    virtual CString GetCommChannelDescription();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageSocketComm)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageSocketComm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    BOOL CheckIPAddress();



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif