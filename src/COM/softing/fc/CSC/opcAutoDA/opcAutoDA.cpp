// OPCAuto.cpp
//
// (c) Copyright 1998 The OPC Foundation
// ALL RIGHTS RESERVED.
//
// DISCLAIMER:
//  This sample code is provided by the OPC Foundation solely to assist
//  in understanding the OPC Specifications and may be used
//  as set forth in the License Grant section of the OPC Specification.
//  This code is provided as-is and without warranty or support of any sort
//  and is subject to the Warranty and Liability Disclaimers which appear
//  in the printed OPC Specification.
//
// CREDITS:
//  This code was generously provided to the OPC Foundation by
//  Jim Hansen, FactorySoft Inc.
//
// CONTENTS:
//  This file is part of the OPC Data Access Automation 2.0 wrapper dll.
//  This file contains functions required for an in-process COM server.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH
//
//

#include "stdafx.h"
#include "initguid.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
   OBJECT_ENTRY(CLSID_OPCServer, COPCAutoServerObject)
END_OBJECT_MAP()

UINT OPCSTMFORMATDATATIME = 0;
UINT OPCSTMFORMATWRITECOMPLETE = 0;

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   lpReserved;
#ifdef _MERGE_PROXYSTUB
   if (!PrxDllMain(hInstance, dwReason, lpReserved))
      return FALSE;
#endif
   if (dwReason == DLL_PROCESS_ATTACH)
   {
      OPCSTMFORMATDATATIME = RegisterClipboardFormat(_T("OPCSTMFORMATDATATIME"));
      OPCSTMFORMATWRITECOMPLETE = RegisterClipboardFormat(_T("OPCSTMFORMATWRITECOMPLETE"));
      _Module.Init(ObjectMap, hInstance);
      DisableThreadLibraryCalls(hInstance);
   }
   else if (dwReason == DLL_PROCESS_DETACH)
      _Module.Term();
   return TRUE;    // ok
}

main()   // This is only to satisfy the linker when using MT lib
{ return 0;}
/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
   if (PrxDllCanUnloadNow() != S_OK)
      return S_FALSE;
#endif
   return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
   if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
      return S_OK;
#endif
   return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
   HRESULT hRes = PrxDllRegisterServer();
   if (FAILED(hRes))
      return hRes;
#endif
   // registers object, typelib and all interfaces in typelib
   return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
   PrxDllUnregisterServer();
#endif
   _Module.UnregisterServer();
   return S_OK;
}


