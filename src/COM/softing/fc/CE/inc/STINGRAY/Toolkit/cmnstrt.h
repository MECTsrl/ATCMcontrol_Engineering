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

#ifndef __CMNSTRT_H__
#define __CMNSTRT_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifndef __TREENODE_H__
#include "toolkit\treenode.h"
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

#define TVIS_EX_DISABLED			0x0100
#define TVIS_EX_HIDDEN				0x0200
#define SEC_NO_IMAGE				(-12)	

#ifndef LVXC_UNINITIALIZED
#define LVXC_UNINITIALIZED (0xFFFFFFFF)
#endif

typedef struct tagLVITEMEX
{
	UINT maskEx;
	UINT stateEx;
	UINT stateExMask;
} LVITEMEX, FAR* LPLVITEMEX;

#define LV_ITEM_EX LVITEMEX


typedef struct tagTV_ITEM_EX
{
	UINT maskEx;
	UINT stateEx;
	UINT stateExMask;
} SEC_TVITEMEX, FAR* LPTV_ITEM_EX;

#define TV_ITEM_EX SEC_TVITEMEX


class SECCustomDragItem
{
public:
	SECCustomDragItem()
	{
		m_rect.SetRectEmpty();
		m_lastRect.SetRectEmpty();
		m_indent = 0;
	}

	CRect m_rect;
	CRect m_lastRect;
	int m_indent;
};


class SECListSubItem
{
public:
	LPTSTR pszText;
	int cchTextMax;
	CRect rcText;

public:
	SECListSubItem()
	{
		pszText = NULL;
		cchTextMax = 0;
		rcText.SetRectEmpty();
	}

	virtual ~SECListSubItem()
	{
		if (pszText != NULL && pszText != LPSTR_TEXTCALLBACK)
			delete[] pszText;
	}
};

class SECListItem
{

public:
    CPtrArray m_arrSubItems;
	LPTSTR pszText;
	UINT state;
	int iImage;
	LPARAM lParam;
	UINT m_cy;
	UINT stateEx;
	BOOL m_bDirty;
public:
	CRect rcText;
	CRect rcIcon;
	CRect rcStateIcon;

	SECListItem()
	{
		m_bDirty = TRUE;
		m_cy = 0;
		state = 0;
		iImage = 0;
		lParam = 0;
		stateEx = 0;
		pszText = NULL;
		rcText.SetRectEmpty();
		rcIcon.SetRectEmpty();
		rcStateIcon.SetRectEmpty();
	}

	virtual ~SECListItem()
	{
		if (pszText != NULL && pszText != LPSTR_TEXTCALLBACK)
			delete[] pszText;
	    while( GetSubItemCount() )
		{
	        VERIFY( DeleteSubItem(1) );
	    }
	}

	// SubItem data
	BOOL AddSubItem()
	{
		SECListSubItem* pSI = new SECListSubItem;
		pSI->pszText = LPSTR_TEXTCALLBACK;
		return m_arrSubItems.Add(pSI);
	}

	BOOL InsertSubItem( int iSubItem )
	{
		SECListSubItem* pSubItem = new SECListSubItem;
		pSubItem->pszText = LPSTR_TEXTCALLBACK;
		m_arrSubItems.InsertAt( iSubItem, pSubItem );
		return iSubItem;
	}

	BOOL DeleteSubItem(int iSubItem)
	{
	    SECListSubItem *pSubItem = GetSubItem(iSubItem);
	    delete pSubItem;
	    m_arrSubItems.RemoveAt((iSubItem-1));
	    return TRUE;
	}

	int GetSubItemCount() const
	{
		return (int)m_arrSubItems.GetSize();
	}

	SECListSubItem* GetSubItem(int iSubItem) const
	{
		SECListSubItem* pSubItem = NULL;

		if ((iSubItem-1) < m_arrSubItems.GetSize())
		{
			pSubItem = (SECListSubItem*)m_arrSubItems[(iSubItem-1)];
			ASSERT_POINTER(pSubItem, SECListSubItem);
		}

		return pSubItem;
	}

	BOOL SetItemData(DWORD dwData) { lParam = (LPARAM)dwData; return TRUE; }
	DWORD GetItemData() const { return (DWORD)lParam; }
};


/////////////////////////////
// AutoDuck tag block block for SECListColumn

// adinsert AutoDuck insertion point for SECListColumn
//@doc SECListColumn
//@class This class has all of the same members of an LV_COLUMN structure,
// with the exception of strText, which is a CString.
class SECListColumn
{
public:
	UINT mask;
	int fmt;
	int cx;
	CString	strText;
	int cchTextMax;
	int iSubItem;
	int iImage;

	SECListColumn()
	{
		mask = 0;
		fmt = 0;
		cx = 0;
		cchTextMax = 0;
		iSubItem = 0;
		iImage = -1;
	}
};

enum SECListSelectAction
{
	ByKeyboard,
	ByMouse,
	UnKnown
};

enum SECListDrawAction
{
	DrawEntire,
	DrawFocus,
	DrawSelect
};

class SECListPaintContext
{
public:
	LV_ITEM lvi;
	LV_ITEM_EX lvix;
	CDC* pdc;
	CRect rect;
	int iCol;
	DWORD dwViewStyle;
	SECListDrawAction da;
	CFont* pFont;
	SECListItem* pItem;
	SECListSubItem* pSubItem;
	SECListColumn* pCol;
	COLORREF rgbText;
	COLORREF rgbTextBkgnd;
	COLORREF rgbItemBkgnd;
	COLORREF rgbIconBkgnd;

	SECListPaintContext(CDC* pDC)
	{
		pdc = pDC;
		pItem = NULL;
		memset(&lvi, 0, sizeof(LV_ITEM));
		memset(&lvix, 0, sizeof(LV_ITEM_EX));
		iCol = 0;
		dwViewStyle = 0;
		da = DrawEntire;
		pFont = NULL;
		pItem = NULL;
		pSubItem = NULL;
		pCol = NULL;
		rgbText = 0;
		rgbTextBkgnd = 0;
		rgbItemBkgnd = 0;
		rgbIconBkgnd = 0;
	}
	virtual ~SECListPaintContext(){}
};

class _Node : public SECTreeNodeX
{
public:
	_Node()
	{
		memset( &m_item, 0, sizeof(m_item) );
		m_item.pszText = NULL;
		m_item.cChildren = 0;
		m_item.iImage = SEC_NO_IMAGE;
		m_item.iSelectedImage = SEC_NO_IMAGE;
		stateEx = 0;
		m_cyPreferred = 0;

	}
	virtual ~_Node()
	{
		if (m_item.pszText != NULL && m_item.pszText != LPSTR_TEXTCALLBACK)
			delete[] m_item.pszText;
	}

		
	// Attributes
public:
	TV_ITEM m_item;
	BOOL    m_bHangingIndent;
	CString	m_strBeforeIndent,
			m_strAfterIndent;       
	int		m_cxTextBeforeIndent;   
	UINT	stateEx;
	UINT	m_cyPreferred;

	// Operations
public:
	BOOL IsVisible()
	{
		//Assume visible until prooved otherwise
		_Node *pNode=this;
		if( stateEx & TVIS_EX_HIDDEN )
			return(FALSE);

		while( pNode->GetParent() )
		{
			pNode = (_Node *)pNode->GetParent();
			if( !pNode->IsExpanded() || (pNode->stateEx & TVIS_EX_HIDDEN) != 0)
				return FALSE;
		}

		return TRUE;
	}
	BOOL    IsDisabled() const
	{
		const _Node *pNode;
		for( pNode=this ; pNode ; pNode=(_Node *)pNode->GetParent() )
		{
			if( pNode->stateEx & TVIS_EX_DISABLED )
				return TRUE;
		}
		return FALSE;
	}
	BOOL    IsHidden() const
	{
		const _Node *pNode;
		for( pNode=this ; pNode ; pNode=(_Node *)pNode->GetParent() )
		{
			if( pNode->stateEx & TVIS_EX_HIDDEN)
				return TRUE;
		}
		return FALSE;
	}
	virtual BOOL IsExpanded() const
	{
		return (m_item.state & TVIS_EXPANDED) != 0;
	}
	virtual void Expand(BOOL bExpand = TRUE)
	{
		if (bExpand)
			m_item.state |= TVIS_EXPANDED;
		else
			m_item.state &= ~TVIS_EXPANDED;
	}
	virtual BOOL OnNextSearchNode( WORD idSearch, SECTreeNode *pNode )
	{
		idSearch; // UNUSED
		pNode; // UNUSED
		ASSERT(FALSE); 
		return FALSE;
	}
};
	
class SECTreePaintContext : public SECListPaintContext
{
public:
	TV_ITEM tvi;
	TV_ITEM_EX tvix;
	_Node* pNode;

	SECTreePaintContext(CDC* pDC) : SECListPaintContext(pDC)
	{
		pNode = NULL;
		memset(&tvi, 0, sizeof(TV_ITEM));
		memset(&tvix, 0, sizeof(TV_ITEM_EX));
	}
	virtual ~SECTreePaintContext(){}
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // __CMNSTRT_H__

#endif //WIN32