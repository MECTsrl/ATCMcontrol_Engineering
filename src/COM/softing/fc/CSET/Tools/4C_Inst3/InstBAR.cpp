// InstBAR.cpp: implementation of the InstBAR class.
//
//////////////////////////////////////////////////////////////////////





#include "stdafx.h"

#include "InstBAR.h"

#include <windows.h>
#include <lm.h>



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




typedef NET_API_STATUS (NET_API_FUNCTION *PFNNetShareGetInfo)(LPWSTR, LPWSTR, DWORD, LPBYTE*);
typedef NET_API_STATUS (NET_API_FUNCTION *PFNNetShareDel)(LPWSTR, LPWSTR, DWORD);
typedef NET_API_STATUS (NET_API_FUNCTION *PFNNetApiBufferFree)(LPVOID);
typedef NET_API_STATUS (NET_API_FUNCTION *PFNNetShareAdd)(LPWSTR, DWORD, LPBYTE, LPDWORD);



FCINST_API LONG FCINST_DECL RemoveBRShare(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HMODULE hDll = NULL;
    PFNNetApiBufferFree procNetApiBufferFree;
    PFNNetShareGetInfo procNetShareGetInfo;
    PFNNetShareDel procNetShareDel;
    NET_API_STATUS res;
    LPBYTE pBuf = NULL;
    WCHAR wszNetName[] = L"4C_BackupAndRestore$"; 

    hDll = LoadLibraryA("netapi32.dll");
    if (NULL == hDll)
    {
        ::MessageBoxA(hWnd, "Failed to remove Backup And Restore share.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }

    procNetShareGetInfo = (PFNNetShareGetInfo)GetProcAddress(hDll, "NetShareGetInfo");
    procNetShareDel = (PFNNetShareDel)GetProcAddress(hDll, "NetShareDel");
    procNetApiBufferFree = (PFNNetApiBufferFree)GetProcAddress(hDll, "NetApiBufferFree");

    if (NULL == procNetShareGetInfo ||
        NULL == procNetShareDel ||
        NULL == procNetApiBufferFree)
    {
        FreeLibrary(hDll);
     
        ::MessageBoxA(hWnd, "Failed to remove Backup And Restore share.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }

    res = procNetShareGetInfo(NULL, wszNetName, 0, &pBuf);
    if (NULL != pBuf)
    {
        procNetApiBufferFree(pBuf);

        pBuf = NULL;
    }

    if (NERR_Success == res)
    {
        res = procNetShareDel(NULL, wszNetName, 0);
    }
    
    if (NERR_Success != res)
    {
        ::MessageBoxA(hWnd, "Failed to remove Backup And Restore share.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
    }

    FreeLibrary(hDll);

    return 0;

}

FCINST_API LONG FCINST_DECL ShareBRDir(HWND hWnd, LPSTR lpszDir)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // create share for backup and restore directory
    HMODULE hDll;
    NET_API_STATUS res;
    SHARE_INFO_2 p;
    DWORD parm_err = 0;
    WCHAR wszDir[MAX_PATH]; 
    WCHAR wszNetName[] = L"4C_BackupAndRestore$"; 
    PFNNetApiBufferFree procNetApiBufferFree;
    PFNNetShareAdd procNetShareAdd;
    PFNNetShareGetInfo procNetShareGetInfo;
    PFNNetShareDel procNetShareDel;
    LPBYTE pBuf = NULL;

    hDll = LoadLibraryA("netapi32.dll");
    if (NULL == hDll)
    {
        ::MessageBoxA(hWnd, "Failed to create Backup And Restore share.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }

    procNetShareAdd = (PFNNetShareAdd)GetProcAddress(hDll, "NetShareAdd");
    procNetShareGetInfo = (PFNNetShareGetInfo)GetProcAddress(hDll, "NetShareGetInfo");
    procNetShareDel = (PFNNetShareDel)GetProcAddress(hDll, "NetShareDel");
    procNetApiBufferFree = (PFNNetApiBufferFree)GetProcAddress(hDll, "NetApiBufferFree");

    if (NULL == procNetShareAdd ||
        NULL == procNetShareGetInfo ||
        NULL == procNetShareDel ||
        NULL == procNetApiBufferFree)
    {
        FreeLibrary(hDll);

        ::MessageBoxA(hWnd, "Failed to create Backup And Restore share.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }

    MultiByteToWideChar(CP_ACP, 0, lpszDir, -1, wszDir, sizeof(wszDir) / sizeof(WCHAR));

    //
    // Fill in the SHARE_INFO_2 structure.
    //

    p.shi2_netname = (LPTSTR)wszNetName;
    p.shi2_type = STYPE_DISKTREE; // disk drive
    p.shi2_remark = (LPTSTR)L"ATCMControl Backup and Restore";
    p.shi2_permissions = 0;    
    p.shi2_max_uses = -1;
    p.shi2_current_uses = 0;    
    p.shi2_path = (LPTSTR)wszDir;
    p.shi2_passwd = NULL; // no password
   
    //
    // Call the NetShareAdd function,
    //  specifying level 2.
    //

    res = procNetShareGetInfo(NULL, wszNetName, 0, &pBuf);

    if (NULL != pBuf)
    {
        procNetApiBufferFree(pBuf);
        pBuf = NULL;
    }

    if (NERR_Success == res)
    {
        procNetShareDel(NULL, wszNetName, 0);
    }

    res=procNetShareAdd(NULL, 2, (LPBYTE)&p, &parm_err);

    if (NERR_Success != res)
    {
        ::MessageBoxA(hWnd, "Failed to create Backup And Restore share.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
    }

    FreeLibrary(hDll);

    return (LONG)res;
}
