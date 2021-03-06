

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__5BF1B284_D1A7_11D4_B35C_0008C77981F8__INCLUDED_)
#define AFX_STDAFX_H__5BF1B284_D1A7_11D4_B35C_0008C77981F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <stdio.h>
#include "debug.h"

/**
* Initialization of memory integrity checks.
*/
#ifdef _DEBUG
#  define ENABLE_MEMORY_CHECK 2
#else
#  define ENABLE_MEMORY_CHECK 0
#endif
#include "cmnintegrity.h"

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__5BF1B284_D1A7_11D4_B35C_0008C77981F8__INCLUDED)
#define nobreak
