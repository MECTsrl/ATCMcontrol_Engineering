/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/IBTSession.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: IBTSession.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/IBTSession.cpp $
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
#include "IBTSession.h"
#include "transfer.h"

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
// CFCCommissioningSession

//------------------------------------------------------------------*
CFCCommissioningSession::CFCCommissioningSession()
  : m_state(not_connected),
    m_pCurrentJob(NULL),
    m_pResource(NULL),
    m_pCommMgt(NULL),
    m_pCheckConnection(NULL),
    m_lKey(0),
    m_dwTSessionNotifyCookie(0),
    m_dwBreakpointNotifyCookie(0),
    m_bRunning(false),
    m_bConnect(false),
    m_clsidComm(),
    m_clsidConn(),
    m_strInit(),
    m_strMachineName(),
    m_sPassword(),
    m_strAddress()
{
    m_hWakeup = CreateEvent(NULL, FALSE, FALSE, NULL);
}

//------------------------------------------------------------------*
CFCCommissioningSession::~CFCCommissioningSession()
{
    CloseHandle(m_hWakeup);
}

void CFCCommissioningSession::SetAddressInfo(CString & strAddress, CString & strMachine, 
                                             CGuid & guid, CString & strInit)
{
    m_strMachineName = strMachine;
    m_clsidConn = guid;
    m_strInit = strInit;
    m_strAddress = strAddress;

    if(m_clsidConn.IsEmpty()) {
        m_clsidComm = CLSID_CommissioningMgt;
    }
    else {
        //everything works over coclass custom connect and ICustomConnect interface.
        m_clsidComm.Empty();
    }
}

//------------------------------------------------------------------*
/*static*/ DWORD CFCCommissioningSession::ThreadMain(LPVOID lpParam)
{
    InterlockedIncrement(&gs_nRunningThreads);
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    CFCCommissioningSession *_this = (CFCCommissioningSession *)lpParam;
    HRESULT hr = _this->ConnectThread();
    _this->Release();

    CoUninitialize();
    InterlockedDecrement(&gs_nRunningThreads);
    return hr;
}

//------------------------------------------------------------------*
HRESULT CFCCommissioningSession::ConnectThread()
{
    HRESULT hr;
    for(;;) {
      WaitForSingleObject(m_hWakeup, 20000);
      m_cs.Lock();
      if (!m_bConnect) {
          _ASSERTE(m_bRunning);
          m_bRunning = false;
          m_cs.Unlock();
          break;
      }
      m_cs.Unlock();
      hr = DoConnect();
      if (SUCCEEDED(hr)) {
          for(;;) {
              WaitForSingleObject(m_hWakeup, m_state == waiting_for_exclusive ? 5000 : 10000);
              m_cs.Lock();
              if (!m_bConnect || m_state == exclusive_established && m_strRequestingUser != m_strCurrentUser) {
                  m_cs.Unlock();
                  SetEvent(m_hWakeup);
                  break;
              }
              m_cs.Unlock();
              hr = DoCheckConnection();
              if (FAILED(hr)) {
                  break;
              }
          }
          hr = DoDisconnect();
      }
    }
    return S_OK;
}

//=======================================================================
//      interface   IFCConnect
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::Connect(BSTR sUserName,BSTR sPassword)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::Connect(%s)\n"),(LPCTSTR) m_strMachineName);

    TCHAR   szUser[_MAX_PATH];
    DWORD   dwSize = FC_ARRAY_LEN(szUser);
    HRESULT hr = S_OK;

    FC_PCHECK_HR(sUserName,E_POINTER);
    FC_PCHECK_HR(sPassword,E_POINTER);

    try {
        CComBSTR sUser = sUserName;

        if (sUser.Length() == 0)
        {   // try setting default NT user
            if (WNetGetUser(NULL, szUser, &dwSize) == NO_ERROR) {
                sUser = szUser;
            }
            else {
                // try environment string
                sUser = _tgetenv(_T("USERNAME"));
            }
        }
        else {
            sUser = sUserName;
        }

        if (sUser.Length() == 0) {
            DWORD   dwSize = sizeof(szUser);
            VERIFY(GetComputerName(szUser, &dwSize) != 0);
            sUser = _T("Node: ");
            sUser += szUser;
        }
        m_cs.Lock();
        m_strRequestingUser = sUser;
        m_sPassword = sPassword;
        m_bConnect = true;
        if (!m_bRunning) {
            AddRef();
            DWORD dwThreadID;
            HANDLE hThread = CreateThread(NULL, 0, ThreadMain, (LPVOID)this, 0, &dwThreadID);
            _ASSERTE(hThread != NULL);
            if (hThread != NULL) {
              CloseHandle(hThread);
              m_bRunning = true;
            }
        }
        SetEvent(m_hWakeup);
        m_cs.Unlock();
    }
    catch(CMemoryException* e) {
        e->Delete();
        hr = E_OUTOFMEMORY;
    }
	return hr;
}

STDMETHODIMP
    CFCCommissioningSession::Disconnect()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::Disconnect(%s)\n"),(LPCTSTR) m_strMachineName);
    
    HRESULT hr = S_OK;
    // update caller that connection will soon be disconnected!
    FireStateChangeHelper(regular_shutdown);

    m_cs.Lock();
    AbortTransfer((DWORD)m_pCurrentJob);
    if (m_bConnect)    // still online?
    {
        m_bConnect = false;
        SetEvent(m_hWakeup);
    }
    m_cs.Unlock();

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::ForceDisconnect(BSTR sUserName, BSTR sPassword, BSTR sEngMachineName)
{
    HRESULT hr = S_OK;
    CComBSTR sUser;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    
    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::ForceDisconnect(%s)\n"),(LPCTSTR) m_strMachineName);

    FC_PCHECK_HR(sUserName,E_POINTER);
    FC_PCHECK_HR(sPassword,E_POINTER);

    TCHAR   szUser[_MAX_PATH];
    if (::SysStringLen(sUserName) == 0)
    {
        // try setting default NT user
        DWORD   dwSize = sizeof(szUser);
        if (WNetGetUser(NULL, szUser, &dwSize) == NO_ERROR) {
            sUser = szUser;
        }
        else {
            // try environment string
            sUser = _tgetenv(_T("USERNAME"));
        }
    }
    else {
        sUser = sUserName;
    }

    if (sUser.Length() == 0) {
        DWORD   dwSize = sizeof(szUser);
        VERIFY(GetComputerName(szUser, &dwSize) != 0);
        sUser = _T("Node: ");
        sUser += szUser;
    }

    m_cs.Lock();
    if (m_pCommMgt && (m_state == waiting_for_exclusive)) {
        hr = m_pCommMgt->ForceLogout(sUser,sPassword,sEngMachineName);
    }
    m_cs.Unlock();
    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::get_SessionState(CSC_SESSION_STATE * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_state;
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCCommissioningSession::get_MachineName(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = m_strMachineName.AllocSysString();
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::get_CurrentUser(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    CSingleLock(&m_cs, TRUE);
    if (m_pCommMgt) {
        return m_pCommMgt->get_CurrentUser(pVal);
    }
    else {
        *pVal = m_strCurrentUser.AllocSysString();
    }
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::get_IsConnected(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pVal == NULL) {
        return E_POINTER;
    }

    *pVal = (m_state == exclusive_established);

	return S_OK;
}



//=======================================================================
//      interface   IFCDownload
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::DownloadDomainList(CSC_TRANSFER_TARGET target, 
                                                    BSTR sListFileName, 
                                                    DWORD *pdwCookie)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pdwCookie == NULL) {
        return E_POINTER;
    }
    *pdwCookie = NULL;
    if (m_state != exclusive_established) {
        TrcPrint(TRC_ERROR, _T("DownloadDomainList: No exclusive connection!! target %s; list file %s\n"),
            target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName));
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    if (m_pCurrentJob != NULL) {
        // still busy!!
        TrcPrint(TRC_ERROR, _T("DownloadDomainList: Still busy!! target %s; list file %s\n"),
            target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName));
        return E_FAIL;
    }

    // get ready to do the actual work!
    CDomainsTransfer *pJob = new CDomainsTransfer;
    if (pJob == NULL) {
        return E_OUTOFMEMORY;
    }
    HRESULT hr;
    CComPtr<ITransfer>  pResource;
    hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    hr = pJob->StartTransfer(target, CDomainsTransfer::downloadDir, CString(sListFileName),m_lKey, pResource, this);
    if (FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("DownloadDomainList: Cannot start transfer!! target %s; list file %s; hr=0x%08X\n"),
            target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName), hr);
        return hr;
    }
    m_pCurrentJob = pJob;
    *pdwCookie = (DWORD)pJob;
    TrcPrint(TRC_INTERFACE, _T("DownloadDomainList: target %s; list file %s; job = 0x%08X\n"),
        target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName), *pdwCookie);
    TRACE(_T("DownloadSession: started transfer job 0x%08X as DOWNLOAD\n"), *pdwCookie);

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::UploadDomainList(CSC_TRANSFER_TARGET source, 
                                                  BSTR sListFileName, 
                                                  DWORD *pdwCookie)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pdwCookie == NULL) {
        return E_POINTER;
    }
    *pdwCookie = NULL;
    if (m_state != exclusive_established) {
        TrcPrint(TRC_ERROR, _T("UploadDomainList: No exclusive connection!! target %s; list file %s\n"),
            source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName));
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    if (m_pCurrentJob != NULL) {
        // still busy!!
        TrcPrint(TRC_ERROR, _T("UploadDomainList: Still busy!! target %s; list file %s\n"),
            source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName));
        return E_FAIL;
    }

    // get ready to do the actual work!
    CDomainsTransfer *pJob = new CDomainsTransfer;
    if (pJob == NULL) {
        return E_OUTOFMEMORY;
    }
    HRESULT hr;
    CComPtr<ITransfer>  pResource;
    hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    hr = pJob->StartTransfer(source, CDomainsTransfer::uploadDir, CString(sListFileName), m_lKey, pResource, this);
    if (FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("UploadDomainList: Cannot start transfer!! target %s; list file %s; hr=0x%08X\n"),
            source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName), hr);
        return hr;
    }
    m_pCurrentJob = pJob;
    *pdwCookie = (DWORD)pJob;
    TRACE(_T("DownloadSession: started transfer job 0x%08X as UPLOAD\n"), *pdwCookie);
    TrcPrint(TRC_INTERFACE, _T("UploadDomainList: target %s; list file %s; job = 0x%08X\n"),
        source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName), *pdwCookie);

	return S_OK;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::DeleteDomainList(CSC_TRANSFER_TARGET source,BSTR sListFileName, DWORD *pdwCookie)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pdwCookie == NULL) {
        return E_POINTER;
    }
    *pdwCookie = NULL;
    if (m_state != exclusive_established) {
        TrcPrint(TRC_ERROR, _T("DeleteDomainList: No exclusive connection!! target %s; list file %s\n"),
            source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName));
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    if (m_pCurrentJob != NULL) {
        // still busy!!
        TrcPrint(TRC_ERROR, _T("DeleteDomainList: Still busy!! target %s; list file %s\n"),
            source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName));
        return E_FAIL;
    }

    // get ready to do the actual work!
    CDomainsTransfer *pJob = new CDomainsTransfer;
    if (pJob == NULL) {
        return E_OUTOFMEMORY;
    }
    HRESULT hr;
    CComPtr<ITransfer>  pResource;
    hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    hr = pJob->StartTransfer(source, CDomainsTransfer::deleteDir, CString(sListFileName), m_lKey, pResource, this);
    if (FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("DeleteDomainList: Cannot start transfer!! target %s; list file %s; hr=0x%08X\n"),
            source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName), hr);
        return hr;
    }
    m_pCurrentJob = pJob;
    *pdwCookie = (DWORD)pJob;
    TrcPrint(TRC_INTERFACE, _T("DeleteDomainList: target %s; list file %s; job = 0x%08X\n"),
        source == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR)CString(sListFileName), *pdwCookie);
    TRACE(_T("DownloadSession: started transfer job 0x%08X as DELETE\n"), *pdwCookie);

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::AbortTransfer(DWORD dwCookie)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("AbortTransfer: job=0x%08X\n"), dwCookie);
    CSingleLock lock(&m_cs, TRUE);
    if (m_pCurrentJob == NULL) {
        return S_FALSE;
    }
    if (dwCookie != (DWORD) m_pCurrentJob) {
        // unknown cookie
        return S_FALSE;
    }
    // do not force thread to stop, let it run out!
    m_pCurrentJob->AbortTransfer();
    m_pCurrentJob = NULL;

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::StartOnlineChange(CSC_TRANSFER_TARGET target)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("StartOnlineChange: target %s;\n"), target == ProgramDomains ? 
        _T("ProgDomains") : _T("VisDomains"));
    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    if (   (m_state != exclusive_established)
        || !pResource)
    {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    hr = pResource->StartOnlineChange(m_lKey, target);
    if (FAILED(hr)) {
        return hr;
    }

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::CommitOnlineChange(CSC_TRANSFER_TARGET target, BSTR sResource, BSTR sProjectName, BSTR sProjectGUID, BSTR sDownloadGUID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CComPtr<ITransfer>  pResource;

    TrcPrint(TRC_INTERFACE, _T("CommitOnlineChange: target %s; resource %s\n"),
        target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR(CString(sResource))));
    
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }

    if ((m_state != exclusive_established) || !pResource) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    
    hr = pResource->CommitOnlineChange(m_lKey, target, sResource, sProjectName ,sProjectGUID, sDownloadGUID);
    if (FAILED(hr)) {
        return hr;
    }

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::CancelOnlineChange(CSC_TRANSFER_TARGET target)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("CancelOnlineChange: target %s\n"),
        target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"));
    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    if (   (m_state != exclusive_established)
        || !pResource)
    {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    hr = pResource->CancelOnlineChange(m_lKey, target);
    if (FAILED(hr)) {
        return hr;
    }
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::DeleteAllDomains(CSC_TRANSFER_TARGET target)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("DeleteAllDomains: target %s\n"),
        target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"));
    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    if (   (m_state != exclusive_established)
        || !pResource)
    {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }
    hr = pResource->DeleteAllDomains(m_lKey, target);
    if (FAILED(hr)) {
        return hr;
    }

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::get_DLoadAttribute(CSC_TRANSFER_TARGET target,BSTR sAttributeName, VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("get_DLoadAttribute: target %s; attribute %s\n"),
        target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"), (LPCTSTR(CString(sAttributeName))));
    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    
    if ((m_state != exclusive_established) || !pResource) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }

    hr = pResource->get_DLoadAttribute(m_lKey, target, sAttributeName, pVal);
    if (FAILED(hr)) {
        return hr;
    }
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::ClearResource(CSC_TRANSFER_TARGET target)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("ClearResource: target %s\n"),
        target == ProgramDomains ? _T("ProgDomains") : _T("VisDomains"));
    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    
    if ((m_state != exclusive_established)  || !pResource) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }

    hr = pResource->ClearResource(m_lKey, target);
    if (FAILED(hr)) {
        return hr;
    }

	return S_OK;
}

STDMETHODIMP
    CFCCommissioningSession::FlashResource()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    TrcPrint(TRC_INTERFACE, _T("FlashResource\n"));

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    
    if ((m_state != exclusive_established)  || !pResource) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;
    }

    hr = pResource->FlashResource(m_lKey);
    if (FAILED(hr)) {
        return hr;
    }

	return S_OK;
}

//=======================================================================
//      interface   IFCBreakpoint
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::SetBreakpoint(BSTR sInstanceName, long lBreakpointNumber, long *phBP)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (phBP == NULL) {
        return E_POINTER;
    }
    *phBP = 0;

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }
    return pBreak->SetBreakpoint(m_lKey, sInstanceName, lBreakpointNumber, phBP);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::ClearBreakpoint(long hBP)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }
    return pBreak->ClearBreakpoint(m_lKey, hBP);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::ClearAllBreakpoints()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }
    return pBreak->ClearAllBreakpoints(m_lKey);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::ContinueTask(BSTR sTaskName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(sTaskName,E_POINTER);

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }

    return pBreak->ContinueTask(m_lKey, sTaskName);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::SingleStep(BSTR sTaskName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(sTaskName,E_POINTER);

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }

    return pBreak->SingleStep(m_lKey, sTaskName);
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::StepOver(BSTR sTaskName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(sTaskName,E_POINTER);

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }

    return pBreak->StepOver(m_lKey, sTaskName);
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::StepOut(BSTR sTaskName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    FC_PCHECK_HR(sTaskName,E_POINTER);

    CComPtr<ITransfer>  pResource;
    HRESULT hr = GetResourcePtr(&pResource);
    if (FAILED(hr)) {
        return hr;
    }
    CComQIPtr<IBreakpoint, &IID_IBreakpoint> pBreak = pResource;
    if (!pBreak) {
        return E_NOINTERFACE;
    }

    return pBreak->StepOut(m_lKey, sTaskName);
}


//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::put_ConnectionID(
            /* [string][in] */ BSTR sId)
{
    if (sId == NULL) {
        return E_POINTER;
    }

    m_Id = sId;

    return S_OK;
}





//=======================================================================
//      interface   _FCRemoteBreakpointNotification from CSC_OnlSrv !!!! Single TA !!!!!
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP CFCCommissioningSession::OnBreakpointReached(/* [in] */ BSTR sTaskName,
                                                          /* [in] */ long bpQuality,
                                                          /* [in] */ SAFEARRAY* arrsInstances,
                                                          /* [in] */ SAFEARRAY* arrlBreakpointNumber)
{
    HRESULT hr;
    CComBSTR sInstanceName;
    long lIndex = 0L,lBreakpointNumber;

    if(!arrsInstances || ! arrlBreakpointNumber) {
        return E_POINTER;
    }

    hr = ::SafeArrayGetElement(arrlBreakpointNumber,&lIndex,&lBreakpointNumber);
    if(FAILED(hr)) {
        return hr;
    }

    hr = ::SafeArrayGetElement(arrsInstances,&lIndex,&sInstanceName);
    if(FAILED(hr)) {
        return hr;
    }

    //d-1234: replace bogus instancename with correct Connection ID
    //TODO: we should better not send Connection ID on the wire
    CString strName;
    strName = sInstanceName;
    strName.Delete(0, strName.Find(_T('.')));
    strName = m_Id + strName;
    CComBSTR sCorrectInstanceName = strName;

    strName = sTaskName;
    strName.Delete(0, strName.Find(_T('.')));
    strName = m_Id + strName;
    CComBSTR sCorrectTaskName = strName;

    Fire_OnBreakpointReached(sCorrectInstanceName,lBreakpointNumber, bpQuality, sCorrectTaskName);

    return S_OK;
}


//------------------------------------------------------------------*
STDMETHODIMP CFCCommissioningSession::OnConfigurationChanged()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComBSTR sAddress;
    sAddress = m_strAddress;
    // fire notification to any breakpoint clients...
    Fire_OnConfigurationChanged(sAddress);
	return S_OK;
}


//=======================================================================
//      interface   _FCCommissioningMgtNotification !!!! Multi TA !!!!!
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::OnUserLoggedin(BSTR sUserName, BSTR sEngMachineName, BOOL bLoggingIn)
{
    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::OnUserLoggedIn(%s): user=%s, CE=%s, flag=%d\n"),
        (LPCTSTR) m_strMachineName,(LPCTSTR) CString(sUserName), (LPCTSTR) CString(sEngMachineName), bLoggingIn);
    return Fire_OnUserLoggedIn(sUserName, sEngMachineName, bLoggingIn);
}

//=======================================================================
//      interface   _FCInternalTransferProgress
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::OnTransferProgress(DWORD dwCookie, BSTR sCurrentDomainName,
                                           CSC_TRANSFER_STATE state, float fltPercentDone)
{
    // must always be executed in STA context, i.e. when calling, use correctly marshalled ptr!!
    CString strDomain;
    CComBSTR sDomain = sCurrentDomainName;
    if (sDomain.Length() > 0) {
        strDomain = sDomain;
    }
    else {
        strDomain = _T("-none-");
    }
#ifdef _DEBUG
    CString strState;
    switch (state) {
    case idle:
        strState = _T("Idle");
        break;
    case initiating:
        strState = _T("Initiating");
        break;
    case transfering_domain:
        strState = _T("Transfer");
        break;
    case aborting:
        strState = _T("Aborting");
        break;
    case cleanup:
        strState = _T("Cleanup");
        break;
    default:
        strState = _T("??Unknown??");
        break;
    }
    TRACE(_T("OnTransferProgress: cookie=0x%08X, domain=%s, state=%s, done=%.2f\n"),
        dwCookie, (LPCTSTR)strDomain, (LPCTSTR)strState, fltPercentDone);
    TrcPrint(TRC_INTERFACE, _T("OnTransferProgress: cookie=0x%08X, domain=%s, state=%s, done=%.2f\n"),
        dwCookie, (LPCTSTR)strDomain, (LPCTSTR)strState, fltPercentDone);
#else
    TrcPrint(TRC_INTERFACE, _T("OnTransferProgress: cookie=0x%08X, domain=%s, state=%d, done=%.2f\n"),
        dwCookie, (LPCTSTR)strDomain, state, fltPercentDone);
#endif // _DEBUG

    m_cs.Lock();
    if (   (state == aborting)
        || (state == cleanup))
    {
        TRACE(_T("DownloadSession: transfer job 0x%08X has completed with state = %d\n"), dwCookie, state);
        m_pCurrentJob = NULL;
    }
    m_cs.Unlock();

    HRESULT hr = Fire_OnTransferStateChanged(dwCookie, sCurrentDomainName, state, fltPercentDone);
    
    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::OnTransferError(DWORD dwCookie, BSTR sCurrentDomainName,
                                           CSC_TRANSFER_STATE state, HRESULT hError)
{
    CString strDomain;
    CComBSTR sDomain = sCurrentDomainName;
    if (sDomain.Length() > 0) {
        strDomain = sDomain;
    }
    else {
        strDomain = _T("-none-");
    }
#ifdef _DEBUG
    CString strState;
    switch (state) {
    case idle:
        strState = _T("Idle");
        break;
    case initiating:
        strState = _T("Initiating");
        break;
    case transfering_domain:
        strState = _T("Transfer");
        break;
    case aborting:
        strState = _T("Aborting");
        break;
    case cleanup:
        strState = _T("Cleanup");
        break;
    default:
        strState = _T("??Unknown??");
        break;
    }
    TRACE(_T("OnTransferError: cookie=0x%08X, domain=%s, state=%s, error=0x%08X\n"),
        dwCookie, (LPCTSTR)strDomain, (LPCTSTR)strState, hError);
    TrcPrint(TRC_INTERFACE, _T("OnTransferError: cookie=0x%08X, domain=%s, state=%s, error=0x%08X\n"),
        dwCookie, (LPCTSTR)strDomain, (LPCTSTR)strState, hError);
#else
    TrcPrint(TRC_INTERFACE, _T("OnTransferError: cookie=0x%08X, domain=%s, state=%d, error=0x%08X\n"),
        dwCookie, (LPCTSTR)strDomain, state, hError);
#endif // _DEBUG

    HRESULT hr = Fire_OnTransferError(dwCookie, sCurrentDomainName, state, hError);
    
    return hr;
}



//------------------------------------------------------------------*
STDMETHODIMP
    CFCCommissioningSession::DoConnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    ICommissioningMgt* pMgt = NULL;
    IConnection* pConn = NULL;
    HRESULT hr;
    // check first if reconnect is already on schedule to avoid too busy retries
    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::DoConnect(%s)\n"), (LPCTSTR) m_strMachineName);
        
    // try connect
    m_state = waiting_for_connect;
    FireStateChangeHelper(waiting_for_connect);

    hr = ConnectToOnlineServer(&pMgt);
    if(FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::DoConnect: Error, cannot connect to online server for IF ICommissioningMgt! hr=0x%08X\n"), hr);
        m_state = interrupted;
        FireStateChangeHelper(interrupted, hr);
        return hr;
    }

    hr = pMgt->QueryInterface(IID_IConnection,(void**)&pConn);
    if(FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::DoConnect: Error, cannot connect to online server for IF ICommissioningMgt! hr=0x%08X\n"), hr);
        pMgt->Release();
        m_state = interrupted;
        FireStateChangeHelper(interrupted, hr);
        return hr;
    }

    //SUCCEEDED ... try login
    _ASSERTE(!m_pCommMgt);
    m_cs.Lock();
    m_pCommMgt = pMgt;
    m_pCheckConnection = pConn;
    m_state = waiting_for_exclusive;
    m_cs.Unlock();

    pConn = NULL;
    pMgt = NULL;

    // setup advise to receive changes in transfer session (currently logged in user)
    FireStateChangeHelper(waiting_for_exclusive);

    // setup advise to login as exclusive
    _ASSERTE(m_dwTSessionNotifyCookie == 0);
    hr = AtlAdvise(m_pCommMgt, GetUnknown(), IID__FCCommissioningMgtNotification, &m_dwTSessionNotifyCookie);
    if (FAILED(hr)) {
        TRACE(_T("CFCCommissioningSession::DoConnect: Error, cannot advise _FCCommissioningMgtNotification! hr=0x%08X\n"), hr);
        TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::DoConnect: Error, cannot advise _FCCommissioningMgtNotification! hr=0x%08X\n"), hr);
        DoDisconnect(interrupted, hr);
        return hr;
    }

    // first attempt to access control exclusively
    hr = TryLoginForCommissioning();
    if (FAILED(hr)) {
        if (hr != E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED) {
            DoDisconnect(interrupted, hr);
            return hr;
        }
        hr = S_FALSE;
    }
	return hr;
}


//------------------------------------------------------------------*
STDMETHODIMP CFCCommissioningSession::DoDisconnect(CSC_SESSION_STATE newState /*= not_connected */, HRESULT hError /*= S_OK */)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;
    BOOL bChanged;
    ICommissioningMgt* pCommMgt = NULL;
    ITransfer* pResource = NULL;
    IConnection* pConn = NULL;

    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::DoDisconnect(%s)\n"), (LPCTSTR) m_strMachineName);

    m_cs.Lock();
    bChanged = (m_state != newState);
    m_state = newState;
    m_strCurrentUser.Empty();
    // actually disconnect
    pCommMgt = m_pCommMgt;
    pResource = m_pResource;
    pConn = m_pCheckConnection;
    m_pCommMgt = NULL;
    m_pResource = NULL;
    m_pCheckConnection = NULL;
    m_cs.Unlock();

    if (pResource && m_dwBreakpointNotifyCookie != 0) {
        // unadvise breakpoints
        hr = AtlUnadvise(pResource, IID__FCRemoteBreakpointNotification, m_dwBreakpointNotifyCookie);
        if (FAILED(hr)) {
            TRACE(_T("CFCCommissioningSession::DoDisconnect: Error, cannot Unadvise _FCRemoteBreakpointNotification! hr = 0x%08X\n"), hr);
            TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::DoDisconnect: Error, cannot Unadvise _FCRemoteBreakpointNotification! hr = 0x%08X\n"), hr);
        }
        m_dwBreakpointNotifyCookie = 0;
    }
    
    FC_RELEASE_PTR(pResource);

    if (pCommMgt && m_lKey != 0) {
        hr = pCommMgt->Logout(m_lKey);
        if (FAILED(hr)) {
            TRACE(_T("CFCCommissioningSession::DoDisconnect: Error, cannot LogOut! hr = 0x%08X\n"), hr);
            TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::DoDisconnect: Error, cannot LogOut! hr = 0x%08X\n"), hr);
        }
        m_lKey = 0;
    }

    if (pCommMgt && m_dwTSessionNotifyCookie != 0) {
        hr = AtlUnadvise(pCommMgt, IID__FCCommissioningMgtNotification, m_dwTSessionNotifyCookie);
        if (FAILED(hr)) {
            TRACE(_T("CFCCommissioningSession::DoDisconnect: Error, cannot Unadvise IID__FCCommissioningMgtNotification! hr = 0x%08X\n"), hr);
            TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::DoDisconnect: Error, cannot Unadvise IID__FCCommissioningMgtNotification! hr = 0x%08X\n"), hr);
        }
        m_dwTSessionNotifyCookie = 0;
    }

    FC_RELEASE_PTR(pCommMgt);
    FC_RELEASE_PTR(pConn);
    
    if (bChanged || hError!=S_OK) {
        FireStateChangeHelper(newState, hError);
    }
	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP CFCCommissioningSession::DoCheckConnection()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr = S_OK;

    if (!m_pCommMgt) {
        return E_NOINTERFACE;
    }

    // are still trying to connect exclusively??
    if (m_state == waiting_for_exclusive)
    {
        // another attempt to access control exclusively
        hr = TryLoginForCommissioning();
        if (SUCCEEDED(hr))
        {
            return S_OK;    
        }
        // check for real problems!
        if (hr == E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED) {
            // connection seems to be ok
            return S_FALSE;  // StateChange already notified in subroutine!
        }
        // notify interrupted connection below...
    }
    else
    {
        //Check active connection
        hr = m_pCheckConnection->CheckConnection();
        if(SUCCEEDED(hr)) {
            return hr;
        }        
    }

    // we have lost connection, notify client...
    ::TrcPrint(TRC_ERROR, _T("lost connection of CFCCommissioningSession(%s): hr=0x%08X, state=%s\n"),
        (LPCTSTR) m_strMachineName,hr, GetSessionStateString(m_state));
    m_state = interrupted;
    FireStateChangeHelper(interrupted, hr);
	return hr;
}

//------------------------------------------------------------------*
/**
 * Helper to call Fire_OnStateChanged() from MTA context only.
 *
 * @param   newState - new session (connection) state.
 * @param   hError - possible error code.
 * @see     CSC_SESSION_STATE.
*/
void CFCCommissioningSession::FireStateChangeHelper(CSC_SESSION_STATE newState, HRESULT hError /*= S_OK*/)
{
    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::OnStateChanged(%s) state='%s'\n"),
        (LPCTSTR) m_strMachineName,GetSessionStateString(newState));
    Fire_OnStateChanged(GetUnknown(), newState, hError);
}

//------------------------------------------------------------------*
/**
 * Helper method to attempt to login exclusively from MTA context only.
 *  
 * @return      HRESULT: E_NOINTERFACE - No commissioning management
 *                       interface, no ITransfer interface obtained,
 *                       or no connection point container obtained.<br>
 *                       S_FALSE - user decided to disconnect in the 
 *                       meantime.<br>
 *                       Any of the return values of 
 *                       IConnectionPointContainer::FindConnectionPoint.
 * @see                  Connection point interfaces.
*/
HRESULT CFCCommissioningSession::TryLoginForCommissioning(void)
{
    CString strUser,strTmp;
    CComBSTR sPassword,sHostName;
    IUnknown *pUnk = NULL;
    ITransfer *pRes = NULL;
    HRESULT hr = S_OK;

    if (!m_pCommMgt) {
        return E_NOINTERFACE;
    }

    m_cs.Lock();
    if (!m_bConnect) {
        m_cs.Unlock();
        // user decided to disconnect in the meantime, dont go further now!
        return S_FALSE;
    }
    strUser = m_strRequestingUser;
    sPassword = m_sPassword;
    m_cs.Unlock();
    
    hr = GetLocalComputerName(strTmp);
    sHostName = strTmp;

    TrcPrint(TRC_INTERFACE, _T("CFCCommissioningSession::TryLoginForCommissioning(%s) user='%s'\n"),
        (LPCTSTR) m_strMachineName, strUser);

    hr = m_pCommMgt->Login(CComBSTR(strUser),sPassword, sHostName, &m_lKey, &pUnk);
    if(FAILED(hr)) {
        ::TrcPrint(TRC_WARNING, _T("Login(%s) returned 0x%08X\n"),strUser,hr);
        FireStateChangeHelper(waiting_for_exclusive, hr);
        return hr;
    }

    //SUCCEEDED ...
    hr = pUnk->QueryInterface(IID_ITransfer, (void **)&pRes);
    pUnk->Release();
    if (FAILED(hr)) {
        TRACE(_T("CFCCommissioningSession::TryLoginForCommissioning: Error, cannot QI IID_ITransfer! hr = 0x%08X\n"), hr);
        TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::TryLoginForCommissioning: Error, cannot QI IID_ITransfer! hr = 0x%08X\n"), hr);
        return hr;
    }

    // advise breakpoint notifications from control
    hr = AtlAdvise(pRes, GetUnknown(), IID__FCRemoteBreakpointNotification, &m_dwBreakpointNotifyCookie);
    if (FAILED(hr)) {
        pRes->Release();
        TRACE(_T("CFCCommissioningSession::TryLoginForCommissioning: Error, cannot Advise _FCRemoteBreakpointNotification! hr = 0x%08X\n"), hr);
        TrcPrint(TRC_ERROR, _T("CFCCommissioningSession::TryLoginForCommissioning: Error, cannot Advise _FCRemoteBreakpointNotification! hr = 0x%08X\n"), hr);
        return hr;
    }

    _ASSERTE(!m_pResource);
    m_cs.Lock();
    m_strCurrentUser = strUser;
    m_pResource = pRes;
    m_state = exclusive_established;
    m_cs.Unlock();
    FireStateChangeHelper(exclusive_established);
    return hr;
}


//------------------------------------------------------------------*
/**
 * GetResourcePtr
 *  helper to get unmarshalled resource ptr, called from main thread only (STA)
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CFCCommissioningSession::GetResourcePtr(ITransfer **ppRes)
{
    if (ppRes == NULL) {
        return E_POINTER;
    }
    *ppRes = NULL;
    
    // check first if connected to fresh session (in MTA)
    CSingleLock lock (&m_cs, TRUE);
    if (!m_pResource || m_state != exclusive_established) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    *ppRes = m_pResource;
    (*ppRes)->AddRef();

    return S_OK;
}



//=======================================================================
//=======================================================================



//------------------------------------------------------------------*
HRESULT
    CFCCommissioningSession::ConnectToOnlineServer(ICommissioningMgt **ppMgt)
{
    MULTI_QI queue[1];
    COSERVERINFO     serverInfo;
	DWORD clsctx;
    CComPtr<ICustomConnect> pConn;
    CComPtr<IUnknown>pUnk;
    CComBSTR sInit;
    HRESULT hr = S_OK;


    FC_PCHECK_HR(ppMgt,E_POINTER);
    *ppMgt = NULL;

    queue[0].pItf = NULL;
    queue[0].hr = 0;
	serverInfo.dwReserved1 = 0;
	serverInfo.dwReserved2 = 0;
    USES_CONVERSION;
	serverInfo.pwszName = T2OLE(m_strMachineName);
	serverInfo.pAuthInfo = 0;
	clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;

    if(m_clsidConn.IsEmpty()) {
        queue[0].pIID = &IID_ICommissioningMgt;
        
        hr = ::CoCreateInstanceEx(m_clsidComm, NULL, 
            clsctx, &serverInfo,FC_ARRAY_LEN(queue), queue);

        if(SUCCEEDED(hr)) {
            *ppMgt = (ICommissioningMgt*) queue[0].pItf;
        }
        return hr;
    }
    //custom connect case
    queue[0].pIID = &IID_ICustomConnect;

    hr = ::CoCreateInstanceEx(m_clsidConn, NULL, 
        clsctx, &serverInfo,FC_ARRAY_LEN(queue), queue);
    if(FAILED(hr)) {
        return hr;
    }
    
    pConn.Attach((ICustomConnect*)queue[0].pItf);
    sInit = m_strInit;

    hr = pConn->CreateCommissioningMgt(sInit,&pUnk);
    if(FAILED(hr)) {
        return hr;
    }

    return pUnk->QueryInterface(IID_ICommissioningMgt,(void**)ppMgt);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: IBTSession.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 16  *****************
 * User: Su           Date: 10.03.03   Time: 9:58
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Enhanced traces to deliver a more meaningful output.
 * 
 * *****************  Version 15  *****************
 * User: Jd           Date: 25.02.03   Time: 11:49
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Custom Online commands in non exclusive sessions.
 * 
 * *****************  Version 14  *****************
 * User: Jd           Date: 4.02.03    Time: 14:40
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 13  *****************
 * User: Jd           Date: 4.02.03    Time: 9:21
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Changes for new callback interface into engineering (custom online
 * commands).
 * 
 * *****************  Version 12  *****************
 * User: Ed           Date: 6/14/02    Time: 11:45a
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 11  *****************
 * User: Su           Date: 12.09.01   Time: 15:27
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Renamed OnNewDownload to OnConfigurationChanged and removed parameters
 * really not used.
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 17.08.01   Time: 15:41
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * workaround to reset BPs after coldstart/download
 * 
 * *****************  Version 9  *****************
 * User: Su           Date: 17.07.01   Time: 17:58
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 8  *****************
 * User: Su           Date: 13.07.01   Time: 17:47
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revised interfaces for version V2.0
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 10.07.01   Time: 12:23
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 6  *****************
 * User: Ed           Date: 4/19/01    Time: 3:41p
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 5  *****************
 * User: Su           Date: 3.04.01    Time: 13:25
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 4  *****************
 * User: Su           Date: 19.03.01   Time: 19:01
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Revision
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 15.03.01   Time: 19:38
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * Removed not used/implemented IF methods. New header, removed private
 * trace files, replaced by tracea.lib
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:48
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
