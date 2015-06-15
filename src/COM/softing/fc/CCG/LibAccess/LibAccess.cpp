// CLibLL.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f CLibLLps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>

#include "CG_4CLAccess_i.c"
#include "LowLevelLib.h"
#include "lic\fc_slbase.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_FC4CLAccess, C4CLAccessLib)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_FC4CLAccessLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        _Module.Term();
    }
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}




/////////////////////////////////////////////////////////////////////////////
// SLIC_GetVersionFromFileA - Special DLL Interface for License Manager
SLIC_GET_FILEVER_INFO_PROC_HDR
//should be defines as: STDAPI SLIC_GetVersionFromDataFile(const char* pszFile, char szVersion[/*nBuffer*/], long nBuffer)
{
    HRESULT                 hr;
    BSTR                    sErr;
    BSTR                    sVerInfo;
    CComObject<CLibObject>* pLib;


    if(!pszFile || !szVersion)
        return E_INVALIDARG;
    if(nBuffer<1)
        return TYPE_E_BUFFERTOOSMALL;

    
    szVersion[0] = 0;
    sErr = NULL;

    //create instance of library-accessor COM-object and open the archive:
    hr = pLib->CreateInstance(&pLib);
    if(hr != S_OK)
        return hr;
    pLib->AddRef();
    hr = pLib->openAr(pszFile, false, &sErr);
    if(hr != S_OK)
    {
        if(sErr)
            SysFreeString(sErr);
        pLib->Release();
        return hr;
    }

    if(pLib->GetFileAsBSTR(L"__VERSION", NULL, L"v", &sVerInfo)==S_OK)
    {
        int iRet = FC_StringToMultiByte(sVerInfo, szVersion, nBuffer);
        if(iRet==0)
            hr = E_UNEXPECTED; 
        else if(iRet>=nBuffer)
            hr = TYPE_E_BUFFERTOOSMALL;
    }
    else
    {
        hr = E_FAIL;
    }

    pLib->closeAr(NULL);
    pLib->Release();
    return hr;
}


