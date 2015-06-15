/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AddOnHandler.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AddOnHandler.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AddOnHandler.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "StdAfx.h"
#include "resource.h"
#include <initguid.h>
#include "AddOnHandler.h"

#include "AddOnHandler_i.c"
#include "AAddOnObject.h"

/* ----  Target Specific Includes:    ----------------------------------------- */

/* ----  Local Defines:   ----------------------------------------------------- */

/* ----  Global Variables:   -------------------------------------------------- */

CComModule _Module;

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_[*TN]AddonObject, CAAddOnObject)
END_OBJECT_MAP()

/* ---------------------------------------------------------------------------- */
/**
 * class CAddOnHandlerApp
 *
 */
class CAddOnHandlerApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddOnHandlerApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAddOnHandlerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAddOnHandlerApp, CWinApp)
	//{{AFX_MSG_MAP(CAddOnHandlerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAddOnHandlerApp theApp;

/* ---------------------------------------------------------------------------- */
/**
 * InitInstance
 *
 */
BOOL CAddOnHandlerApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_ADDONHANDLERLib);
    return CWinApp::InitInstance();
}

/* ---------------------------------------------------------------------------- */
/**
 * ExitInstance
 *
 */
int CAddOnHandlerApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/* ---------------------------------------------------------------------------- */
/**
 * DllCanUnloadNow
 *
 */
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/* ---------------------------------------------------------------------------- */
/**
 * DllGetClassObject
 *
 */
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/* ---------------------------------------------------------------------------- */
/**
 * DllRegisterServer
 *
 */
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/* ---------------------------------------------------------------------------- */
/**
 * DllUnregisterServer
 *
 */
STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */


/* ---------------------------------------------------------------------------- */
