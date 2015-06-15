// InstProfibus.cpp: Implementierung der Klasse CInstProfibus.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "InstProfibus.h"
#include "InstServices.h"

#include "winsvc.h"
#include "direct.h"

#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




FCINST_API LONG FCINST_DECL CheckForOldProfiboard(HWND hWnd, LONG showMessage)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HKEY hKey;
    LONG lRet = 0;
    
    //Check for PROFIboard V5.21 uninstall program
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PROFIBUS WinNT RTS",
        0, KEY_READ, &hKey))
    {
        OutputDebugString("4C_Inst3: CheckForOldProfiboard: PROFIBoard V5.21 uninstall program found.");
        RegCloseKey(hKey);
        lRet = -1;
    }
    
    //Check for PROFIboard V5.22 uninstall program
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\PROFIBUS Windows NT Device Driver",
        0, KEY_READ, &hKey))
    {
        OutputDebugString("4C_Inst3: CheckForOldProfiboard: PROFIBoard V5.22 uninstall program found.");
        RegCloseKey(hKey);
        lRet = -1;
    }
    
    // check service control manager if a PROFIboard service is known
    char szName[MAX_PATH];
    DWORD dwSize = sizeof(szName);
    SC_HANDLE hSC = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if (hSC)
    {
        BOOL bRet;
        bRet = GetServiceDisplayName(hSC, "Profibrd", szName, &dwSize);
        bRet |= GetServiceDisplayName(hSC, "Profiprt", szName, &dwSize);
        CloseServiceHandle(hSC);
        if (bRet)
        {
            if (ExistSrv(hWnd, "PROFIbrd") > 0)
            {
                OutputDebugString("4C_Inst3: CheckForOldProfiboard: PROFIBoard service found.");
                lRet = -1;
            }
        }
    }
    
    if (lRet==-1 && showMessage!=0)
    {
        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_PB_EXISTS);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_SETFOREGROUND|MB_TOPMOST);
    }

    return lRet;
}

FCINST_API LONG FCINST_DECL InstallRTCESrv(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    SC_HANDLE hSC;
    SC_HANDLE hSrv;
    BOOL bRet = TRUE;
    static char szPBCpl[MAX_PATH];

    OutputDebugString("InstallRTCESrv entered\n");


    // bug in PROFIBUS uninstall: it does mot delete its control panel application
    // delete it in case RTCE is not already installed on this system
    GetSystemDirectory(szPBCpl, sizeof(szPBCpl));
    lstrcat(szPBCpl, "\\pbcpl.cpl");
    DeleteFile(szPBCpl);

    hSC = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if (hSC)
    {
        OutputDebugString("OpenSCManager success\n");

        if (ExistSrv(hWnd, "PROFIprt") <= 0)
        {
            OutputDebugString("PROFIprt server does not exist\n");

            hSrv = CreateService(hSC, "PROFIprt", "PROFIBUS Protocol Driver (Softing)", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, 
                                 SERVICE_DISABLED, SERVICE_ERROR_NORMAL, 
                                 "%SystemRoot%\\System32\\drivers\\PROFIprt.sys", 
                                 "Extended base", NULL, TEXT("PROFIbrd\0"), NULL, NULL);
            if (hSrv)
            {
                CloseServiceHandle(hSrv);
            }
            else
            {
                OutputDebugString("Error creating PROFIprt server \n");
                bRet = FALSE;
            }
        }

        if (ExistSrv(hWnd, "PROFIbrd") <= 0)
        {
            OutputDebugString("PROFIbrd server does not exist\n");

            hSrv = CreateService(hSC, "PROFIbrd", "PROFIBUS Hardware Driver (Softing)", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, 
                                 SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, 
                                 "%SystemRoot%\\System32\\drivers\\PROFIbrd.sys", 
                                 "Extended base", NULL, NULL, NULL, NULL);
            if (hSrv)
            {
                CloseServiceHandle(hSrv);
            }
            else
            {
                OutputDebugString("Error creating PROFIbrd server \n");
                bRet = FALSE;
            }
        }


        if (ExistSrv(hWnd, "RTCE") <= 0)
        {
            OutputDebugString("RTCE server does not exist\n");

            hSrv = CreateService(hSC, "RTCE", "4Control: RTCE Driver", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, 
                                 SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, 
                                 "%SystemRoot%\\System32\\drivers\\rtce.sys", 
                                 "Extended base", NULL, TEXT("PROFIbrd\0"), NULL, NULL);
            if (hSrv)
            {
                CloseServiceHandle(hSrv);
            }
            else
            {
                OutputDebugString("Error creating RTCE server\n");

                bRet = FALSE;
            }
        }

       
        CloseServiceHandle(hSC);
    }

    if (bRet==TRUE)
    {
        OutputDebugString("Starting services \n");
        OutputDebugString("Starting PROFIbrd \n");
        StartSrv(hWnd, "PROFIbrd");
        OutputDebugString("Starting RTCE \n");
        StartSrv(hWnd, "RTCE");

        OutputDebugString("Services started\n");

    }

    OutputDebugString("Ending \n");

	return bRet;
}

FCINST_API LONG FCINST_DECL UninstallRTCESrv(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    SC_HANDLE hSC;
    SC_HANDLE hSrv;
    long lRet;

    hSC = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if (hSC)
    {
        hSrv = OpenService(hSC, "RTCE", DELETE);
        if(hSrv)
        {
            lRet = StopSrv(hWnd, "RTCE");
            if (lRet >= 0)
            {
                DeleteService(hSrv);
                CloseServiceHandle(hSrv);
            }

            hSrv = OpenService(hSC, "PROFIbrd", DELETE);
            if(hSrv)
            {
                lRet = StopSrv(hWnd, "PROFIbrd");
                if (lRet >= 0)
                {
                    DeleteService(hSrv);
                    CloseServiceHandle(hSrv);
                }
            }

            hSrv = OpenService(hSC, "PROFIprt", DELETE);
            if(hSrv)
            {
                lRet = StopSrv(hWnd, "PROFIprt");
                if (lRet >= 0)
                {
                    DeleteService(hSrv);
                    CloseServiceHandle(hSrv);
                }
            }
        }

        CloseServiceHandle(hSC);
    }

    return (lRet >= 0);
}