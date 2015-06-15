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

#ifndef __TRVWX_H__
#define __TRVWX_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SEC_TREEBASEV_H__
#include "toolkit\trbaseV.h"
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
// AutoDuck tag block block for SECTreeView 
// adinsert AutoDuck insertion point for SECTreeView 
//@doc SECTreeView 
//@class SECTreeView | The Stingray enhanced tree view class. The implementation of this class is defined in the SEC_TREECLASS class .
//@base public | SEC_TREECLASS
//@xref <c SEC_TREECLASS>  <c SEC_LISTCLASS>

class SECTreeView : public SECTreeBaseV
{
	DECLARE_DYNCREATE(SECTreeView)

public:
	//@access Construction/Initialization
	//@cmember
	/* Construction*/
	SECTreeView();
	virtual ~SECTreeView();

	SECTreeView& GetTreeCtrl() const;


protected:
	DECLARE_MESSAGE_MAP()
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TRVWX_H__

#endif // WIN32

