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
//  Description:	ot_thumbnail.h - Thumbnail component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_THUMBNAIL_H__
#define  __OTM_THUMBNAIL_H__

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
// *  Thumbnail Classes
// *
// ***********************************************************

#ifdef OT_BLD_THUMBNAIL

	//Needed by SECTNFileDialog
	#ifndef __AFXDLGS_H__
	#include <afxdlgs.h>
	#endif

	//SECTNBitmap
	#ifndef __SECTNBITMAP_H__
	#include "toolkit\sectnbmp.h"
	#endif //__SECTNBITMAP_H__

	//SECTNDC
	#ifndef __SECTNDC_H__
	#include "toolkit\sectndc.h"
	#endif // __SECTNDC_H__

	//SECTNDocument
	#ifndef __SECTNDOC_H__
	#include "toolkit\sectndoc.h"
	#endif //__SECTNDOC_H__

	//SECTNFileDialog
	#ifndef __SECTNFILEDLG_H__
	#include "toolkit\sectndlg.h"
	#endif // __SECTNFILEDLG_H__

	//SECTNView
	#ifndef __SECTNVW_H__
	#include "toolkit\sectnvw.h"
	#endif // __SECTNVW_H__

	//SECTNWinApp
	#ifndef __SECTNWIN_H__
	#include "toolkit\sectnwin.h"
	#endif //__SECTNWIN_H__

#else
	#pragma message("Warning:  The thumbnail component was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_THUMBNAIL

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

#endif // __OTM_THUMBNAIL_H__