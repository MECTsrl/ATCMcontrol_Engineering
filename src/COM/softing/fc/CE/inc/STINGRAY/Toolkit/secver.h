// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//
//  Author:			Scot Wingo
//  Description:	Version information for SEC
//  Created:		Tue Aug 29 14:55:33 1995
// 

#ifndef __SECVER_H__
#define __SECVER_H__

/////////////////////////////////////////////////////////////////////////////
// Master version numbers
/////////////////////////////////////////////////////////////////////////////

#define _SEC     1      // SEC classes
#define _SEC_VER 0x0701 // SEC OT Version 7.01

// Include OT-Pro version number, if found
#ifdef WIN32
#include "ProVer.h"
#endif

//
// Automatic library inclusion macros that use the #pragma/lib feature
//
// Define _SECNOAUTOLIB when compiling SEC so that it does
// not try and link to itself. -chicken and egg.
//
#ifndef _SECNOAUTOLIB

// If a custom autolink was used, do not bother with the defaults
#ifndef SEC_CUSTOM_AUTOLINK

// Ok, no custom autolink was used, use the default (i.e. if you only 
// included "secall.h". The default is the configuration last set by the
// build wizard
#if _MFC_VER <= 0x0200		// 16 bit
	#if !(defined(_SECDLL) || defined(_SECEXT))
		#ifndef _AFXDLL
			#ifdef _DEBUG
				#define _SECAUTOLIBNAME "ot16d.lib"
			#else
				#define _SECAUTOLIBNAME "ot16r.lib"
			#endif
		#else
			#ifdef _DEBUG
				#define _SECAUTOLIBNAME "ota16d.lib"
			#else
				#define _SECAUTOLIBNAME "ota16r.lib"
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#define _SECAUTOLIBNAME "ot5116d.lib"
		#else
			#define _SECAUTOLIBNAME "ot5116r.lib"
		#endif
	#endif
#else		// 32 bit
	#ifndef _UNICODE
		#if !defined(_SECDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701d.lib"
		#elif !defined(_SECDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701.lib"
		#elif !defined(_SECDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701ad.lib"
		#elif !defined(_SECDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701a.lib"
		#elif defined(_SECDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701asd.lib"
		#elif defined(_SECDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701as.lib"
		#elif defined(_SECDLL) && !defined(_AFXDLL)
			#error _SECDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#else
		#if !defined(_SECDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701ud.lib"
		#elif !defined(_SECDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701u.lib"
		#elif !defined(_SECDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701aud.lib"
		#elif !defined(_SECDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701au.lib"
		#elif defined(_SECDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701asud.lib"
		#elif defined(_SECDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _SECAUTOLIBNAME "OT701asu.lib"
		#elif defined(_SECDLL) && !defined(_AFXDLL)
			#error _SECDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#endif
#endif //16-bits

#endif	// SEC_CUSTOM_AUTOLINK

#if _MFC_VER >= 0x0300

// You may turn off SEC++ specific compilation message
// by adding _SECNOMSG to your C/C++ settings.
#ifndef _SECNOMSG
#pragma message( "Objective Toolkit will automatically link with " _SECAUTOLIBNAME )
#endif //_SECNOMSG

// Perform autolink here:
#pragma comment(lib, _SECAUTOLIBNAME)

#endif //end of 32-bit message

#endif //_SECNOAUTOLIB
/////////////////////////////////////////////////////////////////////////////
// Standard preprocessor symbols if not already defined

#if !defined(_WIN32)
#include "toolkit\gxtchar.h"

// provide types and macros that 32-bit wtypes.h usually provides
#define TEXT    _T
#define LPCTSTR LPCSTR
#define LPTSTR  LPSTR

#endif

#define SEC_DATA

#endif // __SECVER_H__
