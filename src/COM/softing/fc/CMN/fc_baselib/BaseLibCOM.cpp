/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/BaseLibCOM.cpp 1     28.02.07 19:00 Ln $
*----------------------------------------------------------------------------*
*
* =FILENAME            $Workfile: BaseLibCOM.cpp $
*                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/BaseLibCOM.cpp $
*
* =PROJECT             7VH102  ATCMControl
*
* =SWKE                4CMN
*
* =COMPONENT           BaseLib
*
* =CURRENT      $Date: 28.02.07 19:00 $
*           $Revision: 1 $
*
* =ENVIRONMENT             Win32:
*                      MS Windows NT 4.0
*                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
*                      MS Java 2.0
*                      MS Visual SourceSafe 5.0
*
* =REFERENCES
*==
*----------------------------------------------------------------------------*
*
*==
*----------------------------------------------------------------------------*
* =DESCRIPTION
*    @DESCRIPTION@
*==
*----------------------------------------------------------------------------*
* =MODIFICATION
*  08.07.98  Je      File created
*  see history at end of file !
*==
*******************************************************************************
H<<*/


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f smallATLps.mk in the project directory.

#include "stdafx.h"
#include "comcat.h"
#include "initguid.h"

#include "BaseLibCOM.h"
#include "BaseLibCOM_i.c"

#include "BaseLibWrap.h"

// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_FCBaseLib, CFC_BaseLib)
END_OBJECT_MAP()



/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_Module.Init(ObjectMap, hInstance);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();
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
    HRESULT hr = ::CreateComponentCategory(CATID_4CObjects, g_szCATDESC_4CObjects);
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
	_Module.UnregisterServer(TRUE);
	return S_OK;
}

/*
*----------------------------------------------------------------------------*
*  $History: BaseLibCOM.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 2  *****************
 * User: Ed           Date: 10/24/01   Time: 11:52a
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:12
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * utils and baselib from V1.3
 * 
 * *****************  Version 3  *****************
 * User: Je           Date: 9.07.98    Time: 9:18
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * removed DEBUG_NEW
 * 
 * *****************  Version 2  *****************
 * User: Je           Date: 8.07.98    Time: 18:47
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * moved BaseLib COM wrapper into BaseLib itself
 * * CheckString w/o HRESULT error if baselib syntax error
 * 
 * *****************  Version 1  *****************
 * User: Je           Date: 8.07.98    Time: 11:21
 * Created in $/4Control/COM/softing/fc/CMN/baselib
*==
*----------------------------------------------------------------------------*
*/

// 08.07.98  Generated by VC++5.0 VBScript '4C Macros.dsm' Version 1.1/Je
// ***************************  E O F  ******************************
// $Workfile: BaseLibCOM.cpp $
