// InstConsole.cpp: Implementierung der Klasse CInstConsole.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "InstConsole.h"

#define COBJMACROS
#include <initguid.h>
#include <iadmw.h>
#include <iiscnfg.h>  // MD_ & IIS_MD_ #defines header file.

#include "InstServices.h"

#include "resource.h"

#include "InstRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




FCINST_API void FCINST_DECL DeleteVirtualConsoleDir(HWND hWnd)
{ 
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // IIS 5.0: deleting the "W3SVC\Parameters\Virtual Roots\/4C" key so not sufficient!
  HRESULT hr;
  IMSAdminBase *pAdmin;
  METADATA_HANDLE hWebRoot;
  CoInitialize(NULL);

  hr = CoCreateInstance(CLSID_MSAdminBase, NULL, CLSCTX_ALL, IID_IMSAdminBase, (void **)&pAdmin);
  if (SUCCEEDED(hr)) {
    hr = pAdmin->OpenKey(METADATA_MASTER_ROOT_HANDLE, L"/LM/W3SVC/1/Root", METADATA_PERMISSION_READ|METADATA_PERMISSION_WRITE, 10000, &hWebRoot);
    if (SUCCEEDED(hr)) {
      hr = pAdmin->DeleteKey(hWebRoot, L"/4C");
      pAdmin->CloseKey(hWebRoot);
    }
    pAdmin->Release();
  }
  CoUninitialize();
}



FCINST_API void FCINST_DECL CreateVirtualConsoleDir(HWND hWnd, LPSTR lpszPath)
{ 
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  // IIS 5.0: deleting the "W3SVC\Parameters\Virtual Roots\/4C" key so not sufficient!
  HRESULT hr;
  IMSAdminBase *pAdmin;
  METADATA_HANDLE hWebRoot;
  METADATA_RECORD MetaRecord;
  WCHAR szPathW[MAX_PATH];
  CoInitialize(NULL);


  hr = CoCreateInstance(CLSID_MSAdminBase, NULL, CLSCTX_ALL, IID_IMSAdminBase, (void **)&pAdmin);
  if (SUCCEEDED(hr)) {
    hr = pAdmin->OpenKey(METADATA_MASTER_ROOT_HANDLE, L"/LM/W3SVC/1/Root", METADATA_PERMISSION_READ|METADATA_PERMISSION_WRITE, 10000, &hWebRoot);
    if (SUCCEEDED(hr)) {
      LPWSTR lpsz4C = L"/4C";
      hr = pAdmin->AddKey(hWebRoot, lpsz4C);

      MultiByteToWideChar(CP_ACP, 0, IIS_CLASS_WEB_VDIR, -1, szPathW, MAX_PATH);
      MD_SET_DATA_RECORD(&MetaRecord,MD_KEY_TYPE,METADATA_INHERIT,IIS_MD_UT_SERVER,
                                     STRING_METADATA,sizeof(WCHAR)*(wcslen(szPathW)+1),szPathW);
      hr = pAdmin->SetData(hWebRoot, lpsz4C, &MetaRecord);

      MultiByteToWideChar(CP_ACP, 0, lpszPath, -1, szPathW, MAX_PATH);
      MD_SET_DATA_RECORD(&MetaRecord,MD_VR_PATH,METADATA_INHERIT,IIS_MD_UT_FILE,
                                     STRING_METADATA,sizeof(WCHAR)*(wcslen(szPathW)+1),szPathW);
      hr = pAdmin->SetData(hWebRoot, lpsz4C, &MetaRecord);

      DWORD dwPermissions = MD_ACCESS_READ|MD_ACCESS_EXECUTE|MD_ACCESS_SCRIPT;
      MD_SET_DATA_RECORD(&MetaRecord,MD_ACCESS_PERM,METADATA_INHERIT,IIS_MD_UT_FILE,
                                     DWORD_METADATA,sizeof(DWORD),&dwPermissions);
      hr = pAdmin->SetData(hWebRoot, lpsz4C, &MetaRecord);

      pAdmin->CloseKey(hWebRoot);
    }
    pAdmin->Release();
  }
  CoUninitialize();
}



FCINST_API  void FCINST_DECL StartConsoleServices(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    MB("StartConsoleServices 0", MB_OK);
    if (ExistSrv(hWnd, "IISADMIN") == SRV_NOTRUNNING)
    {
        MB("StartConsoleServices 1", MB_OK);
        StartSrv(hWnd, "IISADMIN");
    }
    if (ExistSrv(hWnd, "W3SVC") == SRV_NOTRUNNING)
    {
        MB("StartConsoleServices 2", MB_OK);
        StartSrv(hWnd, "W3SVC");
    }
    if (ExistSrv(hWnd, "MSFTPSVC") == SRV_NOTRUNNING)
    {
        MB("StartConsoleServices 3", MB_OK);
        StartSrv(hWnd, "MSFTPSVC");
    }
    if (ExistSrv(hWnd, "GOPHERSVC") == SRV_NOTRUNNING)
    {
        MB("StartConsoleServices 4", MB_OK);
        StartSrv(hWnd, "GOPHERSVC");
    }
    if (ExistSrv(hWnd, "SMTPSVC") == SRV_NOTRUNNING)
    {
        MB("StartConsoleServices 5", MB_OK);
        StartSrv(hWnd, "SMTPSVC");
    }
    if (ExistSrv(hWnd, "NNTPSVC") == SRV_NOTRUNNING)
    {
        MB("StartConsoleServices 6", MB_OK);
        StartSrv(hWnd, "NNTPSVC");
    }
    MB("StartConsoleServices end", MB_OK);
   
}


FCINST_API  void FCINST_DECL StopConsoleServices(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    MB("StopConsoleServices 0", MB_OK);

    if (ExistSrv(hWnd, "W3SVC") == SRV_RUNNING)
    {
         MB("StopConsoleServices 1", MB_OK);
         StopSrv(hWnd, "W3SVC");
    }
    if (ExistSrv(hWnd, "MSFTPSVC") == SRV_RUNNING)
    {
        MB("StopConsoleServices 2", MB_OK);
        StopSrv(hWnd, "MSFTPSVC");
    }
    if (ExistSrv(hWnd, "GOPHERSVC") == SRV_RUNNING)
    {
        MB("StopConsoleServices 3", MB_OK);
        StopSrv(hWnd, "GOPHERSVC");
    }
    if (ExistSrv(hWnd, "SMTPSVC") == SRV_RUNNING)
    {
        MB("StopConsoleServices 4", MB_OK);
        StopSrv(hWnd, "SMTPSVC");
    }
    if (ExistSrv(hWnd, "NNTPSVC") == SRV_RUNNING)
    {
        MB("StopConsoleServices 5", MB_OK);
        StopSrv(hWnd, "NNTPSVC");
    }
    if (ExistSrv(hWnd, "IISADMIN") == SRV_RUNNING)
    {
        MB("StopConsoleServices 6", MB_OK);
        StopSrv(hWnd, "IISADMIN");
    }
    MB("StopConsoleServices end", MB_OK);
}


FCINST_API  void FCINST_DECL GetIEInstallDir(HWND hWnd, LPTSTR lpszPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    TCHAR szIEPath[MAX_PATH];
    DWORD dwType;
    DWORD dwSize;


    TCHAR szRegistryKey[] = _T("SOFTWARE\\Microsoft\\IE Setup\\Setup");

    dwSize = MAX_PATH;
    if (QueryRegistryValue(HKEY_LOCAL_MACHINE, szRegistryKey, _T("Path"), &dwType, (LPBYTE)szIEPath, &dwSize))
    {
        szIEPath[dwSize] = 0;
    }
    else
    {
        szIEPath[0] = 0;
    }

    _tcscat(szIEPath, _T("\\IExplore.exe"));

    _tcscpy(lpszPath, szIEPath);
}
