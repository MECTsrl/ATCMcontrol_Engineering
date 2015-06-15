// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f ParseGSDps.mk in the project directory.

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "defs.h"

#include "pgsd_i.c"
#include "ParseGSD_i.c" //CLSID and LIBID

#include "ParseGsdFile.h"
#include "DP_Station.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_ParseGsdFile, CParseGsdFile)
END_OBJECT_MAP()

class CParseGSDApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CParseGSDApp theApp;

BOOL CParseGSDApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

int CParseGSDApp::ExitInstance()
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
	// registers object, typelib and all interfaces in typelib
    /* register standard typelibrary ParseGSD.tlb first */
    _Module.RegisterTypeLib(_T("\\2"));
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	//do not unreg typelib - interface id's haven't changed.
    /* unregister standard typelibrary ParseGSD.tlb first */
    _Module.UnRegisterTypeLib(_T("\\2"));
    _Module.UnregisterServer(TRUE);
	return S_OK;
}

LANGID GSD_GetInstallLanguage()
{
    //TODO how to set ?
    return MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT);
}

HRESULT AllocSysString(BSTR* ps,CString & str)
{
    FC_PCHECK_HR(ps,E_POINTER);
    try {
        *ps = NULL;
        *ps = str.AllocSysString();
    }
    catch(CMemoryException * e) {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    return S_OK;
}


bool GSD_FormatMessage(CString & str, HRESULT hr, ...)
{
	va_list arg_ptr;
    bool bRes = false;

	try {
		LPTSTR pszOut = NULL;

        str.Empty();

		pszOut = str.GetBuffer(GSD_PARSE_MSG_BUFSIZE);
		va_start(arg_ptr,hr);
		bool bRes = GSD_FormatMsgArg(_Module.GetResourceInstance(),pszOut,GSD_PARSE_MSG_BUFSIZE,hr,arg_ptr);    
		va_end(arg_ptr);
		str.ReleaseBuffer();
	}
	catch(CMemoryException * e) {
		e->Delete();
		return false;
	}
    return bRes;
}


bool GSD_FormatMsgArg(HINSTANCE hResHandle,LPTSTR pszOut, size_t sizeBuf, HRESULT hr,va_list pArg)
{
    DWORD dwRes;
    LPVOID lpMsgBuf = NULL;
    LANGID langId,deflangId;

    langId = GSD_GetInstallLanguage();
    deflangId = ::GetSystemDefaultLangID();
    *pszOut = 0;

    //try with hmodule message table and installed language
    dwRes = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE ,
        hResHandle,hr,
        langId, // installed language
        (LPTSTR) &lpMsgBuf,0,&pArg);

    if(dwRes) {
        lstrcpyn(pszOut,(LPCTSTR) lpMsgBuf,sizeBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
        lpMsgBuf = NULL;
    }
    
    if(dwRes) {
        return true;
    }

    //try with hmodule message table default language
    dwRes = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE ,
        hResHandle,hr,
        deflangId, // Default language
        (LPTSTR) &lpMsgBuf,0,&pArg);

    if(dwRes) {
        lstrcpyn(pszOut,(LPCTSTR) lpMsgBuf,sizeBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
        lpMsgBuf = NULL;
    }

    if(dwRes) {
        return true;
    }

    //try with system message table alone and default language
    dwRes = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,hr,
        deflangId, // Default language
        (LPTSTR) &lpMsgBuf,0,NULL);

    if(dwRes) {
        lstrcpyn(pszOut,(LPCTSTR) lpMsgBuf,sizeBuf);
    }

    if(lpMsgBuf) {
        ::LocalFree(lpMsgBuf);
    }

    if(dwRes) {
        return true;
    }

    return false;
}
