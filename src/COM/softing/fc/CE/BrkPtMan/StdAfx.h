
#ifndef __STDAFX_H_
#define __STDAFX_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>
#include <afxtempl.h>

// for reference count debug output:
//#define _ATL_DEBUG_INTERFACES
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#include <atlcom.h>
#include <atlhost.h>
#include <atlctl.h>
#include <afxcmn.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __STDAFX_H_
