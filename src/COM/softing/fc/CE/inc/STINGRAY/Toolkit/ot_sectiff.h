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
//  Description:	ot_sectiff.h - SECTiff component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_SECTIFF_H__
#define  __OTM_SECTIFF_H__

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
// *  SECTiff
// *
// ***********************************************************

#ifdef OT_BLD_SECTIFF

	#ifndef WIN32
	#pragma warning(disable : 4103)
	#endif // WIN32

	#ifndef __SECTIFF_H__
	#include /* */ "toolkit\sectiff.h"
	#endif //__SECTIFF_H__

	#ifndef WIN32
	#pragma warning(default : 4103)
	#endif // WIN32

#else
	#pragma message("Warning:  TIFF image support was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_TIFF

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

#endif // __OTM_SECTIFF_H__