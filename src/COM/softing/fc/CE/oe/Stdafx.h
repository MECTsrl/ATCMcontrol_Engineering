// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxconv.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#ifdef WIN32
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // WIN32
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifdef _OEDLL 
	#define OE_DATAEXT	 	AFX_DATA_EXPORT
	#define OE_DATADEF  
#endif

#define _OENOAUTOLIB

#include <oedit.h>
