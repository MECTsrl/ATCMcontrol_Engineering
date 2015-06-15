// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//  Author:             Justin Rudd
//  Description:        Declarations for SECListBar
//  Created:            September 1997


#ifdef WIN32

#ifndef __LISTBAR_H__
#define __LISTBAR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __SECBAR_H__
#include "toolkit\secbar.h"
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

//Forward Declarations
class SECShortcutListCtrl;

//@doc SECListBar 
//@class SECListBar is a specialized SECBar that contains the knowledge of how to insert and remove items from its associated CWnd.
// The CWnd will always be a SECShortcutListCtrl or a SECShortcutListCtrl derived class. This class adds simple one liner
// functions around the most commonly used list control functions, such as InsertItem, HitTest, and SetItemData.
//@comm See the DEMO sample in the \SAMPLES\TOOLKIT\STANDARD\TABS\LISTBAR directory for a demonstration of the SECListBar class.
//@base public | SECBar
//@xref<c SECBar> <c SECShortcutBar>
class SECListBar : public SECBar
{

	DECLARE_DYNCREATE( SECListBar )

public:
	//@Access Public Members
	//@Access Constructors/Destructors
	//@cmember
	/* Constructs the SECListBar object.*/
	SECListBar();

	//@cmember
	/* Constructs an SECListBar object from another SECListBar object.*/
	SECListBar( const SECListBar& src );

	//@cmember
	/* Destroys the SECListBar object.*/
	virtual ~SECListBar();

	//@Access Operators
	//@cmember
	/* Overloaded assignment operator for SECListBar objects.*/
	const SECListBar& operator=( const SECListBar& src );

	//@Access Attributes
	//@cmember
	/* Returns the associated SECShortcutListCtrl object.*/
	SECShortcutListCtrl* GetListCtrl();
	const SECShortcutListCtrl* GetListCtrl() const;

	virtual void SetWnd( const CWnd* const pWnd );
	virtual void DestroyWindow();
	virtual void DeleteWindow();

	//@cmember
	/* Sets the notification window.*/
	void SetNotifyWnd( CWnd* pWnd );

	//@cmember
	/* Returns the image list associated with this object.*/
	CImageList* GetImageList(int nImageList) const;

	//@cmember
	/* Associates an image list with the list bar object.*/
	CImageList* SetImageList(CImageList* pImageList, int nImageListType);

	//@cmember
	/* Returns a count of the items contained in the list bar.*/
	int GetItemCount() const;
	

	BOOL GetItem(LV_ITEM* pItem) const;
	BOOL SetItem(const LV_ITEM* pItem);
	
	CString GetItemText(int nItem, int nSubItem) const;
	CString GetItemText(int nItem) const;

	int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
	int GetItemText(int nItem, LPTSTR lpszText, int nLen) const;

	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	BOOL SetItemText(int nItem, LPCTSTR lpszText );
	
	//@cmember
	/* Sets the data for an item in the list bar.*/
	BOOL SetItemData(int nItem, DWORD dwData);

	//@cmember
	/* Gets the data for an item in the list bar */
	DWORD GetItemData(int nItem) const;
	
// Operations

	//@Access Public Operations
	//@cmember
	/* Appends an item to the list bar from the associated image list. */
	int AddItem( LPCTSTR lpszItem, int nImage );

	int InsertItem(const LV_ITEM* pItem);

	//@cmember
	/* Inserts an item into the list bar at a specific location.*/
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	
	//@cmember
	/* Removes an item from the list bar.*/
	BOOL DeleteItem(int nItem);

	//@cmember
	/* Deletes all the items in the list bar.*/
	BOOL DeleteAllItems();
	
	//@cmember
	/* todo Add a description*/
	int HitTest(LV_HITTESTINFO* pHitTestInfo) const;

	//@cmember
	/* Checks to see if the point is on the location as specified by the pFlags parameter.*/
	int HitTest(CPoint pt, UINT* pFlags = NULL) const;
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // #ifndef __LISTBAR_H__
#endif // #ifdef  WIN32

