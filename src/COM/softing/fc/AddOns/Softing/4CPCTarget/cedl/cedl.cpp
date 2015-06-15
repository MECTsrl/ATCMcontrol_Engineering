
// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f cedlps.mk in the project directory.

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "cedl.h"
#include "dl.h"

#include "cedl_i.c"
#include "dl_i.c"

#include "dlrs.h"
#include "dlext.h"
#include "Downld.h"
#include "dlstat.h"

#include "cemsg_i.c"

// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!

STDAPI RegisterStdTypeLib();

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_CEDL, CImpCEDL)
END_OBJECT_MAP()

class CCedlApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CCedlApp theApp;

BOOL CCedlApp::InitInstance()
{
	CString StrAppName;
	//First free the string allocated by MFC at CWinApp startup.
	//The string is allocated before InitInstance is called.
	free((void*)m_pszAppName);
	//Change the name of the application.
	//The CWinApp destructor will free the memory.
	StrAppName.LoadString(IDS_PROJNAME);
	m_pszAppName=_tcsdup(StrAppName);

	// Enforce using of DAO36 and Jet40 by mfc42.dll
   	AfxGetModuleState()->m_dwVersion = 0x0601;
    //EF don't call this: causes an zombie process! AfxDaoInit();

	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

int CCedlApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) {
		return S_OK;
	}
	else 
		return S_FALSE;
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
    //create ATCMControl category id's
    HRESULT hr = Create4C_CATIDs();
    ASSERT(SUCCEEDED(hr));
    if (FAILED(hr)) {
        return hr;
    }
	/** registers object and own type library
     *  the interface typelib generated from cedl.idl will be
     *  registered separately with RegisterStdTypeLib()
     *  //TODO Setup shall register standard type library
    */
    RegisterStdTypeLib();
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	return _Module.UnregisterServer(TRUE);
}

STDAPI RegisterStdTypeLib()
{
    return _Module.RegisterTypeLib(_T("\\2"));
}
