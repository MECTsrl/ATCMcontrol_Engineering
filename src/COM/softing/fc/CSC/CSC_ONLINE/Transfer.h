/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/Transfer.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Transfer.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/Transfer.h $
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

#ifndef __TRANSFER_H_
#define __TRANSFER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "CSC_Online.h"
#include "CSC_OnlineServer.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
*  class CDomainsTransfer
*
*  @see    ....
*/
class CDomainsTransfer
{
// =Construction
public:
                            CDomainsTransfer();
                                // default ctor
    virtual                 ~CDomainsTransfer();
                                // dtor

private:
                            CDomainsTransfer(CDomainsTransfer const & other);
                                // copy ctor
    CDomainsTransfer const &    operator = (CDomainsTransfer const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    typedef enum {
        uploadDir,
        downloadDir,
        deleteDir
    } TransferDirection;

    HRESULT                 StartTransfer(CSC_TRANSFER_TARGET target, TransferDirection transfer,
                                       LPCTSTR pszListFile, LONG lKey,
                                       ITransfer *pResource, IFCDownload *pNotify);
    HRESULT                 AbortTransfer()
                            {
                                m_bRunning = FALSE;
                                return S_OK;
                            }

// =Overrides
public:

protected:
// from class CWorkerThread
    virtual DWORD           InitThreadInstance();
                                // give a chance to initialize
                                // thread will terminate if call returns value != 0
    virtual void            ExitThreadInstance();
                                // give a chance to clean up
    virtual DWORD           Run(void *pJobData);
                                // where the real work is done
                                // thread will terminate if Run() returns value != 0

    static DWORD WINAPI     ThreadMain(LPVOID lpData);

// =Helpers
private:
    void                    FireNewState(CSC_TRANSFER_STATE state);
    void                    FireNextFile(LPCTSTR pszDomainName);
    void                    FireTransferError(HRESULT hError);
    HRESULT                 ParseListLine(LPCTSTR pszLine, CString& strDomainName, CString& strFlatName);
    HRESULT                 UploadDomain(LPCTSTR pszDomainName, LPCTSTR pszFlatName);
    HRESULT                 DownloadDomain(LPCTSTR pszDomainName, LPCTSTR pszFlatName);
    HRESULT                 DeleteDomain(LPCTSTR pszDomainName);
    HRESULT                 InitProgress();
    HRESULT                 UpdateProgress(LPCTSTR pszDomainName);

// =Attributes
protected:

private:
    CSC_TRANSFER_STATE      m_state;
    CSC_TRANSFER_TARGET     m_target;
    TransferDirection       m_transfer;
//    BOOL                    m_bUpload;  // TRUE: it's a upload; FALSE: it's a download
    CString                 m_strListFile;
    BOOL                    m_bRunning;

	
    // streams to marshall interfaces to worker thread
    LPSTREAM                m_pStreamResource;
    LPSTREAM                m_pStreamNotify;
    // BEWARE: marshalled interface pointer to use in worker thread!!!
    CComPtr<ITransfer>      m_pResource;
    LONG                    m_lKey;     // transfer session key as authorization!
    CComPtr<_FCInternalTransferProgress>     m_pNotify;
                                        // controlling object, route notifications via this obj

    CString                 m_strCurrentDomain;
    float                   m_fltPercentDone;
    HRESULT                 m_hrJob;
    ULONG                   m_ulTotalBytes;
    ULONG                   m_ulTotalFiles;
    ULONG                   m_ulCurrentBytes;
    ULONG                   m_ulCurrentFiles;
};

#endif // __TRANSFER_H_

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

/*
*----------------------------------------------------------------------------*
*  $History: Transfer.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 10.07.01   Time: 12:30
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 14.03.01   Time: 19:45
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
*==
*----------------------------------------------------------------------------*
*/



