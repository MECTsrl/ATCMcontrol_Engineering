/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/FCSession.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FCSession.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/FCSession.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           CSC Online Control
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#include "stdafx.h"
#include "refobj.h"
#include "FCSession.h"
#include "VarSession.h"
#include "IBTSession.h"
#include "MsgSession.h"

#include "CSC_OnlineErr.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static const TCHAR  s_sessionSep = _T('|');

class CCommissioningMarshalReq : public CRefObj
{
public:
    /** stream for marshalling*/
    LPSTREAM m_pStream;
    /** machine address to set into*/
    CString m_strMachine;
    /** custom connect guid, if any*/
    CGuid m_guid;
    /** init string for custom initialization*/
    CString m_strInit;
    /** whole address string */
    CString m_strAddress;

    CCommissioningMarshalReq()
        : m_strMachine(), m_guid(), m_strInit() , m_strAddress()
    { 
        m_pStream = NULL; 
        _Module.Lock();
        AddRef();
    }

    virtual ~CCommissioningMarshalReq()
    {
        FC_RELEASE_PTR(m_pStream);
        _Module.Unlock();
    }
};

/////////////////////////////////////////////////////////////////////////////
// a Dummy MTA Thread...
static DWORD  g_dwDummyThreadID;
static HANDLE g_hDummyThread;
static HANDLE g_hEvtTerminate;
static HANDLE g_hEvtInitDone;

static DWORD WINAPI DummyMTAThread(LPVOID lpParam)
{
    InterlockedIncrement(&gs_nRunningThreads);
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    _Module.Lock();
    SetEvent(g_hEvtInitDone);
    WaitForSingleObject(g_hEvtTerminate, INFINITE);
    _Module.Unlock();
    CoUninitialize();
    InterlockedDecrement(&gs_nRunningThreads);
    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CFCSessions


//------------------------------------------------------------------*
CFCSessions::CFCSessions()
{
    m_dwThreadId = GetCurrentThreadId();
}

//------------------------------------------------------------------*
CFCSessions::~CFCSessions()
{
}

//------------------------------------------------------------------*
HRESULT
    CFCSessions::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void
    CFCSessions::FinalRelease()
{
    CleanoutLists();
}

//------------------------------------------------------------------*
void
    CFCSessions::CleanoutLists()
{
    POSITION    pos;
    m_cs.Lock();

    for (pos = m_sessionNameMap.GetStartPosition(); pos != NULL; ) {
        IUnknown    *pUnk;
        CString key;
        m_sessionNameMap.GetNextAssoc(pos, key, pUnk);
        _ASSERTE(pUnk != NULL);
        CComQIPtr<IFCConnect, &IID_IFCConnect> pSession = pUnk;
        if (pSession) {
            pSession->Disconnect();
        }
        pUnk->Release();
    }
    m_sessionNameMap.RemoveAll();
    m_cs.Unlock();
}

//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::Shutdown_Gracefully()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    TRACE(_T("CFCSessions::Shutdown_Gracefully with gs_nRunningThreads=%d\n"), gs_nRunningThreads);
    TrcPrint(TRC_INTERFACE, _T("CFCSessions::Shutdown_Gracefully with gs_nRunningThreads=%d\n"), gs_nRunningThreads);

    CleanoutLists();

    if (g_hDummyThread != NULL) {
        SetEvent(g_hEvtTerminate);
        //WaitForSingleObject(g_hDummyThread, INFINITE);
        CloseHandle(g_hDummyThread);
        CloseHandle(g_hEvtTerminate);
        g_hDummyThread = NULL;
        g_hEvtTerminate = NULL;
        g_dwDummyThreadID = 0;
    }

    bool bQuitMessage = false;
    DWORD dwTimeStart = GetTickCount();

    for (int i=0; gs_nRunningThreads!=0 && i<100; i++) {
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            bQuitMessage |= msg.message == WM_QUIT && msg.hwnd == 0;
            DispatchMessage(&msg);
            if (GetTickCount()-dwTimeStart>10000) {
                i=100;
                break;
            }
        }
        Sleep(100);
    }

    if (bQuitMessage)
        PostQuitMessage(0);

    return gs_nRunningThreads==0 ? S_OK : S_FALSE;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::OpenVarSession(BSTR sAddress, LPUNKNOWN *ppUnknown)
{
    CString strMachine,strAddress,strInit;
    CGuid guid;
    HRESULT hr = S_OK;
    IFCVarSession *pSession = NULL;
    CComObject<CFCVarSession>  *pObj = NULL;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    try {
        if (ppUnknown == NULL) {
            _com_raise_error(E_POINTER);
        }

        if(sAddress == NULL) {
            _com_raise_error(E_POINTER);
        }
        *ppUnknown = NULL;

        strAddress = sAddress;
        ParseAddress(strAddress,strMachine,guid,strInit);
        // try to find session in session list
        LookupSession(varSession, strAddress,ppUnknown);
        if (*ppUnknown != NULL) {
            // found ptr
            return S_OK;
        }

        TrcPrint(TRC_INTERFACE, _T("OpenVarSession to address '%s'\n"),strAddress);
        // session not found, create a new one:
        hr = CComObject<CFCVarSession>::CreateInstance(&pObj);
        // Beware: CreateInstance() does NOT increment ref count, no need to release pObj !!!
        if (FAILED(hr)) {
            _com_raise_error(hr);
        }
    
        hr = pObj->QueryInterface(IID_IFCConnect, (void**) &pSession);
        if (FAILED(hr)) {
            // session IF not supported
            _com_raise_error(E_NOINTERFACE);
        }
    
        *ppUnknown = pSession;
        // caller must release delivered pUnknown !!!
        // set names !!
        pObj->SetAddressInfo(strMachine,guid,strInit);
        // add to session map:
        CString strSessionName;
        strSessionName = BuildSessionName(varSession, strAddress);
        pSession->AddRef();     // we keep our own reference in our map
        m_cs.Lock();
        m_sessionNameMap.SetAt(strSessionName, *ppUnknown);
        m_cs.Unlock();
        // pSession reference is still alive; don't forget to Release() entry in map
    }
    catch(_com_error err) {
        hr = err.Error();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    return hr;
}

//------------------------------------------------------------------*
/**
 * Helper-Function to create the CFCCommissioningSession object
 * FREE Apartment
 */
static DWORD WINAPI CreateIbtSession(LPVOID lpParam)
{
    HRESULT hr;
    CCommissioningMarshalReq* pReq = (CCommissioningMarshalReq*) lpParam;
    
    _ASSERTE(pReq->m_pStream == NULL);

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    IFCConnect *pSession = NULL;
    CComObject<CFCCommissioningSession>   *pObj = NULL;
    hr = CComObject<CFCCommissioningSession>::CreateInstance(&pObj);
    // Beware: CreateInstance() does NOT increment ref count, no need to release pObj !!!
    if (SUCCEEDED(hr)) {
        hr = pObj->QueryInterface(IID_IFCConnect, (void**) &pSession);
        if (SUCCEEDED(hr)) {
            //store the session parameters
            pObj->SetAddressInfo(pReq->m_strAddress,pReq->m_strMachine,
                pReq->m_guid,pReq->m_strInit);
            // session IF supported
            hr = CoMarshalInterThreadInterfaceInStream(IID_IFCConnect, pSession, &pReq->m_pStream);
            pSession->Release();
        }
    }
    _ASSERTE(SUCCEEDED(hr));
    CoUninitialize();
    return hr;
}


//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::OpenCommissioningSession(BSTR sAddress, LPUNKNOWN *ppUnknown)
{
    CString strAddress,strSession;
    HRESULT hr = S_OK;
    IFCConnect *pSession = NULL;
    CFCCommissioningSession *pObj = NULL;
    CCommissioningMarshalReq *pReq = NULL;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    try {
        if (ppUnknown == NULL) {
            _com_raise_error(E_POINTER);
        }

        if(sAddress == NULL) {
            _com_raise_error(E_POINTER);
        }
        *ppUnknown = NULL;

        pReq = new CCommissioningMarshalReq();
        if(!pReq) {
            AfxThrowMemoryException();
        }

        strAddress = sAddress;
        pReq->m_strAddress = strAddress;
        ParseAddress(strAddress,pReq->m_strMachine,pReq->m_guid,pReq->m_strInit);
        // try to find session in session list
        LookupSession(ibtSession, strAddress,ppUnknown);
        if (*ppUnknown != NULL) {
            // found ptr
            FC_RELEASE_PTR(pReq);
            return S_OK;
        }

        TrcPrint(TRC_INTERFACE, _T("OpenCommissioningSession on address '%s'\n"), strAddress);
        //TODO CHECK what is that good for - keeping the lock count > 1 ?
        if (g_hDummyThread == NULL) {
            g_hEvtInitDone = CreateEvent(NULL, TRUE, FALSE, NULL);
            g_hEvtTerminate = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (g_hEvtInitDone != NULL && g_hEvtTerminate != NULL) {
                g_hDummyThread = CreateThread(NULL, 0, DummyMTAThread, NULL, 0, &g_dwDummyThreadID);
                if (g_hDummyThread != NULL) {
                    WaitForSingleObject(g_hEvtInitDone, INFINITE);
                    CloseHandle(g_hEvtInitDone);
                    g_hEvtInitDone = NULL;
                }
            }
        }

        // session not found, create a new one - but create in an MTA and marshal back to
        // the calling STA
        DWORD dwThreadID;
        HANDLE hCreateThread = CreateThread(NULL, 0, CreateIbtSession, (LPVOID)pReq, 0, &dwThreadID);
        if (hCreateThread == NULL) {
            _com_raise_error(E_FAIL);
        }

        WaitForSingleObject(hCreateThread, INFINITE);
        CloseHandle(hCreateThread);

        if(pReq->m_pStream == NULL) {
            _com_raise_error(E_FAIL);
        }
        
        //Stream received - unmarshal the interface now.
        hr = CoGetInterfaceAndReleaseStream(pReq->m_pStream, IID_IFCConnect, (void **)&pSession);
        if (FAILED(hr)) {
            _com_raise_error(hr);
        }
        //do not use it any more after unmarshaling
        pReq->m_pStream = NULL;
        
        strSession = BuildSessionName(ibtSession, strAddress);
        // caller must release delivered [out] pUnknown 
        *ppUnknown = pSession;
        pSession->AddRef();     // we keep our own reference in our map
        m_cs.Lock();
        m_sessionNameMap.SetAt(strSession, *ppUnknown);
        m_cs.Unlock();
        // pSession reference is still alive; don't forget to Release() entry in map
    }
    catch(_com_error err) {
        hr = err.Error();
        FC_RELEASE_PTR(pSession);
    }
    catch(CMemoryException *e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
        FC_RELEASE_PTR(pSession);
    }

    FC_RELEASE_PTR(pReq);
    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::OpenMessageSession(BSTR sAddress, LPUNKNOWN *ppUnknown)
{
    CString strMachine,strAddress,strInit;
    CGuid guid;
    HRESULT hr = S_OK;
    IFCConnect *pSession = NULL;
    CComObject<CFCMessageSession> *pObj = NULL;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    try {
        if (ppUnknown == NULL) {
            _com_raise_error(E_POINTER);
        }

        if(sAddress == NULL) {
            _com_raise_error(E_POINTER);
        }
        *ppUnknown = NULL;

        strAddress = sAddress;
        ParseAddress(strAddress,strMachine,guid,strInit);
        // try to find session in session list
        LookupSession(msgSession,strAddress,ppUnknown);
        if (*ppUnknown != NULL) {
            // found ptr
            return S_OK;
        }

        TrcPrint(TRC_INTERFACE, _T("OpenMessageSession on address '%s'\n"), strAddress);
        hr = CComObject<CFCMessageSession>::CreateInstance(&pObj);
        // Beware: CreateInstance() does NOT increment ref count, no need to release pObj !!!
        if (FAILED(hr)) {
            _com_raise_error(hr);
        }
    
        hr = pObj->QueryInterface(IID_IFCConnect, (void**) &pSession);
        if (FAILED(hr)) {
            // session IF not supported
            _com_raise_error(E_NOINTERFACE);
        }
        
        // set names !!
        pObj->SetAddressInfo(strAddress,strMachine,guid,strInit);

        CString strSessionName;
        strSessionName = BuildSessionName(msgSession, strAddress);

        *ppUnknown = pSession;
        // caller must release delivered pUnknown !!!
        pSession->AddRef();     // we keep our own reference in our map
        m_cs.Lock();
        m_sessionNameMap.SetAt(strSessionName, *ppUnknown);
        m_cs.Unlock();
        // pSession reference is still alive; don't forget to Release() entry in map
    }
    catch(_com_error err) {
        hr = err.Error();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::LookupVarSession(BSTR sAddress, LPUNKNOWN *ppUnknown)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    return LookupSession(varSession, sAddress,ppUnknown);
}

//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::LookupCommissioningSession(BSTR sAddress, LPUNKNOWN * ppUnknown)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    return LookupSession(ibtSession, sAddress, ppUnknown);
}

//------------------------------------------------------------------*
STDMETHODIMP CFCSessions::LookupMessageSession(BSTR sAddress, LPUNKNOWN * ppUnknown)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    return LookupSession(msgSession, sAddress, ppUnknown);
}


//------------------------------------------------------------------*
HRESULT CFCSessions::LookupSession(SessionType type, BSTR sAddress, LPUNKNOWN *ppUnknown)
{
    HRESULT hr = S_OK;
    CString strMachine, strInit;
    CGuid guid;

    try {
        CString strAddress = sAddress;
        LookupSession(type,strAddress,ppUnknown);
        hr = (*ppUnknown) ? S_OK : S_FALSE; 
    }
    catch(_com_error err) {
        hr = err.Error();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
    return hr;
}

void CFCSessions::LookupSession(SessionType type, LPCTSTR pszAddress, LPUNKNOWN *ppUnknown)
{
    CString strSessionName;
    IUnknown *pSession = NULL;

    if (ppUnknown == NULL || pszAddress == NULL) {
        _com_raise_error(E_POINTER);
    }
    *ppUnknown = NULL;
    // try to find session in session list
    strSessionName = BuildSessionName(type,pszAddress);
    m_cs.Lock();
    if (m_sessionNameMap.Lookup(strSessionName, pSession))
    {
        // found session, mark as used and return it
        *ppUnknown = pSession;
        (*ppUnknown)->AddRef();     // return to caller with his/her reference to IF
        m_cs.Unlock();
        return;
    }
    m_cs.Unlock();
    return;
}

//------------------------------------------------------------------*
/*
HRESULT CFCSessions::RemoveFromList(SessionType type, LPCTSTR pszAddress)
{
    HRESULT hr = S_OK;
    CString sSessionName;

    sSessionName = BuildSessionName(type, pszAddress);
    IUnknown *pSession = NULL;
    m_cs.Lock();
    if (m_sessionNameMap.Lookup(sSessionName, pSession))
    {
        m_sessionNameMap.RemoveKey(sSessionName);
        pSession->Release();
        hr = S_OK;
    }
    else {
        _ASSERTE(FALSE);    // must find IVarSession in map or we have inconsistency !!!
        hr = E_FAIL;
    }
    m_cs.Unlock();
    return hr;
}
*/
//------------------------------------------------------------------*
CString CFCSessions::BuildSessionName(SessionType type, LPCTSTR pszAddress)
{
    CString sSessionName;
    if (type == varSession) {
        sSessionName = _T("var");
    }
    else if (type == ibtSession) {
        sSessionName = _T("dload");
    }
    else if (type == msgSession) {
        sSessionName = _T("msg");
    }
    else {
        sSessionName = _T("??");
    }
    sSessionName += s_sessionSep;
    sSessionName += pszAddress;
    sSessionName.MakeUpper();
    return sSessionName;
}

//------------------------------------------------------------------*
/**
 * Parses target specific connection information. The target specific
 * connection information has the form:<br>
 * <b>{w-xy,8E2A6B33-60E9-11D1-BCDD-006097E1BE2F,COM1,9600}</b><br>
 * The parameters of the connection information are separated by 
 * ',' characters. The function separates the machine address, 
 * CLSID of target specific CSC Online server and remainder of the
 * target specific connection information.
 * @param			strAddress - specifies target specific conn-
 *                  ection information (see sample above).
 *                  The returned address information contains the 
 *                  the unwrapped address.
 * @param			CString & strMachine - the address of the host
 *                  machine will be returned there. In the sample this
 *                  would be "w-xy"
 * @param			CGuid & rGuid - The CLSID of the target specific
 *                  CSC Online server (OPC++) will be returned there.
 * @param			CString & strInitn - the remainder of the target
 *                  specific connection information will be returned with
 *                  this parameter. In the sample this would be "COM1,...".
 *
 * @exception       _com_error exception, CMemoryException.
 * @see			    4CSC_Architecture(V2.0).doc section 3.2.1.3
*/
void CFCSessions::ParseAddress(CString & strAddress,CString & strMachine, 
                               CGuid & rGuid, CString & strInit)
{
    int iIndex= 0,iFound = -1;
    CString strTemp,strAddr;
    strAddr = strAddress;

    rGuid.Empty();
    strInit.Empty();
    strMachine.Empty();

    strAddr.TrimLeft(); //trim whitespace characters
    strAddr.TrimRight();

    //Trim leading {[ and trailing }] characters
    strAddr.TrimLeft(gs_szMachineNameLeadIn);
    strAddr.TrimRight(gs_szMachineNameLeadOut);

    strAddress = strAddr; //assign unwrapped address to strAddress

    iFound = strAddr.Find(_T(','),iIndex); //lookup for the first ','
    if(iFound == -1) {
        //no ',' found - assume the form {w-xy}
        strMachine = strAddr;
        return;
    }

    //first ',' found - strip machine address
    strMachine = strAddr.Mid(iIndex,iFound - iIndex);
    strMachine.TrimLeft();
    strMachine.TrimRight();
    
    iIndex = iFound + 1;
    iFound = strAddr.Find(_T(','),iIndex); //lookup for the next ','

    if(iFound == -1) {
        //no ',' found - assume the form {w-xy,GUID}
        strTemp = strAddr.Mid(iIndex);
    }
    else {
        //found - extract guid
        strTemp = strAddr.Mid(iIndex, iFound - iIndex);
    }
    
    strTemp.TrimLeft();
    strTemp.TrimRight();

    //encapsulate with {} characters
    strInit = _T('{');
    strInit += strTemp;
    strInit += _T('}');
    
    rGuid = (LPCTSTR)strInit;
    strInit.Empty();
    //error check - correct GUID
    if(rGuid.IsEmpty()) {
        _com_raise_error(E_CSC_SESSION_BAD_ADDRESS);
    }
    
    if(iFound == -1) {
        return;
    }
    //store the trailing string into strInit
    iIndex = iFound + 1;
    strTemp = strAddr.Mid(iIndex);
    strTemp.TrimLeft();
    strTemp.TrimRight();

    strInit = strTemp;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: FCSession.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 10  *****************
 * User: Su           Date: 10.03.03   Time: 9:56
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Enhanced traces to deliver a more meaningful output.
 * 
 * *****************  Version 9  *****************
 * User: Su           Date: 17.07.01   Time: 17:58
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 8  *****************
 * User: Su           Date: 9.05.01    Time: 12:51
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * CFCMessageSession::SetAddressInfo needs full address as well.
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 30.04.01   Time: 14:45
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Use whole address as key.
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 19.04.01   Time: 18:35
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 3.04.01    Time: 13:28
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * OpenCommissioningSession needs parameters upon creation - hence marshal
 * operation in worker thread changes slightly. CFCSessions COM object is
 * no longer STA - can also be used from FREE apartments.
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 22.03.01   Time: 11:04
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 19.03.01   Time: 19:01
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:37
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Changed IFCSessions interface to have only addess string and no config
 * name. 
 * New header, removed private trace files, replaced by tracea.lib.
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 * *****************  Version 30  *****************
 * User: Ed           Date: 8/09/00    Time: 8:23p
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * fix a silly NT4.0 Problem with ShutdownGracefully
 * 
 *==
 *----------------------------------------------------------------------------*
*/
