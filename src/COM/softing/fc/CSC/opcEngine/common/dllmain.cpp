
//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : DLLMAIN.CPP                                                 |
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//                                                                            |
//  Description : Global Functions of DLL                                     |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "OPCTrace.h"

#ifdef _AFXEXT

//-----------------------------------------------------------------------------
// shared                                                                     |
//-----------------------------------------------------------------------------

#include <afxdllx.h>

static AFX_EXTENSION_MODULE MfcextDLL = { NULL, NULL };
static HINSTANCE g_instance;

//-----------------------------------------------------------------------------
// getInstanceHandle
//
// - get the instance handle of the application
//
// returns:
//		instance handle
//
HINSTANCE getInstanceHandle(void)
{
	return g_instance;
} // getInstanceHandle


//-----------------------------------------------------------------------------
// DllMain
//
// - main routine of DLL
//
// returns:
//		exit code
//
extern "C" int APIENTRY DllMain(
	IN HINSTANCE hInstance,	// instance handle
	IN DWORD dwReason,		// call reason
	IN LPVOID lpReserved)
{
	g_instance = hInstance;
	if (dwReason == DLL_PROCESS_ATTACH)
	{
#ifdef SERVER
		_TRACEC(TL_DEB, TG_OTHER, _T("DllMain"), (_T("OPC Engine DLL loading")));
#endif
		if (!AfxInitExtensionModule(MfcextDLL, hInstance))
			return 0;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
#ifdef SERVER
		_TRACEC(TL_DEB, TG_OTHER, _T("DllMain"), (_T("OPC Engine DLL unloading")));
#endif
		AfxTermExtensionModule(MfcextDLL);
	}
	return 1;   // ok
} // DllMain

#else

//-----------------------------------------------------------------------------
// static                                                                     |
//-----------------------------------------------------------------------------

class OPCApplication : public CWinApp
{
public:
	OPCApplication();
};


OPCApplication::OPCApplication()
{
}

OPCApplication theApp;


//-----------------------------------------------------------------------------
// getInstanceHandle
//
// - get the instance handle of the application
//
// returns:
//		instance handle
//
HINSTANCE getInstanceHandle(void)
{
	return ::AfxGetInstanceHandle();
} // getInstanceHandle

#endif
