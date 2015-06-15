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
//  Author:			Rob Oliver
//  Description:	ot_secdib.h - SECDib component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_SECDIB_H__
#define  __OTM_SECDIB_H__

// ***********************************************************
// *
// *  OT needs 8 byte packing for structures
// *
// ***********************************************************

#if (_MFC_VER >= 0x0300)
// pshpack8.h saves the current #pragma pack value
#include "pshpack8.h"
#else
#pragma pack(8)
#endif

// ***********************************************************
// *
// *  OT optimized include file
// *
// ***********************************************************

#ifndef __SECOPT_H__
    #include "toolkit\secopt.h"
#endif

// ***********************************************************
// *
// *  SECDib
// *
// ***********************************************************

#ifdef CMN_BLD_SECDIB

	// SECImage - ABC for all images
	#ifndef __SFL_SECIMAGE_H__
	#include /* */ "Foundation\Image\MFC\secimage.h"
	#endif //__SFL_SECIMAGE_H__

	// SECDib
	#ifndef __SFL_SECDIB_H__
	#include /* */ "Foundation\Image\MFC\secdib.h"
	#endif //__SFL_SECDIB_H__

#else
	#pragma message("Warning:  DIB image support was not included in the Objective Studio Common library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild the Objective Studio Common Library.")
#endif	// CMN_BLD_SECDIB

// ***********************************************************
// *
// *  Restore default byte packing
// *
// ***********************************************************

#if (_MFC_VER >= 0x0300)
// poppack.h restores the original #pragma pack value
#include /* */ "poppack.h"
#else
// restore default packing value
#pragma pack()
#endif

#ifndef SEC_NO_NAMESPACE_USING
	using namespace stingray::foundation;
#endif

#endif // __OTM_SECDIB_H__