


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// BrkPtMan.cpp : Implementation of DLL Exports.


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
//      Modify the custom build rule for BrkPtMan.idl by adding the following 
//      files to the Outputs.
//          BrkPtMan_p.c
//          dlldata.c
//      To build a separate proxy/stub DLL, 
//      run nmake -f BrkPtManps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "BrkPtMan.h"

#include "utilif.h"
#include "htmlhelp.h"
#include "cesysdef.h"

#include "BrkPtMan_i.c"
#include "BreakPointMan.h"
#include "BreakPointView.h"
#include "CallStackView.h"


// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_BreakPointMan, CBreakPointMan)
OBJECT_ENTRY(CLSID_BreakPointView, CBreakPointView)
OBJECT_ENTRY(CLSID_CallStackView, CCallStackView)
END_OBJECT_MAP()

class CBrkPtManApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrkPtManApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBrkPtManApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ShowHelp(DWORD dwData, UINT nCmd); 
	
};

BEGIN_MESSAGE_MAP(CBrkPtManApp, CWinApp)
	//{{AFX_MSG_MAP(CBrkPtManApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBrkPtManApp theApp;

BOOL CBrkPtManApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_BRKPTMANLib);
    return CWinApp::InitInstance();
}

int CBrkPtManApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
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
    HRESULT hr = Create4C_CATIDs();
    ASSERT(SUCCEEDED(hr));
    if (FAILED(hr)) {
        return hr;
    }
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}

void CBrkPtManApp::ShowHelp(DWORD dwData, UINT nCmd) 
{
	HWND hWnd = GetActiveWindow();

    CString strManualPath;
    UINT    nStrLen = _MAX_PATH;

    HRESULT hr = UTIL_GetInstallPath(strManualPath.GetBuffer(_MAX_PATH), &nStrLen);
    strManualPath.ReleaseBuffer();
    if(SUCCEEDED(hr)) {
        strManualPath += CE_MANUAL_FILENAME;
        switch(nCmd) {
            case HELP_CONTEXT:
                if(dwData >= 0x30000) {
                    dwData -= 0x30000;
                } else if(dwData >= 0x20000) {
                    dwData -= 0x20000;
                } else if(dwData >= 0x10000) {
                    dwData -= 0x10000;
                }
                HtmlHelp(hWnd, strManualPath, HH_HELP_CONTEXT, dwData);
                break;
            case HELP_INDEX:
                break;
            case HELP_FINDER:
                HtmlHelp(hWnd, strManualPath, HH_DISPLAY_TOPIC, NULL);
                break;
        }
    }
    return;
}

void CBrkPtManApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
    ShowHelp(dwData, nCmd);
}

