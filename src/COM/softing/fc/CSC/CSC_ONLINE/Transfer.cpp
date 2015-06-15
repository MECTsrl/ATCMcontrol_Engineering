/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/Transfer.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Transfer.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_ONLINE/Transfer.cpp $
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
#include "Transfer.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//----------------------------- FUNCTION -------------------------------------*
/**
 * ctor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CDomainsTransfer::CDomainsTransfer()
  : m_state(idle),
    m_target(ProgramDomains),
    m_transfer(uploadDir),
    m_fltPercentDone(0.0),
    m_pStreamResource(NULL),
    m_pStreamNotify(NULL),
    m_lKey(0),
    m_ulTotalBytes(0),
    m_ulTotalFiles(0),
    m_ulCurrentBytes(0),
    m_ulCurrentFiles(0),
    m_bRunning(TRUE)
{
}

//----------------------------- FUNCTION -------------------------------------*
/**
 * dtor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CDomainsTransfer::~CDomainsTransfer()
{
}


//------------------------------------------------------------------*
/**
 * %function%
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT
    CDomainsTransfer::StartTransfer(CSC_TRANSFER_TARGET target, TransferDirection transfer,
                                 LPCTSTR pszListFile, LONG lKey,
                                 ITransfer *pResource, IFCDownload *pNotify)
{
    if ((pResource == NULL) || (pNotify == NULL) || (pszListFile == NULL)) {
        return E_INVALIDARG;
    }

	HRESULT hr;	
    // check for valid file path
    CFileStatus fStatus;
    if(!CFile::GetStatus(pszListFile,fStatus)) {
        return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);                
    }

    m_strListFile = pszListFile;
    m_target = target;
    m_transfer = transfer; 
    m_lKey = lKey;

    CComQIPtr<_FCInternalTransferProgress, &IID__FCInternalTransferProgress> pProgress = pNotify;
    if (!pProgress) {
        return E_FAIL;
    }
    
    // marshal interface pointers to worker thread:
    hr = ::CoMarshalInterThreadInterfaceInStream(IID_ITransfer, pResource, &m_pStreamResource);
    if (FAILED(hr)) {
        TRACE(_T("CDomainsTransfer::StartTransfer: Error, cannot marshal ITransfer interface to stream! hr=0x%08X\n"), hr);
        return hr;
    }
    hr = ::CoMarshalInterThreadInterfaceInStream(IID__FCInternalTransferProgress, pProgress, &m_pStreamNotify);
    if (FAILED(hr)) {
        TRACE(_T("CDomainsTransfer::StartTransfer: Error, cannot marshal _FCInternalTransferProgress interface to stream! hr=0x%08X\n"), hr);
        return hr;
    }
    
    
    m_fltPercentDone = 0.0;
    _ASSERTE(m_state == idle);    // have incomplete job hanging here??

    m_strCurrentDomain.Empty();
    FireNewState(idle);

    FireNewState(initiating);

    DWORD dwThreadID;
    HANDLE hThread = CreateThread(NULL, 0, ThreadMain, (LPVOID)this, 0, &dwThreadID);
    if (hThread == NULL)
        return E_FAIL;

    CloseHandle(hThread);
    return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
/*static*/ DWORD CDomainsTransfer::ThreadMain(LPVOID lpData)
{
    CDomainsTransfer *_this = (CDomainsTransfer *)lpData;
    DWORD retcode = _this->InitThreadInstance();
    if (retcode == 0)
        retcode = _this->Run(NULL);
    _this->ExitThreadInstance();
    delete _this;
    return retcode;
}

//----------------------------- FUNCTION -------------------------------------*
/**
 * give a chance to initialize thread data
 *
 * @param           void
 * @return          0:  continue
 *                  !=0 terminate thread
 * @exception       -
*/
/*virtual*/ DWORD
    CDomainsTransfer::InitThreadInstance()
{
    HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return 1;
    }

    // unmarshall interface pointers in this thread/apartment
    if (!m_pStreamResource) {
        TRACE(_T("CDomainsTransfer: Error, no stream to unmarshall IResource interface!\n"));
        return 1;
    }
    ITransfer    *pResource = NULL;
    hr = ::CoGetInterfaceAndReleaseStream(m_pStreamResource, IID_ITransfer, (void **) &pResource);
    if (FAILED(hr) || !pResource) {
        TRACE(_T("CDomainsTransfer: Error, cannot QI for marshalled IResource interface! hr = 0x%08X\n"), hr);
        return 1;
    }
    m_pResource = pResource;
    pResource->Release();
    
    if (!m_pStreamNotify) {
        TRACE(_T("CDomainsTransfer: Error, no stream to unmarshall _FCInternalTransferProgress interface!\n"));
        return 1;
    }
	_FCInternalTransferProgress *pNotify = NULL;
    hr = ::CoGetInterfaceAndReleaseStream(m_pStreamNotify, IID__FCInternalTransferProgress, (void **) &pNotify);
    if (FAILED(hr) || !pNotify) {
        TRACE(_T("CDomainsTransfer: Error, cannot QI for marshalled _FCInternalTransferProgress interface! hr = 0x%08X\n"), hr);
        return 1;
    }
    m_pNotify = pNotify;
    pNotify->Release();

    return 0;
}

//----------------------------- FUNCTION -------------------------------------*
/**
 * give a chance to clean up thread data
 *
 * @param           void
 * @return          void
 * @exception       -
*/
/*virtual*/ void
    CDomainsTransfer::ExitThreadInstance()
{
    m_pStreamResource = NULL;
    m_pResource.Release();
    m_pStreamNotify = NULL;
    m_pNotify.Release();
    ::CoUninitialize();
    return;
}


//----------------------------- FUNCTION -------------------------------------*
/**
 * the work horse: this is the working routine in thread
 *
 * @param           void
 * @return          0:  continue
 *                  !=0 terminate thread
 * @exception       -
*/
/*virtual*/ DWORD
    CDomainsTransfer::Run(void * /*pJobData*/)
{
    m_hrJob = S_OK;
    CString strLine;
    CString strDomainName, strFlatName;

    CStdioFile  file;
    if (!file.Open(m_strListFile, CFile::modeRead | CFile::shareDenyWrite)) {
        TRACE(_T("Transfer: Cannot open list file %s\n"), (LPCTSTR) m_strListFile);
        FireNewState(aborting);
        m_hrJob = E_FAIL;
        return 0xffff;
    }

    InitProgress();

    BOOL    bRunning = m_bRunning;

    while (bRunning && file.ReadString(strLine))
    {
        try
        {
            HRESULT hr = ParseListLine(strLine, strDomainName, strFlatName);
            if (FAILED(hr)) {
                m_hrJob = hr;
                bRunning = FALSE;
                break;
                //                    continue;
            }
            
            // actually transfer single file
            m_strCurrentDomain = strDomainName;
            switch (m_transfer) {
            case uploadDir:
                // do upload of domain
                m_state = transfering_domain;
                hr = UploadDomain(strDomainName, strFlatName);
                UpdateProgress(strFlatName);
                if (FAILED(hr)) {
                    TRACE(_T("UploadDomain: cannot upload domain %s to file %s: hr=0x%08X\n"),
                        (LPCTSTR) strDomainName, (LPCTSTR) strFlatName, hr);
                    FireTransferError(hr);
                }
                break;
            case downloadDir:
                // do download of domain
                m_state = transfering_domain;
                hr = DownloadDomain(strDomainName, strFlatName);
                UpdateProgress(strFlatName);
                if (FAILED(hr)) {
                    TRACE(_T("DownloadDomain: cannot download domain %s from file %s: hr=0x%08X\n"),
                        (LPCTSTR) strDomainName, (LPCTSTR) strFlatName, hr);
                    FireTransferError(hr);
                }
                break;
            case deleteDir:
                m_state = deleted_domain;
                hr = DeleteDomain(strDomainName);
                UpdateProgress(strFlatName);
                if (FAILED(hr)) {
                    TRACE(_T("DeleteDomain: cannot delete domain %s from file %s: hr=0x%08X\n"),
                        (LPCTSTR) strDomainName, (LPCTSTR) strFlatName, hr);
                    FireTransferError(hr);
                }
                break;
            }
            // fire notification that this domain was transferred
            FireNextFile(strDomainName);
        }
        catch (CFileException e)
        {
            m_hrJob = E_FAIL;
            bRunning = FALSE;
        }
        if (bRunning) {
            bRunning = m_bRunning;
        }
        if (!bRunning) {
            FireNewState(aborting);
            TrcPrint(TRC_INTERFACE, _T("Aborting transfer: job=0x%08X\n"), long(this));
        }
    }
    
    m_fltPercentDone = 1.0f;
    file.Close();
    m_strCurrentDomain.Empty();
    if (FAILED(m_hrJob)) {
        FireNewState(aborting);
    }
    else {
        FireNewState(cleanup);
    }

    TRACE(_T("Deleting transfer job 0x%08X!\n"), (DWORD)this);
    return 0xffff;     // have to return something != NULL !!!
}


//------------------------------------------------------------------*
HRESULT CDomainsTransfer::UploadDomain(LPCTSTR pszDomainName, LPCTSTR pszFlatName)
{
    HRESULT hr;
    SAFEARRAY   *psa;

    hr = m_pResource->UploadDomain(m_lKey, m_target, CComBSTR(pszDomainName), &psa);
    if (FAILED(hr)) {
        return hr;
    }
    // check on safearrays format:
    if (   (psa->cDims != 1)
        || (psa->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
        || (psa->cbElements != sizeof(BYTE))
        || (psa->rgsabound[0].lLbound != 0))
    {
        ::SafeArrayDestroy(psa);
        return E_INVALIDARG;
    }

    // write to disk file:
    HANDLE hFile  = ::CreateFile(pszFlatName, GENERIC_WRITE, 0, NULL,
                                 CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        ::SafeArrayDestroy(psa);
        return HRESULT_FROM_WIN32(GetLastError());
    }
    
    LPBYTE pDomainData;
    hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
    if (FAILED(hr)) {
        ::CloseHandle(hFile);
        ::SafeArrayDestroy(psa);
        return hr;
    }
    
    DWORD  nBytes = 0;
    DWORD dwSize = psa->rgsabound[0].cElements;
    if (   !WriteFile(hFile, pDomainData, dwSize, &nBytes,NULL)
        || (nBytes != dwSize))
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }
    TRACE(_T("Uploaded %d bytes for domain %s to file %s\n"), nBytes, pszDomainName, pszFlatName);
    
    ::SafeArrayUnaccessData(psa);
    ::SafeArrayDestroy(psa);
    ::CloseHandle(hFile);
    return hr;
}

//------------------------------------------------------------------*
HRESULT CDomainsTransfer::DownloadDomain(LPCTSTR pszDomainName, LPCTSTR pszFlatName)
{
    HRESULT hr;
    SAFEARRAY   *psa;
    HANDLE hFile  = ::CreateFile(pszFlatName, GENERIC_READ, FILE_SHARE_READ, NULL,
                                 OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return HRESULT_FROM_WIN32(GetLastError());
    }
    
    DWORD dwSize = ::GetFileSize(hFile, NULL);
    if (dwSize >= 0x10000000) {
        CloseHandle(hFile);
        return E_FAIL;
    }
    psa = ::SafeArrayCreateVector(VT_UI1, 0, dwSize);
    if (psa == NULL) {
        CloseHandle(hFile);
        return E_OUTOFMEMORY;
    }

    LPBYTE pDomainData;
    hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
    if (FAILED(hr))
    {
        ::SafeArrayDestroy(psa);
        ::CloseHandle(hFile);
        return hr;
    }

    DWORD  nBytes = 0;
    if (   !ReadFile(hFile, pDomainData, dwSize, &nBytes, NULL)
        || (nBytes != dwSize))
    {
        ::SafeArrayUnaccessData(psa);
        ::SafeArrayDestroy(psa);
        ::CloseHandle(hFile);
        return HRESULT_FROM_WIN32(GetLastError());;
    }
    ::SafeArrayUnaccessData(psa);
    ::CloseHandle(hFile);

    hr = m_pResource->DownloadDomain(m_lKey, m_target, CComBSTR(pszDomainName), &psa);
    TRACE(_T("Downloaded %d bytes for domain %s from file %s\n"), nBytes, pszDomainName, pszFlatName);
    ::SafeArrayDestroy(psa);


    return hr;
}

//------------------------------------------------------------------*
HRESULT CDomainsTransfer::DeleteDomain(LPCTSTR pszDomainName)
{
    HRESULT hr;

    hr = m_pResource->DeleteDomain(m_lKey, m_target, CComBSTR(pszDomainName));
    TRACE(_T("Deleted domain %s\n"), pszDomainName);

    return hr;
}

//------------------------------------------------------------------*
void CDomainsTransfer::FireNewState(CSC_TRANSFER_STATE state)
{
    m_state = state;
    if (m_pNotify) {
        m_pNotify->OnTransferProgress((DWORD)this, CComBSTR(m_strCurrentDomain), state, m_fltPercentDone);
    }
}

//------------------------------------------------------------------*
void CDomainsTransfer::FireNextFile(LPCTSTR pszDomainName)
{
    m_strCurrentDomain = pszDomainName;
    if (m_pNotify) {
        m_pNotify->OnTransferProgress((DWORD) this, CComBSTR(m_strCurrentDomain), m_state, m_fltPercentDone);
    }
}

//------------------------------------------------------------------*
void CDomainsTransfer::FireTransferError(HRESULT hError)
{
    if (m_pNotify) {
        m_pNotify->OnTransferError((DWORD)this, CComBSTR(m_strCurrentDomain), m_state, hError);
    }
}

//------------------------------------------------------------------*
HRESULT CDomainsTransfer::ParseListLine(LPCTSTR pszLine, CString& strDomainName, CString& strFlatName)
{
    CString strLine(pszLine);
    LPTSTR  psz = _tcstok((LPTSTR)(LPCTSTR)strLine, _T(";"));
    if (psz == NULL) {
        return E_FAIL;
    }
    strDomainName = psz;

    psz = _tcstok(NULL, _T(";"));
    if (psz == NULL) {
        return E_FAIL;
    }
    strFlatName = psz;
    return S_OK;
}


//------------------------------------------------------------------*
/**
 * InitProgress
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CDomainsTransfer::InitProgress()
{
    m_ulTotalBytes = 0;
    m_ulTotalFiles = 0;
    m_ulCurrentBytes = 0;
    m_ulCurrentFiles = 0;

    CStdioFile  file;
    if (!file.Open(m_strListFile, CFile::modeRead | CFile::shareDenyWrite)) {
        TRACE(_T("Transfer: Cannot open list file %s\n"), (LPCTSTR) m_strListFile);
        return E_FAIL;
    }

    CString strLine;
    CString strDomainName, strFlatName;
    CFileStatus status;
    HRESULT hr = S_OK;
    try
    {
        while (file.ReadString(strLine))
        {
            HRESULT hr = ParseListLine(strLine, strDomainName, strFlatName);
            if (FAILED(hr)) {
                continue;
            }
            switch (m_transfer) {
            case downloadDir:
                if (CFile::GetStatus(strFlatName, status))
                {
                    m_ulTotalBytes += status.m_size;
                }
                m_ulTotalFiles++;
                break;
            case uploadDir:
            case deleteDir:
                m_ulTotalFiles++;
                break;
            }
        }
    }
    catch (CFileException e)
    {
        hr = E_FAIL;
    }
    file.Close();

    return hr;
}


//------------------------------------------------------------------*
/**
 * UpdateProgress
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
HRESULT
    CDomainsTransfer::UpdateProgress(LPCTSTR pszDomainName)
{
    CFileStatus status;
    switch (m_transfer) {
    case downloadDir:
        if (CFile::GetStatus(pszDomainName, status))
        {
            m_ulCurrentBytes += status.m_size;
        }
        if (m_ulCurrentBytes >= m_ulTotalBytes) {
            m_fltPercentDone = 1.0;
        }
        else
        {
            if (m_ulTotalBytes != 0) {
                m_fltPercentDone = float(m_ulCurrentBytes) / m_ulTotalBytes;
            }
            else {
                m_fltPercentDone = float((m_ulCurrentBytes != 0) ? 1.0 : 0.0);
            }
        }
        break;
    case uploadDir:
    case deleteDir:
        m_ulCurrentFiles++;
        if (m_ulCurrentFiles >= m_ulTotalFiles) {
            m_fltPercentDone = 1.0;
        }
        else
        {
            if (m_ulTotalFiles != 0) {
                m_fltPercentDone = float(m_ulCurrentFiles) / m_ulTotalFiles;
            }
            else {
                m_fltPercentDone = float((m_ulCurrentFiles != 0) ? 1.0 : 0.0);
            }
        }
        break;
    }

    return S_OK;
}

/*
*----------------------------------------------------------------------------*
*  $History: Transfer.cpp $
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
 * User: Su           Date: 10.07.01   Time: 12:27
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Revised interfaces for Version 2.0
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 15.03.01   Time: 19:40
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * New header, removed private trace files, replaced by tracea.lib
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 14.03.01   Time: 17:21
 * Created in $/4Control/COM/softing/fc/CSC/CSC_ONLINE
 * Initial rev.
 * 
 * *****************  Version 15  *****************
 * User: Ed           Date: 3.03.00    Time: 18:05
 * Updated in $/4Control/COM/softing/fc/CSC/CSC_Online
 * moved CFCCommissioningSession to the free apartement
*==
*----------------------------------------------------------------------------*
*/
