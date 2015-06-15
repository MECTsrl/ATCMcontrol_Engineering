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
//  Description:	ot_secjpeg.h - SECJpeg component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_SECJPEG_H__
#define  __OTM_SECJPEG_H__

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
// *  SECJPeg
// *
// ***********************************************************

#ifdef CMN_BLD_SECJPEG
	#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
		#pragma message ( __TIME__ ":  CMN_BLD_SECJPEG" )
	#endif
	// SECJpeg
	#ifndef __SFL_SECJPEG_H__
	#include "Foundation\Image\MFC\secjpeg.h"
	#endif // __SFL_SECJPEG_H_

#else
	#pragma message("Warning:  JPEG image support was not included in the Objective Studio Common library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild the Objective Studio Common Library.")
#endif	// CMN_BLD_SECJPEG

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

#endif // __OTM_SECJPEG_H__