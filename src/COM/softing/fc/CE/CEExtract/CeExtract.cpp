// CeExtract.cpp : Implementierung der DLL-Exporte.


// Hinweis: Proxy/Stub-Information
//      Um eine eigene Proxy/Stub-DLL zu erstellen, 
//      führen Sie nmake -f CeExtractps.mk im Projektverzeichnis aus.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "CeExtract.h"

#include "CeExtract_i.c"
#include "Extract.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Category IDs of ATCMControl:
#include "CatIDs.h"
#define DEFINE_GLOBAL_VARS
#include "CatIDs.h"     // also include definitions !!!

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Extract, CExtract)
END_OBJECT_MAP()

class CCeExtractApp : public CWinApp
{
public:

// Überladungen
	// Überladungen für vom Klassen-Assistenten generierte virtuelle Funktionen
	//{{AFX_VIRTUAL(CCeExtractApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCeExtractApp)
		// HINWEIS - der Klassen-Assistent fügt hier Member-Funktionen hinzu oder löscht sie.
		//    BEARBEITEN SIE NICHT, was Sie in diesen generierten Code-Blöcken sehen !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CCeExtractApp, CWinApp)
	//{{AFX_MSG_MAP(CCeExtractApp)
		// HINWEIS - der Klassen-Assistent fügt hier Umsetzungsmakros hinzu oder löscht sie.
		//    BEARBEITEN SIE NICHT, was Sie in diesen generierten Code-Blöcken sehen !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCeExtractApp theApp;

BOOL CCeExtractApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_CEEXTRACTLib);
    return CWinApp::InitInstance();
}

int CCeExtractApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Verwendet, um zu entscheiden, ob die DLL von OLE aus dem Speicher entfernt werden kann

STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Liefert eine Klassenfabrik zurück, um ein Objekt des angeforderten Typs anzulegen

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Fügt der Systemregistrierung Einträge hinzu

STDAPI DllRegisterServer(void)
{
    // Registriert Objekt, Typelib und alle Schnittstellen in Typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Entfernt Einträge aus der Systemregistrierung

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


