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

#ifndef __SLSTCTL_H__
#define __SLSTCTL_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SEC_LISTBASEC_H__
#include "toolkit\slstbasec.h"
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

/////////////////////////////
// AutoDuck tag block block for SECListCtrl 
// adinsert AutoDuck insertion point for SECListCtrl 
//@doc SECListCtrl 
//@class SECListCtrl | This is an internal base class for Stingray list and tree classes.
// Direct instantiation of this class is not supported.
//@base public | SEC_LISTCLASS
//@xref <c SEC_LISTCLASS>

class SECListCtrl : public SECListBaseC
{
	DECLARE_DYNAMIC( SECListCtrl )

public:
	//@access Construction/Initialization
	//@cmember
	/* Construction*/
	SECListCtrl();
	virtual ~SECListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

void DDX_Control
	( 
		CDataExchange* pdx, 
		int id, 
		SECListCtrl& ctrl 
	);

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// __SLSTCTL_H__

#endif	// WIN32

