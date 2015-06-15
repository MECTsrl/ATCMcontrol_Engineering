/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Config.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Config.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Config.cpp $
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

#include "CSC_OnlineServer.h"

#include "BAR_Config.hpp"
#include "BAR_ArcBackup.hpp"
#include "BAR_ArcRestore.hpp"
#include "BAR_CSCOnlineProxy.hpp"
#include "BAR_DomainList.hpp"
#include "BAR_Trace.hpp"
#include "BAR_Util.hpp"

//====  Local Defines:   ======================================================*

//====  Static Initializations:   =============================================*


const string BAR_Config::m_sArcType = "4CBAR_CONFIGURATION_ARCHIVE_V1.0";
const string BAR_Config::m_sProgramDomainListName = "__.list";
const string BAR_Config::m_sVisualDomainListName  = "__vis.list";

//=============================================================================*
// MEMBER FUNCTIONS OF CLASS BAR_Config
//=============================================================================*

//=============================================================================*
//
// performing a configuration backup
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::DoBackup(const string& sRemoteHost,
                          const string& sPassword,
                          const string& sArcFile,
                          BAR_Callback& callback,
                          bool bUseOwnTlb)
{
    BAR_TRACE(2, "initializing com");
    HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) ) 
    {
        Throw_FatalError(string("COM Initialization failed, ")
                         + HRToString(hr));
    }
    CheckIfCancelled(callback);

    //=== login on online service
    BAR_TRACE(1, "connecting to control");
    BAR_CSCOnlineProxy proxy(callback, bUseOwnTlb);

    proxy.Login(sRemoteHost, sPassword);
    CheckIfCancelled(callback);

    //=== open backup archive
    BAR_TRACE(1, "opening archive");
    BAR_ArcBackup arc(callback);
    arc.Open(sArcFile);
    CheckIfCancelled(callback);

    //=== store arctype
    arc.Store(m_sArcType);

    //=== store password
    arc.Store(sPassword);

    //=== store number of attributes
    arc.Store((long)4);

    //=== store attribute: control version
    arc.Store("ControlVersion");
    try 
    {
        arc.Store(proxy.ReadControlVersion());
    }
    catch(BAR_Error e)
    {
        BAR_TRACE2(2,
                   "catched bar_error: %s, %s",
                   e.GetMessage().c_str(),
                   e.GetDetails().c_str());
        arc.Store(L"");
        Warning_CanNotReadControlVersion(callback);
    }

    //=== store attribute: project guid
    arc.Store("PrjGUID");
    arc.Store(proxy.ReadDLAttr(ProgramDomains, "PrjGUID").m_str);

    //=== store attribute: download guid
    arc.Store("DLoadGUID");
    arc.Store(proxy.ReadDLAttr(ProgramDomains, "DLoadGUID").m_str);

    //=== store attribute: control host
    arc.Store("ControlHost");
    arc.Store(GetUpper(sRemoteHost));

    //=== store visual domains
    StoreDomains(callback, arc, proxy, VisualDomains);

    //=== store program domains
    StoreDomains(callback, arc, proxy, ProgramDomains);

    //=== 6. close backup archive
    BAR_TRACE(1, "closing archive");
    arc.Close();
    CheckIfCancelled(callback);

    //=== 7. logout from online service
    BAR_TRACE(1, "unconnect from control");
    proxy.Logout();

    BAR_TRACE(2, "uninitializing com");
    ::CoUninitialize();

}

//=============================================================================*
//
// performing a configuration restore
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::DoRestore(const string& sRemoteHost, 
                           const string& sPassword,
                           const string& sArcFile,
                           BAR_Callback& callback,
                           bool bUseOwnTlb)
{
    BAR_TRACE(2, "initializing com");
    HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) ) 
    {
        Throw_FatalError(string("COM Initialization failed, ")
                         + HRToString(hr));
    }
    CheckIfCancelled(callback);

    //=== open restore archive
    BAR_TRACE(1, "open archive");
    BAR_ArcRestore arc(callback);
    arc.Open(sArcFile);
    CheckIfCancelled(callback);

    //=== restore archive type
    string sArcType = "";
    arc.Restore(sArcType);

    //=== check archive type
    if ( sArcType != m_sArcType )
    {
        Throw_InvalidArchiveType(sArcType);        
    }

    //=== restore password
    string sStoredPassword;
    arc.Restore(sStoredPassword);

    //=== check password
    if ( sStoredPassword != "" && //=== but only if password is required
         sStoredPassword != sPassword )
    {
        Throw_InvalidPassword();
    }

    //=== restore number of attributes
    long lNoOfAttributes = 0;
    arc.Restore(lNoOfAttributes);

    //=== restore attributes
    string  sStoredControlVersion;
    string  sStoredControlHost;
    wstring wsStoredProjectGUID;
    wstring wsStoredDownloadGUID;
    for(int i = 0; i < lNoOfAttributes; i++)
    {
        //=== restore attribute name
        string sAttrName;
        arc.Restore(sAttrName);

        //=== restore attribute value
        if ( sAttrName == "ControlVersion" )
        {
            arc.Restore(sStoredControlVersion);
        }
        else if ( sAttrName == "PrjGUID" )
        {
            arc.Restore(wsStoredProjectGUID);
        }
        else if ( sAttrName == "DLoadGUID" )
        {
            arc.Restore(wsStoredDownloadGUID);
        }
        else if ( sAttrName == "ControlHost" )
        {
            arc.Restore(sStoredControlHost);
        }
        //=== skip unexpected attributes
        else
        {
            long l = 0;
            arc.Restore(l);
            arc.Skip(l);
        }
    }

    //=== compare control names ( case insensitive )
    if ( sStoredControlHost != GetUpper(sRemoteHost) )
    {
        Throw_WrongControlName(sStoredControlHost);
    }    

    //=== login on online service
    BAR_TRACE(1, "connect to control");
    BAR_CSCOnlineProxy proxy(callback, bUseOwnTlb);
    proxy.Login(sRemoteHost, sPassword);
    CheckIfCancelled(callback);

    //=== compare control versions
    CheckControlVersion(callback, sStoredControlVersion, proxy);
    CheckIfCancelled(callback);

    //=== check project guid
    BAR_TRACE(1, "check project guid");
    if ( wsStoredDownloadGUID != proxy.ReadDLAttr(ProgramDomains, "dloadguid").m_str )
    {
        //=== if guids differ, ask for permission to overwrite actual project
        const string sQuestion = "Actual control configuration "
                                 "belongs to a different project/download. "
                                 " Do you want to overwrite it ?";
        if ( ! callback.IsOverwriteAllowed(sQuestion) )
        {
            Throw_DifferentProject();
        }
    }
    CheckIfCancelled(callback);

    //=== start online change
    BAR_TRACE(1, "start online change");
    proxy.StartOnlineChange();
    CheckIfCancelled(callback);

    //=== delete all domains
    BAR_TRACE(1, "delete configuration");
    proxy.DeleteAllDomains();    
    CheckIfCancelled(callback);

    //=== restore visual domains
    RestoreDomains(callback, arc, proxy);
    CheckIfCancelled(callback);

    //=== restore program domains
    RestoreDomains(callback, arc, proxy);
    CheckIfCancelled(callback);

    //=== commit online change
    BAR_TRACE(1, "commit online change");
    proxy.CommitOnlineChange(wsStoredProjectGUID.c_str(),
                             wsStoredDownloadGUID.c_str());
    CheckIfCancelled(callback);

    //=== logout from online service
    BAR_TRACE(1, "unconnect from control");
    proxy.Logout();
    CheckIfCancelled(callback);

    //=== close backup archive
    BAR_TRACE(1, "close archive");
    arc.Close();

    ::CoUninitialize();
    BAR_TRACE(2, "uninitializing com");
}

//=============================================================================*
//
// backup all domains belonging to the specified transfer target
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::StoreDomains(BAR_Callback& callback,
                                  BAR_ArcBackup& arc,
                                  BAR_CSCOnlineProxy& proxy,                               
                                  CSC_TRANSFER_TARGET trg)
{
    BAR_TRACE1(1, "store %s domains",  TrgToString(trg).c_str());

    //=== store domain list and parse it
    BAR_DomainList domainList(trg);
    StoreDomain(arc, 
                proxy,
                trg,
                GetDomainListName(trg),
                &domainList);

    //=== backup each listed domain
    for( int i = 0; i < domainList.GetSize(); i++ )
    {
        StoreDomain(arc,
                    proxy, 
                    trg,
                    domainList.GetDomainName(i)
                    );
        CheckIfCancelled(callback);
    }

    //=== finished
}

//=============================================================================*
void BAR_Config::StoreDomain(BAR_ArcBackup& arc,
                                 BAR_CSCOnlineProxy& proxy,
                                 CSC_TRANSFER_TARGET trg, 
                                 const string& sDomain,
                                 BAR_DomainList* pDomainList/*=NULL*/
                                )
//=============================================================================*
{   
    const string sDetails = string("trg=") + TrgToString(trg)
                          + ", domain=" + sDomain;
    BAR_TRACE1(3, "store domain, %s", sDetails.c_str());

    //=== upload domain
    SAFEARRAY* psa;
    proxy.UploadDomain(trg, sDomain, psa);

    //=== check format of safearray
    if (    ( psa->cDims != 1) 
         || ( psa->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
         || ( psa->cbElements != sizeof(BYTE))
         || ( psa->rgsabound[0].lLbound != 0)
       )
        {
            ::SafeArrayDestroy(psa);
            Throw_InvalidDomainContent(sDetails);
        }
    DWORD dwDomainSize = (psa)->rgsabound[0].cElements;

   //=== access domain data
    char* pDomainData;
    HRESULT hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
    if ( FAILED(hr) )
    {
        ::SafeArrayDestroy(psa);
        Throw_FatalError(string("could not access data of uploaded domain, ")
                         + sDetails);
    }
 
    //=== if a domain list object is supplied ...
    if ( pDomainList != NULL )
    {
        //=== parse the uploaded domain ( for __.list and __vis.list )
        pDomainList->ParseDomainList(pDomainData, dwDomainSize);

        //===  store transfer target
        arc.Store(TrgToString(trg));

        //=== store number of domains ( including domain list ! )
        arc.Store((long)(pDomainList->GetSize() + 1));
    }

    //=== store domain to archive
    try
    {
        //=== store domain name
        arc.Store(sDomain);

        //=== store domain size
        arc.Store((long)dwDomainSize);

        //=== store domain data
        arc.Store((BYTE*)pDomainData, dwDomainSize);
    }
    catch(BAR_Error e)
    {
        Throw_FatalError(string("storing domain failed")
                         + ", domain=" + sDomain
                         + ", target=" + TrgToString(trg));
    }

    //=== unaccess and destroy domain data
    ::SafeArrayUnaccessData(psa);
    ::SafeArrayDestroy(psa);
    
    //=== finished
}

//=============================================================================*
//
// backup all domains belonging to the specified transfer target
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::RestoreDomains(BAR_Callback& callback,
                                    BAR_ArcRestore& arc,
                                    BAR_CSCOnlineProxy& proxy)
{
    //=== restore transfer target
    string sTrg;
    arc.Restore(sTrg);
    CSC_TRANSFER_TARGET trg;
    if ( sTrg == TrgToString(VisualDomains) )
    {
        trg = VisualDomains;
    }
    else if ( sTrg == TrgToString(ProgramDomains) )
    {
        trg = ProgramDomains;
    }
    else
    {
        Throw_FatalError("invalid transfer target");
    }

    //=== restore number of domains
    long lNoOfDomains = 0;
    arc.Restore(lNoOfDomains);

    BAR_TRACE2(1, "restore %ld %s domains",  lNoOfDomains, sTrg.c_str());

    //=== restore domains
    for( long l = 0; l < lNoOfDomains; l++ )
    {
        RestoreDomain(arc, proxy, trg);
        CheckIfCancelled(callback);
    }

    //=== finished
}

//=============================================================================*
void BAR_Config::RestoreDomain(BAR_ArcRestore& arc,
                                   BAR_CSCOnlineProxy& proxy,
                                   CSC_TRANSFER_TARGET trg)
//=============================================================================*
{   
    //=== restore domain name
    string sDomainName;
    arc.Restore(sDomainName);

    BAR_TRACE2(3, "restore domain, trg=%s, name=%s", 
                   TrgToString(trg).c_str(),
                   sDomainName.c_str());

    //=== restore domain size
    DWORD dwDomainSize = 0;
    arc.Restore((long&)dwDomainSize);

    //=== create safearray
    SAFEARRAY* psaDomainContent = ::SafeArrayCreateVector(VT_UI1,
                                                          0,
                                                          dwDomainSize);
    if (psaDomainContent == NULL)
    {
        Throw_FatalError("could not create safe array");
    }
    
    //=== access safearray data
    LPBYTE pDomainContent = 0;
    HRESULT hr = ::SafeArrayAccessData(psaDomainContent,
                                      (LPVOID FAR*)&pDomainContent);
    if ( FAILED(hr) ) 
    {
        ::SafeArrayDestroy(psaDomainContent);
        Throw_FatalError(string("could not access safe array data")
                         + HRToString(hr));
    }

    //=== restore domain data from archive
    arc.Restore(pDomainContent, dwDomainSize);

    //=== unaccess safe array data
    ::SafeArrayUnaccessData(psaDomainContent);

    //=== restore domain data to control
    proxy.DownloadDomain(trg, sDomainName.c_str(), psaDomainContent);

    //=== destroy domain data
    ::SafeArrayDestroy(psaDomainContent);
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckIfCancelled(BAR_Callback& callback)
{
    if ( callback.IsCancelRequested() )
    {
        Throw_Cancelled(); 
    }
}

//=============================================================================*
//
// compare version of source control with version of target control
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckControlVersion(BAR_Callback& callback,
                                     const string& sSourceControlVersion,
                                     BAR_CSCOnlineProxy& proxy)
{
    BAR_TRACE(1, "check control version");
    BAR_TRACE1(2, "stored control version=%s", sSourceControlVersion.c_str());

    if ( sSourceControlVersion == "" )
    {
        Warning_UnknownSourceControlVersion(callback);
    }

    //=== read version of target control
    string sTargetControlVersion = "";
    try 
    {
        sTargetControlVersion = proxy.ReadControlVersion();
    }
    catch(BAR_Error e)
    {
        BAR_TRACE2(2,
                   "catched bar_error: %s, %s",
                   e.GetMessage().c_str(),
                   e.GetDetails());
        Warning_CanNotReadControlVersion(callback);
    }
 
    //=== compare
    if ( sSourceControlVersion == "" )
    {
        if ( ! callback.IsOverwriteAllowed("The version of the original control is unknown."
                                           "Maybe this configuration will not run properly if restored."
                                           "Do you want to restore anyway ?") )
        {
            Throw_WrongControlVersion("UNKNOWN");
        }

    }
    else if ( sTargetControlVersion == "" )
    {
        if ( ! callback.IsOverwriteAllowed("The version of the target control is unknown."
                                           "Maybe this configuration will not run properly if restored."
                                           "Do you want to restore anyway ?") )
        {
            Throw_WrongControlVersion(sSourceControlVersion);
        }

    }
    else if ( sSourceControlVersion != sTargetControlVersion )
    {
        if ( ! callback.IsOverwriteAllowed("The original control and target control have different versions."
                                           "Maybe this configuration will not run properly if restored."
                                           "Do you want to restore anyway ?") )
        {                                     
            Throw_WrongControlVersion(sSourceControlVersion);
        }
    }
}

//=============================================================================*
//
// get the name of the domain list file for a transfer target
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
string BAR_Config::GetDomainListName(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    if ( trg == ProgramDomains )
        return m_sProgramDomainListName;
    else if ( trg == VisualDomains )
        return m_sVisualDomainListName;
    else
        return "";
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_InvalidDomainContent(const string& sDetails)
{
    throw BAR_Error(ERR_0200, 
                    L"uploaded domain has invalid safe array format",
                    ToWS(sDetails));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_FatalError(const string& sDetails)
{
    throw BAR_Error(ERR_0201, L"fatal error", ToWS(sDetails));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_WrongControlName(const string& sSourceControlName)
{
    throw BAR_Error(ERR_0202, L"wrong name for target control",
                              wstring(L"backup may only be restored "
                                      L"to a control named ")
                              + ToWS(sSourceControlName));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_InvalidPassword()
{
    throw BAR_Error(ERR_0203, L"invalid password");
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_WrongControlVersion(const string& sSrcConVersion)
{
    throw BAR_Error(ERR_0204, L"wrong version of target control",
                              wstring(L"backup may only be restored "
                                      L"to a control of version ")
                              + ToWS(sSrcConVersion));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_DifferentProject()
{
    throw BAR_Error(ERR_0205, L"actual control configuration belongs to a "
                              L"different project");
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_Cancelled()
{
    throw BAR_Error(ERR_0206, L"cancelled by user");
}


//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Warning_CanNotReadControlVersion(BAR_Callback& callback)
{
    callback.SendWarning(BAR_Warning(WRN_0207,
                                     L"could not read version of control"));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Warning_UnknownSourceControlVersion(BAR_Callback& callback)
{
    callback.SendWarning(BAR_Warning(WRN_0208,
                                     L"version of original control is unknown"));
}

//=============================================================================*
void BAR_Config::Throw_InvalidArchiveType(const string& sArcType)
//=============================================================================*
{
    throw BAR_Error(ERR_0209, L"invalid archive format",
                              wstring(L"format=") + ToWS(sArcType) +
                              wstring(L", expected=") + ToWS(m_sArcType)
                              );
}

//=============================================================================*
//=============================================================================*
//=============================================================================*
// OLD IMPLEMENTATION, TO BE REMOVED
//=============================================================================*
//=============================================================================*
//=============================================================================*

/*
const string BAR_Config::m_sProgramDomainListName = "__.list";
const string BAR_Config::m_sVisualDomainListName  = "__vis.list";
*/
/*
//=============================================================================*
//
// performing a configuration backup
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::DoBackup(const string& sRemoteHost,
                          const string& sPassword,
                          const string& sArcFile,
                          BAR_Callback& callback,
                          bool bUseOwnTlb)
{
    BAR_TRACE(2, "initializing com");
    HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) ) 
    {
        Throw_FatalError(string("COM Initialization failed, ")
                         + HRToString(hr));
    }
    CheckIfCancelled(callback);

    //=== 1. login on online service
    BAR_TRACE(1, "connecting to control");
    BAR_CSCOnlineProxy proxy(callback, bUseOwnTlb);

    proxy.Login(sRemoteHost);
    CheckIfCancelled(callback);


    //=== 2. open backup archive
    BAR_TRACE(1, "opening archive");
    BAR_ArcConfigBackup arc(callback);
    arc.Open(sArcFile);
    CheckIfCancelled(callback);

    ULONG ulBytesWritten = 0;

    //=== 3. backup visual domains
    BackupDomains(callback, arc, proxy, VisualDomains, ulBytesWritten);
    CheckIfCancelled(callback);

    //=== 4. backup program domains
    BackupDomains(callback, arc, proxy, ProgramDomains, ulBytesWritten);
    CheckIfCancelled(callback);

    //=== 5. backup attributes
    BackupAttributes(callback, arc, proxy, sPassword, sRemoteHost);
    CheckIfCancelled(callback);

    //=== 6. close backup archive
    BAR_TRACE(1, "closing archive");
    arc.Close();
    CheckIfCancelled(callback);

    //=== 7. logout from online service
    BAR_TRACE(1, "unconnect from control");
    proxy.Logout();

    BAR_TRACE(2, "uninitializing com");
    ::CoUninitialize();
}

//=============================================================================*
//
// performing a configuration restore
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::DoRestore(const string& sRemoteHost, 
                           const string& sPassword,
                           const string& sArcFile,
                           BAR_Callback& callback,
                           bool bUseOwnTlb)
{
    BAR_TRACE(2, "initializing com");
    HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) ) 
    {
        Throw_FatalError(string("COM Initialization failed, ")
                         + HRToString(hr));
    }
    CheckIfCancelled(callback);

    //=== 1. open restore archive
    BAR_TRACE(1, "open archive");
    BAR_ArcConfigRestore arc(callback);
    arc.Open(sArcFile);
    CheckIfCancelled(callback);

    //=== SEE HACK BELOW FOR STRANGE NUMERATION

    //=== 4. login on online service
    BAR_TRACE(1, "connect to control");
    BAR_CSCOnlineProxy proxy(callback, bUseOwnTlb);
    proxy.Login(sRemoteHost);
    CheckIfCancelled(callback);

    //=== HACK BEGIN
    //=== the natural place to check password and control name
    //=== would be after opening the archive and before
    //=== logging in to proxy
    //=== but under WIN95 it happens, that CoCreateInstanceEx
    //=== in proxy.Login() ( creating commissioning management )
    //=== fails with HRESULT 0x800706ba if any IStorage pointer in
    //=== the archive has been created
    //=== ( but pointer to root storage doesnt matter - probably the
    //=== use of ANY storage COM-interface causes the problem )
    //=== to avoid this situation password and control name
    //=== are checked after logging in.
    //=== since if the first IStorage-use follows the login
    //=== then no problem occurs
    //=== under WinNT this problem does not occur !!!
    //=== THIS MAY BECOME A SERIOUS PROBLEM IF PATCHING
    //=== CONFIGURATION DATA WITH A CONTROL NAME SHALL BE ADDED !

    //=== 2. check password
    CheckPassword(arc, sPassword);
    CheckIfCancelled(callback);

    //=== 3. check control name
    CheckControlName(arc, sRemoteHost);
    CheckIfCancelled(callback);

    //=== HACK END

    //=== 5. compare control versions
    CheckControlVersion(callback, arc, proxy);
    CheckIfCancelled(callback);

    //=== 6. check project guid
    CheckProjectGUID(arc, proxy, callback);
    CheckIfCancelled(callback);

    //=== 7. start online change
    BAR_TRACE(1, "start online change");
    proxy.StartOnlineChange();
    CheckIfCancelled(callback);

    //=== 8. delete all domains
    BAR_TRACE(1, "delete configuration");
    proxy.DeleteAllDomains();    
    CheckIfCancelled(callback);

    //=== 9. restore visual domains
    RestoreDomains(callback, arc, proxy, VisualDomains);
    CheckIfCancelled(callback);

    //=== 10. restore program domains
    RestoreDomains(callback, arc, proxy, ProgramDomains);
    CheckIfCancelled(callback);

    //=== 11. commit online change
    BAR_TRACE(1, "commit online change");
    proxy.CommitOnlineChange(arc.GetAttr("PrjGUID"),
                             arc.GetAttr("DLoadGUID"));
    CheckIfCancelled(callback);

    //=== 12. logout from online service
    BAR_TRACE(1, "unconnect from control");
    proxy.Logout();
    CheckIfCancelled(callback);

    //=== 13. close backup archive
    BAR_TRACE(1, "close archive");
    arc.Close();

    ::CoUninitialize();
    BAR_TRACE(2, "uninitializing com");
}

//=============================================================================*
//
// backup all domains belonging to the specified transfer target
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::BackupDomains(BAR_Callback& callback,
                               BAR_ArcConfigBackup& arc,
                               BAR_CSCOnlineProxy& proxy,                               
                               CSC_TRANSFER_TARGET trg,
                               ULONG& ulBytesWritten)
{
    BAR_TRACE1(1, "backup %s domains",  TrgToString(trg).c_str());

    //=== backup domain list and parse it
    BAR_DomainList domainList(trg);
    BackupDomain(arc, 
                 proxy,
                 trg,
                 GetDomainListName(trg),
                 ulBytesWritten,
                 &domainList);

    //=== backup each listed domain
    for( int i = 0; i < domainList.GetSize(); i++ )
    {
        BackupDomain(arc,
                     proxy, 
                     trg,
                     domainList.GetDomainName(i),
                     ulBytesWritten);
        CheckIfCancelled(callback);
    }

    //=== finished
}

//=============================================================================*
void BAR_Config::BackupDomain(BAR_ArcConfigBackup& arc,
                              BAR_CSCOnlineProxy& proxy,
                              CSC_TRANSFER_TARGET trg, 
                              const string& sDomain,
                              ULONG& ulBytesWritten,
                              BAR_DomainList* pDomainList
                             )
//=============================================================================*
{   
    const string sDetails = string("trg=") + TrgToString(trg)
                          + ", domain=" + sDomain;
    BAR_TRACE1(3, "backup domain, %s", sDetails.c_str());

    //=== upload domain
    SAFEARRAY* psa;
    proxy.UploadDomain(trg, sDomain, psa);

    //=== check format of safearray
    if (    ( psa->cDims != 1) 
         || ( psa->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
         || ( psa->cbElements != sizeof(BYTE))
         || ( psa->rgsabound[0].lLbound != 0)
       )
        {
            ::SafeArrayDestroy(psa);
            Throw_InvalidDomainContent(sDetails);
        }
    DWORD dwDomainSize = (psa)->rgsabound[0].cElements;

   //=== access domain data
    char* pDomainData;
    HRESULT hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
    if ( FAILED(hr) )
    {
        ::SafeArrayDestroy(psa);
        Throw_FatalError(string("could not access data of uploaded domain, ")
                         + sDetails);
    }
 
    //=== if a domain list object is supplied,
    //=== parse the uploaded domain ( for __.list and __vis.list )
    if ( pDomainList != NULL )
    {
        pDomainList->ParseDomainList(pDomainData, dwDomainSize);
    }
    
    //=== store domain to archive
     arc.StoreDomain(trg, 
                     sDomain,
                     pDomainData,
                     dwDomainSize,
                     ulBytesWritten);

    //=== unaccess and destroy domain data
    ::SafeArrayUnaccessData(psa);
    ::SafeArrayDestroy(psa);
    
    //=== finished
}

//=============================================================================*
void BAR_Config::BackupAttributes(BAR_Callback& callback,
                                  BAR_ArcConfigBackup& arc,
                                  BAR_CSCOnlineProxy& proxy,
                                  const string& sPassword,
                                  const string& sControlHost)
//=============================================================================*
{   
    //=== backup password
    BAR_TRACE(1, "backup password");
    arc.StoreAttr("Password", sPassword.c_str());

    //=== backup project guid
    BAR_TRACE(1, "backup project guid");
    CComBSTR strPrjGUID = proxy.ReadDLAttr(ProgramDomains, "PrjGUID");
    arc.StoreAttr("PrjGUID", strPrjGUID);

    //=== backup download guid
    BAR_TRACE(1, "backup download guid");
    CComBSTR strDLoadGUID = proxy.ReadDLAttr(ProgramDomains, "DLoadGUID");
    arc.StoreAttr("DLoadGUID", strDLoadGUID);

    //=== backup control version
    BAR_TRACE(1, "backup control version");
    try 
    {
        CComBSTR strControlVersion = proxy.ReadControlVersion().c_str();
        arc.StoreAttr("ControlVersion", strControlVersion);
    }
    catch(BAR_Error e)
    {
        BAR_TRACE2(2,
                   "catched bar_error: %s, %s",
                   e.GetMessage().c_str(),
                   e.GetDetails());
        arc.StoreAttr("ControlVersion", "");
        Warning_CanNotReadControlVersion(callback);
    }

    //=== backup control host name
    BAR_TRACE(1, "backup control host name");
    arc.StoreAttr("ControlHost", GetUpper(sControlHost).c_str());

}

//=============================================================================*
//
// restore domain all domain belonging to the specified transfer target
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::RestoreDomains(BAR_Callback& callback,
                                BAR_ArcConfigRestore& arc,
                                BAR_CSCOnlineProxy& proxy,
                                CSC_TRANSFER_TARGET trg)
{
    BAR_TRACE1(1, "restore %s domains",  TrgToString(trg).c_str());

    vector<CComBSTR> vecDomainNodes;
    arc.GetDomainNodes(trg, vecDomainNodes);
    for( vector<CComBSTR>::const_iterator p = vecDomainNodes.begin();
         p != vecDomainNodes.end();
         ++p
       )
    {
        RestoreDomain(arc, proxy, trg, *p);
        CheckIfCancelled(callback);
    }
}

//=============================================================================*
//
// restore a single domain
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::RestoreDomain(BAR_ArcConfigRestore& arc,
                               BAR_CSCOnlineProxy& proxy,
                               CSC_TRANSFER_TARGET trg, 
                               CComBSTR strDomainNode)
{
    BAR_WTRACE2(3, L"restore domain, trg=%s, node=%s", 
                   ToWS(TrgToString(trg)).c_str(),
                   strDomainNode.m_str);

    //=== read domain data from archive
    CComBSTR strDomainName;
    SAFEARRAY* psaDomainContent = NULL;
    arc.GetDomain(trg, strDomainNode, strDomainName, &psaDomainContent);

    //=== restore domain data to control
    proxy.DownloadDomain(trg, strDomainName, psaDomainContent);

    //=== destroy domain data
    ::SafeArrayDestroy(psaDomainContent);
}

//=============================================================================*
//
// get the name of the domain list file for a transfer target
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
string BAR_Config::GetDomainListName(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{
    if ( trg == ProgramDomains )
        return m_sProgramDomainListName;
    else if ( trg == VisualDomains )
        return m_sVisualDomainListName;
    else
        return "";
}

//=============================================================================*
//
// compare password in archive with password passed as command line argument
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckPassword(BAR_ArcConfigRestore& arc, 
                               const string& sPassword)
{
    BAR_TRACE(1, "check password");
    //=== read required password from archive
    wstring wsReqPassword = arc.GetAttr("Password").m_str;
    
    //=== if required password is empty
    //=== this means no password is required
    if ( wsReqPassword.empty() )
    {
        return;
    }
    //=== otherwise compare
    else if ( ToWS(sPassword) != wsReqPassword )
    {
        Throw_WrongPassword();
    }
}


//=============================================================================*
//
// compare name of source control with name of target control
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckControlName(BAR_ArcConfigRestore& arc, 
                                  const string& sTargetControlName)
{
    BAR_TRACE(1, "check control name");
    //=== read original control name from archive
    wstring wsSourceControlName = arc.GetAttr("ControlHost");

    //=== compare control names ( case insensitive )
    if ( wsSourceControlName != ToWS(GetUpper(sTargetControlName)) )
    {
        Throw_WrongControlName(wsSourceControlName);
    }

}

//=============================================================================*
//
// compare version of source control with version of target control
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckControlVersion(BAR_Callback& callback,
                                     BAR_ArcConfigRestore& arc,
                                     BAR_CSCOnlineProxy& proxy)
{
    BAR_TRACE(1, "check control version");
    //=== read original control version from archive
    wstring wsSourceControlVersion = arc.GetAttr("ControlVersion").m_str;
    if ( wsSourceControlVersion == L"" )
    {
        Warning_UnknownSourceControlVersion(callback);
    }

    //=== read version of target control
    wstring wsTargetControlVersion = L"";
    try 
    {
        wsTargetControlVersion = ToWS(proxy.ReadControlVersion());
    }
    catch(BAR_Error e)
    {
        BAR_TRACE2(2,
                   "catched bar_error: %s, %s",
                   e.GetMessage().c_str(),
                   e.GetDetails());
        Warning_CanNotReadControlVersion(callback);
    }
 
    //=== compare
    if ( wsSourceControlVersion == L"" )
    {
        if ( ! callback.IsOverwriteAllowed("The version of the original control is unknown."
                                           "Maybe this configuration will not run properly if restored."
                                           "Do you want to restore anyway ?") )
        {
            Throw_WrongControlVersion(L"UNKNOWN");
        }

    }
    else if ( wsTargetControlVersion == L"" )
    {
        if ( ! callback.IsOverwriteAllowed("The version of the target control is unknown."
                                           "Maybe this configuration will not run properly if restored."
                                           "Do you want to restore anyway ?") )
        {
            Throw_WrongControlVersion(wsSourceControlVersion);
        }

    }
    else if ( wsSourceControlVersion != wsTargetControlVersion )
    {
        if ( callback.IsOverwriteAllowed("The original control and target control have different versions."
                                         "Maybe this configuration will not run properly if restored."
                                         "Do you want to restore anyway ?") )
        {                                     
            Throw_WrongControlVersion(wsSourceControlVersion);
        }
    }
}

//=============================================================================*
//
// compare project guid of source config with project guid of target project
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckProjectGUID(BAR_ArcConfigRestore& arc,
                                  BAR_CSCOnlineProxy& proxy,
                                  BAR_Callback& callback)
{
    BAR_TRACE(1, "check project guid");
    //=== read original project guid from archive
    const wstring wsOriginalPrjGUID = arc.GetAttr("PrjGUID").m_str;;

    //=== read version of target control
    const wstring wsActualPrjGUID = proxy.ReadDLAttr(ProgramDomains, "PrjGUID");

    //=== compare
    if ( wsOriginalPrjGUID != wsActualPrjGUID )
    {
        //=== if guids differ, ask for permission to overwrite actual project
        const string sQuestion = "Actual control configuration "
                                 "belongs to a different project. "
                                 " Do you want to overwrite it ?";
        if ( ! callback.IsOverwriteAllowed(sQuestion) )
        {
            Throw_DifferentProject();
        }
    }
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::CheckIfCancelled(BAR_Callback& callback)
{
    if ( callback.IsCancelRequested() )
    {
        Throw_Cancelled(); 
    }
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_InvalidDomainContent(const string& sDetails)
{
    throw BAR_Error(ERR_0200, 
                    L"uploaded domain has invalid safe array format",
                    ToWS(sDetails));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_FatalError(const string& sDetails)
{
    throw BAR_Error(ERR_0201, L"fatal error", ToWS(sDetails));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_WrongControlName(const wstring& wsSourceControlName)
{
    throw BAR_Error(ERR_0202, L"wrong name for target control",
                              wstring(L"backup may only be restored "
                                      L"to a control named ")
                              + wsSourceControlName);
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_WrongPassword()
{
    throw BAR_Error(ERR_0203, L"wrong password");
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_WrongControlVersion(const wstring& wsSrcConVersion)
{
    throw BAR_Error(ERR_0204, L"wrong version of target control",
                              wstring(L"backup may only be restored "
                                      L"to a control of version ")
                              + wsSrcConVersion);
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_DifferentProject()
{
    throw BAR_Error(ERR_0205, L"actual control configuration belongs to a "
                              L"different project");
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Throw_Cancelled()
{
    throw BAR_Error(ERR_0206, L"cancelled by user");
}


//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Warning_CanNotReadControlVersion(BAR_Callback& callback)
{
    callback.SendWarning(BAR_Warning(WRN_0207,
                                     L"could not read version of control"));
}

//=============================================================================*
//
// 
//
// @param           
// @return          
// @exception       
//
//=============================================================================*
void BAR_Config::Warning_UnknownSourceControlVersion(BAR_Callback& callback)
{
    callback.SendWarning(BAR_Warning(WRN_0208,
                                     L"version of original control is unknown"));
}
*/

/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Config.cpp $
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
 * User: Jd           Date: 23.07.02   Time: 16:12
 * Updated in $/4Control/COM/softing/fc/AddOns/Softing/4CPCTarget/CBAR/BAR_LIB
 * Fixed some mistakes/changes in V2.0 and added flag -all to backup the
 * gen folder too.
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 24.09.01   Time: 12:01
 * Updated in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * Adapted to new CSC_Online.
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/

// ***************************  E O F  ******************************
// $Workfile: BAR_Config.cpp $
