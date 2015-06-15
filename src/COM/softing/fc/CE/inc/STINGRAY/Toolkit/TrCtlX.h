// 
// Stingray Software Extension Classes
// Copyright (C) 1996 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the  help files for detailed information
// regarding using these classes.
// 
//  Authors:		Dean Hallman and Justin Rudd

#ifdef WIN32

#ifndef __TRCTLX_H__
#define __TRCTLX_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SEC_TREEBASEC_H__
#include "toolkit\trbaseC.h"
#endif

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


///////////// SECTreeCtrl /////////////
/////////////////////////////
// AutoDuck tag block block for SECTreeCtrl 

// adinsert AutoDuck insertion point for SECTreeCtrl 
//@doc SECTreeCtrl 
//@class SECTreeCtrl | The Stingray enhanced tree control class. The implementation of this class is defined in the SEC_TREECLASS class.
//@base public | SEC_TREECLASS
//@xref <c SEC_TREECLASS> <c SEC_LISTCLASS>
class SECTreeCtrl : public SECTreeBaseC
{
	DECLARE_DYNAMIC( SECTreeCtrl )
public:
	//@access Construction/Initialization
	//@cmember
	/* Constructor*/
	SECTreeCtrl();
	virtual ~SECTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};



////////////////////////////////////////
void DDX_Control
	(
		CDataExchange* pdx,
		int idc,
		SECTreeCtrl& tree
	);

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TRCTLX_H__

#endif // WIN32

