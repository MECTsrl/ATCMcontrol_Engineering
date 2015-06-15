#if !defined(AFX_CPNAVIGATOR_H__B338072A_7157_11D1_A4A8_00A024363DFC__INCLUDED_)
#define AFX_CPNAVIGATOR_H__B338072A_7157_11D1_A4A8_00A024363DFC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CpNavigator.h: Haupt-Header-Datei für CPNAVIGATOR.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CCpNavigatorApp: Siehe CpNavigator.cpp für Implementierung

class CCpNavigatorApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_CPNAVIGATOR_H__B338072A_7157_11D1_A4A8_00A024363DFC__INCLUDED)
