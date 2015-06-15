/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_CSCOnlineProxy.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CSCOnlineProxy.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_CSCOnlineProxy.cpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
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
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#include <sstream>

#include "objbase.h"
#include "assert.h"

#include "CSC_OnlineServer.h"
#include "CSC_OnlineServer_i.c"
#include "CSC_OnlSrv.h"
#include "CSC_OnlSrv_i.c"

#include "opcDA_i.c"

#include "BAR_CSCOnlineProxy.hpp"
#include "BAR_TypeLib.hpp"
#include "BAR_Callback.hpp"
#include "BAR_Trace.hpp"
#include "BAR_Util.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_CSCOnlineProxy
//=============================================================================*

//=============================================================================*
/**
 * <B>constructor</B>
 * @param           void
 * @return          void
 * @exception       -
 */
//=============================================================================*
BAR_CSCOnlineProxy::BAR_CSCOnlineProxy(BAR_Callback& callback, 
                                       bool bRegTlbAllowed)
: m_sCurrentUser(""),
  m_sRemoteHost(""),
  m_sLocalHost(""),
  m_bIsComInitialized(false),
  m_bIsLoggedIn(false),
  m_callback(callback),
  m_bIsRegTlbAllowed(bRegTlbAllowed),
  m_pICommissioningMgt(NULL),
  m_pITransfer(NULL),
  m_pIOPCServer(NULL),
  m_lSessionKey(0),
  m_pTlbCSCOnlSrv(NULL)
{
}

//=============================================================================*
/**
 * <B>destructor</B>
 * @param           void
 * @return          void
 * @exception       -
*/
//=============================================================================*
BAR_CSCOnlineProxy::~BAR_CSCOnlineProxy()
{
    Logout();
}

//=============================================================================*
/**
 * <B>Establish an exclusive connection from the scratch.</B>
 * @param           void
 * @return          void
 * @exception       BAR_Error
*/
//=============================================================================*
void BAR_CSCOnlineProxy::Login(const string& sRemoteHost, const string& sPassword)
{
    //=== check if logging in is allowed
    if ( ! IsLoginAllowed() )
    {
        Throw_AlreadyLoggedIn();
    }

    BAR_TRACE1(2, "login started, host=%s", sRemoteHost.c_str());

    m_sRemoteHost = sRemoteHost;

    ConnectToControl(sRemoteHost, sPassword);

    //=== finished
    BAR_TRACE1(2, "logged in, host=%s", sRemoteHost.c_str());
    SetLoggedIn(true);
}


//=============================================================================*
/**
 * <B>Release an exclusive connection.</B>
 * @param           void
 * @return          void
 * @exception       -
*/
//=============================================================================*
void BAR_CSCOnlineProxy::Logout()
{
    if ( IsLoggedIn() )
    {
        BAR_TRACE(2, "logout started");
    }

    DisconnectFromControl();

    //=== unregister type libs if any were registered
    if ( m_pTlbCSCOnlSrv != NULL )
    {
        delete m_pTlbCSCOnlSrv;
        m_pTlbCSCOnlSrv = NULL;
    }

    //=== finished
    if ( IsLoggedIn() )
    {
        BAR_TRACE(2, "logged out");
    }

    SetLoggedIn(false);
}


//=============================================================================*
void BAR_CSCOnlineProxy::ConnectToControl(const string& sRemoteHost, const string& sPassword)
//=============================================================================*
{
    //=== instantiate commissioning management
    m_pICommissioningMgt = (ICommissioningMgt*)
        CreateInstance("commissioning management",
                       IID_ICommissioningMgt,
                       CLSID_CommissioningMgt,
                       sRemoteHost,
                       IsRegTlbAllowed(),
                       "CSC_Onl_Srv",
                       /*LIBID_CSC_OnlineServer*/LIBID_CSC_Srv_Lib,
                       "1",
                       m_pTlbCSCOnlSrv);

    //=== login for commissioning
    LoginForCommissioning(sPassword);

    return;
}

//=============================================================================*
void BAR_CSCOnlineProxy::DisconnectFromControl()
//=============================================================================*
{
    LogoutFromCommissioning();

    ReleaseInstances();

    return;
}

//=============================================================================*
IUnknown* BAR_CSCOnlineProxy::CreateInstance(const string sLabel,
                                             const IID&   iid,
                                             const CLSID& clsid,
                                             const string& sRemoteHost,
                                             bool bLoadTypeLib,
                                             const string& sTlbLabel,
                                             const IID& iidTlb,
                                             const string& sTlbIndex,
                                             BAR_TypeLib*& pTlb
                                             )
//=============================================================================*
{
    BAR_TRACE2(2, "instantiating %s, host=%s", 
                  sLabel.c_str(), sRemoteHost.c_str());

    //=== prepare interface query
    MULTI_QI queue[1];
    queue[0].pIID = &iid;
    queue[0].pItf = NULL;
    queue[0].hr   = 0;
    ULONG cmq = 1;
    DWORD clsctx;

    //=== set up server node info
    COSERVERINFO  serverInfo;
    COSERVERINFO* pServerInfo = &serverInfo;

    serverInfo.dwReserved1 = 0;
    serverInfo.dwReserved2 = 0;
    serverInfo.pwszName    = CComBSTR(sRemoteHost.c_str());
    serverInfo.pAuthInfo   = 0;
    clsctx = CLSCTX_REMOTE_SERVER | CLSCTX_LOCAL_SERVER;

    //=== create instance and get the interface
    HRESULT hr = ::CoCreateInstanceEx(clsid, NULL,clsctx, pServerInfo, cmq, queue);
    if ( FAILED(hr) )
    {
        BAR_TRACE2(2, "could not instantiate %s, %s",
                      sLabel.c_str(), HRToString(hr).c_str());
    }

    //=== if instantiation failed, perhaps there is no type library installed ...
    if ( hr == REGDB_E_IIDNOTREG && bLoadTypeLib )
    {
        BAR_TRACE1(2, "loading %s typelib now", sTlbLabel.c_str());

        //=== register own typelibrary ...
        pTlb = new BAR_TypeLib(m_callback,
                               sTlbLabel,
                               iidTlb,
                               "4cbar.exe",
                               sTlbIndex);

        //=== ... and try again to create instance
        hr = ::CoCreateInstanceEx(clsid, NULL, clsctx, pServerInfo, cmq, queue);
        if ( FAILED(hr) )
        {
            Throw_ConnectionToControlFailed(hr,
                wstring(L"could not create ") + ToWS(sLabel)
                + L", even after loading " + ToWS(sTlbLabel) + L" type library");
        }
    }
    else if ( FAILED(hr) )
    {
        Throw_ConnectionToControlFailed(hr, wstring(L"could not instantiate ")
                                        + ToWS(sLabel));
    }
    else
    {
        BAR_TRACE1(2, "loading %s type library not necessary", sTlbLabel.c_str());
    }

    //=== check if interface query succeeded
    if ( FAILED(queue[0].hr) )
    {
        Throw_ConnectionToControlFailed(hr, wstring(L"could not query ")
                                        + ToWS(sLabel) + L" interface");
    }
    if ( queue[0].pItf == 0 )
    {
        Throw_FatalError(ToWS(sLabel) + L" interface pointer is null");
    }

    //=== finished
    return queue[0].pItf;
}

//=============================================================================*
void BAR_CSCOnlineProxy::ReleaseInstances()
//=============================================================================*
{
    if ( m_pICommissioningMgt != NULL )
    {
        BAR_TRACE(2, "releasing commissioning management interface");
        m_pICommissioningMgt->Release();
        m_pICommissioningMgt = NULL;
    }
}

//=============================================================================*
void BAR_CSCOnlineProxy::LoginForCommissioning(const string& sPassword)
//=============================================================================*
{
    //=== open a commissioning session
    //=== and retrieve unknown interface
    IUnknown* pUnk = NULL;
    const string& sUser      = GetCurrentUserName();
    const string& sLocalHost = GetLocalHostName();

    HRESULT hr = m_pICommissioningMgt->Login(
        CComBSTR(sUser.c_str()),
        CComBSTR(sPassword.c_str()),
        CComBSTR(sLocalHost.c_str()),
        &m_lSessionKey,
        &pUnk);
    if ( FAILED(hr) )
    {
        Throw_LoginForCommissioningFailed(hr);
    }
    assert( pUnk != NULL );
    if ( pUnk == NULL )
        Throw_FatalError(L"null interface from loginforcommissioning()");
    BAR_TRACE3(2, "logged in for commissioning"
                  ", user=%s, localhost=%s, key=(%i)",
                  sUser.c_str(), sLocalHost.c_str(), m_lSessionKey);
    
    //=== query transfer interface via unknown interface
    BAR_TRACE(2, "querying transfer interface");
    hr = pUnk->QueryInterface(IID_ITransfer, (void**)&m_pITransfer);
    pUnk->Release();
    if ( FAILED(hr) )
        Throw_ConnectionToControlFailed(hr, 
                                        L"querying transfer interface failed");
    assert( m_pITransfer != NULL );
    if ( m_pITransfer == NULL )
        Throw_FatalError(L"transfer interface is null after successful query");
}

//=============================================================================*
void BAR_CSCOnlineProxy::LogoutFromCommissioning()
//=============================================================================*
{
    //=== cancel all online changes
    //=== for defensive reasons
    CancelOnlineChange();

    //=== release transfer interface
    if ( m_pITransfer != NULL )
    {
        BAR_TRACE(2, "releasing transfer interface");
        m_pITransfer->Release();
        m_pITransfer = NULL;
    }

    //=== logout from commissioning session
    if ( ( m_pICommissioningMgt != NULL ) && ( m_lSessionKey != 0 ) )
    {
        BAR_TRACE1(2, "logging out from commissioning, key=(%i)",
                      m_lSessionKey);
        HRESULT hr = m_pICommissioningMgt->Logout(m_lSessionKey);
        if ( FAILED(hr) )
        {
            Warning_LogoutFromCommissioningFailed(hr);
        }
        m_lSessionKey = 0;
    }
}

//=============================================================================*
void BAR_CSCOnlineProxy::UploadDomain(CSC_TRANSFER_TARGET trg,
                                      const string& sDomain,
                                      SAFEARRAY*& psa)
//=============================================================================*
{
    const string sDetails = string("trg=") + TrgToString(trg)
                          + ", domain=" + sDomain;
    BAR_TRACE1(4, "uploading domain, %s", sDetails.c_str());

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not upload domain before logged in");
    }

    psa = NULL;
	/* V1.3
    HRESULT hr = m_pITransfer->UploadDomain(m_lSessionKey,
                                            trg,
                                            CComBSTR(""),
                                            CComBSTR(sDomain.c_str()),
                                            &psa);
	*/
    HRESULT hr = m_pITransfer->UploadDomain(m_lSessionKey,
                                            trg,
                                            CComBSTR(sDomain.c_str()),
                                            &psa);

    if ( FAILED(hr) )
    {
        Throw_UploadDomainFailed(hr, ToWS(sDetails));
    }
    
    assert( psa != NULL );
    if ( psa == NULL )
    {
        Throw_FatalError(wstring(L"null domain uploaded, ") + ToWS(sDetails));
    }
}

//=============================================================================*
CComBSTR BAR_CSCOnlineProxy::ReadDLAttr(CSC_TRANSFER_TARGET trg,
                                        const string& sAttrName)
//=============================================================================*
{
    BAR_TRACE2(4, "read download attribute, trg=%s, attr=%s",
                   TrgToString(trg).c_str(), sAttrName.c_str());

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not read download attribute before logged in");
    }


    VARIANT vtAttrVal;

    //=== read download attribute
    HRESULT hr = m_pITransfer->get_DLoadAttribute(m_lSessionKey,                                          
                                                  trg,
                                                  //CComBSTR(""),
                                                  CComBSTR(sAttrName.c_str()),
                                                  &vtAttrVal);
    if ( FAILED(hr) )
    {
        Throw_ReadingDLAttrFailed(hr, trg, ToWS(sAttrName));
    }
    if ( vtAttrVal.vt != VT_BSTR )
    {
        Throw_FatalError(L"read download attribute has wrong type");
    }

    //=== finished
    return vtAttrVal.bstrVal;
}
 
//=============================================================================*
void BAR_CSCOnlineProxy::DeleteAllDomains()
//=============================================================================*
{
    BAR_TRACE(2, "deleting all domains");

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not delete domains before logged in");
    }

    //=== delete all domains
    HRESULT hr = m_pITransfer->DeleteAllDomains(m_lSessionKey,
                                                ProgramDomains/*,
                                                L""*/);
    if ( FAILED(hr) )
    {
        Throw_DeletingAllDomainsFailed(hr, ProgramDomains);
    }
    hr = m_pITransfer->DeleteAllDomains(m_lSessionKey,
                                        VisualDomains/*,
                                        L""*/);
    if ( FAILED(hr) )
    {
        Throw_DeletingAllDomainsFailed(hr, VisualDomains);
    }
}

//=============================================================================*
void BAR_CSCOnlineProxy::StartOnlineChange()
//=============================================================================*
{
    BAR_TRACE(2, "starting online change");

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not start online change before logged in");
    }

    //=== start online change
    HRESULT hr = m_pITransfer->StartOnlineChange(m_lSessionKey,
                                                 ProgramDomains/*,
                                                 L""*/);
    if ( FAILED(hr) )
    {
        Throw_StartOnlineChangeFailed(hr, ProgramDomains);
    }
    hr = m_pITransfer->StartOnlineChange(m_lSessionKey,
                                         VisualDomains/*,
                                         L""*/);
    if ( FAILED(hr) )
    {
        Throw_StartOnlineChangeFailed(hr, VisualDomains);
    }
}

//=============================================================================*
void BAR_CSCOnlineProxy::CommitOnlineChange(CComBSTR strPrjGUID,
                                            CComBSTR strDLoadGUID)
//=============================================================================*
{
    BAR_TRACE(2, "commiting online change");

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not commit online change before logged in");
    }

    //=== start online change
    HRESULT hr = m_pITransfer->CommitOnlineChange(m_lSessionKey,
                                                  ProgramDomains,
                                                  L"",
                                                  L"",
                                                  strPrjGUID,
                                                  strDLoadGUID);
    if ( FAILED(hr) )
    {
        Throw_CommitOnlineChangeFailed(hr, ProgramDomains);
    }
    hr = m_pITransfer->CommitOnlineChange(m_lSessionKey,
                                          VisualDomains,
                                          L"",
                                          L"",
                                          strPrjGUID,
                                          strDLoadGUID);
    if ( FAILED(hr) )
    {
        Throw_CommitOnlineChangeFailed(hr, VisualDomains);
    }
}

//=============================================================================*
void BAR_CSCOnlineProxy::DownloadDomain(CSC_TRANSFER_TARGET trg,
                                        CComBSTR strDomainName,
                                        SAFEARRAY* psaDomainContent)
//=============================================================================*
{
    const wstring wsDetails = wstring(L"trg=") 
                             + ToWS(TrgToString(trg))
                             + L", domain=" + strDomainName.m_str;
    BAR_WTRACE1(4, L"downloading domain, %s", wsDetails.c_str());

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not download domain before logged in");
    }

    HRESULT hr = m_pITransfer->DownloadDomain(m_lSessionKey,
                                              trg,
                                              //L"",
                                              strDomainName,
                                              &psaDomainContent);
    if ( FAILED(hr) )
    {
        Throw_DownloadDomainFailed(hr, trg, strDomainName.m_str);
    }
}
    
//=============================================================================*
void BAR_CSCOnlineProxy::CancelOnlineChange()
//=============================================================================*
{
    if ( m_pITransfer != NULL )
    {
        BAR_TRACE(2, "cancelling all pending online changes");
        m_pITransfer->CancelOnlineChange(m_lSessionKey,
                                         ProgramDomains/*,
                                         L""*/);
        m_pITransfer->CancelOnlineChange(m_lSessionKey,
                                         VisualDomains/*,
                                         L""*/);
    }
}

//=============================================================================*
string BAR_CSCOnlineProxy::ReadControlVersion()
//=============================================================================*
{
    BAR_TRACE(4, "read control version");

    if ( ! IsLoggedIn() )
    {
        Throw_FatalError(L"can not read control version before logged in");
    }

    //=== instantiate opc server
//    BAR_TypeLib* pTlb = NULL;
//    IOPCServer* pIOPCServer = (IOPCServer*)CreateInstance("opc server",
//                                                         IID_IOPCServer,
//                                                          CLSID_OPCServer,
//                                                          m_sRemoteHost,
//                                                          "OPCDA",
//                                                          LIBID_OPCDA,
//                                                          "2",
//                                                        pTlb);
    //=== instantiate opc server
    IOPCServer* pIOPCServer = (IOPCServer*) CreateInstance("opc server",
                                                 IID_IOPCServer,
                                                 CLSID_OPCServer,
                                                 m_sRemoteHost,
                                                 false,
                                                 "CSC_Onl_Srv",
                                                 /*LIBID_OPCDA*/LIBID_CSC_Srv_Lib,
                                                 "1",
                                                 m_pTlbCSCOnlSrv);


    //=== get status item
    OPCSERVERSTATUS *pStatus = NULL;
    HRESULT hr = pIOPCServer->GetStatus(&pStatus);

    if ( FAILED(hr) )
    {
        Throw_ReadControlVersionFailed(hr);
    }
    if ( pStatus == NULL )
    {
        Throw_FatalError(L"opc server status is null");
    }
    WORD wMajorVersion = pStatus->wMajorVersion;
    WORD wMinorVersion = pStatus->wMinorVersion;
    ::CoTaskMemFree(pStatus);

    //=== build version string
    ostringstream ossControlVersion;
    ossControlVersion << (int)wMajorVersion << "." << (int)wMinorVersion;
    string sControlVersion = ossControlVersion.str();

    //=== release opc server
    if ( pIOPCServer != NULL )
    {
        pIOPCServer->Release();
    }
/*    //=== unregister type library
    if ( pTlb != NULL )
    {
        delete pTlb;
    }
*/
    //=== finished
    BAR_TRACE1(1, "control version=%s", sControlVersion.c_str());
    return sControlVersion.c_str();
}

//=============================================================================*
const string& BAR_CSCOnlineProxy::GetCurrentUserName()
//=============================================================================*
{
    if ( m_sCurrentUser.empty() )
    {
        DWORD dwSize = _MAX_PATH;
        char* szCurrentUser = new char[dwSize];
        if ( ::GetUserName(szCurrentUser, &dwSize) )
        {
            m_sCurrentUser = szCurrentUser;
        }
        else
        {
            Throw_FatalError(L"could not determine current user name");
        }
    }

    return m_sCurrentUser;
}

//=============================================================================*
const string& BAR_CSCOnlineProxy::GetLocalHostName()
//=============================================================================*
{
    if ( m_sLocalHost.empty() )
    {
        DWORD dwSize = _MAX_PATH;
        char* szLocalHost = new char[dwSize];
        if ( ::GetComputerName(szLocalHost, &dwSize) )
        {
            m_sLocalHost = szLocalHost;
        }
        else
        {
            Throw_FatalError(L"could not determine local host name");
        }
    }

    return m_sLocalHost;
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_AlreadyLoggedIn() const
//=============================================================================*
{
    throw BAR_Error(ERR_0100, L"csc-online already logged in"); 
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_ConnectionToControlFailed(HRESULT hr, 
                                                         const wstring& wsDetails) const
//=============================================================================*
{
   throw BAR_Error(ERR_0102, 
                    L"connection to control failed", 
                    wsDetails + L", " + HRToWString(hr)
                    );
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_FatalError(const wstring& wsDetails ) const
//=============================================================================*
{
    throw BAR_Error(ERR_0103, L"fatal error", wsDetails);
}

//=============================================================================*
void BAR_CSCOnlineProxy::Warning_LogoutFailed(HRESULT hr,
                                              const wstring& wsDetails,
                                              long lSessionKey) const
//=============================================================================*
{
    wostringstream ossDetails;
    ossDetails << wsDetails
               << L", key=" << lSessionKey << L", "
               << HRToWString(hr);
    m_callback.SendWarning(BAR_Warning(WRN_0104,
                                       L"logout failed",
                                       ossDetails.str()));
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_Cancelled() const
//=============================================================================*
{
    throw BAR_Error(ERR_0105, L"action cancelled by user");
}

//=============================================================================*
void BAR_CSCOnlineProxy::Warning_LogoutFromCommissioningFailed(HRESULT hr) const
//=============================================================================*
{
    m_callback.SendWarning(BAR_Warning(WRN_0108,
                                      L"logout from commissioning failed",
                                      HRToWString(hr)));
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_UploadDomainFailed(HRESULT hr,
                                                  const wstring& wsDetails ) const
//=============================================================================*
{
    throw BAR_Error(ERR_0109, L"uploading domain failed", 
                              HRToWString(hr)  + L", " + wsDetails);
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_ReadingDLAttrFailed(HRESULT hr,
                                                   CSC_TRANSFER_TARGET trg,
                                                   const wstring& wsAttrName) const
//=============================================================================*
{
    throw BAR_Error(ERR_0110, L"reading download attribute failed",
                              HRToWString(hr)  
                              + L", attr=" + wsAttrName);
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_StartOnlineChangeFailed(HRESULT hr,
                                                       CSC_TRANSFER_TARGET trg) const
//=============================================================================*
{
    throw BAR_Error(ERR_0111, L"starting online change failed",
                              HRToWString(hr)  
                              + L", trg=" + ToWS(TrgToString(trg)));
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_CommitOnlineChangeFailed(HRESULT hr,
                                                        CSC_TRANSFER_TARGET trg) const
//=============================================================================*
{
    throw BAR_Error(ERR_0111, L"commiting online change failed",
                              HRToWString(hr)  
                              + L", trg=" + ToWS(TrgToString(trg)));
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_DeletingAllDomainsFailed(HRESULT hr,
                                                        CSC_TRANSFER_TARGET trg) const
//=============================================================================*
{
    throw BAR_Error(ERR_0112, L"deleting all domains failed",
                              HRToWString(hr)  
                              + L", trg=" + ToWS(TrgToString(trg)));
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_DownloadDomainFailed(HRESULT hr,
                                                    CSC_TRANSFER_TARGET trg,
                                                    const wstring& wsDomainName) const
//=============================================================================*
{
    throw BAR_Error(ERR_0113, L"downloading domain failed",
                              HRToWString(hr)
                              + L", trg=" + ToWS(TrgToString(trg))
                              + L", domain=" + wsDomainName
                              );
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_LoginForCommissioningFailed(HRESULT hr) const
//=============================================================================*
{
    throw BAR_Error(ERR_0114, L"login for commissioning failed",
                              wstring(L"maybe another process has an "
                                      L"exclusive connection to the control, ")
                              + HRToWString(hr)
                              );
}

//=============================================================================*
void BAR_CSCOnlineProxy::Throw_ReadControlVersionFailed(HRESULT hr) const
//=============================================================================*
{
    throw BAR_Error(ERR_0115, L"reading control version failed",
                              HRToWString(hr)
                              );
}

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CSCOnlineProxy.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 24.09.01   Time: 12:01
 * Updated in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * Adapted to new CSC_Online.
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 12.07.01   Time: 14:46
 * Updated in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * changed function class due to changed interface 'ITransfer'
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_CSCOnlineProxy.cpp $
