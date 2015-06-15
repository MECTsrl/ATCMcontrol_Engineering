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
//  Description:	ot_keyshortcut.h - Keyboard shortcut component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_KEYSHORTCUT_H__
#define  __OTM_KEYSHORTCUT_H__

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
// *  Keyboard Shortcuts
// *
// ***********************************************************

#ifdef OT_BLD_KBSCUT

	// SECShortcuts
	#ifndef __SCUTACCL_H__
	#include "toolkit\scutaccl.h"
	#endif //__SCUTACCL_H__

	// SECCommandList
	#ifndef __SCUTCMDS_H__
	#include "toolkit\scutcmds.h"
	#endif //__SCUTCMDS_H__

	// SECShortcutDlg
	#ifndef __SCUTDIALOG_H__
	#include "toolkit\scutdlg.h"
	#endif //__SCUTDIALOG_H__

	// SECAssignKeyDlg
	#ifndef __SCUTKDIALOG_H__
	#include "toolkit\scutkdlg.h"
	#endif //__SCITKDIALOG_H__

#else
	#pragma message("Warning:  The keyboard shortcut component was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_KBSCUT

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

#endif // __OTM_KEYSHORTCUT_H__