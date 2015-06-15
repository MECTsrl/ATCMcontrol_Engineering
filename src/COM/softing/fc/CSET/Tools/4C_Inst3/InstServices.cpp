// InstServices.cpp: Implementierung der Klasse CInstServices.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "InstServices.h"


#include "winsvc.h"
#include "direct.h"

#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




FCINST_API LONG FCINST_DECL ExistSrv(HWND hWnd, LPSTR lpszSrv)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    char szSrvKey[MAX_PATH] = "SYSTEM\\CurrentControlSet\\Services\\";
    LONG lRet = 0;
    HKEY hKey;
    BOOL bRun;
    
    lstrcat(szSrvKey, lpszSrv);
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSrvKey, 0, KEY_READ, &hKey))
    {
        if (IsSrvRunning(lpszSrv, &bRun))
        {
            lRet = (bRun) ? SRV_RUNNING : SRV_NOTRUNNING;
        }
        
        RegCloseKey(hKey);
    }
    
    return lRet;
}



FCINST_API LONG FCINST_DECL StartSrv(HWND hWnd, LPSTR lpszSrv)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (ExistSrv(hWnd, lpszSrv) != SRV_NOTRUNNING)
    {
        return 0;
    }

    LONG lRet = 0;
    SC_HANDLE hSC;
    SC_HANDLE hSrv;
    BOOL bError = FALSE;
    int i;
    SERVICE_STATUS ServiceStatus;
    DWORD dwCheckPoint = (DWORD)-1;
    
    hSC = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if (hSC)
    {
        hSrv = OpenService(hSC, lpszSrv, SERVICE_START | SERVICE_QUERY_STATUS);
        if(hSrv)
        {
            if (!QueryServiceStatus(hSrv, &ServiceStatus))
            {
                bError = TRUE;
            }
            
            if (!bError && ServiceStatus.dwCurrentState != SERVICE_RUNNING
                && ServiceStatus.dwCurrentState != SERVICE_PAUSED)
            {
                if (!StartService(hSrv, 0, NULL))
                {
                    bError = TRUE;
                }
                else
                {
                    i = 240;
                    do
                    {
                        
                        if (!QueryServiceStatus(hSrv, &ServiceStatus))
                        {
                            bError = TRUE;
                            break;
                        }
                        
                        if (ServiceStatus.dwCurrentState == SERVICE_RUNNING ||
                            ServiceStatus.dwCurrentState == SERVICE_PAUSED)
                        {
                            break;
                        }
                        
                        Sleep(250);
                        i--;
                        if (dwCheckPoint != ServiceStatus.dwCheckPoint)
                        {
                            i = 240 + ServiceStatus.dwWaitHint / 250; // wait 60 seconds + wait hint time
                            dwCheckPoint = ServiceStatus.dwCheckPoint;
                        }
                        
                        if (i == 0)
                        {
                            bError = TRUE;
                            break;
                        }
                    }
                    while (TRUE);
                }
            }
            else
            {
            }
            
            CloseServiceHandle(hSrv);
            
        }
        else
        {
            bError = TRUE;
        }
        
        CloseServiceHandle(hSC);
    }
    else
    {
        bError = TRUE;
    }
    
    if (bError)
    {       
        lRet = -1;
    }

	// Don't show error message if opcenum could not be started. opcenum is a shared
	// component and I don't know when it is really deinstalled. So to avoid an 
	// error message in the last setup after deinstalling we supress the error message
	// at all.
    if (bError && strcmpi(lpszSrv, "opcenum") != 0)
    {
        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SERVICE_NOT_STARTED, lpszSrv);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

    }
    else
    {
       CString msg = "Service " + (CString)lpszSrv + " started";
        MB(msg, MB_OK);
    }
    
    return lRet;
}



FCINST_API LONG FCINST_DECL StopSrv(HWND hWnd, LPSTR lpszSrv)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (ExistSrv(hWnd, lpszSrv) != SRV_RUNNING)
    {
        return 0;
    }

    LONG lRet = 0;
    int i;
    SC_HANDLE hSC;
    SC_HANDLE hSrv;
    BOOL bError = FALSE;
    SERVICE_STATUS ServiceStatus;
    DWORD dwCheckPoint = (DWORD)-1;
    hSC = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
    if (hSC)
    {
        hSrv = OpenService(hSC, lpszSrv, SERVICE_STOP | SERVICE_QUERY_STATUS);
        if(hSrv)
        {
            if (!QueryServiceStatus(hSrv, &ServiceStatus))
            {
                bError = TRUE;
            }
            
            if (!bError && ServiceStatus.dwCurrentState != SERVICE_STOPPED)
            {
                if (!ControlService(hSrv, SERVICE_CONTROL_STOP, &ServiceStatus))
                {
                    bError = TRUE;
                }
                else
                {
                    i = 240;
                    do
                    {
                        if (!QueryServiceStatus(hSrv, &ServiceStatus))
                        {
                            bError = TRUE;
                            break;
                        }
                        
                        if (ServiceStatus.dwCurrentState == SERVICE_STOPPED)
                        {
                            break;
                        }
                        
                        Sleep(250);
                        i--;
                        if (dwCheckPoint != ServiceStatus.dwCheckPoint)
                        {
                            i = 240 + ServiceStatus.dwWaitHint / 250; // wait 60 seconds + wait hint time
                            dwCheckPoint = ServiceStatus.dwCheckPoint;
                        }
                        
                        if (i == 0)
                        {
                            bError = TRUE;
                            break;
                        }
                    }
                    while (TRUE);
                }
            }
            else
            {
            }
            
            CloseServiceHandle(hSrv);
        }
        else 
        {
            bError = TRUE;
        }
        
        CloseServiceHandle(hSC);
    }
    else
    {
        bError = TRUE;
    }
    
    if (bError)
    {
        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SERVICE_NOT_STOPPED, lpszSrv);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        lRet = -1;
    }
    return lRet;
}


FCINST_API LONG FCINST_DECL AskIfStopService(HWND hWnd, LPSTR lpszSrv, LPSTR lpszName)
{
    if (ExistSrv(hWnd, lpszSrv) == SRV_RUNNING)
    {
        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_ASK_STOP_SERVICE, lpszName);
        EnableWindow(hWnd, FALSE);
        if (::MessageBox(hWnd, strText, strCaption, MB_YESNO|MB_SETFOREGROUND|MB_TOPMOST) == IDYES)
        {
            EnableWindow(hWnd, TRUE);
            return 0;
        }
        else
        {
            EnableWindow(hWnd, TRUE);
            return -1;
        }
    }
    return 0;
}






BOOL IsSrvRunning(LPSTR lpszSrv, BOOL * pbRun)
{
  BOOL bRet = FALSE;
  SC_HANDLE hSC;
  SC_HANDLE hSrv;
  SERVICE_STATUS ServiceStatus;

  hSC = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
  if (hSC)
  {
    hSrv = OpenService(hSC, lpszSrv, SERVICE_QUERY_STATUS);
    if(hSrv)
    {
      if (QueryServiceStatus(hSrv, &ServiceStatus))
      {
          *pbRun = (ServiceStatus.dwCurrentState == SERVICE_RUNNING ||
                    ServiceStatus.dwCurrentState == SERVICE_PAUSED);
          bRet = TRUE;
      }

      CloseServiceHandle(hSrv);
    }

    CloseServiceHandle(hSC);
  }

  return bRet;
}