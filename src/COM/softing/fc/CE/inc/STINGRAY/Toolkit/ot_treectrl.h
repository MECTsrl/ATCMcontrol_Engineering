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
//  Description:	ot_treectrl.h - Tree control component header
//  Created:		Tue Aug 27 14:08:25 1998
//
//

#ifndef __OTM_TREECTRL_H__
#define  __OTM_TREECTRL_H__

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
// *  Enhanced Win32 List Control
// *
// ***********************************************************

#ifdef OT_BLD_TREECTRL

	// SECListTip
	#ifndef __SLSTCTIP_H__
	#include "toolkit\slstctip.h"
	#endif // __SLSTCTIP_H__

	// SECListClient
	#ifndef __SLSTEDIT_H__
	#include "toolkit\slstedit.h"
	#endif // __SLSTEDIT_H__

	// SECListCtrl
	#ifndef __SLSTCTL_H__
	#include "toolkit\slstctl.h"
	#endif // __SLSTCTL_H__

	// SECListView
	#ifndef __SLSTVW_H__
	#include "toolkit\slstvw.h"
	#endif // __SLSTVW_H__

	// SECTreeNode
	#ifndef __TREENODE_H__
	#include "toolkit\TreeNode.h"
	#endif // __TREENODE_H__

	// SECTreeCtrl
	#ifndef __TRCTLX_H__
	#include "toolkit\TrCtlX.h"
	#endif // __TRCTLX_H__

	// SECTreeView
	#ifndef __TRVWX_H__
	#include "toolkit\TrVwX.h"
	#endif // __TRVWX_H__

#else
	#pragma message("Warning:  The tree control component was not included in the Objective Toolkit library.")
	#pragma message("          Add this component to the library using the BuildWizard and rebuild Objective Toolkit.")
#endif // OT_BLD_TREECTRL

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

#endif // __OTM_TREECTRL_H__