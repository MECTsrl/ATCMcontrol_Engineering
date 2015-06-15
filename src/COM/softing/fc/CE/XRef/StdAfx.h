
// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__ACCCB9AD_3D74_466F_A687_352317782900__INCLUDED_)
#define AFX_STDAFX_H__ACCCB9AD_3D74_466F_A687_352317782900__INCLUDED_

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
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlhost.h>
#include <atlctl.h>

//{{AFX_INSERT_LOCATION}}
#include <fstream.h>

class CXrefItem;
class CXrefFileScope;
class CXref;
class CXrefFind;
class CXrefDefinition;
class CXrefDeclaration;

#include "SortedStringMap.h"	// Added by ClassView
#include "XRefDefinition.h"	// Added by ClassView
#include "XRefFileScope.h"	// Added by ClassView
#include "XRefdat.h"	// Added by ClassView
#include "XRefFind.h"	// Added by ClassView

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ACCCB9AD_3D74_466F_A687_352317782900__INCLUDED)

