/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/utilfile.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: utilfile.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/utilfile.cpp $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CMN
 *
 * =COMPONENT           UTIL
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==Utility functions for all SWKE's
 *	implementation file with file interface functions
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: utilfile.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 15.01.02   Time: 11:11
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * UTIL_TouchFile()
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 6.11.01    Time: 16:21
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * UTIL_GetLocalMachineName(): use registry entry
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:10
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * utils and baselib from V1.3
 * 
 * *****************  Version 8  *****************
 * User: Rf           Date: 26.03.99   Time: 11:11
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * UTIL_IsAbsolutePath returns S_OK for any path with a directory
 * beginning with a (back)slash
 * 
 * *****************  Version 7  *****************
 * User: Su           Date: 11/06/98   Time: 11:00a
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * UTIL_GetTempPath - get the system temp path.
 * UTIL_GetTempFileName - allocate a temp file in system temp path.
 * 
 * *****************  Version 6  *****************
 * User: Vo           Date: 12.10.98   Time: 15:13
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * UTIL_GetFileExt: Check for empty extension
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 7/09/98    Time: 2:16p
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * new function 'get guid from domainname'
 * 
 * *****************  Version 4  *****************
 * User: Vo           Date: 30.06.98   Time: 16:54
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * UTIL_GetDataTypeName
 * 
 * *****************  Version 3  *****************
 * User: Vo           Date: 9.06.98    Time: 14:12
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * New function UTIL_IsAdmin
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 6/03/98    Time: 8:12p
 * Updated in $/4Control/COM/softing/fc/CMN/UTIL
 * UTIL_GetFileExt
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 4/06/98    Time: 3:35p
 * Created in $/4Control/COM/softing/fc/CMN/UTIL
 * initial rev.
 *==
 ******************************************************************************
H<<*/


#include "stdafx.h"
//implementation file
#define UTIL_IMP
#include "assert.h"
#include "util.h"
#include "utilif.h"


//------------------------------------------------------------------*
/**
 * is absolute path.
 *
 *  uses _tsplitpath() function.
 *
 * @param           pszPath: path string
 * @return          S_OK: path is absolute
 *                  S_FALSE: path is not absolute
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_IsAbsolutePath(LPCTSTR pszPath)
{
	TCHAR szDir[_MAX_DIR+1];

	_tsplitpath(pszPath, NULL, szDir, NULL, NULL);

    if (szDir[0] == _T ('\\') || szDir[0] == _T ('/'))
    {
        return (S_OK);
    }

	return (S_FALSE);
}


//------------------------------------------------------------------*
/**
 * get file extension.
 *
 * @param           pszPath: file name incl. full path
 * @param           pszExt: extension (in/out)
 * @param           SizeChars: length of pszExt buffer in characters
 * @return          S_OK: ok
 *                  E_FAIL: error occurred
 *                  E_OUTOFMEMORY: no memory left
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetFileExt(IN LPCTSTR pszPath, LPTSTR pszExt, size_t SizeChars)
{
	CString strDir,strDrive,strFile,strExt;
	LPTSTR pszDir,pszDrive,pszFile,pszEx;

	try{
		pszDir = strDir.GetBuffer(_MAX_DIR+1);
		pszDrive = strDrive.GetBuffer(_MAX_DRIVE+1);
		pszFile = strFile.GetBuffer(_MAX_FNAME+1);
		pszEx = strExt.GetBuffer(_MAX_EXT+1);

		_tsplitpath(pszPath,pszDrive,pszDir,pszFile,pszEx);

		strDir.ReleaseBuffer();
		strDrive.ReleaseBuffer();
		strFile.ReleaseBuffer();
		strExt.ReleaseBuffer();

        if (strExt.GetLength() > 0)
        {
		    strExt = strExt.Mid(1); //skip the leading '.'
        }

		if(SizeChars <= (size_t) strExt.GetLength()) {
			return E_FAIL;
		}

		_tcscpy(pszExt,strExt);

		return S_OK;
	}
	catch(CMemoryException *e) {
		e->Delete();
		return E_OUTOFMEMORY;
	}

}

//------------------------------------------------------------------*
/**
 * get file title.
 *
 *  The file title is used in the windows explorer. It is the filename
 *  with or without extension depending on desktop display options.
 *
 * @param           pszPath: file name incl. full path
 * @param           pszTitle: file title (in/out)
 * @param           SizeChars: length of pszExt buffer in characters
 * @return          S_OK: ok
 *                  E_INVALIDARG: arguments not correct
 *                  E_FAIL: buffer pszTitle too small
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetFileTitle(IN LPCTSTR pszPath, LPTSTR pszTitle, size_t SizeChars)
{
	int i;
    CFile File;
	CString strTitle,strPath;
	
	if(!pszTitle || !pszPath){
		assert(pszTitle && pszPath);
		return E_INVALIDARG;
	}

	*pszTitle = _T('\0');
	strPath = pszPath;

    File.SetFilePath(strPath); //skip off the path
    strPath = File.GetFileName();

	for(i = strPath.GetLength()-1;i >= 0;i--)
		if(strPath[i] == _T('.')) {
			break;
		}

	if(i<0) {
		strTitle = strPath;
	}
	else {
		strTitle = strPath.Left(i);
	}

	if(SizeChars <= (size_t) strTitle.GetLength()) {
		return E_FAIL;
	}

	_tcscpy(pszTitle,strTitle);
	return S_OK;
}



//------------------------------------------------------------------*
/**
 * get module path.
 *
 *  uses GetModuleFileName().
 *
 * @param           hInstance: instance handle of module
 * @param           pszPath: module path (in/out)
 * @param           SizeChars: length of pszExt buffer in characters
 * @return          S_OK: ok
 *                  E_FAIL: buffer too small
 *                  E_OUTOFMEMORY: no memory left
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetModulePath(HINSTANCE hInstance, LPTSTR pszPath, size_t SizeChars)
{
    CString strPath;
    DWORD dwRes;
    int nPos;
	HRESULT hr;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	assert(pszPath);
	assert(SizeChars > 0);

	*pszPath = _T('\0');

	hr = S_OK;
    LPTSTR lpBuffer = new TCHAR[_MAX_PATH];
    if(!lpBuffer) {
		assert(lpBuffer);
        return E_OUTOFMEMORY;
    }

    dwRes = GetModuleFileName(hInstance,lpBuffer,_MAX_PATH);
    if(!dwRes) {
		hr = HRESULT_FROM_WIN32(GetLastError());
        delete [] lpBuffer;
        return hr;
    }        
    strPath = lpBuffer;
    delete [] lpBuffer;

    nPos = strPath.ReverseFind(_T('\\'));
    strPath = strPath.Left(nPos+1);

	if(SizeChars <= (size_t) strPath.GetLength()) {
		return E_FAIL;
	}

	_tcscpy(pszPath,strPath);
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * make machine name.
 *
 *  put machine name in braces {}.
 *  to remove braces use UTIL_StripMachineName()
 *
 * @param           pszMachineName: machine name (in/out)
 * @param           SizeChars: length of pszMachineName buffer in characters
 * @return          S_OK: ok
 *                  E_FAIL: buffer too small
 * @exception       -
 * @see             UTIL_StripMachineName()
*/
UTILIF HRESULT UTIL_MakeMachineName(LPTSTR pszMachineName, size_t SizeChars) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString strMachine = pszMachineName;

    if(strMachine.Find(UTIL_ADDR_START_DELIMITER) != 0) {
        strMachine = UTIL_ADDR_START_DELIMITER + strMachine;
    }
    if(strMachine.Find(UTIL_ADDR_END_DELIMITER) != strMachine.GetLength() - 1) {
        strMachine = strMachine + UTIL_ADDR_END_DELIMITER;
    }
    if(SizeChars < (size_t)strMachine.GetLength() + 1) {
        return(E_FAIL);
    }
    _tcsncpy(pszMachineName, strMachine, strMachine.GetLength() + 1);
    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * strip machine name.
 *
 *  Remove braces {} from machine name added by UTIL_MakeMachineName()
 *
 * @param           pszMachineName: machine name (in/out)
 * @param           SizeChars: length of pszMachineName buffer in characters
 * @return          S_OK: ok
 *                  E_FAIL: buffer too small
 * @exception       -
 * @see             UTIL_MakeMachineName()
*/
UTILIF HRESULT UTIL_StripMachineName(LPTSTR pszMachineName, size_t SizeChars) 
{ 
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString strMachine = pszMachineName;
    int iPos;
	
    if((iPos = strMachine.Find(UTIL_ADDR_START_DELIMITER)) == 0) {
        strMachine = strMachine.Mid(iPos + 1);
    }
    if(strMachine.Find(UTIL_ADDR_END_DELIMITER) 
        == (iPos = strMachine.GetLength() - 1)) {
        strMachine = strMachine.Left(iPos);
    }
    if(SizeChars < (size_t)strMachine.GetLength() + 1) {
        return(E_FAIL);
    }
    _tcsncpy(pszMachineName, strMachine, strMachine.GetLength() + 1);
    return(S_OK);
}

//------------------------------------------------------------------*
/**
 * get local machine name.
 *
 *  get local machine name from registry key:
 *  path:       "SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName"
 *  variable:   "ComputerName"
 *
 * @param           pszMachineName: local machine name (in/out)
 * @param           SizeChars: length of pszMachineName buffer in characters
 * @return          S_OK: ok
 *                  S_FALSE: variable not found
 *                  E_FAIL: registry key not found
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetLocalMachineName(LPTSTR pszMachineName, size_t SizeChars) 
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString strLocalMachine;

    // get local machine name from the following key in the registry:
    // HKLM\SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName\ComputerName

    long    lResult;
    HKEY    hKey;
    lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName"), 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        return E_FAIL;
    }

    DWORD   dwBufSize = _MAX_PATH;
    DWORD   dwType;
    lResult = ::RegQueryValueEx(hKey, _T("ComputerName"), NULL, &dwType, (unsigned char*)strLocalMachine.GetBuffer(_MAX_PATH), &dwBufSize);
    strLocalMachine.ReleaseBuffer();

    if (lResult != ERROR_SUCCESS) 
    {
        ::RegCloseKey(hKey);
        return S_FALSE;
    }
    ::RegCloseKey(hKey);

    _tcsncpy(pszMachineName, strLocalMachine, strLocalMachine.GetLength() + 1);
    return(S_OK);
}




//------------------------------------------------------------------*
/**
 * get temp path.
 *
 *  uses Win32 function GetTempPath().
 *
 * @param           pszPath:
 * @param           SizeChars: length of pszExt buffer in characters
 * @return          S_OK: ok
 *                  E_INVALIDARG: argument invalid
 *                  E_FAIL: buffer too small
 *                  E_OUTOFMEMORY: no memory left
 * @exception       -
 * @see             -
*/
UTILIF HRESULT UTIL_GetTempPath(OUT LPTSTR pszPath,size_t SizeChars)
{
    CString strHelp;
    LPTSTR pszBuffer;

    try {
        assert(pszPath);
        if(!pszPath) {
            return E_INVALIDARG;
        }

        pszBuffer = strHelp.GetBuffer(_MAX_PATH);
        assert(pszBuffer);
    
        if(!::GetTempPath(_MAX_PATH,pszBuffer)) {
            //no temp path;
            strHelp.ReleaseBuffer();
            return HRESULT_FROM_WIN32(GetLastError());
        }

        strHelp.ReleaseBuffer();

	    if(SizeChars <= (size_t) strHelp.GetLength()) {
		    return E_FAIL;
	    }

	    _tcscpy(pszPath,strHelp);

    }
    catch(CMemoryException* e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }

    return S_OK;
}

//------------------------------------------------------------------*
/**
 * get temp file name.
 *
 *  Build temporary file name using GetTempPath() and GetTempFileName():
 *  <temp_path>\<pszPrefix><Number>.tmp
 *  e.g.: pszPrefix = "4CTmpFile"
 *  "C:\Tmp\4CTmpFile128.tmp"
 *
 * @param           pszPrefix: name prefix (if empty: "4C" is used)
 * @param           pszPath: temporary file name incl path (in/out)
 * @param           SizeChars: length of pszPath buffer in characters
 * @return          S_OK: ok
 *                  E_INVALIDARG: argument invalid
 *                  E_FAIL: buffer too small
 *                  other error HRESULT from GetTempPath() or GetTempFileName()
 * @exception       -
 * @see             
*/
UTILIF HRESULT UTIL_GetTempFileName(IN LPCTSTR pszPrefix, OUT LPTSTR pszPath,IN size_t SizeChars)
{
	CString strPath;		//only path
    CString strFile;        //file
    CString strPrefix;      //for the prefix
	LPTSTR	pszBuffer;		//for temp file name
	HRESULT	hr = S_OK;			

    try {
        assert(pszPath);
        if(!pszPath) {
            return E_INVALIDARG;
        }

        if(pszPrefix) {
            strPrefix = pszPrefix;
        }
        else {
            strPrefix = UTIL_TEMPFILE_PREFIX;
        }

        pszBuffer = strPath.GetBuffer(_MAX_PATH);
        assert(pszBuffer);
    
        if(!::GetTempPath(_MAX_PATH,pszBuffer)) {
            //no temp path;
            strPath.ReleaseBuffer();
            return HRESULT_FROM_WIN32(GetLastError());
        }

        strPath.ReleaseBuffer();

        pszBuffer = strFile.GetBuffer(_MAX_PATH);
        assert(pszBuffer);

	    if(!::GetTempFileName(strPath,strPrefix,0,pszBuffer)) {
            strFile.ReleaseBuffer();
		    return HRESULT_FROM_WIN32(GetLastError());
	    }

        strFile.ReleaseBuffer();

	    if(SizeChars <= (size_t) strFile.GetLength()) {
		    return E_FAIL;
	    }

	    _tcscpy(pszPath,strFile);
    }
    catch(CMemoryException* e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    return S_OK;
}


// SIS, 15.01.02:
//------------------------------------------------------------------*
/**
 * touch file.
 *
 *  change modified time to current time.
 *
 * @param           lpszFilePath: file name incl. full path
 * @return          successful?
 * @exception       -
 * @see             -
*/
UTILIF BOOL UTIL_TouchFile(LPCTSTR lpszFilePath)
{
    // get file status
    CFileStatus tStatus;
    if(!CFile::GetStatus(lpszFilePath, tStatus))
    {
        return FALSE;
    }

    tStatus.m_mtime = CTime::GetCurrentTime();

    CFile::SetStatus(lpszFilePath, tStatus);

    return TRUE;
}

