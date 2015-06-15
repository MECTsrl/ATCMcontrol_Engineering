#if !defined(AFX_STDAFX_H__B3380728_7157_11D1_A4A8_00A024363DFC__INCLUDED_)
#define AFX_STDAFX_H__B3380728_7157_11D1_A4A8_00A024363DFC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//      oder häufig verwendete, projektspezifische Include-Dateien,
//      die nur in unregelmäßigen Abständen geändert werden.

#define VC_EXTRALEAN		// Selten benutzte Teile der Windows-Header nicht einbinden

#include <afxctl.h>         // MFC-Unterstützung für ActiveX-Steuerelemente
#include <afxcmn.h>			// MFC support for Windows Common Control classes 
#include <afxtempl.h>

#pragma warning ( disable : 4100 ) 

// Nachstehende zwei Include-Anweisungen löschen,  falls die MFC nicht verwendet werden sollen
//  Datenbankklassen
#include <afxdb.h>			// MFC-Datenbankklassen
#include <afxdao.h>			// MFC DAO-Datenbankklassen

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_STDAFX_H__B3380728_7157_11D1_A4A8_00A024363DFC__INCLUDED_)
