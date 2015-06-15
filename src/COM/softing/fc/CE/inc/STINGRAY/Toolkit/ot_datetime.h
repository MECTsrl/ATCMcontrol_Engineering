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
//  Description:	ot_datetime.h - Date time edit component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_DATETIME_H__
#define  __OTM_DATETIME_H__

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
// *  DateTime Controls
// *
// ***********************************************************

#if defined(OT_BLD_DATETIME) && defined(WIN32)

	#ifndef __DTCTRL_H__
	#include "toolkit\dtctrl.h"
	#endif // __DTCTRL_H__

	#ifndef __DTGADGET_H__
	#include "toolkit\dtgadget.h"
	#endif // __DTGADGET_H__

#else
	#pragma message("Warning:  The datetime edit control was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_DATETIME

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

#endif // __OTM_DATETIME_H__