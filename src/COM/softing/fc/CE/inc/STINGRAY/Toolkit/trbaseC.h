// 
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the  help files for detailed information
// regarding using these classes.
// 
//  Authors:		Dean Hallman and Justin Rudd
#ifndef __SEC_TREEBASEC_H__
#define __SEC_TREEBASEC_H__

#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32


//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

//**************************************************
#undef SEC_TREECLASS
#undef SEC_TREEBASE

// define class name and base class for SECTreeBaseC
#define SEC_TREECLASS SECTreeBaseC
#define SEC_TREEBASE SECListCtrl
// now include the prameterized header
#include "toolkit\trcore.h"

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // WIN32
#endif // __SEC_TREEBASEC_H__
