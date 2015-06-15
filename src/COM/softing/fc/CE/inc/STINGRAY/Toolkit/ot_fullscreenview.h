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
//  Description:	ot_combobox.h - Enhanced CComboBoxEx component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_FULLSCREENVIEW_H__
#define  __OTM_FULLSCREENVIEW_H__

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
// *  Full Screen View Class
// *
// ***********************************************************

#ifdef OT_BLD_FULLSCREENVIEW


	#if (_MFC_VER >= 0x0421)

		// full screen view class header
		#ifndef __SECFULLSCREEN_H__
		#include /* */ "toolkit\fsview.h"
		#endif // __SECFULLSCREEN_H__

	#else
		#pragma message("Warning!  The full screen view class requires MFC version 4.21 (VC 5.0) or greater.")
		#pragma message("          The template classes used by the full screen view class implementation 
        #pragma message("          will not work with this version of MFC.  This class will not be compiled.")
	#endif // _MFC_VER >= 0x0421



#else
	#pragma message("Warning:  The full screen view class was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild the Objective Toolkit library.")
#endif	// OT_BLD_FULLSCREENVIEW

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

#endif // __OTM_FULLSCREENVIEW_H__