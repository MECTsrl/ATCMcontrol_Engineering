
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "afxctl.h"
#include "cemainerror.h"
#include "cecompman.h"
#include "CEProjInfo.h"
#include "download.h"
#include "utilif.h"
#include "targetset.h"
#include "cedl_i.c"
#include "MsgWnd.h"
#include "Resource.h"

//----  Static Initializations:   ----------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CDownload

BEGIN_INTERFACE_MAP(CDownload, CCmdTarget)
	INTERFACE_PART(CDownload, IID_IDLEvents,DLEvents)
END_INTERFACE_MAP()


IMPLEMENT_DYNAMIC(CDownload, CCmdTarget)

CDownload::CDownload(REFCLSID rCLSID, const CString & strTarget) : m_pDL() 
    , m_strTarget(strTarget)
{
    m_dwCookie = 0;
	m_dwDownloaderCookie = 0;
    m_pTypeInfo = NULL;
    m_bPending = false;
    m_bReg = false;
    m_clsid = rCLSID;
    m_Type = CEDL_DOWNLD_INVALID;

    // SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK
    m_bConsoleFlag = FALSE;
    m_bDownloadConsole = FALSE;
#endif
    m_pWaitCursor = NULL;
}

CDownload::~CDownload()
{
    StopWait();
}

ULONG CDownload::AddRef ()
{
    return ExternalAddRef();
}

ULONG CDownload::Release()
{
    return ExternalRelease();
}

IUnknown* CDownload::GetDownloadComponent()
{
    HRESULT hr = S_OK;
    IUnknown *pUnk = NULL;

    if(!m_pDL) {
        return NULL;
    }

    hr = m_pDL.QueryInterface(&pUnk);
    return pUnk;
}


HRESULT CDownload::Create(CEDL_DOWNLD_TYPE type)
{
    CComBSTR sGenPath,sTarget;
    HRESULT hr = S_OK;
    LPUNKNOWN pUnkDL = NULL;
    CComPtr<IUnknown> pUnk;
    CComQIPtr<ICEProjInfo> pInfo;
    CCEStateManager *pStateManager = NULL;
    CString str;
#ifdef CONSOLE_DOWNLOAD_HACK
    CString strConsoleTarget;
    strConsoleTarget.Format("%s__vis", m_strTarget);
    CComBSTR    sConsoleTarget(strConsoleTarget);
#endif


    try {
        if(m_clsid.IsEmpty()) {
            //no downloader guid set - error return //TODO correct error
            _com_raise_error(E_FAIL);
        }

        pUnk.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        if(!pUnk) {
            _com_raise_error(E_CECOMPMAN_COMPONENT_NOT_ALIVE);
        }

        pInfo = pUnk;
        if(!pInfo) {
            _com_raise_error(E_NOINTERFACE);
        }

        pInfo->getProjectGenPath(&sGenPath);

        if(!m_pDL) {
            //create it
            hr = m_pDL.CoCreateInstance(m_clsid);
		    if(FAILED(hr)) {
                UTIL_DisplaySysError(hr,NULL,NULL);
			    _com_raise_error(hr);
		    }

            hr = LoadTypeInfo();
            if(FAILED(hr)) {
                //only a message 
                UTIL_DisplaySysError(hr,NULL,NULL);
                hr = S_OK;
            }
       
            Advise();
	    }
        else {

		    /* SU: dont forget to realize that if you call Exit during an pending
		       download, the download connection will be destroyed !!!
		    */
		    if(IsDownloading()) {
			    _com_raise_error(E_FAIL);
		    }
        }

        pStateManager = CEGetStateManager ();
        if (pStateManager) {
            pStateManager->TriggerDownloadInitiated(m_strTarget);    
        }

        hr = m_pDL->Exit();
        sTarget = m_strTarget;

#ifdef CONSOLE_DOWNLOAD_HACK
        if(m_bConsoleFlag && m_bDownloadConsole)
        {
	        hr = m_pDL->Init(sConsoleTarget,sGenPath,false);
        }
        else
        {
    	    hr = m_pDL->Init(sTarget,sGenPath,false);
        }
#else
	    hr = m_pDL->Init(sTarget,sGenPath,false);
#endif
	    
        if(FAILED(hr)) {
            TriggerDownloadStateChange(true);
		    _com_raise_error(hr);
	    }

        m_Type = type;
    }
    catch(_com_error err) {
        Destroy();
        hr = err.Error();
    }
    catch(CMemoryException* e) {
        e->Delete();
        Destroy();
        hr = E_OUTOFMEMORY;
    }
    return hr;
}

void CDownload::Destroy()
{
    CleanupDownload();
    StopWait();
    Unadvise();

    if(m_pDL) {
        m_pDL.Release();
    }

    FC_RELEASE_PTR(m_pTypeInfo);
}

// show wait cursor during download
void CDownload::StartWait()
{
    if(m_pWaitCursor)
    {
        delete m_pWaitCursor;
    }
    m_pWaitCursor = new CWaitCursor;
}


// hide wait cursor
void CDownload::StopWait()
{
    if(m_pWaitCursor)
    {
        delete m_pWaitCursor;
        m_pWaitCursor = NULL;
    }
}



// SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK

void CDownload::SetConsoleFlag(BOOL bConsoleFlag /*=TRUE*/)
{
    m_bConsoleFlag = bConsoleFlag;
}

#endif



void CDownload::CleanupDownload()
{
    m_bPending = false;
    m_Type = CEDL_DOWNLD_INVALID;
    Detach();
    if(m_pDL)
    {
        m_pDL->Exit();
    }
}

HRESULT CDownload::LoadTypeInfo(void)
{
    HRESULT hr;    
    LPTYPELIB ptlib = NULL;
    LPTYPEINFO ptinfo = NULL;
    
    hr = LoadRegTypeLib(LIBID_CEDLLib, 2, 0, LOCALE_SYSTEM_DEFAULT, &ptlib);    
    
    if (FAILED(hr)) {
        return hr;
    }
    
    // Get type information for interface of the object.
    hr = ptlib->GetTypeInfoOfGuid(__uuidof(IDLEvents), &ptinfo);
    if (FAILED(hr)){
        ptlib->Release();
        return hr;
    }    
    
    ptlib->Release();
    m_pTypeInfo = ptinfo;    
    return S_OK;
}



HRESULT CDownload::Advise(void)
{
    IUnknown* pUnkDL = NULL;
    IUnknown* pUnkEvnt = NULL;

	if(m_dwCookie) {
		return S_OK;
	}

    assert(m_pDL);
	
    HRESULT hr = InternalQueryInterface((IID*)&IID_IUnknown,(void **) &pUnkEvnt);
    if(FAILED(hr)) {
        return hr;
    }

    m_pDL->QueryInterface(IID_IUnknown,(void**)&pUnkDL);
    if(FAILED(hr)) {
        pUnkEvnt->Release();
        return hr;
    }

    if(!AfxConnectionAdvise(pUnkDL,IID_IDLEvents,pUnkEvnt,TRUE,&m_dwCookie)) {
        m_dwCookie = 0UL;
        return E_FAIL;
    }
           
    FC_RELEASE_PTR(pUnkDL);
    FC_RELEASE_PTR(pUnkEvnt);
    return S_OK;
}

HRESULT CDownload::Unadvise(void)
{
    IUnknown* pUnkDL = NULL;
    IUnknown* pUnkEvnt = NULL;

    if(!m_dwCookie) {
		return S_OK;
	}
    
	assert(m_pDL);

    HRESULT hr = InternalQueryInterface((IID*)&IID_IUnknown,(void **) &pUnkEvnt);
    if(FAILED(hr)) {
        return hr;
    }

    m_pDL->QueryInterface(IID_IUnknown,(void**)&pUnkDL);

    if(FAILED(hr)) {
        pUnkEvnt->Release();
        return hr;
    }

    AfxConnectionUnadvise(pUnkDL,IID_IDLEvents,pUnkEvnt,TRUE,m_dwCookie);
    m_dwCookie = 0UL;

    FC_RELEASE_PTR(pUnkDL);
    FC_RELEASE_PTR(pUnkEvnt);
    return S_OK;
}


HRESULT CDownload::Download(LPCTSTR pszUser, CEDL_DOWNLD_OPTIONS eOptions /*=DOWNLD_NO_OPTIONS*/)
{
    CComBSTR sUser,sPwd;
    CComQIPtr<IDLEx> pDLEx;
    HRESULT hr = S_OK;
    CCEStateManager *pStateManager = NULL;
    CTargetOnl* pTarget = NULL;
    long lHWND = 0L;
    
    assert(m_pDL);

	if(m_bPending) { //already pending - continue
		return S_FALSE;
	}

    pTarget = GetTarget();
    if(!pTarget) {
        return S_FALSE;
    }

    hr = m_pDL->Attach(pTarget->GetExclusiveSession());
	if(hr != S_OK) {
		return hr;
	}

    sUser = pszUser;
    sPwd = _T(""); 
    m_bPending = true;


#ifdef CONSOLE_DOWNLOAD_HACK
    if(m_bConsoleFlag)
    {
        CString strConsoleTarget;
        strConsoleTarget.Format("%s__vis\\loader.list", m_strTarget);
        CString strPath;
        CComPtr<IUnknown> pUnk;
        CComQIPtr<ICEProjInfo> pInfo;
        CComBSTR sGenPath;
        CFileStatus rStatus;

        pUnk.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        if(!pUnk) {
            _com_raise_error(E_CECOMPMAN_COMPONENT_NOT_ALIVE);
        }
        pInfo = pUnk;
        if(!pInfo) {
            _com_raise_error(E_NOINTERFACE);
        }
        pInfo->getProjectGenPath(&sGenPath);
        strPath = sGenPath;
        strPath = strPath + "\\" + strConsoleTarget;

        if(CFile::GetStatus(strPath, rStatus))
            m_bDownloadConsole = TRUE;
        else
            m_bDownloadConsole = FALSE;
    }
#endif

    StartWait();        // StopWait is called in XDLEvents::OnDownloadState()

    if(m_Type == CEDL_DOWNLD_SELECT) {
        CWnd* pWnd = AfxGetMainWnd();
        if(pWnd) {
            HWND hWnd = pWnd->GetSafeHwnd();
            lHWND = reinterpret_cast<long>(hWnd);
        }

        pDLEx = m_pDL;
        if(pDLEx) {
            hr = pDLEx->DownloadSelect(lHWND,sUser);
        }
        else {

            hr = E_NOINTERFACE;
        }
    }
    else 
    {
        hr = m_pDL->DownloadTarget(sUser,sPwd,m_Type, eOptions);
        if (SUCCEEDED (hr)) 
        {
            pStateManager = CEGetStateManager ();
            if (pStateManager != NULL) 
            {
                pStateManager->TriggerDownloadStarted (m_strTarget);
            }
        }
    }

    if(FAILED(hr)) 
    {
        CleanupDownload();
        StopWait();
    }
    return hr;
}

// SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK
HRESULT CDownload::DownloadConsole()
{
    // console is already downloaded -> return FALSE
    if(!m_bConsoleFlag || !m_bDownloadConsole)
    {
        return S_FALSE;
    }
    CComBSTR sUser,sPwd;
    HRESULT hr = S_OK;
    CTargetOnl* pTarget = NULL;
  
    assert(m_pDL);
    
    sPwd = _T(""); 

    hr = Create(CEDL_DOWNLD_ALL);
    if(hr != S_OK)
    {
        return hr;
    }

    m_bDownloadConsole = FALSE;     // don't call download console any more now

    if(hr != S_OK)
    {
        return E_FAIL;
    }

    pTarget = GetTarget();
    if(!pTarget) 
    {
        return E_FAIL;
    }

    hr = m_pDL->Attach(pTarget->GetExclusiveSession());
	if(hr != S_OK) 
    {
		return hr;
	}

    sUser = m_strUser;
    sPwd = _T(""); 
    m_bPending = true;

    StartWait();        // StopWait is called in XDLEvents::OnDownloadState()

    hr = m_pDL->DownloadTarget(sUser,sPwd,CEDL_DOWNLD_ALL, DOWNLD_NO_OPTIONS);

    if(hr != S_OK) 
    {
        CleanupDownload();
        StopWait();
    }

    return hr;
}
#endif


void CDownload::Detach()
{
    if(!m_pDL) {
        return;
    }
	m_pDL->Detach();
}

bool CDownload::IsDownloading ()
{
    return m_bPending;
}


void CDownload::Abort()
{
    if(!m_pDL) {
        return;
    }
    m_pDL->Abort();
}


CTargetOnl* CDownload::GetTarget()
{
    CTargetSet* pTargetSet = NULL;
    CTargetOnlBase* pBase = NULL;
    CTargetOnl* pTarget = NULL;

    pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet) {
        return NULL;
    }

    pBase = pTargetSet->GetTarget(m_strTarget);
    if(!pBase) {
        return NULL;
    }

    pTarget = dynamic_cast<CTargetOnl*>(pBase);
    return pTarget;
}

void CDownload::TriggerDownloadStateChange (bool bAborted)
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager == NULL)
        return;

    if (bAborted)
    {
        pStateManager->TriggerDownloadAborted (m_strTarget);
        return;
    }
    pStateManager->TriggerDownloadComplete (m_strTarget);
}


//------------------------------------------------------------------*
/**
 * get target download version.
 *
 * @param           [in/out] rstrVersionGUID: version guid from control
 * @param           [in/out] rstrProjName: project name from control
 * @return          hr
 * @exception       -
 * @see             -
*/
HRESULT CDownload::GetTargetDLVersion(CString& rstrVersionGUID, CString& rstrProjName)
{
    HRESULT hr;
    CComPtr<IDLProperties> pDLProperties;
    CComBSTR sGUID;
    CComBSTR sProjName;

    ASSERT (m_pDL != NULL);
    if (m_pDL == NULL)
    {
        return (E_FAIL);
    }

    hr = m_pDL->QueryInterface (IID_IDLProperties, (LPVOID *)&pDLProperties);
    if (FAILED (hr))
    {
        return (hr);
    }

    hr = pDLProperties->GetTargetDLVersion(&sGUID, &sProjName);
    if (FAILED (hr))
    {
        return (hr);
    }

    rstrVersionGUID = sGUID;
    rstrProjName = sProjName;
    return (S_OK);
}


void CDownload::ErrorMessage()
{
    ::AfxMessageBox(IDS_DOWNLOAD_ERROR, MB_ICONEXCLAMATION);
    // don't switch to download tab, relevant error messages can be found in target tab (LN)(??)
    //CMsgWnd* pMsgWnd = dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", false));
    //if(pMsgWnd)
    //{
    //    pMsgWnd->ShowDownloadWindow();
    //}
}

/////////////////////////////////////////////////////////////////////////////
// embedded interface XDLEvents methods

STDMETHODIMP_(ULONG) CDownload::XDLEvents::AddRef ()
{
	METHOD_PROLOGUE_EX (CDownload, DLEvents)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CDownload::XDLEvents::Release ()
{
	METHOD_PROLOGUE_EX (CDownload, DLEvents)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CDownload::XDLEvents::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CDownload, DLEvents)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}

STDMETHODIMP CDownload::XDLEvents::OnDownloadState(BSTR sTarget, BSTR sDomain,
    float fPercentage,CEDL_DOWNLD_STATE State)
{
    METHOD_PROLOGUE_EX (CDownload, DLEvents)

    switch(State) {
        case DOWNLD_READY:
            if(pThis->m_Type == CEDL_DOWNLD_SELECT) {
                //only react on abort for this type
                break;
            }
            pThis->CleanupDownload();
            pThis->TriggerDownloadStateChange (false);
            pThis->StopWait();
// SIS: TODO preliminary
#ifdef CONSOLE_DOWNLOAD_HACK
            pThis->DownloadConsole();
#endif
            break;
        case DOWNLD_INTR:
            pThis->StopWait();
            break;
        case DOWNLD_ERROR:
            pThis->ErrorMessage();
            pThis->StopWait();
            break;
        case DOWNLD_TRANSFER:
            break;
        case DOWNLD_UPLOAD:
			break;
		case DOWNLD_ABORTED:
            pThis->CleanupDownload();
            pThis->TriggerDownloadStateChange (true);
            pThis->StopWait();
            break;
        default:
            break;
    }
    return S_OK;
}


STDMETHODIMP CDownload::XDLEvents::OnDownloadError(BSTR sTarget, HRESULT hr, BSTR sDescr)
{
    METHOD_PROLOGUE_EX (CDownload, DLEvents)
    pThis->Abort();
    return S_OK;
}

STDMETHODIMP CDownload::XDLEvents::GetTypeInfoCount(UINT* pctinfo)
{
    METHOD_PROLOGUE_EX (CDownload, DLEvents)

    if(!pThis->m_pTypeInfo) {
        return E_NOTIMPL;
    }

    *pctinfo = 1;
    return S_OK;
}

STDMETHODIMP CDownload::XDLEvents::GetTypeInfo(UINT itinfo, LCID lcid,ITypeInfo** pptinfo)
{
    METHOD_PROLOGUE_EX (CDownload, DLEvents)

    if(!pThis->m_pTypeInfo) {
        return E_NOTIMPL;
    }

    if(itinfo != 0) {
        *pptinfo = NULL;
        return (ResultFromScode(DISP_E_BADINDEX));
    }

    pThis->m_pTypeInfo->AddRef();
    *pptinfo = pThis->m_pTypeInfo;
    return S_OK;   
}

STDMETHODIMP CDownload::XDLEvents::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
{
    METHOD_PROLOGUE_EX (CDownload, DLEvents)

    if(!pThis->m_pTypeInfo) {
        return E_NOTIMPL;
    }

    HRESULT hr = DispGetIDsOfNames(pThis->m_pTypeInfo,rgszNames,cNames,rgdispid);
    return hr;
}

STDMETHODIMP CDownload::XDLEvents::Invoke (DISPID dispid, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams, LPVARIANT pvarResult,
	LPEXCEPINFO pexcepinfo, UINT* puArgErr)
{
    METHOD_PROLOGUE_EX (CDownload, DLEvents)

    if(!pThis->m_pTypeInfo) {
        return E_NOTIMPL;
    }

    HRESULT hr = DispInvoke(this,pThis->m_pTypeInfo,dispid,wFlags,pDispParams,
        pvarResult,pexcepinfo,puArgErr);
    return hr;
}

BEGIN_MESSAGE_MAP(CDownload, CCmdTarget)
	//{{AFX_MSG_MAP(CDownload)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownload message handlers


