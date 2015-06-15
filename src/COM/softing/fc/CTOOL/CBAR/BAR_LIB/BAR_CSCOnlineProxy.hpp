/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_CSCOnlineProxy.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_CSCOnlineProxy.hpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_CSCOnlineProxy.hpp $
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

#ifndef _BAR_CSCONLINEPROXY_H_
#define _BAR_CSCONLINEPROXY_H_

//====  Aggregate Includes:   =================================================*
#include <atlbase.h>

#include <string>
//using namespace ::std;
using ::std::string;
using ::std::wstring;

#include "BAR_Callback.hpp"
#include "opcDA.h"

//====  Forward Class Definitions:   ==========================================*
struct ICommissioningMgt;
struct ITransfer;
class BAR_TypeLib;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
/**
 *  class BAR_CSCOnlineProxy
 *
 *  @see    ....
*/
//=============================================================================*
class BAR_CSCOnlineProxy
{

public: //=== con-, destruction and assignment

     BAR_CSCOnlineProxy(BAR_Callback& callback,
                        bool bRegTlbAllowed=true);
    ~BAR_CSCOnlineProxy(); //=== default destructor

public: //=== access

public: //=== operations

    void Login(const string& sRemoteHost, const string& sPassword);
    void Logout();

    CComBSTR ReadDLAttr(CSC_TRANSFER_TARGET trg, const string& sAttrName);

    string ReadControlVersion();

    void UploadDomain(CSC_TRANSFER_TARGET trg,
                      const string& sDomain,                     
                      SAFEARRAY*& psa);

    
    void StartOnlineChange();

    void DeleteAllDomains();

    void DownloadDomain(CSC_TRANSFER_TARGET trg,
                        CComBSTR strDomainName,
                        SAFEARRAY* psaDomainContent);

    void CommitOnlineChange(CComBSTR strPrjGUID,
                            CComBSTR strDLoadGUID);

    void CancelOnlineChange();

public: //=== overrides

private: //=== helpers

    static enum { ERR_0100 = 100, 
                  ERR_0101 = 101,
                  ERR_0102 = 102,
                  ERR_0103 = 103,
                  WRN_0104 = 104,
                  ERR_0105 = 105,
                  ERR_0106 = 106,
                  WRN_0107 = 107,
                  WRN_0108 = 108,
                  ERR_0109 = 109,
                  ERR_0110 = 110,
                  ERR_0111 = 111,
                  ERR_0112 = 112,
                  ERR_0113 = 113,
                  ERR_0114 = 114,
                  ERR_0115 = 115
    };

    //=== accessing state information
    bool IsComInitialized() const;
    bool IsLoginAllowed()   const;
    bool IsLoggedIn()       const;
    bool IsRegTlbAllowed()  const;

    void CheckIfCancelled() const;      

    //=== modifying state
    void SetComInitialized(bool bComInitialized);
    void SetLoggedIn(bool bLoggedIn);

    //=== resource management

    void ConnectToControl(const string& sRemotehost, const string& sPassword);
    void DisconnectFromControl();

    IUnknown* CreateInstance(const string sLabel,
                             const IID&   iid,
                             const CLSID& clsid,
                             const string& sRemoteHost,
                             bool bLoadTypeLib,
                             const string& sTlbLabel,
                             const IID& iidTlb,
                             const string& sTlbIndex,
                             BAR_TypeLib*& pTlb);
    void ReleaseInstances();

    void LoginForCommissioning(const string& sPassword);
    void LogoutFromCommissioning();
    
    //=== accessing system info
    const string& GetCurrentUserName();
    const string& GetLocalHostName();
    
    //=== errors/warnings
    void Throw_AlreadyLoggedIn                () const;
    void Throw_ConnectionToControlFailed      (HRESULT hr, 
                                               const wstring& wsDetails) const;
    void Throw_FatalError                     (const wstring& wsDetails) const;
    void Warning_LogoutFailed                 (HRESULT hr,
                                               const wstring& wsDetails,
                                               long lSessionKey) const;
    void Throw_Cancelled                      () const;
    void Warning_LogoutFromCommissioningFailed(HRESULT hr) const;

    void Throw_UploadDomainFailed             (HRESULT hr,
                                               const wstring& wsDetails) const;
    void Throw_ReadingDLAttrFailed            (HRESULT hr,
                                               CSC_TRANSFER_TARGET trg,
                                               const wstring& wsAttrName) const;
    void Throw_StartOnlineChangeFailed        (HRESULT hr,
                                               CSC_TRANSFER_TARGET trg) const;
    void Throw_CommitOnlineChangeFailed       (HRESULT hr,
                                               CSC_TRANSFER_TARGET trg) const;
    void Throw_DeletingAllDomainsFailed       (HRESULT hr,
                                               CSC_TRANSFER_TARGET trg) const;
    void Throw_DownloadDomainFailed           (HRESULT hr,
                                               CSC_TRANSFER_TARGET trg,
                                               const wstring& wsDomainName) const;
    void Throw_LoginForCommissioningFailed    (HRESULT hr) const;
    void Throw_ReadControlVersionFailed       (HRESULT hr) const;

private: //=== attributes

    const bool m_bIsRegTlbAllowed;

    string m_sRemoteHost;

    string m_sCurrentUser;
    string m_sLocalHost;

    bool m_bIsComInitialized;
    bool m_bIsLoggedIn;

    BAR_Callback& m_callback;

    ICommissioningMgt* m_pICommissioningMgt;
    ITransfer*         m_pITransfer;
    IOPCServer*        m_pIOPCServer;

    long m_lSessionKey;

    BAR_TypeLib* m_pTlbCSCOnlSrv;
};

//=============================================================================*
inline bool BAR_CSCOnlineProxy::IsRegTlbAllowed() const
//=============================================================================*
{ return m_bIsRegTlbAllowed; }

//=============================================================================*
inline bool BAR_CSCOnlineProxy::IsLoginAllowed() const
//=============================================================================*
{ return ! ( m_bIsComInitialized || m_bIsLoggedIn ); }

//=============================================================================*
inline bool BAR_CSCOnlineProxy::IsComInitialized() const
//=============================================================================*
{ return m_bIsComInitialized; }

//=============================================================================*
inline bool BAR_CSCOnlineProxy::IsLoggedIn() const
//=============================================================================*
{ return m_bIsLoggedIn; }

//=============================================================================*
inline void BAR_CSCOnlineProxy::CheckIfCancelled() const
//=============================================================================*
{ if ( m_callback.IsCancelRequested() ) Throw_Cancelled(); }

//=============================================================================*
inline void BAR_CSCOnlineProxy::SetLoggedIn(bool bLoggedIn)
//=============================================================================*
{ m_bIsLoggedIn = bLoggedIn; }

//=============================================================================*
inline void BAR_CSCOnlineProxy::SetComInitialized(bool bComInitialized)
//=============================================================================*
{ m_bIsComInitialized = bComInitialized; }

//=============================================================================*
#endif // _BAR_CSCONLINEPROXY_H

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_CSCOnlineProxy.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 24.09.01   Time: 12:01
 * Updated in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * Adapted to new CSC_Online.
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:32
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_CSCOnlineProxy.hpp $
