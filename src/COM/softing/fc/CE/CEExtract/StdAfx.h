// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//      oder häufig verwendete, projektspezifische Include-Dateien,
//      die nur in unregelmäßigen Abständen geändert werden.

#if !defined(AFX_STDAFX_H__C55A88AA_6A3E_11D2_9273_00104B4808FB__INCLUDED_)
#define AFX_STDAFX_H__C55A88AA_6A3E_11D2_9273_00104B4808FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//Sie können eine Klasse von CComModule ableiten und diese verwenden, um etwas zu überschreiben,
//Sie sollten jedoch den Namen von _Module nicht ändern
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__C55A88AA_6A3E_11D2_9273_00104B4808FB__INCLUDED)
