/////////////////////////////////////////////////////////////////////////////
//@doc 
//@module SflVersion.h | Version information and automatic linking settings.
// 
// Stingray Foundation Library<tm>
// <nl>Copyright <cp> 2000 Stingray Software, Inc. All rights reserved.
// 
// This source code is only intended as a supplement to the Stingray
// Foundation Library User's Guide and related electronic documentation provided with 
// the library. See these sources for detailed information regarding the 
// Stingray Foundation Library product.
// 
// Author: GF
// <nl>Created: 1/00
// 
/////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __SFLVERSION_H__
#define __SFLVERSION_H__

#ifndef _SFL_MFC_SUPPORT
#ifdef _AFX
#define _SFL_MFC_SUPPORT
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Master version number

#define _SFL_VER 0x0103

#ifndef _SFLNOAUTOLIB

// If a custom autolink was used, do not bother with the defaults
#ifndef SFL_CUSTOM_AUTOLINK


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


#endif	// SFL_CUSTOM_AUTOLINK


#ifdef _SFLAUTOLIBNAME
	// You may turn off SFL specific compilation message
	// by adding _SFLNOMSG to your C/C++ settings.
	#ifndef _SFLNOMSG
		#pragma message("SFL will automatically link with " _SFLAUTOLIBNAME)
	#endif //_SFLNOMSG

	#pragma comment(lib, _SFLAUTOLIBNAME)
#else
	#pragma message("Warning: Unable to link with any SFL library!")
#endif // _SFLAUTOLIBNAME

#endif // _SFLNOAUTOLIB

#endif // __SFLVERSION_H__

/////////////////////////////////////////////////////////////////////////////
