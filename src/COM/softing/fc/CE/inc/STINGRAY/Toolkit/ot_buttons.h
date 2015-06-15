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
//  Description:	ot_buttons.h - Button controls component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_BUTTONS_H__
#define  __OTM_BUTTONS_H__

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
// *  Button Classes
// *
// ***********************************************************

#ifdef CMN_BLD_BUTTON_CTRL

	// SECBtns
	#ifndef __SECBTNS_H__
	#include /* */ "foundation\controls.h"
	#endif // __SECBTNS_H__

#else
	#pragma message("Warning:  The button controls were not included in the Objective Studio Common library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild the Objective Studio Common library.")
#endif	// CMN_BLD_BUTTON_CTRL

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

#endif // __OTM_BUTTONS_H__