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
//  Author:			Scot Wingo
//  Description:	secopt.h - optimization header file.
//  Created:		Tue Aug 26 20:48:25 1998
//
//

#ifndef __SECOPT_H__
#define __SECOPT_H__

//
// Check for 32 bit environment
//
#ifndef WIN32
    #error WIN32 is not defined.  This version of Objective Toolkit requires a 32-bit compiler.
#endif

// ***********************************************************
// *
// *  BuildWizard preliminary includes
// *
// ***********************************************************

#if !defined(__SEC_BWDEFINES_H)
#include "toolkit\BWDef.h"
#endif //__SEC_BWDEFINES_H

// CImageList, etc.
#if !defined(__AFXCMN_H__)
#include <afxcmn.h>
#endif

// Common Library 
// If OT is a DLL, then CMN must also be a DLL

#ifdef _SECDLL
	#ifndef _SFLDLL
		#define _SFLDLL
	#endif
#endif // _SECDLL


// ***********************************************************
// *
// *  Common library components - Now it is Foundation Library
// *
// ***********************************************************

// OT needs 8 byte packing for structures
#if _MFC_VER >= 0x0300
// pshpack8.h saves the current #pragma pack value
#include /* */ "pshpack8.h"
#else
#pragma pack(8)
#endif

//Prevent the ATL related class be included
#define _SFL_NO_ATL


#include "Foundation\SflCommon.h"

#ifdef WIN32
#include "foundation\SflDef.h"
#endif

#include "Foundation\SflRes.h"

#include "Foundation\Image.h"

#ifndef _SECDC_H_
#include "Foundation\Graphics\MFC\secdc.h"
#endif

// ***********************************************************
// *
// *  SEC CRect Extension 
// *
// ***********************************************************

#ifndef __SEC_RECT_H__
#include "Foundation\Graphics\MFC\secrect.h"
#endif

// ***********************************************************
// *
// *  Bitmap loading
// *
// ***********************************************************

#ifndef __SLOADBMP_H__
#include "Foundation\Graphics\MFC\sloadbmp.h"
#endif // __SLOADBMP_H__

// ***********************************************************
// *
// *  Color Well
// *
// ***********************************************************

// SECColorWell
#ifndef __SECWELL_H__
#include "Foundation\Controls\MFC\secwell.h"
#endif // __SECWELL_H__

// ***********************************************************
// *
// *  Button Classes
// *
// ***********************************************************

// SECBtns
#ifndef __SECBTNS_H__
#include "Foundation\Controls\MFC\secbtns.h"
#endif // __SECBTNS_H__

// ***********************************************************
// *
// *  Design Patterns
// *
// ***********************************************************

#ifndef __SEC_PATTERNS_H
#include "Foundation\patterns.h"
#endif


// ***********************************************************
// *
// *  Version info
// *
// ***********************************************************

#ifndef __SECVER_H__
#include "toolkit\secver.h"
#endif

// ***********************************************************
// *
// *  Objective Toolkit Resources
// *
// ***********************************************************

#include "toolkit\secres.h"

// ***********************************************************
// *
// *  Objective Toolkit PRO Resources
// *
// ***********************************************************


// ***********************************************************
// *
// *  Common Classes
// *
// ***********************************************************
#ifdef OT_BLD_COMMON

	#ifndef __SECAUX_H__
	#include "toolkit\secaux.h"
	#endif // __SECAUX_H__

	// dll support
	#ifndef __SECDLL_H__
	#include "toolkit\secdll.h"
	#endif // __SECDLL_H_

#endif // OT_BLD_COMMON


// ***********************************************************
// *
// *  Internationalization
// *
// ***********************************************************

#ifdef OT_BLD_SLOCALE

	// SECLocale
	#ifndef __SLOCALE_H__
	#include "toolkit\slocale.h"
	#endif // __SLOCALE_H__

#endif // OT_BLD_SLOCALE

#if _MFC_VER >= 0x0300
// poppack.h restores the original #pragma pack value
#include /* */ "poppack.h"
#else
// restore default packing value
#pragma pack()
#endif

#endif // __SECOPT_H__

