/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/CSC_OnlSrv.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CSC_OnlSrv.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/CSC_OnlSrv.cpp $
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

//#include <stdio.h>
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "NSpaceHolder.h"

// CD1: check namespace periodically
#include "iecConfiguration.h"
#include "iecResource.h"

//----  Local Defines:   -------------------------------------------*


//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "CommissioningMgt.h"
#include "ControlMgt.h"
#include "CustomConnect.h"

// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!


//----  Global Initializations:   ----------------------------------*
// The one and only NT service object
CExeModule _Module;

CLSID CLSID_OPCServer;
CLSID CLSID_ControlMgt;
CLSID CLSID_CommissioningMgt;
LPCTSTR g_StartParam = NULL;
HINSTANCE g_hInstance;
BOOL g_DebugFlag = FALSE;

static void InitTrace(HMODULE hMod);


//--------------------------- COM OBJECT MAP ---------------------------------*
BEGIN_OBJECT_MAP(ObjectMap)
    OBJECT_ENTRY(CLSID_CustomConnect, CCustomConnect)
END_OBJECT_MAP()

//--------------------------- COM OBJECT MAP ---------------------------------*
BEGIN_OBJECT_MAP(ObjectMap2)
	OBJECT_ENTRY(CLSID_ControlMgt, CControlMgt)
	OBJECT_ENTRY(CLSID_CommissioningMgt, CCommissioningMgt)
END_OBJECT_MAP()

// Although some of these functions are big they are declared inline since they are only used once

//------------------------------------------------------------------*
inline void CExeModule::Init()
{
    CComModule::Init(g_StartParam != NULL ? ObjectMap2 : ObjectMap, g_hInstance);

    AfxSetResourceHandle(g_hInstance);        // used for CString etc !!!

    g_strRegBasePath.Format(_T("Software\\%s\\%s"), g_cs4Control, g_csApp); 
    if (g_StartParam != NULL) {
        HRESULT hr;
        TCHAR szProgID[256];
        USES_CONVERSION;
        lstrcpy(szProgID, g_ProgID_OPCServer);
        lstrcat(szProgID, g_StartParam);
        hr = CLSIDFromProgID(T2W(szProgID), &CLSID_OPCServer);
        assert(SUCCEEDED(hr));
        lstrcpy(szProgID, g_ProgID_ControlMgt);
        lstrcat(szProgID, g_StartParam);
        hr = CLSIDFromProgID(T2W(szProgID), &CLSID_ControlMgt);
        assert(SUCCEEDED(hr));
        lstrcpy(szProgID, g_ProgID_CommissioningMgt);
        lstrcat(szProgID, g_StartParam);
        hr = CLSIDFromProgID(T2W(szProgID), &CLSID_CommissioningMgt);
        assert(SUCCEEDED(hr));
    }
}

//------------------------------------------------------------------*
inline HRESULT CExeModule::RegisterServer()
{
    _Module.Init();

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Add service entries
    VERIFY(SUCCEEDED(UpdateRegistryFromResource(IDR_CSC_ONLINE_SRV, TRUE)));
    
    // Add object entries
    // this registers OPC interface etc 
    // (all declared as DECLARE_REGISTRY_RESOURCEID(IDR_xx) in their respective class object headers)
    hr = Create4C_CATIDs();
    if (FAILED(hr)) {
        AfxMessageBox(IDS_ERROR_CANNOT_CREATE_4C_CATIDS, MB_OK | MB_ICONSTOP);
        return hr;
    }

    // create CATIDs for OPC DA2.0
    hr = ::CreateComponentCategory(CATID_OPCDAServer10, g_szCATDESC_OPCDAServer10);
    if (FAILED(hr)) {
        AfxMessageBox(IDS_ERROR_CANNOT_CREATE_OPC_CATIDS, MB_OK | MB_ICONSTOP);
        return hr;
    }

    hr = ::CreateComponentCategory(CATID_OPCDAServer20, g_szCATDESC_OPCDAServer20);
    if (FAILED(hr)) {
        AfxMessageBox(IDS_ERROR_CANNOT_CREATE_OPC_CATIDS, MB_OK | MB_ICONSTOP);
        return hr;
    }

    hr = CComModule::RegisterServer(TRUE);
    if (FAILED(hr)) {
        AfxMessageBox(IDS_ERROR_CANNOT_REGISTER_4C_SRV, MB_OK | MB_ICONSTOP);
        return hr;
    }

    TCHAR szName[256];
    DWORD dwIndex = 0;
    for(;;) {
      LONG lResult = RegEnumKey(HKEY_CLASSES_ROOT, dwIndex, szName, 256);
      if (lResult != ERROR_SUCCESS)
          break;
      dwIndex++;
      if (_tcsnicmp(szName, g_ProgID_OPCServer, _tcslen(g_ProgID_OPCServer)) == 0)
      {
          _Module.RegisterServerInstance(szName + _tcslen(g_ProgID_OPCServer));
      }
    }

    /*
      GenericPointer<OPCEngine>   pEngine;
      pEngine = ::getOPCEngine();
      assert(!!pEngine);
      hr = E_FAIL;
      if (pEngine->initialize(OPCEngine::OutProc, CLSID_OPCServer, IDR_DUMMY_REGSCRIPT, _Module.GetModuleInstance())) {
          hr = pEngine->registerServer();
          pEngine->terminate();
      }
      if (FAILED(hr)) {
          AfxMessageBox(IDS_ERROR_CANNOT_REGISTER_4C_OPC_SRV, MB_OK | MB_ICONSTOP);
          return hr;
      }
    */

    CoUninitialize();
    _Module.Term();
    return S_OK;
}

//------------------------------------------------------------------*
inline HRESULT CExeModule::UnregisterServer()
{
    CString strPath;
    CRegKey key;
    LONG lResult;

    _Module.Init();
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    strPath.Format (_T ("Software\\%s\\%s"), g_cs4Control, g_csApp);
    lResult = key.Open (HKEY_LOCAL_MACHINE, strPath);
    if (lResult == ERROR_SUCCESS)
    {
        key.RecurseDeleteKey (g_csSysVar);
    }

    // Remove service entries
    UpdateRegistryFromResource(IDR_CSC_ONLINE_SRV, FALSE);

    // Remove object entries
    CComModule::UnregisterServer(TRUE);

    CLSID clsGuid;
    TCHAR szName[256];
    DWORD dwIndex = 0;
    for(;;) {
      lResult = RegEnumKey(HKEY_CLASSES_ROOT, dwIndex, szName, 256);
      if (lResult != ERROR_SUCCESS)
          break;
      dwIndex++;
      if (_tcsnicmp(szName, g_ProgID_OPCServer, _tcslen(g_ProgID_OPCServer)) == 0)
      {
          if (FAILED(CLSIDFromProgID(CComBSTR(szName), &clsGuid)))
              continue;
          _Module.UnregisterServerInstance(szName + _tcslen(g_ProgID_OPCServer));
          dwIndex = 0;
      }
      if (_tcsnicmp(szName, g_ProgID_ControlMgt, _tcslen(g_ProgID_ControlMgt)) == 0)
      {
          if (FAILED(CLSIDFromProgID(CComBSTR(szName), &clsGuid)))
              continue;
          _Module.UnregisterServerInstance(szName + _tcslen(g_ProgID_ControlMgt));
          dwIndex = 0;
      }
      if (_tcsnicmp(szName, g_ProgID_CommissioningMgt, _tcslen(g_ProgID_CommissioningMgt)) == 0)
      {
          if (FAILED(CLSIDFromProgID(CComBSTR(szName), &clsGuid)))
              continue;
          _Module.UnregisterServerInstance(szName + _tcslen(g_ProgID_CommissioningMgt));
          dwIndex = 0;
      }
    }

    /*
    GenericPointer<OPCEngine>   pEngine;
    pEngine = ::getOPCEngine();
    assert(!!pEngine);
    hr = E_FAIL;
    if (pEngine->initialize(OPCEngine::OutProc, CLSID_OPCServer, IDR_DUMMY_REGSCRIPT, _Module.GetModuleInstance())) {
        hr = pEngine->unregisterServer();
        pEngine->terminate();
    }
    */

    CoUninitialize();
    _Module.Term();
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT CExeModule::RegisterServerInstance(LPCTSTR lpszParam)
{
    USES_CONVERSION;
    HRESULT hr;
    TCHAR szProgID[256];
    WCHAR szGuid[100];
    CLSID guid;
    _ATL_REGMAP_ENTRY map[3];
    map[0].szKey = L"PARAM";
    map[0].szData = T2W(lpszParam);
    map[1].szKey = L"GUID";
    map[1].szData = szGuid;
    map[2].szKey = NULL;
    map[2].szData = NULL;

    lstrcpy(szProgID, g_ProgID_OPCServer);
    lstrcat(szProgID, lpszParam);
    hr = CLSIDFromProgID(T2W(szProgID), &guid);
    if (FAILED(hr)) {
        hr = CoCreateGuid(&guid);
    }
    if (FAILED(hr))
        return hr;
    StringFromGUID2(guid, szGuid, 100);
    hr = UpdateRegistryFromResource(IDR_OPCSERVER, TRUE, map);
    if (FAILED(hr))
        return hr;

    lstrcpy(szProgID, g_ProgID_ControlMgt);
    lstrcat(szProgID, lpszParam);
    hr = CLSIDFromProgID(T2W(szProgID), &guid);
    if (FAILED(hr)) {
        hr = CoCreateGuid(&guid);
    }
    if (FAILED(hr))
        return hr;
    StringFromGUID2(guid, szGuid, 100);
    hr = UpdateRegistryFromResource(IDR_CONTROL_MGT, TRUE, map);
    if (FAILED(hr))
        return hr;

    lstrcpy(szProgID, g_ProgID_CommissioningMgt);
    lstrcat(szProgID, lpszParam);
    hr = CLSIDFromProgID(T2W(szProgID), &guid);
    if (FAILED(hr)) {
        hr = CoCreateGuid(&guid);
    }
    if (FAILED(hr))
        return hr;
    StringFromGUID2(guid, szGuid, 100);
    hr = UpdateRegistryFromResource(IDR_COMMISSIONING_MGT, TRUE, map);
    if (FAILED(hr))
        return hr;

    TCHAR szConfig[_MAX_PATH + 1];
    GetModuleFileName(_Module.GetModuleInstance(), szConfig, _MAX_PATH);
    LPTSTR  psz = _tcsrchr(szConfig, _T('\\'));
    if (psz != NULL) {
        *psz = _T('\0');
    }

    lstrcat(szConfig, _T("\\config\\"));
    BOOL bOK = CreateDirectory(szConfig, NULL);
    lstrcat(szConfig, _T("_"));
    lstrcat(szConfig, lpszParam);
    lstrcat(szConfig, _T("_"));
    bOK = CreateDirectory(szConfig, NULL);

    return hr;
}

//------------------------------------------------------------------*
HRESULT CExeModule::UnregisterServerInstance(LPCTSTR lpszParam)
{
    USES_CONVERSION;
    HRESULT hr;
    TCHAR szProgID[256];
    WCHAR szGuid[100];
    CLSID guid;
    _ATL_REGMAP_ENTRY map[3];
    map[0].szKey = L"PARAM";
    map[0].szData = T2W(lpszParam);
    map[1].szKey = L"GUID";
    map[1].szData = szGuid;
    map[2].szKey = NULL;
    map[2].szData = NULL;

    lstrcpy(szProgID, g_ProgID_OPCServer);
    lstrcat(szProgID, lpszParam);
    hr = CLSIDFromProgID(T2W(szProgID), &guid);
    if (SUCCEEDED(hr)) {
        StringFromGUID2(guid, szGuid, 100);
        hr = UpdateRegistryFromResource(IDR_OPCSERVER, FALSE, map);
    }

    lstrcpy(szProgID, g_ProgID_ControlMgt);
    lstrcat(szProgID, lpszParam);
    hr = CLSIDFromProgID(T2W(szProgID), &guid);
    if (SUCCEEDED(hr)) {
        StringFromGUID2(guid, szGuid, 100);
        hr = UpdateRegistryFromResource(IDR_CONTROL_MGT, FALSE, map);
    }

    lstrcpy(szProgID, g_ProgID_CommissioningMgt);
    lstrcat(szProgID, lpszParam);
    hr = CLSIDFromProgID(T2W(szProgID), &guid);
    if (SUCCEEDED(hr)) {
        StringFromGUID2(guid, szGuid, 100);
        hr = UpdateRegistryFromResource(IDR_COMMISSIONING_MGT, FALSE, map);
    }

    return S_OK;
}

//------------------------------------------------------------------*
LONG CExeModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0)
    {
        bActivity = true;
    }
    return l;
}

//------------------------------------------------------------------*
// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}

//------------------------------------------------------------------*
//Monitors the shutdown event
void CExeModule::MonitorShutdown()
{
    const DWORD dwTimeOut = 5; // time for EXE to be idle before shutting down

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
    DWORD idleCount = 0;
    while (1)
    {
        if (!bActivity && m_nLockCnt == 0)
        {
            idleCount ++;
            if (idleCount>dwTimeOut)
                break;
        }
        else
        {
            idleCount = 0;
            bActivity = false;
        }
        Sleep(1000);
    }
    PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

bool CExeModule::StartMonitor()
{
    DWORD dwThreadID;
    HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    CloseHandle(h);
    return (h != NULL);
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration

//------------------------------------------------------------------*
DWORD CExeModule::Run()
{
    HRESULT hr;

    _Module.Init();
    _Module.dwThreadID = GetCurrentThreadId();
    _Module.bActivity = false;

    // free threaded server!!
    hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    assert(SUCCEEDED(hr));

    BOOL bNoSecurity = FALSE;
    CRegKey keyApp;
    LONG lRes = keyApp.Open(HKEY_LOCAL_MACHINE, g_strRegBasePath, KEY_READ);
    if (lRes == ERROR_SUCCESS)
    {
        DWORD   dwNoSecurity = FALSE;
        keyApp.QueryValue(dwNoSecurity, _T("DisableDCOMSecurity"));
        bNoSecurity = (dwNoSecurity != 0);
    }

    if (bNoSecurity)
    {
        // This provides a NULL DACL which will allow access to everyone.
        hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL,
            RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL);
        assert(SUCCEEDED(hr));
    }

    InitTrace(_Module.GetModuleInstance());

    if (g_StartParam != NULL) {
        TRACE(_T("Creating namespace holder...\n"));
        NameSpaceHolder *pNSpace = new NameSpaceHolder;
        assert(pNSpace != NULL);
        if (pNSpace == NULL) {
            TRACE(_T("ERROR: cannot start 4CC namespace: out of memory\n"));
            return 0;
        }
        hr = pNSpace->InitInstance();
        if (FAILED(hr)) {
            TRACE(_T("ERROR: cannot initialize namespace: hr=0x%08X\n"), hr);
            return 0;
        }
        // now switch namespaces:
        SetGlobalNamespace(pNSpace);
        pNSpace->release();
    }

    hr = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE);
    assert(SUCCEEDED(hr));

    if (g_StartParam != NULL) {
        NameSpaceHolder *pNSpace;
        GetGlobalNamespace(&pNSpace);
        if (pNSpace) {
            pNSpace->StartEngine();
            pNSpace->release();
        }
    }

    if (!g_DebugFlag)
        _Module.StartMonitor();

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }

    _Module.RevokeClassObjects();
    if (g_StartParam != NULL) {
        // also releases namespace
        NameSpaceHolder *pNSpace;
        GetGlobalNamespace(&pNSpace);
        if (pNSpace) {
            pNSpace->ExitInstance();
            pNSpace->release();
        }
        SetGlobalNamespace(NULL);
    }

    TrcTerminate();

    CoUninitialize();
    _Module.Term();
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
//

//------------------------------------------------------------------*
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, 
    HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
    //lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
    g_hInstance = hInstance;

    static const TCHAR szTokens[] = _T(" \t");

    for (LPCTSTR lpszToken = _tcstok(lpCmdLine, szTokens);
         lpszToken != NULL;
         lpszToken = _tcstok(NULL, szTokens) )
    {
        if (*lpszToken != _T('-') && *lpszToken != _T('/'))
            continue;

        lpszToken++;

        if (lstrcmpi(lpszToken, _T("param")) == 0)
        {
            g_StartParam = _tcstok(NULL, szTokens);
            continue;
        }

        if (lstrcmpi(lpszToken, _T("UnregServer")) == 0)
        {
            return _Module.UnregisterServer();
        }

        // Register as Service
        if (lstrcmpi(lpszToken, _T("RegServer")) == 0)
        {
            return _Module.RegisterServer();
        }

        if (lstrcmpi(lpszToken, _T("Debug")) == 0)
        {
            g_DebugFlag = TRUE;
        }

        // Starting as COM server?
        if (  (lstrcmpi(lpszToken, _T("Embedding")) == 0)
            ||(lstrcmpi(lpszToken, _T("Automation")) == 0))
        {
            return _Module.Run();
        }

    }

    return 0;
}


//------------------------------------------------------------------*
/*static*/ void InitTrace(HMODULE hMod)
{
    TrcInitialize();

    enum TraceMode mode = noTrace;
    WORD wLevel = TRC_FULL;
    UINT uFileSize = 500;   // 500 KB file size

    CRegKey keyApp;
    CRegKey keyTrace;

    LONG lRes = keyApp.Open(HKEY_LOCAL_MACHINE, g_strRegBasePath);
    if (lRes == ERROR_SUCCESS)
    {
        lRes = keyTrace.Open(keyApp, _T("Trace"));
        if (lRes == ERROR_SUCCESS)
        {
            DWORD   dwMode = (DWORD) noTrace;
            keyTrace.QueryValue(dwMode, _T("Mode"));
            switch (dwMode) {
            case fileContTrace: // trace to file, swap to second file 
                mode = fileContTrace;
                break;
            case noTrace:       // trace is completely disabled
            case ringTrace:     // trace to ringbuffer in memory, no file trace
            case fileStartupTrace:  // trace to file until maxTraceSize is reached,
            default:
                mode = noTrace;
                break;
            }
            DWORD   dwLevel = TRC_FULL;
            keyTrace.QueryValue(dwLevel, _T("Level"));
            wLevel = (WORD) dwLevel;
            DWORD dwFileSize = uFileSize;
            keyTrace.QueryValue(dwFileSize, _T("MaxFileSize"));
            uFileSize = dwFileSize;

            keyTrace.SetValue(uFileSize, _T("MaxFileSize"));
            keyTrace.SetValue((DWORD) mode, _T("Mode"));
            keyTrace.SetValue((DWORD) wLevel, _T("Level"));
            keyTrace.SetValue(_T("0: no trace\n")
                              _T("3: continuous trace with file swapping\n"),
                              _T("ModeComment"));
            keyTrace.SetValue(
                    _T("0x0001: TRC_FATAL = fatal error, highest trace level\n")
                    _T("0x0002: TRC_ERROR = serious error\n")
                    _T("0x0004: TRC_WARNING = internal warning\n")
                    _T("0x0008: TRC_BUS_ERROR = error on communication bus\n")
                    _T("0x0010: TRC_BUS_DIAGNOSE = diagnostic message on bus\n")
                    _T("0x0020: TRC_USER_OPERATION = log important user interaction/operation\n")
                    _T("0x0040: TRC_INTERFACE = log action/callback on user interface\n")
                    _T("0x0080: TRC_COMM_EVENT = log communication layer event\n")
                    _T("0x0100: TRC_READ_VALUE = log read operation with value\n")
                    _T("0x0200: TRC_WRITE_VALUE = log write operation with value\n"),
                    _T("LevelComment"));

        }
    }
    
    ::TrcSetMode(mode);
    ::TrcSetLevel(wLevel);
    ::TrcSetFileSize(uFileSize);

    TCHAR szTrcFile[_MAX_PATH + sizeof(TCHAR)];
    ::GetModuleFileName(hMod, szTrcFile, _MAX_PATH);
    LPTSTR  psz = _tcsrchr(szTrcFile, _T('\\'));
    if (psz != NULL) {
        *psz = _T('\0');
    }

    CString strBase = szTrcFile;
    _tcscat(szTrcFile, _T("\\CSC_OnlSrv.trc"));
    ::TrcSetFilename(szTrcFile);
    
    GenericPointer<OPCEngine>    pEngine = ::getOPCEngine();
    if (mode != noTrace)
    {
        // set trace of opc engine
        CString strTrc1 = strBase, strTrc2 = strBase;
        strTrc1 += _T("\\OPC_log1.trc");
        strTrc2 += _T("\\OPC_log2.trc");
        pEngine->setTraceFile(strTrc1, strTrc2, uFileSize * 1000);
#ifndef _DEBUG  // limit error log if release build
        pEngine->setTraceLevel(OPCENGINE_TRACE_ALL, OPCENGINE_TRACE_ALL, OPCENGINE_TRACE_NOTHING, OPCENGINE_TRACE_NOTHING);
#endif
    }
    if (mode == noTrace) {
        pEngine->setTraceLevel(OPCENGINE_TRACE_NOTHING, OPCENGINE_TRACE_NOTHING, OPCENGINE_TRACE_NOTHING, OPCENGINE_TRACE_NOTHING);
    }

    TrcPrint(TRC_WARNING, _T("**** Starting trace with TrcLevel 0x%04x !\n"), wLevel);
}

/* ---------------------------------------------------------------------------- */
