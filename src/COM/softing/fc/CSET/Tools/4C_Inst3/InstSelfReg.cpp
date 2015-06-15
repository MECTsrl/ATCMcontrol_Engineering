// InstSelfReg.cpp: Implementierung der Klasse CInstSelfReg.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "InstSelfReg.h"

#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



FCINST_API LONG FCINST_DECL SelfregDLL(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    MB("SelfregDLL", MB_OK);
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO			siStartInfo;
    char szCmd[MAX_PATH] = "regsvr32  -s  ";

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW; 

    lstrcat(szCmd, "\"");
    lstrcat(szCmd, lpszPath);
    lstrcat(szCmd, lpszFile);
    lstrcat(szCmd, "\"");

    
    CString str = szCmd;
    MB(str, MB_OK);


	if (CreateProcess(NULL, szCmd, NULL, NULL, TRUE, 
                      0, NULL, NULL, &siStartInfo, &piProcInfo))	
    {
        DWORD dwReturnCode;
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
        GetExitCodeProcess(piProcInfo.hProcess, &dwReturnCode);

        /*if (dwReturnCode != 0)
        {
            CString strCaption;
            strCaption.LoadString(IDS_MSG_BOX_TITLE);
            CString strText;
            strText.FormatMessage(IDS_ERR_SELFREG_ERROR, (CString)lpszPath+(CString)lpszFile, dwReturnCode);
            ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
        }*/

	    CloseHandle(piProcInfo.hThread);
	    CloseHandle(piProcInfo.hProcess);
        MB("SelfregDLL 0", MB_OK);

        return 0;
    }

    CString strCaption;
    strCaption.LoadString(IDS_MSG_BOX_TITLE);
    CString strText;
    strText.FormatMessage(IDS_ERR_SELFREG_FAILED, (CString)lpszPath+(CString)lpszFile);
    ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
    
    return -1;
}



FCINST_API LONG FCINST_DECL UnregDLL(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    MB("UnregDLL", MB_OK);

    PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO			siStartInfo;
    char szCmd[MAX_PATH] = "regsvr32  -s -u ";

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW; 

    lstrcat(szCmd, "\"");
    lstrcat(szCmd, lpszPath);
    lstrcat(szCmd, lpszFile);
    lstrcat(szCmd, "\"");

    
    CString str = szCmd;
    MB(str, MB_OK);


	if (CreateProcess(NULL, szCmd, NULL, NULL, TRUE, 
                      0, NULL, NULL, &siStartInfo, &piProcInfo))	
    {
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
	    CloseHandle(piProcInfo.hThread);
	    CloseHandle(piProcInfo.hProcess);
            MB("UnregDLL 0", MB_OK);

        return 0;
    }
    MB("UnregDLL -1", MB_OK);

    return -1;
}



FCINST_API LONG FCINST_DECL SelfregExe(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    MB("SelfregExe", MB_OK);

	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO			siStartInfo;
    char szCmd[MAX_PATH];

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW; 

    lstrcpy(szCmd, "\"");
    lstrcat(szCmd, lpszPath);
    lstrcat(szCmd, lpszFile);
    lstrcat(szCmd, "\"");
    lstrcat(szCmd, " -RegServer");

    CString str = szCmd;
    MB(str, MB_OK);

	if (CreateProcess(NULL, szCmd, NULL, NULL, TRUE, 
                      0, NULL, NULL, &siStartInfo, &piProcInfo))	
    {
        DWORD dwReturnCode;
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
        GetExitCodeProcess(piProcInfo.hProcess, &dwReturnCode);

        /*if (dwReturnCode!=0)
        {
            CString strCaption;
            strCaption.LoadString(IDS_MSG_BOX_TITLE);
            CString strText;
            strText.FormatMessage(IDS_ERR_SELFREG_ERROR, szCmd, dwReturnCode);
            ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
        }*/

	    CloseHandle(piProcInfo.hThread);
	    CloseHandle(piProcInfo.hProcess);
        MB("SelfregExe 0", MB_OK);
        return 0;
    }

    CString strCaption;
    strCaption.LoadString(IDS_MSG_BOX_TITLE);
    CString strText;
    strText.FormatMessage(IDS_ERR_SELFREG_FAILED, szCmd);
    ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

    return -1;
}


FCINST_API LONG FCINST_DECL UnregExe(HWND hWnd, LPSTR lpszPath, LPSTR lpszFile)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    MB("UnregExe", MB_OK);

    PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO			siStartInfo;
    char szCmd[MAX_PATH];

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.dwFlags = STARTF_USESHOWWINDOW; 

    lstrcpy(szCmd, "\"");
    lstrcat(szCmd, lpszPath);
    lstrcat(szCmd, lpszFile);
    lstrcat(szCmd, "\"");
    lstrcat(szCmd, " -UnregServer");

    
    CString str = szCmd;
    MB(str, MB_OK);


	if (CreateProcess(NULL, szCmd, NULL, NULL, TRUE, 
                      0, NULL, NULL, &siStartInfo, &piProcInfo))	
    {
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);
	    CloseHandle(piProcInfo.hThread);
	    CloseHandle(piProcInfo.hProcess);
        MB("UnregExe 0", MB_OK);

        return 0;
    }

    MB("UnregExe -1", MB_OK);

    return -1;
}
