// 
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc.
// All Rights Reserved
// 
// Default.h : Generated Configuration Info for 
//                    configuration "Default"
//

// To automatically link your application with this configuration:
//
// Add 
// #include "config/Default.h" 
// before any other SFL include
// in your application's stdafx.h file.

#ifndef _SFLBWCONFIG_H_
#define _SFLBWCONFIG_H_


#ifndef _SFLNOAUTOLIB

	// This file is used to autolink a custom build configuration. Do not 
	// include more than 1 custom autolink.
	#ifdef SFL_CUSTOM_AUTOLINK
	#pragma message("Error, including more than 1 autolink header.")
	#endif

	// Define this preprocessor such that secver.h knows not to use the
	// default autolink information
	#define SFL_CUSTOM_AUTOLINK

	#ifndef _SFL_MFC_SUPPORT // Auto link options for ATL compatible builds
		#ifndef _UNICODE
			#if !defined(_SFLDLL) && defined(_DEBUG) 
				#define _SFLAUTOLIBNAME "sfl103wd.lib"
			#elif !defined(_SFLDLL) && !defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103w.lib"
			#elif defined(_SFLDLL) && defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103wsd.lib"
			#elif defined(_SFLDLL) && !defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103ws.lib"
			#endif
		#else
			#if !defined(_SFLDLL) && defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103wud.lib"
			#elif !defined(_SFLDLL) && !defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103wu.lib"
			#elif defined(_SFLDLL) && defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103wsud.lib"
			#elif defined(_SFLDLL) && !defined(_DEBUG)
				#define _SFLAUTOLIBNAME "sfl103wsu.lib"
			#endif
		#endif // _UNICODE		
	#else // Auto link options for MFC compatible library builds
		#ifndef _UNICODE
				#if !defined(_SFLDLL) && !defined(_AFXDLL) && defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103d.lib"
				#elif !defined(_SFLDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103.lib"
				#elif !defined(_SFLDLL) && defined(_AFXDLL) && defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103ad.lib"
				#elif !defined(_SFLDLL) && defined(_AFXDLL) && !defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103a.lib"
				#elif defined(_SFLDLL) && defined(_AFXDLL) && defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103asd.lib"
				#elif defined(_SFLDLL) && defined(_AFXDLL) && !defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103as.lib"
				#elif defined(_SFLDLL) && !defined(_AFXDLL)
					#error _SFLDLL is defined but _AFXDLL is not. You must define both.
				#endif
		#else
				#if !defined(_SFLDLL) && !defined(_AFXDLL) && defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103ud.lib"
				#elif !defined(_SFLDLL) && !defined(_AFXDLL) && !defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103u.lib"
				#elif !defined(_SFLDLL) && defined(_AFXDLL) && defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103aud.lib"
				#elif !defined(_SFLDLL) && defined(_AFXDLL) && !defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103au.lib"
				#elif defined(_SFLDLL) && defined(_AFXDLL) && defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103asud.lib"
				#elif defined(_SFLDLL) && defined(_AFXDLL) && !defined(_DEBUG)
					#define _SFLAUTOLIBNAME "sfl103asu.lib"
				#elif defined(_SFLDLL) && !defined(_AFXDLL)
					#error _SFLDLL is defined but _AFXDLL is not. You must define both.
				#endif
		#endif //_UNICODE
	#endif // _SFL_MFC_SUPPORT
#endif // _SFL_NO_AUTOLIB

// If using the autolink, also add any preprocessor defines that are unique
// to this configuration
#ifdef SFL_CUSTOM_AUTOLINK

#ifndef SFL_BLD_GRAPHICS
#define SFL_BLD_GRAPHICS
#endif

#ifndef SFL_BLD_LAYOUTMGR
#define SFL_BLD_LAYOUTMGR
#endif

#ifndef SFL_BLD_EVENTS
#define SFL_BLD_EVENTS
#endif

#ifndef SFL_BLD_PRINTPREVIEW
#define SFL_BLD_PRINTPREVIEW
#endif

#ifndef SFL_BLD_IMAGE
#define SFL_BLD_IMAGE
#endif

#ifndef SFL_BLD_COMPATIBILITY
#define SFL_BLD_COMPATIBILITY
#endif



#pragma message("SFL autolink configuration: Default")

#endif



#endif // _SECBWCONFIG_H_

