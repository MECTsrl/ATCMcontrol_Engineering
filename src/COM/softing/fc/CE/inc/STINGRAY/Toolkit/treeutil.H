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
//  Author: Brad King

#ifdef WIN32

#ifndef __TREEUTIL_H__
#define __TREEUTIL_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
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


/////////////////////////////////////////////////////////////////
// subitem text mapping
struct _TREEITEM {};	// maps of HTREEITEMS won't work without this, 'cause it's not in the VC headers.

class SECSubItemTextStorage
{
public:
	BOOL ReorderItemAll(int nSubItemOld, int nSubItemNew);
	BOOL InsertItemAll(int nSubItemNew);
	typedef CMap<HTREEITEM, HTREEITEM&, CStringArray*, CStringArray*> mapSubItemText;

	SECSubItemTextStorage();
	virtual ~SECSubItemTextStorage();

	// operations
	BOOL AddOrModifyString( HTREEITEM hItem, int nSubItem, LPCTSTR lpszItem );
	BOOL GetString( HTREEITEM hItem,  int iSubItem, LPTSTR pszText, int cchTextMax ) const;
	void DeleteItem( HTREEITEM hItem );
	BOOL ReorderItems( HTREEITEM hItem, int nSubItemOld, int nSubItemNew);
protected:
	mapSubItemText m_mapItems;

};




//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __TREEUTIL_H__

#endif // WIN32
