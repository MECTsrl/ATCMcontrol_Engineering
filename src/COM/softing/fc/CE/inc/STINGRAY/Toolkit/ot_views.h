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
//  Description:	ot_views.h - Pan/Zoom view classes
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_VIEWS_H__
#define  __OTM_VIEWS_H__

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
// *  View Classes
// *
// ***********************************************************

#ifdef OT_BLD_PANZOOM

	#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
		#pragma message ( __TIME__ ":  OT_BLD_PANZOOM" )
	#endif

	// SECZoomView
	#ifndef __SECZMVW_H__
	#include "toolkit\seczmvw.h"
	#endif //__SECZMVW_H__

	// SECPanView
	#ifndef __SECPANVW_H__
	#include "toolkit\secpanvw.h"
	#endif //__SECPANVW_H__

	// SECPanWnd - The OverView Window
	#ifndef __SECPANWN_H__
	#include "toolkit\secpanwn.h"
	#endif //__SECPANWN_H__

#else
	#pragma message("Warning:  The pan/zoom view component was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_PANZOOM

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

#endif // __OTM_VIEWS_H__