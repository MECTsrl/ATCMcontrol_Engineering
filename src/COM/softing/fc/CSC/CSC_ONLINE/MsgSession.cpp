/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/MsgSession.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: MsgSession.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/MsgSession.cpp $
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
 *  14.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#include "stdafx.h"
#include "cfacility.h"
#include "MsgSession.h"

#include "CSC_OnlineErr.h"
#include "CSC_OnlSrv.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFCMessageSessionThread
//------------------------------------------------------------------*
CFCMessageSessionThread::CFCMessageSessionThread()
{
   m_pMsgSession = NULL;
   m_pMsgSessionStream = NULL;
   m_bShutdownRequest = false;
   m_hShutdownRequest = CreateEvent(NULL, TRUE, FALSE, NULL);
   m_sNodeName = NULL;
   m_cookie = 0;
   m_pMessage = NULL;
   m_pCheckConnection = NULL;
   m_dwMsgNotifyCookie = 0;
   m_pRawMsgSession = NULL;
   m_lKey = 0;
}

//------------------------------------------------------------------*
CFCMessageSessionThread::~CFCMessageSessionThread()
{
   CloseHandle(m_hShutdownRequest);
   FC_FREESYSTRING(m_sNodeName);
   FC_RELEASE_PTR(m_pRawMsgSession);
}

//------------------------------------------------------------------*
/*static*/ DWORD CFCMessageSessionThread::ThreadMain(LPVOID lpParam)
{
    InterlockedIncrement(&gs_nRunningThreads);
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    CFCMessageSessionThread *_this = (CFCMessageSessionThread *)lpParam;
    HRESULT hr = _this->ConnectThread();
    _this->Release();

    CoUninitialize();
    InterlockedDecrement(&gs_nRunningThreads);
    return hr;
}

//------------------------------------------------------------------*
HRESULT CFCMessageSessionThread::ConnectThread()
{
    HRESULT hr = CoGetInterfaceAndReleaseStream(m_pMsgSessionStream, IID__FCMessageSessionNotificationInternal,
                                                (void **)&m_pMsgSession);
    m_pMsgSessionStream = NULL;
    if (FAILED(hr))
      return hr;

    while(!m_bShutdownRequest) {
        hr = DoConnect();
        if (FAILED(hr)) {
            m_pMsgSession->OnStateChanged(m_cookie, interrupted, hr);
            WaitForSingleObject(m_hShutdownRequest, 20000);
            continue;
        }
        m_pMsgSession->OnStateChanged(m_cookie, RW_established, S_OK);
        while (!m_bShutdownRequest) {
            if (WAIT_TIMEOUT == WaitForSingleObject(m_hShutdownRequest, 10000)) {
                hr = DoCheckConnection();
                if (FAILED(hr)) {
                  m_pMsgSession->OnStateChanged(m_cookie, interrupted, hr);
                  break;
                }
            }
        }
    }

    DoDisconnect();
    m_pMsgSession->Release();
    m_pMsgSession = NULL;
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT CFCMessageSessionThread::DoConnect()
{
    HRESULT hr;

    if (m_pMessage != NULL)
        return S_OK;

    TrcPrint(TRC_INTERFACE, _T("MsgSession: DoConnect(%s)\n"),(LPCTSTR) m_pRawMsgSession->m_strMachineName);

    hr = ConnectToOnlineServer(&m_pMessage);
    if (FAILED(hr))
        return hr;

    if (m_pMessage == NULL)
        return E_FAIL;

    hr = m_pMessage->QueryInterface(IID_IConnection,(void**)&m_pCheckConnection); 
    if (FAILED(hr))
        return hr;

    hr = Login();
    if (FAILED(hr)) {
        DoDisconnect();
        return hr;
    }

    hr = AtlAdvise(m_pMessage, GetUnknown(), IID__ICERemoteMessageEvent, &m_dwMsgNotifyCookie);
    if (FAILED(hr)) {
        DoDisconnect();
        return hr;
    }
    hr = m_pMessage->Connect(m_lKey);
    if (FAILED(hr)) {
        TRACE(_T("MsgSession::Connected callback: error while trying to connect to message source; 0x%08X\n"), hr);
        TrcPrint(TRC_ERROR, _T("MsgSession::Connected callback: error while trying to connect to message source; 0x%08X\n"), hr);
        DoDisconnect();
        return hr;
    }

    return hr;
}

//------------------------------------------------------------------*
HRESULT CFCMessageSessionThread::DoDisconnect()
{
    HRESULT hr;

    if (m_pMessage != NULL) {

        TrcPrint(TRC_INTERFACE, _T("MsgSession: DoDisconnect(%s)\n"),(LPCTSTR) m_pRawMsgSession->m_strMachineName);

        hr = m_pMessage->Disconnect(m_lKey);

        if (m_dwMsgNotifyCookie != 0UL) {
            hr = AtlUnadvise(m_pMessage, IID__ICERemoteMessageEvent, m_dwMsgNotifyCookie);
        }
        m_dwMsgNotifyCookie = 0;

        Logout();

        FC_RELEASE_PTR(m_pCheckConnection);
        FC_RELEASE_PTR(m_pMessage);
        CoDisconnectObject(this, 0);
    }
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT CFCMessageSessionThread::DoCheckConnection()
{
    HRESULT hr = E_CSC_NO_CONNECT_TO_CONTROL;

    if (m_pCheckConnection)
    {
        hr = m_pCheckConnection->CheckConnection();
        if(SUCCEEDED(hr)) {
            return hr;
        }
        // we have lost connection
        TRACE(_T("lost connection of MsgSession! hr=0x%08X\n"), hr);
        ::TrcPrint(TRC_ERROR, _T("lost connection of MsgSession! hr=0x%08X\n"), hr);
        //Do not call any interface method except Release()
        m_dwMsgNotifyCookie = 0;
        m_lKey = 0;

        FC_RELEASE_PTR(m_pCheckConnection);
        FC_RELEASE_PTR(m_pMessage);
        CoDisconnectObject(this, 0);
    }

	return hr;
}

//------------------------------------------------------------------*
HRESULT CFCMessageSessionThread::StartThread(CFCMessageSession *pRawMsgSession, long cookie)
{
    HRESULT hr = pRawMsgSession->get_MachineName(&m_sNodeName);
    if (FAILED(hr))
      return hr;
    hr = CoMarshalInterThreadInterfaceInStream(IID__FCMessageSessionNotificationInternal,
                                               (_FCMessageSessionNotificationInternal*)pRawMsgSession,
                                               &m_pMsgSessionStream);
    if (FAILED(hr))
      return hr;

    pRawMsgSession->AddRef();
    m_pRawMsgSession = pRawMsgSession;
    m_cookie = cookie;

    AddRef();
    DWORD threadID;
    HANDLE hThread = CreateThread(NULL, 0, ThreadMain, (LPVOID)this, 0, &threadID);
    if (hThread == NULL)
      return E_FAIL;

    CloseHandle(hThread);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CFCMessageSessionThread::StopThread()
{
    m_bShutdownRequest = true;
    SetEvent(m_hShutdownRequest);
    return S_OK;
}


//=======================================================================
//      interface   _ICERemoteMessageEvent
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSessionThread::OnNewMessage(BSTR sMessage)
{
    HRESULT hr;
    hr = m_pMsgSession->OnMessageReceived(m_cookie, sMessage);
    return hr;
}


//------------------------------------------------------------------*
HRESULT
    CFCMessageSessionThread::ConnectToOnlineServer(IMessages **ppMessages)
{
    MULTI_QI queue[1];
    COSERVERINFO     serverInfo;
	DWORD clsctx;
    CComPtr<ICustomConnect> pConn;
    CComPtr<IUnknown>pUnk;
    CComBSTR sInit;
    HRESULT hr = S_OK;

    FC_PCHECK_HR(ppMessages,E_POINTER);
    *ppMessages = NULL;

    queue[0].pItf = NULL;
    queue[0].hr = 0;
    // set up server node info:
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;
    USES_CONVERSION;
	serverInfo.pwszName = T2OLE(m_pRawMsgSession->m_strMachineName);
	serverInfo.pAuthInfo = 0;
	clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;

    if(m_pRawMsgSession->m_clsidConn.IsEmpty()) {
        queue[0].pIID = &IID_IMessages;

        hr = ::CoCreateInstanceEx(m_pRawMsgSession->m_clsidCtrl, NULL, 
            clsctx, &serverInfo,FC_ARRAY_LEN(queue),queue);

        if(SUCCEEDED(hr)) {
            *ppMessages = (IMessages *)queue[0].pItf;
        }
        return hr;
    }

    //custom connect case
    queue[0].pIID = &IID_ICustomConnect;

    hr = ::CoCreateInstanceEx(m_pRawMsgSession->m_clsidConn, NULL, 
        clsctx, &serverInfo, FC_ARRAY_LEN(queue), queue);
    if(FAILED(hr)) {
        return hr;
    }

    pConn.Attach((ICustomConnect*)queue[0].pItf);
    sInit = m_pRawMsgSession->m_strInit;
    
    hr = pConn->CreateControlManagement(sInit,&pUnk);
    if(FAILED(hr)) {
        return hr;
    }
    
    return pUnk->QueryInterface(IID_IMessages,(void**)ppMessages);
}

HRESULT CFCMessageSessionThread::Login()
{
    HRESULT hr;
    CComQIPtr<IControlMgt> pControlMgt;
    
    _ASSERTE(m_pRawMsgSession);

    if(m_lKey) {
        //already logged in
        return S_OK;
    }
    
    if(!m_pMessage) {
        return E_NOINTERFACE;
    }

    pControlMgt = m_pMessage; //applies a QI
    if(!pControlMgt) {
        return E_NOINTERFACE;
    }

    hr = pControlMgt->Login(m_pRawMsgSession->m_sUser,
        m_pRawMsgSession->m_sPassword,&m_lKey);

    if(FAILED(hr)) {
        m_lKey = 0;
    }

    return hr;   
}

HRESULT CFCMessageSessionThread::Logout()
{
    HRESULT hr;
    CComQIPtr<IControlMgt> pControlMgt;
    
    if(!m_lKey || !m_pMessage) {
        //already logged out
        return S_OK;
    }

    pControlMgt = m_pMessage; //applies a QI
    if(!pControlMgt) {
        return E_NOINTERFACE;
    }

    hr = pControlMgt->Logout(m_lKey);
    m_lKey = 0;
    return hr;   
}

/////////////////////////////////////////////////////////////////////////////
// CFCMessageSession

//------------------------------------------------------------------*
CFCMessageSession::CFCMessageSession()
    : m_sUser() , m_strMachineName() , m_clsidConn(), m_clsidCtrl(),
    m_strInit(), m_strAddress(), m_sPassword()
{
    m_state = not_connected;
    m_cookieThread = 1;
    m_pSessionThread = NULL;
}

//------------------------------------------------------------------*
CFCMessageSession::~CFCMessageSession()
{
}

void CFCMessageSession::SetAddressInfo(CString & strAddress, CString & strMachine, 
                                       CGuid & guid, CString & strInit)
{
    m_strAddress = strAddress;
    m_strMachineName = strMachine;
    m_clsidConn = guid;
    m_strInit = strInit;

    if(m_clsidConn.IsEmpty()) {
        m_clsidCtrl = CLSID_ControlMgt;
    }
    else {
        //everything works over coclass custom connect and ICustomConnect interface.
        m_clsidCtrl.Empty();
    }
}

//=======================================================================
//      interface   IMessageSession
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::Connect(/*[in, string]*/BSTR sUserName,/*[in, string]*/BSTR sPassword)
{
    HRESULT hr = S_OK;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    TrcPrint(TRC_INTERFACE, _T("MsgSession: Connect(%s)\n"),(LPCTSTR) m_strMachineName);
    
    FC_PCHECK_HR(sUserName,E_POINTER);
    m_sUser = sUserName;
    m_sPassword = sPassword;
    if (m_pSessionThread == NULL) {
        CComObject<CFCMessageSessionThread> *pSessionThread;
        hr = CComObject<CFCMessageSessionThread>::CreateInstance(&pSessionThread);
        pSessionThread->AddRef();
        m_pSessionThread = pSessionThread;
        m_state = waiting_for_connect;
        FireStateChangeHelper(waiting_for_connect);
        hr = m_pSessionThread->StartThread(this, m_cookieThread);
    }
    
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::Disconnect()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    TrcPrint(TRC_INTERFACE, _T("MsgSession: Disconnect(%s)\n"),(LPCTSTR) m_strMachineName);
    HRESULT hr = S_OK;

    if (m_pSessionThread != NULL) {
        m_cookieThread++;
        m_state = regular_shutdown;
        FireStateChangeHelper(regular_shutdown);
        hr = m_pSessionThread->StopThread();
        m_pSessionThread->Release();
        m_pSessionThread = NULL;
        m_state = not_connected;
        m_sUser.Empty();
        FireStateChangeHelper(not_connected);
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCMessageSession::get_MachineName(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_strMachineName.AllocSysString();
    return S_OK;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::get_IsConnected(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (m_state == RW_established);
	return S_OK;
}

STDMETHODIMP CFCMessageSession::get_SessionState(/*[out, retval]*/CSC_SESSION_STATE *pVal)
{
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_state;
    return E_NOTIMPL;
}

STDMETHODIMP CFCMessageSession::get_CurrentUser(/*[out, retval]*/ BSTR *pVal)
{
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = m_sUser;
    return S_OK;
}

STDMETHODIMP CFCMessageSession::ForceDisconnect(/*[in, string]*/ BSTR sUserName, /*[in, string]*/BSTR sPassword,
        /*[in, string]*/ BSTR sEngMachineName)
{
    //simply calls Disconnect.
    return Disconnect();
}


//=======================================================================
//      interface   _FCMessageSessionNotificationInternal
//=======================================================================

STDMETHODIMP
    CFCMessageSession::OnStateChanged(/*[in]*/ long cookie, /*[in]*/ CSC_SESSION_STATE newState, /*[in]*/ HRESULT hrError)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (cookie != m_cookieThread)
        return E_FAIL;
    m_state = newState;
    FireStateChangeHelper(newState,hrError);
    return S_OK;
}

STDMETHODIMP 
    CFCMessageSession::OnMessageReceived(/*[in]*/ long cookie,/*[in]*/ BSTR sMessage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (cookie != m_cookieThread)
        return E_FAIL;
    return Fire_OnNewMessage(CComBSTR(m_strAddress), E_FACILITY_CONTROL, S_OK, sMessage);
}
//=======================================================================
//      interface   IMessageHistory
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::GetMinMaxHistoryIDs(long *pFirst, long *pLast)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::GetHistoryMessage(long idHistory, BSTR * psMessage, DATE * pdt)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::get_MaxHistoryLength(long *pVal)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::put_MaxHistoryLength(long newVal)
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCMessageSession::ClearHistoryBuffer()
{
    return E_NOTIMPL;
}

//------------------------------------------------------------------*
/**
 * Helper to call Fire_OnStateChanged().
 *
 * @param   newState - new session (connection) state.
 * @param   hError - possible error code.
 * @see     CSC_SESSION_STATE.
*/
void CFCMessageSession::FireStateChangeHelper(CSC_SESSION_STATE newState, HRESULT hError /*= S_OK*/)
{
    TrcPrint(TRC_INTERFACE, _T("MsgSession: OnStateChanged(%s) state='%s'\n"),
        (LPCTSTR) m_strMachineName,GetSessionStateString(newState));
    Fire_OnStateChanged(GetUnknown(), newState, hError);
}

/*
 *----------------------------------------------------------------------------*
 *  $History: MsgSession.cpp $
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
 * User: Su           Date: 10.03.03   Time: 10:00
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Enhanced traces to deliver a more meaningful output. Introduced
 * FireStateChangeHelper to trace all state changes on session.
 * 
 * *****************  Version 9  *****************
 * User: Su           Date: 12.09.01   Time: 19:56
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 8  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for version V2.0
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 10.07.01   Time: 12:24
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 9.05.01    Time: 12:51
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * CFCMessageSession::SetAddressInfo needs full address as well.
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 19.04.01   Time: 16:11
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 19.03.01   Time: 19:01
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 15.03.01   Time: 19:39
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header, removed private trace files, replaced by tracea.lib.
 * Removed 
 * IFCMessageSession completely, can be replaced by IFCConnect.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 *==
 *----------------------------------------------------------------------------*
*/

