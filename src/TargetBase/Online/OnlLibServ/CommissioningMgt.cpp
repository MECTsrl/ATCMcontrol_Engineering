/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/CommissioningMgt.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommissioningMgt.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/CommissioningMgt.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "iecResource.h"
#include "CommissioningMgt.h"
#include "CommissioningSession.h"
#include "CSC_OnlineErr.h"
#include "NSpaceHolder.h"
#include "connection.h"
#include "config.h"

/////////////////////////////////////////////////////////////////////////////
// CCommissioningMgt

CCriticalSection CCommissioningMgt::m_cs;
CCommissioningMgt* CCommissioningMgt::sm_pCommissioningMgt;


//------------------------------------------------------------------*
CCommissioningMgt::CCommissioningMgt()
{
    m_lKey = 0;
    m_pCommissioningSession = NULL;
}

//------------------------------------------------------------------*
CCommissioningMgt::~CCommissioningMgt()
{
    if (m_pCommissioningSession != NULL)
        m_pCommissioningSession->Release();
}

//------------------------------------------------------------------*
ULONG CCommissioningMgt::InternalRelease()
{
    CSingleLock secured(&m_cs, TRUE);
    ULONG rv = _ThreadModel::Decrement(&m_dwRef);
    if (this == sm_pCommissioningMgt && m_dwRef == 1) {
        UpdateSystemVariables(FALSE);

        m_lKey = 0;

        CCommissioningSession *pCommissioningSession = m_pCommissioningSession;
        m_pCommissioningSession = NULL;

        if (sm_pCommissioningMgt) {
            sm_pCommissioningMgt->Release();
            sm_pCommissioningMgt = NULL;
        }
        secured.Unlock();

        if (pCommissioningSession) {
            pCommissioningSession->DropSession();
            pCommissioningSession->Release();
        }
    }
    return rv;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningMgt::get_CurrentUser(BSTR *pVal)
{
    if (pVal == NULL) {
        return E_POINTER;
    }
    *pVal = NULL;
    HRESULT hr = S_OK;
    m_cs.Lock();
    CString strLoggedInUserName;
    if (m_lKey != 0 && m_pCommissioningSession != NULL)
        hr = m_pCommissioningSession->CheckSession();
    if (SUCCEEDED(hr)) {
        if (sm_pCommissioningMgt != NULL)
            strLoggedInUserName = sm_pCommissioningMgt->m_strLoggedInUserName;
        if (strLoggedInUserName.IsEmpty())
            hr = S_FALSE;
        *pVal = strLoggedInUserName.AllocSysString();
    }
    m_cs.Unlock();

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningMgt::Login(BSTR sUserName, BSTR sPassword, BSTR sEngMachineName, LONG *plKey, LPUNKNOWN *ppUnkSession)
{
    if (   (ppUnkSession == NULL)
        || (plKey == NULL))
    {
        return E_POINTER;
    }

    CString strPassword = sPassword;
    CSecurity::SECURITY_RIGHTS rights = g_security.GetUserRights(strPassword);
    if (rights != CSecurity::NoSecurity && rights != CSecurity::Engineering)
    {
        return E_ACCESSDENIED;
    }

    // check if free session to download possible:
    CSingleLock secured(&m_cs, TRUE);
    if (sm_pCommissioningMgt != NULL)
    {
        // session seems to be busy, check if retry of same user from same engineering!
        CString strUser = sUserName;
        CString strEng = sEngMachineName;
        if ((strUser.CompareNoCase(sm_pCommissioningMgt->m_strLoggedInUserName) != 0)
            || (strEng.CompareNoCase(sm_pCommissioningMgt->m_strEngMachineName) != 0))
        {
            // different user !!!!!!
            TrcPrint(TRC_ERROR, _T("CCommissioningMgt::LoginForCommissioning: Only single download at same time allowed\n"));
            return E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED;
        }
        else {
            // same user again, kick previous session out!
            secured.Unlock();

            if (m_lKey != 0)
               Logout(m_lKey);
            else
               ForceLogout(sUserName, sPassword, sEngMachineName);

            secured.Lock();
            if (sm_pCommissioningMgt != NULL)
              return E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED;
        }
    }
    *ppUnkSession = NULL;
    *plKey = 0;
    CComObject<CCommissioningSession>   *p;
    HRESULT hr = CComObject<CCommissioningSession>::CreateInstance(&p);
    if (FAILED(hr)) {
        return hr;
    }
    p->m_strLoggedInUserName = sUserName;
    p->m_strEngMachineName = sEngMachineName;
    p->m_lSessionKey = (LONG) p;

    p->AddRef();
    if (m_pCommissioningSession)
        m_pCommissioningSession->Release();
    m_pCommissioningSession = p;
    hr = p->Connect();
    *ppUnkSession = p->GetUnknown();
    (*ppUnkSession)->AddRef();
    m_lKey = (LONG) p;
    *plKey = m_lKey;
    if (sm_pCommissioningMgt)
        sm_pCommissioningMgt->Release();
    sm_pCommissioningMgt = this;   // keep a reference to mgt to force logout on release!!!!
    sm_pCommissioningMgt->AddRef();

    m_strLoggedInUserName = sUserName;
    m_strEngMachineName   = sEngMachineName;

    UpdateSystemVariables(TRUE);

    TrcPrint(TRC_INTERFACE, _T("CCommissioningMgt::LoginForCommissioning user %s from CE %s\n"),
        (LPCTSTR) m_strLoggedInUserName, (LPCTSTR) m_strEngMachineName);

    secured.Unlock();

    hr = Fire_OnUserLoggedin(sUserName, sEngMachineName, TRUE);

    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningMgt::Logout(LONG lKey)
{
    CSingleLock secured(&m_cs, TRUE);
    if (m_lKey == 0) {
        return S_FALSE;
    }
    
    if (lKey != m_lKey) {
        // invalid or unknown key
        return E_FAIL;
    }

    CString strLoggedInUserName = m_strLoggedInUserName;
    CString strEngMachineName = m_strEngMachineName;

    UpdateSystemVariables(FALSE);

    m_strLoggedInUserName.Empty();
    m_strEngMachineName.Empty();
    m_lKey = 0;

    if (sm_pCommissioningMgt) {
        sm_pCommissioningMgt->Release();
        sm_pCommissioningMgt = NULL;
    }

    secured.Unlock();
    
    if (m_pCommissioningSession) {
        m_pCommissioningSession->DropSession();
        m_pCommissioningSession->Release();
        m_pCommissioningSession = NULL;
    }

    
    Fire_OnUserLoggedin(CComBSTR(strLoggedInUserName), CComBSTR(strEngMachineName), FALSE);
    TrcPrint(TRC_INTERFACE, _T("CCommissioningMgt::LogOut user %s on CE %s\n"),
        (LPCTSTR) strLoggedInUserName, (LPCTSTR) strEngMachineName);

	return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningMgt::ForceLogout(/*[in, string]*/ BSTR sUserName, /*[in, string]*/ BSTR sPassword, /*[in, string]*/ BSTR sEngMachineName)
{
    CString strUser = sUserName;
    CString strPassword = sPassword;
    CString strCE = sEngMachineName;

    TrcPrint(TRC_WARNING, _T("ForceLogout by user '%s' from CE on '%s'\n"), (LPCTSTR) strUser, (LPCTSTR) strCE);

    CSecurity::SECURITY_RIGHTS rights = g_security.GetUserRights(strPassword);
    if (rights != CSecurity::NoSecurity && rights != CSecurity::Engineering)
    {
        return E_ACCESSDENIED;
    }

    CSingleLock secured(&m_cs, TRUE);
    if (m_lKey != 0 || sm_pCommissioningMgt == NULL) {
        return S_FALSE;
    }
    
    UpdateSystemVariables(FALSE);

    sm_pCommissioningMgt->m_lKey = 0;

    CCommissioningMgt *pCommissioningMgt = sm_pCommissioningMgt;
    sm_pCommissioningMgt = NULL;

    secured.Unlock();

    DWORD tid;
    HANDLE hThread = CreateThread(NULL, 0, LogOutHelper, (LPVOID)pCommissioningMgt, 0, &tid);

    WaitForSingleObject(hThread, 3000);
    CloseHandle(hThread);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningMgt::CheckConnection()
{
    return S_OK;
}

//------------------------------------------------------------------*
DWORD WINAPI CCommissioningMgt::LogOutHelper(LPVOID lpParam)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    CCommissioningMgt *pCommissioningMgt = (CCommissioningMgt *)lpParam;
    if (pCommissioningMgt) {

       if (pCommissioningMgt->m_pCommissioningSession) {
           pCommissioningMgt->m_pCommissioningSession->DropSession();
           pCommissioningMgt->m_pCommissioningSession->Release();
           pCommissioningMgt->m_pCommissioningSession = NULL;
       }

       pCommissioningMgt->Fire_OnUserLoggedin(CComBSTR(pCommissioningMgt->m_strLoggedInUserName), CComBSTR(pCommissioningMgt->m_strEngMachineName), FALSE);
       pCommissioningMgt->Release();
    }
    CoUninitialize();
	return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CCommissioningMgt::UpdateSystemVariables(BOOL bLogOn)
{
    HRESULT hr;

    FILETIME    now;
    ::CoFileTimeNow(&now);
    SYSTEMTIME sysNow;
    ::FileTimeToSystemTime(&now, &sysNow);
    DATE    dtNow;
    ::SystemTimeToVariantTime(&sysNow, &dtNow);
    CComVariant vtNow = dtNow;
    vtNow.vt = VT_DATE;

    LPOLESTR pszBuf = ::SysAllocStringLen(NULL, _MAX_PATH);
    if (pszBuf == NULL) {
        return E_OUTOFMEMORY;
    }
    CString strNow;
    hr = ::BL_VariantToString(&vtNow, BL_DT, L"", 0, pszBuf, _MAX_PATH);
    if (SUCCEEDED(hr)) {
        // create copy with correct length set !
        strNow = pszBuf;
        ::SysFreeString(pszBuf);
    }
    
    // resource system variables
    GenericPointer<CSysVarPool> pResSys = (CSysVarPool*) NameSpaceHolder::GetResourceDebugSysVars(0);
    if (!pResSys) {
        return E_FAIL;
    }
    // update variables on start of debug session
    if (bLogOn)
    {
        hr = pResSys->UpdateValue(g_csSVarDebugIsEnabled, _T("-1"));
        hr = pResSys->UpdateValue(g_csSVarDebugStartedDate, strNow);
        hr = pResSys->UpdateValue(g_csSVarDebugUser, m_strLoggedInUserName);
        hr = pResSys->UpdateValue(g_csSVarDebugHost, m_strEngMachineName);
    }
    else
    {
        hr = pResSys->UpdateValue(g_csSVarDebugIsEnabled, _T("0"));
        hr = pResSys->UpdateValue(g_csSVarDebugStartedDate, _T(""));
        hr = pResSys->UpdateValue(g_csSVarDebugUser, _T(""));
        hr = pResSys->UpdateValue(g_csSVarDebugHost, _T(""));
    }

    return S_OK;
}

HRESULT CCommissioningMgt::ibnBreakPointNotify(BSTR taskName, LONG bpQuality, BSTR instanceName, LONG breakpointNumber)
{
    HRESULT hr = E_FAIL;
    CCommissioningSession* pCommissioningSession = NULL;
    
    m_cs.Lock();
    if (sm_pCommissioningMgt != NULL && sm_pCommissioningMgt->m_pCommissioningSession != NULL) {
        pCommissioningSession = sm_pCommissioningMgt->m_pCommissioningSession;
        pCommissioningSession->AddRef();
    }
    m_cs.Unlock();

    if (pCommissioningSession != NULL) {
        hr = pCommissioningSession->
                ibnBreakPointNotify(taskName, bpQuality, instanceName, breakpointNumber);

        pCommissioningSession->Release();
    }
    return hr;
}

HRESULT CCommissioningMgt::ibnConfigurationChanged()
{
    HRESULT hr = E_FAIL;
    CCommissioningSession* pCommissioningSession = NULL;
    
    m_cs.Lock();
    if (sm_pCommissioningMgt != NULL && sm_pCommissioningMgt->m_pCommissioningSession != NULL) {
        pCommissioningSession = sm_pCommissioningMgt->m_pCommissioningSession;
        pCommissioningSession->AddRef();
    }
    m_cs.Unlock();

    if (pCommissioningSession != NULL) {
        hr = pCommissioningSession->ibnConfigurationChanged();

        pCommissioningSession->Release();
    }
    return hr;
}

void CConnection::HandleBreakpoints(UINT nBreakpoints, XBPNotification *pNotifications)
{
    for (UINT i=0; i<nBreakpoints; i++)
      {
          CSC_BREAKPOINT_INFO info;
          info.cls_id = TRANSLATE_SHORT(pNotifications[i].BP.uCode);
          info.obj    = TRANSLATE_SHORT(pNotifications[i].BP.uInst);
          info.offset = TRANSLATE_LONG (pNotifications[i].BP.ulCodePos);
          info.type   = TRANSLATE_SHORT(pNotifications[i].BP.uType);
          info.task   = TRANSLATE_SHORT(pNotifications[i].uTask);
          CString path = _T("__unknown");
          long    bpnr = -1;
          CSC_ReverseLookupBreakpoint(info, path, bpnr);
          CString task = _T("__unknown");
          CSC_ReverseLookupTask(info.task, task);
          long qual = TRANSLATE_SHORT(pNotifications[i].uState) == BP_STATE_LEAVED ? -1 : 0;
          CCommissioningMgt::ibnBreakPointNotify(CComBSTR(task), qual, CComBSTR(path), bpnr);
      }
}

/* ---------------------------------------------------------------------------- */
