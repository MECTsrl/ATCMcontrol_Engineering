
#ifndef __DOWNLOAD_H_
#define __DOWNLOAD_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CONSOLE_DOWNLOAD_HACK

//----  Aggregate Includes:   --------------------------------------*
#include "cedl.h"
#include "TargetOnlBase.h"
#include "TargetOnl.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CDownload command target

class CDownload : public CCmdTarget
{
	DECLARE_DYNAMIC(CDownload)

    CComQIPtr<IDL> m_pDL;
    DWORD       m_dwCookie;     //advise of the IDLEvents if
	DWORD		m_dwDownloaderCookie; //cookie for register to msg window 
    bool        m_bPending;
    bool        m_bReg;
    LPTYPEINFO  m_pTypeInfo;
    CString     m_strTarget;
    CGuid       m_clsid;
    CEDL_DOWNLD_TYPE m_Type;
// SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK
    BOOL        m_bConsoleFlag;
    BOOL        m_bDownloadConsole;
    CString     m_strUser;
#endif
    CWaitCursor* m_pWaitCursor;

//private Operations
    HRESULT Advise(void);
    HRESULT Unadvise(void);
    HRESULT LoadTypeInfo(void);
	void Detach();
    void TriggerDownloadStateChange (bool bAborted);
    void CleanupDownload();
    CTargetOnl* GetTarget();
// Operations
public:
    CDownload(REFCLSID,const CString & strTarget);
	virtual ~CDownload();

    ULONG AddRef();
    ULONG Release();

	HRESULT Create(CEDL_DOWNLD_TYPE);
    void Destroy();

    void StartWait();
    void StopWait();

// SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK
    void SetConsoleFlag(BOOL bConsoleFlag = TRUE);
#endif

    HRESULT Download(LPCTSTR pszUser, CEDL_DOWNLD_OPTIONS eOptions = DOWNLD_NO_OPTIONS);
	bool IsDownloading ();
	void Abort();
    void OnDebugSessionCB (CTargetOnl* pTarget, CSC_SESSION_STATE sessionState, HRESULT hrError);

    HRESULT GetTargetDLVersion(CString& rstrVersionGUID, CString& rstrProjName);

    IUnknown* GetDownloadComponent();

    void ErrorMessage();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownload)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDownload)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

    BEGIN_INTERFACE_PART(DLEvents, IDLEvents)
        STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
        STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid,ITypeInfo** pptinfo);
        STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
        STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams, LPVARIANT pvarResult,
	        LPEXCEPINFO pexcepinfo, UINT* puArgErr);
        STDMETHOD(OnDownloadState)(BSTR sTarget, BSTR sDomain, float fPercentage,CEDL_DOWNLD_STATE State);
        STDMETHOD(OnDownloadError)(BSTR sTarget, HRESULT hr, BSTR sDescr);
    END_INTERFACE_PART(DLEvents)

    DECLARE_INTERFACE_MAP()

// SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK
    HRESULT DownloadConsole();
#endif
};

#endif // __DOWNLOAD_H_
