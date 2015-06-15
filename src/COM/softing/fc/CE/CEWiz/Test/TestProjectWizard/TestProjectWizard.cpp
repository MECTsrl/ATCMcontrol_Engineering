// TestProjectWizard.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To merge the proxy/stub code into the object DLL, add the file 
//      dlldatax.c to the project.  Make sure precompiled headers 
//      are turned off for this file, and add _MERGE_PROXYSTUB to the 
//      defines for the project.  
//
//      If you are not running WinNT4.0 or Win95 with DCOM, then you
//      need to remove the following define from dlldatax.c
//      #define _WIN32_WINNT 0x0400
//
//      Further, if you are running MIDL without /Oicf switch, you also 
//      need to remove the following define from dlldatax.c.
//      #define USE_STUBLESS_PROXY
//
//      Modify the custom build rule for TestProjectWizard.idl by adding the following 
//      files to the Outputs.
//          TestProjectWizard_p.c
//          dlldata.c
//      To build a separate proxy/stub DLL, 
//      run nmake -f TestProjectWizardps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "TestProjectWizard.h"
#include "dlldatax.h"

#include "TestProjectWizard_i.c"
#include "WizardComObj.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_WizardComObj, CWizardComObj)
END_OBJECT_MAP()

class CTestProjectWizardApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestProjectWizardApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTestProjectWizardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTestProjectWizardApp, CWinApp)
	//{{AFX_MSG_MAP(CTestProjectWizardApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTestProjectWizardApp theApp;

BOOL CTestProjectWizardApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
    hProxyDll = m_hInstance;
#endif
    _Module.Init(ObjectMap, m_hInstance, &LIBID_TESTPROJECTWIZARDLib);
    return CWinApp::InitInstance();
}

int CTestProjectWizardApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllCanUnloadNow() != S_OK)
        return S_FALSE;
#endif
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
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

const GUID CATID_4ControlProjectWizard = {0x57131651,0xCABA,0x11d4,{0xB3,0x52,0x00,0x08,0xC7,0x79,0x81,0xF8}};

HRESULT RegisterCategory()
{
    HRESULT hresult;
    ICatRegister *pCatRegister;
    CATID usedCategoryIds[1];
    CATEGORYINFO categoryInfo[1];
    
    hresult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,0,CLSCTX_ALL,
        IID_ICatRegister,(void**) &pCatRegister);
    if (SUCCEEDED(hresult)) {
        categoryInfo[0].catid = CATID_4ControlProjectWizard; 
        categoryInfo[0].lcid = MAKELCID(
            MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT),SORT_DEFAULT);
        wcscpy(categoryInfo[0].szDescription,OLESTR("4ControlProjectWizardCategory"));
        hresult = pCatRegister->RegisterCategories(
            sizeof(categoryInfo)/sizeof(*categoryInfo),categoryInfo);
        if (SUCCEEDED(hresult)) {
            usedCategoryIds[0] = CATID_4ControlProjectWizard;
            hresult = pCatRegister->RegisterClassImplCategories(
                CLSID_WizardComObj,
                sizeof(usedCategoryIds)/sizeof(*usedCategoryIds),
                usedCategoryIds);
        }
        pCatRegister->Release();
    }
    return hresult;
}

HRESULT UnregisterCategory()
{
    HRESULT hresult;
    ICatRegister *pCatRegister;
    CATID usedCategoryIds[1];
    
    hresult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,0,CLSCTX_ALL,
        IID_ICatRegister,(void**) &pCatRegister);
    if (SUCCEEDED(hresult)) {
        usedCategoryIds[0] = CATID_4ControlProjectWizard;
        hresult = pCatRegister->UnRegisterClassImplCategories(CLSID_WizardComObj,
            sizeof(usedCategoryIds)/sizeof(*usedCategoryIds),usedCategoryIds);
        pCatRegister->Release();
    }
    return hresult;
}

STDAPI DllRegisterServer(void)
{
    HRESULT hresult;

    hresult = S_OK;
#ifdef _MERGE_PROXYSTUB
    hresult = PrxDllRegisterServer();
#endif
    if (SUCCEEDED(hresult)) {
        hresult = _Module.RegisterServer(TRUE);
    }
    if (SUCCEEDED(hresult)) {
        hresult = RegisterCategory();
    }
    return hresult;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    UnregisterCategory();
    return _Module.UnregisterServer(TRUE);
}



