#ifndef __OEDIT_H__
#define __OEDIT_H__
//
// Stingray Software Objective Diagram
// Copyright (C) 1997 Stingray Software, Inc.
// All rights reserved.
//
// Distribution of the source code is forbidden
// without written consent from Stingray Software.
//

// Needed for Import
#ifndef OE_DATAEXT
	#define OE_DATAEXT     AFX_DATA_IMPORT
	#define OE_DATADEF
#endif

#include /* */ <editres.h>
#include /* */ <oefinddlg.h>
#include /* */ <oepageset.h>
#include /* */ <secedit.h>
#include /* */ <seceditview.h>
#include /* */ <seceditctrl.h>
#include /* */ <struct.h>
#include /* */ <secedithint.h>
#include /* */ <regex.h>


/////////////////////////////////////////////////////////////////////////////
// Master version number
/////////////////////////////////////////////////////////////////////////////

#define _OE_VER 0x0103 // Objective Edit 1.03
/////////////////////////////////////////////////////////////////////////////
// Automatic library inclusion macros that use the #pragma/lib feature
/////////////////////////////////////////////////////////////////////////////

#if (_MFC_VER > 0x0421)
	// If you get this warning message, call Stingray for an update to
	// Objective Edit that is compatible with this version of MFC.
	#pragma message("Warning! This release of Objective Edit is not supported with versions")
	#pragma message("         of MFC newer than 4.21. Contact Stingray Software to obtain an update.")
#endif

#ifndef _OENOAUTOLIB
#if _MFC_VER == 0x0421		// MFC 4.21 (Visual C++ 5.0)
	#ifndef _UNICODE
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50d.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50ad.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50a.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50asd.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50as.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#else
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50ud.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50u.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50aud.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50au.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50asud.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE50asu.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must define both.
		#endif
	#endif
#elif _MFC_VER == 0x0420	// MFC 4.2 (Visual C++ 4.2)
	#ifndef _UNICODE
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42d.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42ad.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42a.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42asd.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42as.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#else
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42ud.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42u.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42aud.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42au.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42asud.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE42asu.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#endif
#elif _MFC_VER == 0x0410	// MFC 4.1 (Visual C++ 4.1)
	#ifndef _UNICODE
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41d.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41ad.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41a.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41asd.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41as.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#else
		// Unicode not supported for VC 4.1
		#error Unicode not supported for Objective Edit with VC 4.1
		/*
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41ud.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41u.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41aud.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41au.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41asud.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE41asu.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must defined both.
		#endif
		*/
	#endif
#elif _MFC_VER == 0x0400	// MFC 4.0 (Visual C++ 4.0)
	#ifndef _UNICODE
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40d.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40ad.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40a.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40asd.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40as.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must defined both.
		#endif
	#else
		// Unicode not supported for VC 4.0
		#error Unicode not supported for Objective Edit with VC 4.0
		/*
		#if !defined(_OEDLL) && !defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40ud.lib"
		#elif !defined(_OEDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40u.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40aud.lib"
		#elif !defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40au.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40asud.lib"
		#elif defined(_OEDLL) && defined(_AFXDLL) && !defined(_DEBUG)
			#define _OEAUTOLIBNAME "OE40asu.lib"
		#elif defined(_OEDLL) && !defined(_AFXDLL)
			#error _OEDLL is defined but _AFXDLL is not. You must define both.
		#endif
		*/
	#endif
#else						// Must be 16-bit! (Visual C++ 1.52)
	#pragma message("Warning! This release of Objective Edit is not supported with 16-bit versions")
	#pragma message("         of Visual C++. Contact Stingray Software to obtain an update.")
#endif

#pragma message("")
#pragma message("**********Objective Edit Message**********")
#pragma message("")

#if _MFC_VER >= 0x0400
  #pragma comment(lib, _OEAUTOLIBNAME)
  #pragma message("  Your application will automatically link with-> " _OEAUTOLIBNAME)
#else
  #pragma message("  You should link your application with-> " _OEAUTOLIBNAME)
#endif

#pragma message("")
#pragma message("**********Objective Edit Message**********")
#pragma message("")

#endif //_OENOAUTOLIB

/////////////////////////////////////////////////////////////////////////////


#endif //__OEDIT_H__