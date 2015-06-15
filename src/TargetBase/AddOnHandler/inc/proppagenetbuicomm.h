#if !defined(_PROPPAGENETBUICOMM_)
#define _PROPPAGENETBUICOMM_

#include "BaseCommPropertyPage.h"
#include "AddonBaseResource.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageNetBUIComm dialog

class CPropPageNetBUIComm : public CBaseCommPropertyPage
{
//	DECLARE_DYNCREATE(CPropPageNetBUIComm)

// Construction
public:
	CPropPageNetBUIComm();
	~CPropPageNetBUIComm();

// Dialog Data
	//{{AFX_DATA(CPropPageNetBUIComm)
	enum { IDD = IDD_PROP_PAGE_COMM_NETBUI };
	CString	m_strCNAddress;
	//}}AFX_DATA

    // communication channel
    virtual CString GetCommChannel();
    virtual CString GetCommChannelDescription();

    // connect parameters
    virtual void SetConnectParameter(const CString& crstrParameter);
    virtual CString GetConnectParameter();



// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageNetBUIComm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageNetBUIComm)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_PROPPAGENETBUICOMM_)