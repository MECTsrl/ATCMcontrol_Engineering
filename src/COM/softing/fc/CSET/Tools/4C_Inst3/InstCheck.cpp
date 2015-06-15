/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/InstCheck.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: InstCheck.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3/InstCheck.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
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
 *  01.03.2000  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


// InstCheck.cpp: implementation of the InstCheck class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "4C_Inst3.h"
#include "InstCheck.h"
#include "Product.h"
#include "BuildNr.h"
#include "InstRegistry.h"

#include <accctrl.h>
#include <aclapi.h>

#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





FCINST_API LONG FCINST_DECL ModifyEnvironmentVariable(HWND hWnd, BOOL bSystem, LPTSTR pszName, LPTSTR pszValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // Modifies an environment variable
    HKEY hKey;
    HKEY hKeyReg;
    LPTSTR pszRegistryKey;
    DWORD dwType = REG_EXPAND_SZ;
    DWORD dwSize;
    LONG lRet= TRUE;
    LPBYTE pValue = NULL;
    LPBYTE pRegValue = NULL;

    if(bSystem)
    {
        // Modify system environment variable
        hKey = HKEY_LOCAL_MACHINE;
        pszRegistryKey = _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
    } 
    else
    {
        // Modify user environment variable
        hKey = HKEY_CURRENT_USER;
        pszRegistryKey = _T("Environment");
    }

    // Open registry key
    if (ERROR_SUCCESS != RegOpenKeyEx(hKey, pszRegistryKey, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKeyReg))
    {
        return FALSE;
    }

    // Retrieve data
    if (ERROR_SUCCESS != RegQueryValueEx(hKeyReg, pszName, 0, &dwType, NULL, &dwSize))
    {
        return FALSE;
    }
    pRegValue = new BYTE[dwSize];
    if (ERROR_SUCCESS != RegQueryValueEx(hKeyReg, pszName, 0, &dwType, pRegValue, &dwSize))
    {
        lRet = FALSE;
    }
    else
    {
        // Compare
        TCHAR * pszRet = _tcsstr((LPCTSTR)pRegValue, pszValue);
        TCHAR * pszNewValue = new TCHAR[dwSize + _tcsclen(pszValue) + 1];
        if(pszRet == NULL)
        {
            // Add
            _tcscpy(pszNewValue, (LPCTSTR)pRegValue);
            if(pszNewValue[_tcsclen(pszNewValue) - 1] != ';')
            {
                _tcscat(pszNewValue, ";");
            }
            _tcscat(pszNewValue, pszValue);

            // Set data
            if (ERROR_SUCCESS != RegSetValueEx(hKeyReg, pszName, 0, dwType, (LPBYTE)pszNewValue, _tcsclen(pszNewValue)))
            {
                lRet = FALSE;
            }
            lRet = 2;
        }
        else
        {
            // 'pszName' already exists
            lRet = 1;
        }
        delete pszNewValue;
    }

    RegCloseKey(hKeyReg);

    delete pRegValue;

    return lRet;
}




FCINST_API LONG FCINST_DECL CheckForInstalledMFC(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    static char szPath[MAX_PATH];
    static char szFile[MAX_PATH];
    LARGE_INTEGER li;
    OSVERSIONINFO OsVersionInfo;

    GetSystemDirectory(szPath, sizeof(szPath));

    li.QuadPart = 6;
    li.QuadPart <<= 0x20;
    li.QuadPart += 8797;
    li.QuadPart <<= 0x10;
    lstrcpy(szFile, szPath);
    lstrcat(szFile, "\\msvcrt.dll");
    // must be at least 6.0.8397.0
    if (!CheckVersionNumberOfFile(szFile, li))
    {
        return -1;
    }

    li.QuadPart = 6;
    li.QuadPart <<= 0x20;
    li.QuadPart += 8168;
    li.QuadPart <<= 0x10;
    lstrcpy(szFile, szPath);
    lstrcat(szFile, "\\msvcirt.dll");
    // must be at least 6.0.8168.0
    if (!CheckVersionNumberOfFile(szFile, li))
    {
        return -1;
    }

    li.QuadPart = 6;
    li.QuadPart <<= 0x20;
    li.QuadPart += 8665;
    li.QuadPart <<= 0x10;
    lstrcpy(szFile, szPath);
    lstrcat(szFile, "\\mfc42.dll");
    // must be at least 6.0.8665.0
    if (!CheckVersionNumberOfFile(szFile, li))
    {
        return -1;
    }

    OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&OsVersionInfo);
    if (OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        lstrcpy(szFile, szPath);
        lstrcat(szFile, "\\mfc42u.dll");
        // must be at least 6.0.8665.0
        if (!CheckVersionNumberOfFile(szFile, li))
        {
            return -1;
        }
    }

    return 0;
}


FCINST_API LONG FCINST_DECL StartExecutable(HWND hWnd, LPSTR lpszPath, LPSTR lpszCmd, LPSTR lpszParam)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO			siStartInfo;

    CString strCmd;
    strCmd = _T("\"") + (CString)lpszPath +(CString)lpszCmd + _T("\"") + (CString)lpszParam;

    LPSTR lpszFullCmd = strCmd.GetBuffer(strCmd.GetLength());
    

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW; 

	if (CreateProcess(NULL, lpszFullCmd, NULL, NULL, TRUE, 
                      0, NULL, NULL, &siStartInfo, &piProcInfo))	
    {
        DWORD dwReturnCode;
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
        GetExitCodeProcess(piProcInfo.hProcess, &dwReturnCode);
        
        if (dwReturnCode!=0)
        {
            CString strCaption;
            strCaption.LoadString(IDS_MSG_BOX_TITLE);
            CString strText;
            strText.FormatMessage(IDS_ERR_EXE_FAILED, strCmd, dwReturnCode);
            ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
        }

	    CloseHandle(piProcInfo.hThread);
	    CloseHandle(piProcInfo.hProcess);

        strCmd.ReleaseBuffer();

        return 0;
    }

    strCmd.ReleaseBuffer();

    {
        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_EXE_START_FAILED, strCmd);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
    }

    return -1;
}



FCINST_API LONG FCINST_DECL ModifyFileSecurity (HWND hWnd, LPSTR lpszPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    DWORD result;
    PACL dacl = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    PSID pEveryoneSID = NULL;
    EXPLICIT_ACCESS ea[1];
    if(! AllocateAndInitializeSid( &SIDAuthWorld, 1,
        SECURITY_WORLD_RID,
        0, 0, 0, 0, 0, 0, 0,
        &pEveryoneSID) )
        return GetLastError();
    
    ea[0].grfAccessPermissions = GENERIC_ALL;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance= SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea[0].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ea[0].Trustee.pMultipleTrustee = NULL;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName  = (LPTSTR) pEveryoneSID;
    
    
    result = SetEntriesInAcl(1, ea, NULL, &dacl);
    if (result == 0)
    {
        result = SetNamedSecurityInfo(lpszPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
                                        NULL, NULL, dacl, NULL);
    }

    if (result!=0)
    {
        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_MOD_FILE_SEC, lpszPath);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
    }
    
    FreeSid(pEveryoneSID);
    AccFree(dacl);
    return result;
}




FCINST_API LONG FCINST_DECL GetProductVersionNumber(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile, LPSTR lpszVersion)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strVersion = "2.00.0000";
    LONG ret = 0;

    CString strFullFile = (CString)lpszPath + (CString)lpszFile;

    if (!GetFileVersion(strFullFile, strVersion))
    {
        ret = -1;
    }

    CString str = _T("Version Nr: ") + strVersion;
    MB(str,MB_OK);

    lpszVersion = new char[strVersion.GetLength()+1];
    lstrcpy(lpszVersion, strVersion);

    return ret;
}






BOOL CheckVersionNumber(LPCSTR pszVersion, LARGE_INTEGER liCheckVersion)
{
	LARGE_INTEGER liVersion;
	int iVal;

	liVersion.QuadPart = 0;
	iVal = 0;
	while (*pszVersion)
	{
		if ('.' == *pszVersion)
		{
			liVersion.QuadPart <<= 16;
			liVersion.QuadPart += iVal;
			iVal = 0;
		}
		else
		{
			iVal = 10 * iVal + *pszVersion - '0';
		}

		pszVersion++;
	}

	liVersion.QuadPart <<= 16;
	liVersion.QuadPart += iVal;
	return (liVersion.QuadPart >= liCheckVersion.QuadPart);
}


BOOL GetFileVersion(LPCSTR pszPath, CString & strVersion)
{
    DWORD dwSize;
    DWORD dwDummy;
    VS_FIXEDFILEINFO * pVersion;
    UINT uiSize;
    LPVOID p;

    dwSize = GetFileVersionInfoSize((LPSTR)pszPath, &dwDummy);
    if (!dwSize)
    {
        return FALSE;
    }

    p = malloc(dwSize);
    if (NULL != p)
    {
        GetFileVersionInfo((LPTSTR)pszPath, 0, dwSize, p);
        VerQueryValue(p, "\\", (LPVOID *)&pVersion, &uiSize);
        strVersion.Format("%d.%d.%d.%d",
                          HIWORD(pVersion->dwProductVersionMS),
                          LOWORD(pVersion->dwProductVersionMS),
                          HIWORD(pVersion->dwProductVersionLS),
                          LOWORD(pVersion->dwProductVersionLS));

        free(p);
    }

    return TRUE;
}


BOOL CheckVersionNumberOfFile(LPCSTR lpszPath, LARGE_INTEGER liVersion)
{
    DWORD dwSize;
    DWORD dwDummy;
    VS_FIXEDFILEINFO * pVersion;
    UINT uiSize;
    LPVOID p;
    LARGE_INTEGER liFileVersion;
    BOOL bRet = FALSE;

    dwSize = GetFileVersionInfoSize((LPSTR)lpszPath, &dwDummy);
    if (!dwSize)
    {
        return FALSE;
    }

    p = malloc(dwSize);
    if (NULL != p)
    {
        GetFileVersionInfo((LPTSTR)lpszPath, 0, dwSize, p);
        VerQueryValue(p, "\\", (LPVOID *)&pVersion, &uiSize);

        liFileVersion.QuadPart = HIWORD(pVersion->dwFileVersionMS);
	    liFileVersion.QuadPart <<= 0x10;
        liFileVersion.QuadPart += LOWORD(pVersion->dwFileVersionMS);
	    liFileVersion.QuadPart <<= 0x10;
        liFileVersion.QuadPart += HIWORD(pVersion->dwFileVersionLS);
	    liFileVersion.QuadPart <<= 0x10;
        liFileVersion.QuadPart += LOWORD(pVersion->dwFileVersionLS);
        bRet = (liFileVersion.QuadPart >= liVersion.QuadPart);
        free(p);
    }

    return bRet;
}

HRESULT GetLocalMachineName(LPTSTR pszMachineName, size_t SizeChars) 
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

/*
 *----------------------------------------------------------------------------*
 *  $History: InstCheck.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 2  *****************
 * User: Ln           Date: 27.10.06   Time: 15:19
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 16  *****************
 * User: Gen4c        Date: 25.07.03   Time: 17:27
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * version format changed
 * 
 * *****************  Version 15  *****************
 * User: Jd           Date: 16.09.02   Time: 16:36
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 14  *****************
 * User: Jd           Date: 22.05.02   Time: 15:30
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * Added TOPMOST to all message boxes, to force visibility of message
 * boxes during setup.
 * 
 * *****************  Version 13  *****************
 * User: Jd           Date: 22.05.02   Time: 14:12
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * UninstallRTCE corrected, 
 * moved all dialog boxes into foreground.
 * 
 * *****************  Version 12  *****************
 * User: Jd           Date: 28.11.01   Time: 14:39
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * Added HWnd parameter, for installer windows handle. Display message
 * boxes in error cases.
 * 
 * *****************  Version 11  *****************
 * User: Jd           Date: 20.11.01   Time: 16:13
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 10  *****************
 * User: Jd           Date: 15.11.01   Time: 16:59
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 9  *****************
 * User: Jd           Date: 15.11.01   Time: 16:02
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 8  *****************
 * User: Jd           Date: 15.11.01   Time: 14:20
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * Check if mfc.dlls have been copied, or a reboot is necessary.
 * 
 * *****************  Version 7  *****************
 * User: Jd           Date: 28.09.01   Time: 14:06
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 6  *****************
 * User: Jd           Date: 27.09.01   Time: 16:31
 * Updated in $/4Control/COM/softing/fc/CSET/Tools/4C_Inst3
 * 
 * *****************  Version 5  *****************
 * User: Jd           Date: 23.08.01   Time: 17:33
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * Added modify folder security for console users folder.
 * 
 * *****************  Version 4  *****************
 * User: Kt           Date: 1.06.01    Time: 16:57
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * set IE minimal version to 5.00.2614.0
 * 
 * *****************  Version 3  *****************
 * User: Kt           Date: 9.04.01    Time: 15:03
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added 'FindRegistryKey()'
 * changed 'CheckJavaSDK()'
 * -> check JavaSDK versions higher than 3.1
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 2.03.01    Time: 12:25
 * Updated in $/4Control/COM/softing/fc/CSET/4C_Inst3
 * added sourcesafe header
 *==
 *----------------------------------------------------------------------------*
*/