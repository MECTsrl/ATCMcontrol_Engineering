// CpNavigator.cpp: Implementierung von CCpNavigatorApp und DLL-Registrierung.

#include "stdafx.h"
#include "CpNavigator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCpNavigatorApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xb3380721, 0x7157, 0x11d1, { 0xa4, 0xa8, 0, 0xa0, 0x24, 0x36, 0x3d, 0xfc } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CCpNavigatorApp::InitInstance - DLL-Initialisierung

BOOL CCpNavigatorApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// ZU ERLEDIGEN: Hier den Code für das Initialisieren der eigenen Module einfügen.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CCpNavigatorApp::ExitInstance - DLL-Beendigung

int CCpNavigatorApp::ExitInstance()
{
    // ZU ERLEDIGEN: Hier den Code für das Beenden der eigenen Module einfügen.

    // ED++
    // HACK: have to cleanup the temp directory somehow...
    char szTemp[MAX_PATH], szFindWhat[MAX_PATH];
    WIN32_FIND_DATA findData;
    if (GetTempPath(MAX_PATH, szTemp))
    {
        wsprintf(szFindWhat, "%s\\JVM*.tmp", szTemp);
        HANDLE h = FindFirstFile(szFindWhat, &findData);
        if (h != INVALID_HANDLE_VALUE)
        {
            do {
                if (0 == (findData.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_READONLY
                                                       |FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM))
                    && findData.nFileSizeHigh == 0 && findData.nFileSizeLow == 0)
                {
                    wsprintf(szFindWhat, "%s\\%s", szTemp, findData.cFileName);
                    DeleteFile(szFindWhat);
                }
            } while (FindNextFile(h, &findData));
            FindClose(h);
        }
    }
    // ED--

    return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Fügt der Systemregistrierung Einträge hinzu

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Entfernt Einträge aus der Systemregistrierung

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
