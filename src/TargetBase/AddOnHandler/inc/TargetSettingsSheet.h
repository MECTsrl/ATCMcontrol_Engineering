#ifndef _TARGETSETTINGSSHEET_H_
#define _TARGETSETTINGSSHEET_H_

#include "AddonBaseResource.h"
#include "CEProjInfo.h"

class CTargetSettingsSheet;
class CBaseTargetData;


/////////////////////////////////////////////////////////////////////////////
// CDebugSettingsPage dialog

//------------------------------------------------------------------*
/**
 * CDebugSettingsPage
 *
 * @see             
*/
class CDebugSettingsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDebugSettingsPage)

// Construction
public:
	CDebugSettingsPage();
	~CDebugSettingsPage();
    void Init(CTargetSettingsSheet* pParent);

// Dialog Data
	//{{AFX_DATA(CDebugSettingsPage)
	enum { IDD = IDD_DEBUG_PROPPAGE };
	int		m_iDebugOpt;
	BOOL	m_bNCCDisabled;
	//}}AFX_DATA

public:
    BOOL                    m_bSupportNCC;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDebugSettingsPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDebugSettingsPage)
	afx_msg void OnMarkDirty();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CTargetSettingsSheet*   m_pParent;
    BOOL                    m_bOldNCCStatus;
};


/////////////////////////////////////////////////////////////////////////////
// CConnectSettingsPage dialog

class CConnectSettingsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectSettingsPage)

// Construction
public:
	CConnectSettingsPage();
	~CConnectSettingsPage();
    void Init(CTargetSettingsSheet* pParent);

// Dialog Data
	//{{AFX_DATA(CConnectSettingsPage)
	enum { IDD = IDD_CONNECT_PROPPAGE };
	CString	m_strConnectTimeout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectSettingsPage)
  	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConnectSettingsPage)
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeConnectTimeout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CTargetSettingsSheet* m_pParent;

};


/////////////////////////////////////////////////////////////////////////////
// CTargetSettingsSheet

//------------------------------------------------------------------*
/**
 * CTargetSettingsSheet
 *
 * @see             
*/
class CTargetSettingsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTargetSettingsSheet)

// Construction
public:
	CTargetSettingsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTargetSettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

    virtual ~CTargetSettingsSheet();
    
    virtual HRESULT Init(const CString& strFileName, ICEProjInfo* pICEProjInfo, CBaseTargetData* pTargetData);
    virtual BOOL StoreChanges();
    
    void SetChanged(BOOL changed);

protected:
    int ReadDebugState(BOOL& bNCC);
    CString ReadConnectTimeout();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetSettingsSheet)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CTargetSettingsSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CDebugSettingsPage              m_debugPage;
    CConnectSettingsPage            m_connectPage;
    CString                 m_strFileName;
    CComPtr<ICEProjInfo>    m_pICEProjInfo;
    CBaseTargetData*        m_pBaseTargetData;
    BOOL                    m_bChanged;

    CString                 m_strOrigDebug;
};

#endif