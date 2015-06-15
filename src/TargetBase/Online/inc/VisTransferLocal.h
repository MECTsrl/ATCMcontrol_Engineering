/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/VisTransferLocal.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: VisTransferLocal.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/VisTransferLocal.h $
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

#ifndef __VISTRANSFERLOCAL_H_
#define __VISTRANSFERLOCAL_H_

//------------------------------------------------------------------*
/**
*  class CVisTransferLocal
*
*  @see    ....
*/
class CVisTransferLocal  
{
// =Construction
public:
	                        CVisTransferLocal(LPCTSTR pszMachineName);
	virtual                 ~CVisTransferLocal();

// =Access
public:

// =Operations
public:
    // Domain operations to/from Console via file copy in local filesystem:
    HRESULT                 Connect();
    HRESULT                 Close();
    HRESULT                 StartOnlineChange();
    HRESULT                 CommitOnlineChange();
    HRESULT                 CancelOnlineChange();
	HRESULT                 DownloadDomain(IN BSTR sDomainName, IN SAFEARRAY/*(unsigned char)*/ **ppContent);
	HRESULT                 UploadDomain(IN BSTR sDomainName, OUT SAFEARRAY/*(unsigned char)*/ **ppContent);
	HRESULT                 DeleteAllDomains();
	HRESULT                 DeleteDomain(IN BSTR sDomainName);
	HRESULT                 GetDomainList(OUT SAFEARRAY **ppListDomainNames);

// =Helpers
private:
    HRESULT                 DeepDelete(LPCTSTR pszCurrentPath);
    HRESULT                 CheckDirectory(LPCTSTR pszBaseDomainName);

// =Attributes
protected:

private:
    CString                 m_strBasePath;
    CString                 m_strConfigPath;
};

#endif

/* ---------------------------------------------------------------------------- */
