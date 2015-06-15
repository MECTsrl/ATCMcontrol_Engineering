/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Config.hpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Config.hpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Config.hpp $
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

#ifndef _BAR_CONFIG_H_
#define _BAR_CONFIG_H_

//====  Aggregate Includes:   =================================================*
#include <atlbase.h>

#include <string>
using namespace ::std;

#include "CSC_OnlineServer.h"

//====  Forward Class Definitions:   ==========================================*
class BAR_Callback;
class BAR_CSCOnlineProxy;
class BAR_ArcBackup;
class BAR_ArcRestore;
class BAR_DomainList;

//====  Defines:    ===========================================================*

//====  Globals:    ===========================================================*

//====  Prototypes:    ========================================================*

//=============================================================================*
class BAR_Config
//=============================================================================*
{
public: //=== con-, destruction and assignment

public: //=== access

public: //=== operations
    
    static void DoBackup(const string& sRemoteHost,
                         const string& sPassword,
                         const string& sArcFile,
                         BAR_Callback& callback,
                         bool bUseOwnTlb=true);
    static void DoRestore(const string& sRemoteHost,
                          const string& sPassword,
                          const string& sArcFile,
                          BAR_Callback& callback,
                          bool bUseOwnTlb=true);

public: //=== overrides

private: //=== helpers

    enum {
        ERR_0200 = 200,
        ERR_0201 = 201,
        ERR_0202 = 202,
        ERR_0203 = 203,
        ERR_0204 = 204,
        ERR_0205 = 205,
        ERR_0206 = 206,
        WRN_0207 = 207,
        WRN_0208 = 208,
        ERR_0209 = 209
    };

    static void StoreDomains(BAR_Callback& callback,
                             BAR_ArcBackup& arc,
                             BAR_CSCOnlineProxy& proxy,
                             CSC_TRANSFER_TARGET trg);

    static void StoreDomain(BAR_ArcBackup& arc,
                            BAR_CSCOnlineProxy& proxy,
                            CSC_TRANSFER_TARGET trg, 
                            const string& sDomain,
                            BAR_DomainList* pDomainList=NULL);

    static void RestoreDomains(BAR_Callback& callback,
                               BAR_ArcRestore& arc,
                               BAR_CSCOnlineProxy& proxy);
                                
    static void RestoreDomain(BAR_ArcRestore& arc,
                              BAR_CSCOnlineProxy& proxy,
                              CSC_TRANSFER_TARGET trg);
    
    static string GetDomainListName(CSC_TRANSFER_TARGET trg);

    static void CheckIfCancelled(BAR_Callback& callback);

    static void CheckControlVersion(BAR_Callback& callback,
                                    const string& sSourceControlVersion,
                                    BAR_CSCOnlineProxy& proxy);

    static void Throw_InvalidDomainContent(const string& sDetails);
    static void Throw_FatalError(const string& sDetails);
    static void Throw_WrongControlName(const string& wsSourceControlName);
    static void Throw_InvalidPassword();
    static void Throw_WrongControlVersion(const string& sSrcConVersion);
    static void Throw_DifferentProject();
    static void Throw_Cancelled();
    static void Warning_CanNotReadControlVersion(BAR_Callback& callback);
    static void Warning_UnknownSourceControlVersion(BAR_Callback& callback);
    static void Throw_InvalidArchiveType(const string& sArcType);

private: //=== attributes

    static const string m_sProgramDomainListName;
    static const string m_sVisualDomainListName;

    static const string m_sArcType;
};

#endif // _BAR_CONFIG_H

//=============================================================================*
//=============================================================================*
//=============================================================================*
// OLD HEADER, TO BE REMOVED
//=============================================================================*
//=============================================================================*
//=============================================================================*

/*
//=============================================================================*
//
//  class BAR_Config
//
//  @see    ....
//
//=============================================================================*
class BAR_Config
{
public: //=== con-, destruction and assignment

public: //=== access

public: //=== operations
    
    static void DoBackup(const string& sRemoteHost, 
                         const string& sPassword,
                         const string& sArcFile,
                         BAR_Callback& callback,
                         bool  bUseOwnTlb=true);
    static void DoRestore(const string& sRemoteHost,
                          const string& sPassword,
                          const string& sArcFile,
                          BAR_Callback& callback,
                          bool bUseOwnTlb=true);

public: //=== overrides

private: //=== helpers

    enum {
        ERR_0200 = 200,
        ERR_0201 = 201,
        ERR_0202 = 202,
        ERR_0203 = 203,
        ERR_0204 = 204,
        ERR_0205 = 205,
        ERR_0206 = 206,
        WRN_0207 = 207,
        WRN_0208 = 208
    };

    static void BackupDomains(BAR_Callback& callback,
                              BAR_ArcConfigBackup& arc,
                              BAR_CSCOnlineProxy& proxy,
                              CSC_TRANSFER_TARGET trg,
                              ULONG& ulBytesWritten);

    static void BackupDomain(BAR_ArcConfigBackup& arc,
                             BAR_CSCOnlineProxy& proxy,
                             CSC_TRANSFER_TARGET trg, 
                             const string& sDomain,
                             ULONG& ulBytesWritten,
                             BAR_DomainList* pDomainList=NULL);

    static void BackupAttributes(BAR_Callback& callback,
                                 BAR_ArcConfigBackup& arc,
                                 BAR_CSCOnlineProxy& proxy,
                                 const string& sPassword,
                                 const string& sControlHost);

    static void RestoreDomains(BAR_Callback& callback,
                               BAR_ArcConfigRestore& arc,
                               BAR_CSCOnlineProxy& proxy,
                               CSC_TRANSFER_TARGET trg);

    static void RestoreDomain(BAR_ArcConfigRestore& arc,
                              BAR_CSCOnlineProxy& proxy,
                              CSC_TRANSFER_TARGET trg, 
                              CComBSTR strDomainNode);

    static void CheckPassword(BAR_ArcConfigRestore& arc, 
                              const string& sPassword);
    static void CheckControlName(BAR_ArcConfigRestore& arc, 
                                 const string& sTargetControlName);
    static void CheckControlVersion(BAR_Callback& callback,
                                    BAR_ArcConfigRestore& arc,
                                    BAR_CSCOnlineProxy& proxy);
    static void CheckProjectGUID(BAR_ArcConfigRestore& arc,
                                 BAR_CSCOnlineProxy& proxy,
                                 BAR_Callback& callback);

    static string GetDomainListName(CSC_TRANSFER_TARGET trg);

    static void CheckIfCancelled(BAR_Callback& callback);

    static void Throw_InvalidDomainContent(const string& sDetails);
    static void Throw_FatalError(const string& sDetails);
    static void Throw_WrongControlName(const wstring& wsSourceControlName);
    static void Throw_WrongPassword();
    static void Throw_WrongControlVersion(const wstring& wsSrcConVersion);
    static void Throw_DifferentProject();
    static void Throw_Cancelled();
    static void Warning_CanNotReadControlVersion(BAR_Callback& callback);
    static void Warning_UnknownSourceControlVersion(BAR_Callback& callback);

private: //=== attributes

    static const string m_sProgramDomainListName;
    static const string m_sVisualDomainListName;
};
*/

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Config.hpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Config.hpp $
