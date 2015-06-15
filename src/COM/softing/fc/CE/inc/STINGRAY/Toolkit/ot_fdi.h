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
//  Description:	ot_fdi.h - FDI (MDI alternative) component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_FDI_H__
#define  __OTM_FDI_H__

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
// *  Floating Document Interface
// *
// ***********************************************************

#ifdef OT_BLD_FDI

	// SECFrameWnd
	#ifndef __SWINMDI_H__
	#include "toolkit\swinmdi.h"
	#endif //__SWINMDI_H__

	// SECMDIFrameWnd
	#ifndef __SWINFRM_H__
	#include "toolkit\swinfrm.h"
	#endif //__SWINFRM_H__

	// SECCDialogBar
	#ifndef __SBARDLG_H__
	#include "toolkit\sbardlg.h"
	#endif //__SBARDLG_H__

	// SECDockBar
	#ifndef __SECBARDOCK_H__
	#include "toolkit\sbardock.h"
	#endif //__SECBARDOCK_H__

	// SECControlBar
	#ifndef __SBARCORE_H__
	#include "toolkit\sbarcore.h"
	#endif //__SBARCORE_H__

	// SECToolBar
	#ifndef __SBARTOOL_H__
	#include "toolkit\sbartool.h"
	#endif //__SBARTOOL_H__

	// SECStatusBar
	#ifndef __SBARSTAT_H__
	#include "toolkit\sbarstat.h"
	#endif //__SBARSTAT_H__


	#ifndef __SDOCKCNT_H__
	#include "toolkit\sdockcnt.h"
	#endif //__SDOCKCNT_H__

	// SECDockContext
	#ifndef __SDOCKSTA_H__
	#include "toolkit\sdocksta.h"
	#endif

	// SECControlBarManager
	#ifndef __SBARMGR_H__
	#include "toolkit\sbarmgr.h"
	#endif //__SBARMGR_H__

	// SECOleServerItem
	#ifndef __SECOLEDOCK_H__
	#include "toolkit\SOleDock.h"
	#endif

	#ifndef __FDIMAIN_H__
	#include "toolkit\fdimain.h"
	#endif //__FDIMAIN_H__

	#ifndef __FDICHILD_H__
	#include "toolkit\fdichild.h"
	#endif //__FDICHILD_H__

#else
	#pragma message("Warning:  FDI support was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_FDI

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

#endif // __OTM_FDI_H__