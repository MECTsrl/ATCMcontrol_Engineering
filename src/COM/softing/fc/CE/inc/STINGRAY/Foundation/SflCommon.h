/////////////////////////////////////////////////////////////////////////////
// SflCommon.h : SFL macros and utility classes
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL utility classes
//

#pragma once
#ifndef __SFLCOMMON_H__
#define __SFLCOMMON_H__


#if !defined(_SFL)
#define _SFL
#endif


#define _X86_

#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE _UNICODE
#endif
#if !defined(_UNICODE) && defined(UNICODE)
#define _UNICODE UNICODE
#endif

//////////////////////////////////////////////////////////////
// If _AFX defined, assume _SFL_MFC_SUPPORT is desired.

#ifndef _SFL_MFC_SUPPORT
#ifdef _AFX
#define _SFL_MFC_SUPPORT
#endif
#endif

#if defined(_ATL) && !defined(_SFL_NO_ATL)
#define _SFL_ATL_SUPPORT
#endif
#if defined(_SFL_ATL_SUPPORT) && defined(_SFL_NO_ATL)
#undef _SFL_ATL_SUPPORT
#endif

//////////////////////////////////////////////////////////////
// Include version info and autolink pragmas
#include <Foundation\SflVersion.h>

//////////////////////////////////////////////////////////////
// Produce rror if trying to compile with old Common and MVC
// headers

#if defined(_CMN_VER)
#if (_CMN_VER != 0x0FFF)
#error SFL cannot be compiled with older versions of the Stingray Common library. Check your include path.
#endif
#endif

#if defined(_MVC_VER)
#if (_MVC_VER != 0x0FFF)
#error SFL cannot be compiled with older versions of the Stingray MVC library. Check your include path.
#endif
#endif

//////////////////////////////////////////////////////////////
// Define sfl abbreviation for stingray::foundation namespace

namespace stingray {
namespace foundation {
};
};

#ifndef _SFL_NO_NAMESPACE_ABBREVIATION
namespace sfl=stingray::foundation;
#endif

#include <stdarg.h>
#include <windef.h>
#if _MSC_VER > 1100
#include <basetsd.h>
#endif
#include <winbase.h>
#include <winuser.h>

// Debugging support
// Debug macros automatically nooped in release mode
#include <crtdbg.h>

//#if !defined(_AFX) || !defined(_WTL_VER)
//#if defined(_SFL_MFC_SUPPORT)
//#undef _SFL_MFC_SUPPORT
//#endif
//#endif

#ifdef _SFLDLL

#include <Foundation\SflDll.h>

#ifdef _SFLDLL_IMPL
#define SFLAPI  __declspec(dllexport)
#else
#define SFLAPI	__declspec(dllimport)
#endif

#else

#define SFLAPI

#endif


/////////////////////////////////////////////////////////////////////////////
// Custom Messages

#ifndef SFL_WM_USER
#define SFL_WM_USER   (WM_USER+1000)
#endif

#define WM_GETPRINTABLE                (SFL_WM_USER+1)
#define WM_UIUPDATE                    (SFL_WM_USER+2)
#define WM_CLOSE_PRINT_PREVIEW         (SFL_WM_USER+3)

/////////////////////////////////////////////////////////////////////////////
// Macros

#define MESSAGE_HANDLER_DELEGATE(msg, func) \
	if(uMsg == msg) \
	{ \
		lResult = func(uMsg, wParam, lParam, bHandled); \
	}


#if (_WIN32_IE >= 0x0400)
#define ICC_ALL 0x2FFF
#else
#define ICC_ALL 0x4FF
#endif


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

// Define min and max template functions

template<class _Ty> inline
const _Ty& _SFLMIN(const _Ty& _X, const _Ty& _Y)
{
	return (_Y < _X ? _Y : _X);
}

template<class _Ty> inline
const _Ty& _SFLMAX(const _Ty& _X, const _Ty& _Y)
{
	return (_X < _Y ? _Y : _X);
}

#endif __SFLCOMMON_H__
