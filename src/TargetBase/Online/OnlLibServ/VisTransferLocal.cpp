/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/VisTransferLocal.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: VisTransferLocal.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/VisTransferLocal.cpp $
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
#include "VisTransferLocal.h"
#include "CSC_OnlineErr.h"
#include "fc_tools/fc_fileIO.h"


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CVisTransferLocal
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CVisTransferLocal::CVisTransferLocal(LPCTSTR pszConfigPath)
{
    // save path to config directory
    TCHAR szTrcFile[_MAX_PATH + 1];
    ::GetModuleFileName(_Module.GetModuleInstance(), szTrcFile, _MAX_PATH);
    LPTSTR  psz = _tcsrchr(szTrcFile, _T('\\'));
    if (psz != NULL) {
        *psz = _T('\0');
    }

    m_strBasePath = szTrcFile;
	m_strConfigPath = m_strBasePath + pszConfigPath;
    FC_CreateSubDir(m_strConfigPath);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CVisTransferLocal::~CVisTransferLocal()
{
}

//------------------------------------------------------------------*
/**
 * Connect
 *
 * @param           void
 * @return          S_OK; HRESULT on error
 * @exception       -
*/
HRESULT
    CVisTransferLocal::Connect()
{
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * Connect
 *
 * @param           void
 * @return          S_OK; HRESULT on error
 * @exception       -
*/
HRESULT
    CVisTransferLocal::Close()
{
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * Connect
 *
 * @param           void
 * @return          S_OK; HRESULT on error
 * @exception       -
*/
HRESULT
    CVisTransferLocal::StartOnlineChange()
{
    return S_FALSE;
}

//------------------------------------------------------------------*
/**
 * Connect
 *
 * @param           void
 * @return          S_OK; HRESULT on error
 * @exception       -
*/
HRESULT
    CVisTransferLocal::CommitOnlineChange()
{
    return S_FALSE;
}

//------------------------------------------------------------------*
/**
 * Connect
 *
 * @param           void
 * @return          S_OK; HRESULT on error
 * @exception       -
*/
HRESULT
    CVisTransferLocal::CancelOnlineChange()
{
    return S_FALSE;
}


//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::DownloadDomain(BSTR sDomainName, /*[in]*/ SAFEARRAY/*(unsigned char)*/ **ppsaContent)
{
    HRESULT hr;
    
    // check safearray format:
    if (   ((*ppsaContent)->cDims != 1)
        || ((*ppsaContent)->fFeatures & (FADF_BSTR|FADF_UNKNOWN|FADF_DISPATCH|FADF_VARIANT))
        || ((*ppsaContent)->cbElements != sizeof(BYTE))
        || ((*ppsaContent)->rgsabound[0].lLbound != 0))
    {
        return E_INVALIDARG;
    }
    
    // check if subdirectory in Console already exists, create it if necessary!
    CString strBaseDomainName = sDomainName;
    hr = CheckDirectory(strBaseDomainName);
    if (FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("Download domain '%s' via local copy to console: CheckDirectory returned 0x%08X\n"),
            (LPCTSTR) strBaseDomainName, hr);
        return hr;
    }
    
    // open file for writing to console:
    CString strDomainName = m_strConfigPath;
    strDomainName += sDomainName;
    HANDLE hFile = ::CreateFile(strDomainName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
    SAFEARRAY   *psa = NULL;
    if (hFile == INVALID_HANDLE_VALUE)
    {
        hr = HRESULT_FROM_WIN32(::GetLastError());
        TrcPrint(TRC_ERROR, _T("Opening domain '%s' for write via local copy to console: code=0x%08X\n"),
            (LPCTSTR) strBaseDomainName, hr);
    }

    psa = *ppsaContent;
    // write with local file write:
    LPBYTE pDomainData;
    hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
    if (FAILED(hr)) {
        TrcPrint(TRC_ERROR, _T("Cannot access SafeArray while writing domain '%s': code=0x%08X\n"),
            (LPCTSTR) strDomainName, hr);
        CloseHandle(hFile);
        return hr;
    }
    
    DWORD dwBytes = psa->rgsabound[0].cElements;
    DWORD   dwWritten = 0;

    BOOL bOK = ::WriteFile(hFile, pDomainData, dwBytes, &dwWritten, NULL);
    if (!bOK)
    {
        hr = HRESULT_FROM_WIN32(::GetLastError());
        TrcPrint(TRC_ERROR, _T("Writing domain '%s' via local copy to console: code=0x%08X\n"),
            (LPCTSTR) strDomainName, hr);
    }
    ::SafeArrayUnaccessData(psa);
    CloseHandle(hFile);
    TRACE(_T("VisDomain '%s' written via local copy, %d bytes\n"), (LPCTSTR) strDomainName, dwWritten);
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::UploadDomain(BSTR sDomainName, /*[out, retval]*/ SAFEARRAY/*(unsigned char)*/ **ppsaContent)
{
    if (ppsaContent == NULL) {
        return E_POINTER;
    }
    *ppsaContent = NULL;

    HRESULT hr;
    CString strDomainName = m_strConfigPath;
    strDomainName += sDomainName;
    // open file for writing to console:
    SAFEARRAY   *psa = NULL;
    
    HANDLE hFile = ::CreateFile(strDomainName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        hr = HRESULT_FROM_WIN32(::GetLastError());
        TrcPrint(TRC_ERROR, _T("Opening domain '%s' for read via local copy from console: code=0x%08X\n"),
            (LPCTSTR) CString(sDomainName), hr);
        return hr;
    }

    DWORD dwSize = ::GetFileSize(hFile, NULL);
    if (dwSize == 0xFFFFFFFF) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        TrcPrint(TRC_ERROR, _T("Cannot filesize of domain '%s' for read via local copy: code=0x%08X\n"),
            (LPCTSTR) CString(sDomainName), hr);
        CloseHandle(hFile);
        return hr;
    }

    psa = ::SafeArrayCreateVector(VT_UI1, 0, dwSize);
    if (psa == NULL) {
        CloseHandle(hFile);
        return E_OUTOFMEMORY;
    }
    
    // read with local file read:
    LPBYTE pDomainData;
    hr = ::SafeArrayAccessData(psa, (LPVOID FAR*)&pDomainData);
    if (FAILED(hr)) {
        ::SafeArrayDestroy(psa);
        CloseHandle(hFile);
        return hr;
    }
    DWORD   dwRead = 0;
    BOOL bOK = ::ReadFile(hFile, pDomainData, dwSize, &dwRead, NULL);
    ::SafeArrayUnaccessData(psa);
    if (!bOK)
    {
        hr = HRESULT_FROM_WIN32(::GetLastError());
        TrcPrint(TRC_ERROR, _T("Reading domain '%s' via local copy from console: code=0x%08X\n"),
            (LPCTSTR) strDomainName, hr);
        ::SafeArrayDestroy(psa);
    }
    TRACE(_T("VisDomain '%s' read via local copy, %d bytes\n"), (LPCTSTR) strDomainName, dwRead);
    CloseHandle(hFile);
        
    *ppsaContent = psa;
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::DeleteAllDomains()
{
    HRESULT hr;
    
    // do a recursive delete of all directories below this finder!!
    hr = DeepDelete(_T(""));

    return S_OK;
}


//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::DeepDelete(LPCTSTR pszCurrentPath)
{
    CString strCurrDir;
    DWORD dwLen = ::GetCurrentDirectory(_MAX_PATH + 1, strCurrDir.GetBuffer(_MAX_PATH + 1));
    assert(dwLen <= _MAX_PATH);
    strCurrDir.ReleaseBuffer();

    HRESULT hr = S_OK;
    // list all files and directories in current dir:
    CString strCurrentPath = m_strConfigPath;
    strCurrentPath += pszCurrentPath;
    if (!SetCurrentDirectory(strCurrentPath)) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        TrcPrint(TRC_ERROR, _T("VisDomain: DeepDelete cannot changeDir to '%s', hr=0x%08X\n"),
            (LPCTSTR) strCurrentPath, hr);
        return hr;
    }
    TRACE(_T("VisDomain: DeepDelete in dir. '%s'...\n"), (LPCTSTR) strCurrentPath);

    
    // step 1: remove files in curr dir and build list of subdirectories:
    CStringList strDirList;
    CFileFind finder;
    BOOL bMore = finder.FindFile(_T("*"));
    while (bMore)
    {
        bMore = finder.FindNextFile();
        CString strFile = finder.GetFileName();
        if (finder.IsDirectory()) {
            // first add directories to list,
            if (   (strFile != _T("."))
                && (strFile != _T("..")))
            {
                // exclude parent dir ("..") and current dir (".")
                strDirList.AddTail(strFile);
            }
        }
        else {
            // delete this file
            BOOL    bOK = ::DeleteFile(strFile);
            TRACE(_T("VisDomain: DeepDelete removed file '%s', bOK=%d hr=0x%08X\n"),
                (LPCTSTR) strFile, bOK, bOK ? 0 : HRESULT_FROM_WIN32(::GetLastError()));
        }
    }
    finder.Close();
    
    // step 2: work thru list of directories:
    CString strDir;
    while (!strDirList.IsEmpty())
    {
        strDir = strDirList.RemoveHead();
        // recurse down this directory
        CString strPath = pszCurrentPath;
        strPath += strDir;
        hr = DeepDelete(strPath + _T("\\"));
        // delete the directory itself:
        CString strDeletePath = m_strConfigPath;
        strDeletePath += strPath;
        // rename directory first to avoid creating zombies
        BOOL bOK = MoveFile(strDeletePath, strDeletePath + _T("~"))
                 &&RemoveDirectory(strDeletePath + _T("~"));
        if (!bOK) {
            TrcPrint(TRC_ERROR, _T("VisDomain: DeepDelete removed dir '%s', bOK=%d hr=0x%08X\n"),
                (LPCTSTR) strDeletePath, bOK, bOK ? S_OK : HRESULT_FROM_WIN32(::GetLastError()));
        }
        TRACE(_T("VisDomain: DeepDelete removed directory '%s', bOK=%d hr=0x%08X\n"), (LPCTSTR) strDeletePath, bOK, bOK ? 0 : ::GetLastError());
    }
    
    if (!SetCurrentDirectory(strCurrDir)) {
        hr = HRESULT_FROM_WIN32(GetLastError());
        TrcPrint(TRC_ERROR, _T("VisDomain: DeepDelete cannot reset curr Dir to '%s', hr=0x%08X\n"),
            (LPCTSTR) strCurrDir, hr);
    }
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::DeleteDomain(BSTR sDomainName)
{
    CString strDomainName = m_strConfigPath;
    strDomainName += sDomainName;

    ::DeleteFile(strDomainName);
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::GetDomainList(/*[out, retval]*/ SAFEARRAY **ppListDomainNames)
{
    if (ppListDomainNames == NULL) {
        return E_POINTER;
    }
    *ppListDomainNames = NULL;
    return E_NOTIMPL;
}


//------------------------------------------------------------------*
HRESULT
    CVisTransferLocal::CheckDirectory(LPCTSTR pszBaseDomainName)
{
    HRESULT hr;
    // isolate directory path
    // %TBD% CD4: assume only ONE level subdirectories!!!!!!!!!!!!!!!!!!
    CString strBasePath = pszBaseDomainName;
    LPTSTR  psz = _tcsrchr(strBasePath, _T('\\'));
    if (psz != NULL)
    {
        // have a sub directory path, create it if necessary!
        *psz = _T('\0');
        CString strFullPath = m_strConfigPath;
        strFullPath += strBasePath;     // have complete path to domain
        // test if this directory already exists!
        DWORD dwAttr = ::GetFileAttributes(strFullPath);
        if (dwAttr == 0xFFFFFFFF)
        {
            // error, could be that dir does not exist at all
            hr = GetLastError();
            if (   (hr == ERROR_FILE_NOT_FOUND)
                || (hr == ERROR_PATH_NOT_FOUND))
            {
                // directory not known yet, try to create it!
                if (!CreateDirectory(strFullPath, NULL)) {
                    // cannot create it -> error
                    hr = HRESULT_FROM_WIN32(::GetLastError());
                    TrcPrint(TRC_ERROR, _T("VisDomain: CheckDirectory   CreateDirectory for dir '%s' returned last error 0x%08X!\n"),
                        (LPCTSTR) strFullPath, hr);
                    return hr;
                }
            }
            else {
                return HRESULT_FROM_WIN32(hr);
            }
        }
        else
        {
            // check attributes if it is a directory
            if ((dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                // path name is NOT a directory !!!
                return HRESULT_FROM_WIN32(ERROR_FILE_EXISTS);
            }
        }
    }
    else {
        // no subdir specified, is always ok!
        // assuming that setup has created a valid base path as virtual FTP directory !!!
        return S_OK;
    }
    return S_OK;
}

/* ---------------------------------------------------------------------------- */
