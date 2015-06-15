// StandardProjectWizard.cpp : Implementation of DLL Exports.


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
//      Modify the custom build rule for StandardProjectWizard.idl by adding the following 
//      files to the Outputs.
//          StandardProjectWizard_p.c
//          dlldata.c
//      To build a separate proxy/stub DLL, 
//      run nmake -f StandardProjectWizardps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "StandardProjectWizard.h"
#include "dlldatax.h"

#include "StandardProjectWizard_i.c"
#include "ProjectWizardComObj.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif


// ****************************************************************************
// ****************************************************************************
// !!!! Temporary code: registrating the categories should be done by the setup
// ****************************************************************************
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!
// end temp code
// ****************************************************************************
// ****************************************************************************



CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_ProjectWizardComObj, CProjectWizardComObj)
END_OBJECT_MAP()

class CStandardProjectWizardApp : public CWinApp
{
    public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStandardProjectWizardApp)

	public:
        virtual BOOL InitInstance();
        virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(StandardProjectWizardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CStandardProjectWizardApp, CWinApp)
	//{{AFX_MSG_MAP(StandardProjectWizardApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStandardProjectWizardApp theApp;

BOOL CStandardProjectWizardApp::InitInstance()
{
    CString sApplicationName;

    sApplicationName.LoadString(IDS_WIZARD_NAME);
    free((void *) m_pszAppName);
    m_pszAppName = (LPCTSTR) malloc(sApplicationName.GetLength()+1);
    strcpy((TCHAR *) m_pszAppName,sApplicationName);

#ifdef _MERGE_PROXYSTUB
    hProxyDll = m_hInstance;
#endif
    _Module.Init(ObjectMap, m_hInstance, &LIBID_STANDARDPROJECTWIZARDLib);
    return CWinApp::InitInstance();
}

int CStandardProjectWizardApp::ExitInstance()
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

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry



// ****************************************************************************
// ****************************************************************************
// !!!! Temporary code: registrating the categories should be done by the setup
// ****************************************************************************

/*
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
                CLSID_ProjectWizardComObj,
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
        hresult = pCatRegister->UnRegisterClassImplCategories(CLSID_ProjectWizardComObj,
            sizeof(usedCategoryIds)/sizeof(*usedCategoryIds),usedCategoryIds);
        pCatRegister->Release();
    }
    return hresult;
}
*/

// end temp code
// ****************************************************************************
// ****************************************************************************




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

    // ****************************************************************************
    // ****************************************************************************
    // !!!! Temporary code: registrating the categories should be done by the setup
    // ****************************************************************************
    HRESULT hr = Create4C_CATIDs();
    if (FAILED(hr)) {
        return hr;
    }
    //if (SUCCEEDED(hresult)) {
    //    hresult = RegisterCategory();
    //}
    // end temp code
    // ****************************************************************************
    // ****************************************************************************

    return hresult;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}