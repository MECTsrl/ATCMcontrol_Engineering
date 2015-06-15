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

#if defined(WIN32)


//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef UNDER_CE
#define ZeroMemory(Dest,size) memset((Dest),0,(size))
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#endif //UNDER_CE

#ifndef __SECAUX_H__
#include "toolkit\secaux.h"
#endif

#ifndef __TREEUTIL_H__
#include "toolkit\treeutil.h"
#endif

#ifndef __SLSTCTIP_H__
#include "toolkit\slstctip.h"
#endif

#include "toolkit\treenode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Registered window message for NotifyEndLabelEditChangeComplete
const UINT sec_rm_tree_notify_end_label_edit_change=RegisterWindowMessage(SEC_TREECTRL_NOTIFYENDLABELEDITCHG_REGMSG);

/////////////////////////////////////////////
//@doc SEC_TREECLASS
//@mfunc Construct an SEC_TREECLASS.

SEC_TREECLASS::SEC_TREECLASS()
{
	m_pNodeRoot	= NULL;
	InitRootNode();
	SetIndent(20);	
	m_bHideDisabledItems = FALSE;
	m_dwTreeStyle = 0;
	m_dwTreeStyleEx = 0;
	m_pSubItemTextStorage = NULL;
	m_idAutoExpandTimer = 0;
	m_nAutoExpandDelay = 1000;
	m_nMaxAnimations = 5;
	m_nAnimateCount = 0;
	m_bScrollOnExpand = TRUE;
	//  Since it is perfectly valid to set the image list before creating or attaching the
	//  tree control I have to OR in this style to comply with KB - Q131287
	m_dwListStyle |= LVS_SHAREIMAGELISTS|LVS_REPORT;
}


SEC_TREECLASS::~SEC_TREECLASS()
{
	if ( m_pSubItemTextStorage )
		delete m_pSubItemTextStorage;

	if( m_pNodeRoot )
		delete m_pNodeRoot;
}


//@doc SEC_TREECLASS
//@mfunc Creates an SEC_TREECLASS window.
//@syntax BOOL Create( DWORD dwStyle, const RECT& rect,	CWnd* pParentWnd, UINT uID,	CCreateContext* pContext=NULL)
//@syntax BOOL Create( DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd, UINT uID, CCreateContext* pContext=NULL)
//@rdesc Nonzero if the tree view control is successfully created; otherwise 0.
//@parm  DWORD | dwStyle | Specifies the tree view control’s style.  See Remarks below.
//@parm  DWORD | dwExStyle | Specifies the tree view control’s extended style.  See Remarks below.
//@parm  const RECT& | rect | Specifies the tree view control’s size and position.  It can be either a RECT structure or a CRect object.
//@parm  CWnd* | pParentWnd | Specifies the tree view control's parent window.  It must not be NULL.
//@parm  UINT | uID | Specifies the tree view control’s ID. 
//@parm  CCreateContext* | pContext | The create context of the window.
//@comm The following styles can be applied to a tree view control:
//
//@flag TVS_HASLINES | The tree view control has lines linking child items to their corresponding parent items.
//@flag TVS_LINESATROOT | The tree view control has lines linking child items to the root of the hierarchy.
//@flag	TVS_HASBUTTONS | The tree view control adds a button to the left of each parent item.
//@flag TVS_EDITLABELS | The tree view control allows the user to edit the labels of tree view items.
//@flag	TVS_SHOWSELALWAYS | Causes a selected item to remain selected when the tree view control loses focus.
//@flag TVS_DISABLEDRAGDROP | The tree view control is prevented from  sending TVN_BEGINDRAG notification messages.<nl>
//
//@normal The following extended styles are defined by SEC_TREECLASS:
//
//@flag TVXS_MULTISEL | Use with multiple selection enabled.  See also SECListCtrl::EnableMultiSelect.
//@flag	TVXS_WORDWRAP | Use with wordwrap enabled.  See also SECListCtrl::EnableWordWrap.
//@flag	TVXS_COLUMNHEADER | Use with column header control enabled.  See also SECListCtrl::EnableHeaderCtrl.
//@flag	TVXS_FLYBYTOOLTIPS | Use with tool tips enabled.  See also SECListCtrl::EnableToolTips.
//
//@end
BOOL SEC_TREECLASS::Create( DWORD dwStyle, const RECT& rect,	CWnd* pParentWnd, UINT uID,	CCreateContext* pContext /*=NULL*/)
{
	return Create(dwStyle, 0, rect, pParentWnd, uID, pContext);
}


BOOL SEC_TREECLASS::Create( DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd, UINT uID, CCreateContext* pContext )
{
	InsertColumn( 0, _T(""), LVCFMT_CENTER, 0, 0, -1, FALSE );
#ifndef UNDER_CE
	// If NT 3.51 or below, high 4 bits of extended style
	// will cause creation failure
	DWORD dwWindowsVer=(DWORD)(LOBYTE(LOWORD(GetVersion())));
	if(dwWindowsVer<4)
		dwExStyle&=0x0fffffff;
#else
	dwExStyle&=0x0fffffff;
#endif //UNDER_CE	
	BOOL bOk = SEC_TREEBASE::Create( dwStyle, dwExStyle, rect, pParentWnd, uID, pContext );
	ASSERT(bOk);

	return bOk;
}


void SEC_TREECLASS::ParseCreateStruct
	(
		CREATESTRUCT& cs
	)
{
	DWORD dwTreeStyle = cs.style & TVS_STYLES;
	cs.style &= ~TVS_STYLES;

	DWORD dwTreeExStyle = cs.dwExStyle & TVXS_STYLES;
	cs.dwExStyle &= ~TVXS_STYLES;

	ModifyTreeCtrlStyle( 0, dwTreeStyle, FALSE );
	ModifyTreeCtrlStyleEx( 0, dwTreeExStyle, FALSE );

	SEC_TREEBASE::ParseCreateStruct(cs);
}



//@doc SEC_TREECLASS
//@mfunc Create a SEC_TREECLASS on a dialog template.
//@rdesc Nonzero if the call was successful.
//@parm  UINT | idc | The resource ID of the control.
//@parm  CWnd* | pWndDlg | Pointer to the dialog box.
//@comm Call this function from OnInitDialog and pass the IDC of
//  the tree control as specified in the dialog editor.
BOOL SEC_TREECLASS::SubclassTreeCtrlId( UINT idc, CWnd* pWndDlg )
{
	CTreeCtrl *pMsTreeCtrl = (CTreeCtrl *)pWndDlg->GetDlgItem(idc);
	if( !pMsTreeCtrl )
	{
		ASSERT(FALSE);
		return(FALSE);
	}
	
	DWORD dwStyle=pMsTreeCtrl->GetStyle();

	DWORD dwExStyle = pMsTreeCtrl->GetExStyle();
	// if tooltips are not disabled for the Win32 control, then set the TVXS_FLYBYTOOLTIPS ex-style.
	if(!(dwStyle & TVS_NOTOOLTIPS))
		dwExStyle |= TVXS_FLYBYTOOLTIPS;
	
	// in addition to style, we need to place the new window
	// in the same z-order or tabs won't work correctly
	CWnd* pPrevWnd = NULL;
	pPrevWnd = pMsTreeCtrl->GetNextWindow( GW_HWNDPREV );

	CRect rect;
	pMsTreeCtrl->GetWindowRect(rect);
	pWndDlg->ScreenToClient(rect);
	pMsTreeCtrl->DestroyWindow();

	BOOL bOk = Create( dwStyle, dwExStyle, rect, pWndDlg, idc );

	if( !bOk )
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	// now set the z order of the new window
	if ( pPrevWnd )
		SetWindowPos( pPrevWnd, 0, 0, 0 ,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}



//@doc SEC_TREECLASS
//@mfunc Retrieves the bounding rectangle for an item and determines whether it is visible or not.
//@rdesc Nonzero if the item is visible, with the bounding rectangle contained in lpRect. Otherwise, 0 with lpRect uninitialized.
//@parm  HTREEITEM | hti | The handle of a tree view control item.
//@parm  LPRECT | lpRect | Pointer to a RECT structure that receives the bounding rectangle.  The coordinates are relative to the upper-left corner of the tree view control.
//@parm  UINT | nCode | Portion of the tree view item for which to retrieve the bounding rectangle. It can be one of these values:
//		LVIR_BOUNDS	Returns the bounding rectangle of the entire item, including the icon and label.
//		LVIR_ICON	Returns the bounding rectangle of the icon or small icon.
//		LVIR_LABEL	Returns the bounding rectangle of the item text.
//@comm Call this function to retrieve the bounding rectangle and determine whether it is visible or not.
BOOL SEC_TREECLASS::GetItemRect( HTREEITEM hti, LPRECT lpRect, UINT nCode ) const
{
	int nItem = ItemIndex(hti);
	return SEC_TREEBASE::GetItemRect(nItem, lpRect, nCode);
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the amount, in pixels, that child items are indented relative to their parents.
//@rdesc The amount of indentation measured in pixels.
//@comm Call this function to retrieve the amount, in pixels, that child items are indented relative to their parent items.
UINT SEC_TREECLASS::GetIndent() const
{
	return m_nIndent;
}


//@doc SEC_TREECLASS
//@mfunc Sets the width of indentation for a tree view control and redraws the control to reflect the new width.
//@rdesc void 
//@parm UINT | nIndent | Width, in pixels, of the indentation. If nIndent is less than the system-defined minimum width, the new width is set to the system-defined minimum
//@comm Call this function to set the width of indentation for a tree view control and redraw the control to reflect the new width.
void SEC_TREECLASS::SetIndent(UINT nIndent)
{
	if ( (nIndent%2) != 0 )
	{
		// The tree indention must be even because we have a dotted
		// pen where every other pixel is set.  Unless the indention
		// is even, the hierarchy lines will appear blotchy.
		TRACE(_T("SECTreeCtrl indention is not an even number, and will be incremented.\n"));
		nIndent++;
	}
	m_nIndent=nIndent;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the next tree view item.
//@rdesc The handle of the next item if successful; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@parm  UINT | nCode | A flag indicating the type of relation to hItem.  This flag can be one of the following values:
//	TVGN_CARET - retrieves the currently selected item.
//	TVGN_CHILD - retrieves the first child item.  The hItem parameter must be NULL.
//	TVGN_DROPHILITE - retrieves the item that is the target of a drag-and-drop operation.
//	TVGN_FIRSTVISIBLE - retrieves the first visible item.
//
//	TVGN_NEXT - retrieves the next sibling item.
//	TVGN_NEXTVISIBLE - retrieves the next visible item that follows the specified item.
//	TVGN_PARENT - retrieves the parent of the specified item.
//	TVGN_PREVIOUS - retrieves the previous sibling item.
//	TVGN_PREVIOUSVISIBLE - retrieves the first visible item that precedes the specified item.
//	TVGN_ROOT - retrieves the first child item of the root item of which the specified item is a part.
//
//	TVGN_SELECTED - gets next selected item if more than one.
//	TVGN_FIRSTSELECTED - retrieves the first selected item.
//	TVGN_NEXTSELECTED - retrieves the next selected item.
//	TVGN_PREVIOUSSELECTED - retrieves the previous selected item.
//@comm Call this function to retrieve the tree view item that has the specified relationship, indicated by the nCode parameter, to hItem.
HTREEITEM SEC_TREECLASS::GetNextItem(HTREEITEM hti, UINT nCode) const
{
	switch( nCode )
	{
	case TVGN_NEXT:
		return GetNextSiblingItem(hti);
	case TVGN_PREVIOUS:
		return GetPrevSiblingItem(hti);
	case TVGN_CHILD:
		return GetChildItem(hti);
	case TVGN_PARENT:
		return GetParentItem(hti);
	case TVGN_ROOT:
		return GetRootItem(hti);
	case TVGN_CARET:
		return GetCaretItem();
	case TVGN_FIRSTVISIBLE:
		return GetFirstVisibleItem();
	case TVGN_NEXTVISIBLE:
		return GetNextVisibleItem(hti);
	case TVGN_PREVIOUSVISIBLE:
		return GetPrevVisibleItem(hti);
	case TVGN_DROPHILITE:
		return GetDropHilightItem();
	case TVGN_FIRSTSELECTED:
		return GetFirstSelectedItem();
	case TVGN_NEXTSELECTED:
		return GetNextSelectedItem(hti);
	case TVGN_PREVIOUSSELECTED:
		return GetPrevSelectedItem(hti);
	default:
		ASSERT(FALSE);
		return NULL;
	}
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the tree view item that is the child of the item specified.
//@rdesc The handle of the child item if successful; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to retrieve the tree view item that is the child of the item specified by hItem.
HTREEITEM SEC_TREECLASS::GetChildItem(HTREEITEM hti) const
{
	SECTreeNode *pNode;

	if( hti==TVI_ROOT || !hti)
		pNode = m_pNodeRoot;
	else
		pNode = GetNode(hti);

	ASSERT(pNode);
	
	return (HTREEITEM)pNode->GetFirstChild();
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the next sibling of the item specified.
//@rdesc The handle of the next sibling item; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to retrieve the next sibling of hItem.
HTREEITEM SEC_TREECLASS::GetNextSiblingItem(HTREEITEM hti) const
{
	SECTreeNode *pNode;

	if( hti==TVI_ROOT || !hti)
		pNode = m_pNodeRoot;
	else
		pNode = GetNode(hti);

	ASSERT(pNode);
	
	return (HTREEITEM)pNode->GetNextSibling();
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the previous sibling of the item specified.
//@rdesc The handle of the previous sibling item; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to retrieve the previous sibling of hItem.
HTREEITEM SEC_TREECLASS::GetPrevSiblingItem(HTREEITEM hti) const
{
	SECTreeNode *pNode;

	if( hti==TVI_ROOT || !hti)
		pNode = m_pNodeRoot;
	else
		pNode = GetNode(hti);

	ASSERT(pNode);
	
	return (HTREEITEM)pNode->GetPrevSibling();
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the parent of the item specified.
//@rdesc The handle of the parent item; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to retrieve the parent of hItem. If the parent is TVI_ROOT, a NULL will be returned.
HTREEITEM SEC_TREECLASS::GetParentItem(HTREEITEM hti) const
{
	SECTreeNode *pNode;

	if( hti==TVI_ROOT || !hti)
		pNode = m_pNodeRoot;
	else
		pNode = GetNode(hti);

	ASSERT(pNode);
	
	if( pNode->GetParent() == m_pNodeRoot )
		return NULL;
	else
		return (HTREEITEM)pNode->GetParent();
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the first visible item of the tree view control.
//@rdesc The handle of the first visible item; otherwise NULL.
//@comm Call this function to retrieve the first visible item of the tree view control.
HTREEITEM SEC_TREECLASS::GetFirstVisibleItem() const
{
	// Retrieves the first visible item.
	Node* pNode = NULL;
	int nNext = GetTopIndex();
	if (nNext != -1)
		pNode = GetNodeAt(nNext);
	return (HTREEITEM)pNode;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the last visible item of the tree view control.
//@rdesc The handle of the last visible item; otherwise NULL.
//@comm Call this function to retrieve the last visible item of the tree view control.
HTREEITEM SEC_TREECLASS::GetLastVisibleItem() const
{
	int iLast = SEC_TREEBASE::GetItemCount();
    if( iLast<0 )
        return NULL;
	return (HTREEITEM)GetNodeAt(iLast-1);
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the next visible item after the item specified.
//@rdesc The handle of the next visible item; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to retrieve the next visible item of hItem.
HTREEITEM SEC_TREECLASS::GetNextVisibleItem(HTREEITEM hti) const
{
	int nItem = ItemIndex(hti);

	if (nItem >= 0)
	{
		// Optimize the case when the given item is visible.
		// In this case, the next item is simply the next item
		// in the list.
		nItem++;
		if (nItem < GetItemCount())
			return (HTREEITEM)GetNodeAt(nItem);
	}
	else
	{
		// Given item is not in the list control.  So, we have
		// to find the nearest neighbor that is.
		Node *pNode;
		
		pNode = GetNode(hti);
		
		if( pNode )
		{
			pNode = (Node *)pNode->GetNextInDisplayOrder();
			for( ; pNode ; pNode=(Node *)pNode->GetNextInDisplayOrder() )
			{
				if( pNode->IsVisible() )
				{
					return (HTREEITEM)pNode;
				}
			}
		}
	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the previous visible item to the item specified.
//@rdesc The handle of the previous visible item; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to retrieve the previous visible item of hItem.
HTREEITEM SEC_TREECLASS::GetPrevVisibleItem(HTREEITEM hti) const
{
	if(! (hti==TVI_ROOT || hti==(HTREEITEM)m_pNodeRoot) )
	{
		int nItem = ItemIndex(hti);
		if (nItem >= 0)
		{
			// Optimize the case when the given item is visible.
			// In this case, the prev item is simply the prev item
			// in the list.
			nItem--;
			if (nItem != -1)
				return (HTREEITEM)GetNodeAt(nItem);
		}
		else
		{
			// Given item is not in the list control.  So, we have
			// to find the nearest neighbor that is.
			Node *pNode = GetNode(hti);
			while( (pNode=(Node *)pNode->GetPrevInDisplayOrder()) != NULL )
			{
				if( pNode->IsVisible())
				{
					return (HTREEITEM)pNode;
				}
			}
		}
	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the first item currently selected in the tree view control.
//@rdesc The handle of the first item currently selected; otherwise NULL.
//@comm Retrieves the first item currently selected in the tree view control.
HTREEITEM SEC_TREECLASS::GetFirstSelectedItem() const
{
	Node* pNode = NULL;
	int nIndex = SEC_TREEBASE::GetNextItem(-1, LVNI_SELECTED|LVNI_BELOW);
	if (nIndex != -1)
		pNode = GetNodeAt(nIndex);
	return (HTREEITEM)pNode;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the next item currently selected in the tree view control.
//@rdesc The handle of the next item currently selected; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Retrieves the next item currently selected in the tree view control.
HTREEITEM SEC_TREECLASS::GetNextSelectedItem(HTREEITEM hti) const
{
	int nItem = ItemIndex(hti);
	if (nItem >= 0)
	{
		nItem = SEC_TREEBASE::GetNextItem(nItem, LVNI_SELECTED|LVNI_BELOW);
		if (nItem != -1)
			return (HTREEITEM)GetNodeAt(nItem);
	}
	else
	{
		// Given item is not in the list control.  So, we have
		// to find the nearest neighbor that is.
		Node *pNode = GetNode(hti);
		while( (pNode=(Node *)pNode->GetPrevInDisplayOrder()) != NULL )
		{
			if( pNode->IsVisible())
			{
				return GetNextSelectedItem((HTREEITEM)pNode);
				break;
			}
		}

	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the previous item currently selected in the tree view control.
//@rdesc The handle of the previous item currently selected; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Retrieves the previous item currently selected in the tree view control.
HTREEITEM SEC_TREECLASS::GetPrevSelectedItem(HTREEITEM hti) const
{
	int nItem = ItemIndex(hti);
	if (nItem >= 0)
	{
		nItem = SEC_TREEBASE::GetNextItem(nItem, LVNI_SELECTED|LVNI_ABOVE);
		if (nItem != -1)
			return (HTREEITEM)GetNodeAt(nItem);
	}
	else
	{
		// Given item is not in the list control.  So, we have
		// to find the nearest neighbor that is.
		Node *pNode;
		
		pNode = GetNode(hti);
		
		if( pNode )
		{
			pNode = (Node *)pNode->GetNextInDisplayOrder();
			for( ; pNode ; pNode=(Node *)pNode->GetNextInDisplayOrder() )
			{
				if( pNode->IsVisible() )
				{
					return GetPrevSelectedItem((HTREEITEM)pNode);
					break;
				}
			}
		}
	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the item that currently has the caret (i.e., keyboard focus).
//@rdesc The handle of the item that currently has the caret; otherwise NULL.
//@comm Call this function to retrieve the item that has the keyboard focus.
HTREEITEM SEC_TREECLASS::GetCaretItem() const
{
	//Retrieves the caret item.
    int nIndex = GetFocusIndex();

	if( nIndex == -1 )
		return NULL; 

	return (HTREEITEM)GetNodeAt(nIndex);
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the currently selected item of the tree view control.
//@rdesc The handle of the selected item; otherwise NULL.
//@comm In single select mode, returns the selected item. 
// In multi-select mode returns the focus item. This function calls GetCaretItem().
HTREEITEM SEC_TREECLASS::GetSelectedItem() const
{
	return GetCaretItem();
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the item that is the target of a drag-and-drop operation.
//@rdesc The handle of the current drop target if successful; otherwise NULL.
//@comm Call this function to retrieve the item that is the target of a drag-and-drop operation, as set by SelectDropTarget.
//@xref <mf SEC_TREECLASS::SelectDropTarget>
HTREEITEM SEC_TREECLASS::GetDropHilightItem() const
{
    //Retrieves the item that is the target of a drag-and-drop operation.
	Node* pNode = NULL;
	int nIndex = SEC_TREEBASE::GetDropHilightIndex();
	if (nIndex != -1)
		pNode = GetNodeAt(nIndex);
	return (HTREEITEM)pNode;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the root item of the entire tree view control, or a specified item.
//@rdesc The handle of the root item; otherwise NULL.
//@parm HTREEITEM | hti | Handle of a tree item to find the root of; default is NULL.
//@comm Retrieves the root level item of which the specified item is a part.
HTREEITEM SEC_TREECLASS::GetRootItem(HTREEITEM hti) const
{
	SECTreeNode *pNode;

	//Retrieves the first child item of the root item of which the specified item is a part. 
	if( hti==TVI_ROOT || !hti)
		pNode = m_pNodeRoot;
	else
		pNode = GetNode(hti);

	ASSERT(pNode);
	
	if( pNode != m_pNodeRoot )
	{
		while (pNode && pNode->GetParent() != m_pNodeRoot)
			pNode = pNode->GetParent();
		return (HTREEITEM)pNode->GetFirstChild();
	}
	else
		return (HTREEITEM)m_pNodeRoot->GetFirstChild();
}


//@doc SEC_TREECLASS
//@mfunc Gets the next displayed item.
//@rdesc The handle of the next visible item; otherwise NULL;
//@parm HTREEITEM | hti | Handle of an item in the tree.
//@comm Gets the next displayed item after hti.
HTREEITEM SEC_TREECLASS::GetNextItemInDisplayOrder(HTREEITEM hti) const
{
	Node *pNode=GetNode(hti);
	ASSERT(pNode);
    return (HTREEITEM) pNode->GetNextInDisplayOrder();
}


//@doc SEC_TREECLASS
//@mfunc Gets the previous displayed item.
//@rdesc The handle of the previous visible item; otherwise NULL;
//@parm HTREEITEM | hti | Handle of an item in the tree.
//@comm Gets the previous displayed item before hti.
HTREEITEM SEC_TREECLASS::GetPrevItemInDisplayOrder(HTREEITEM hti) const
{
	Node *pNode=GetNode(hti);
	ASSERT(pNode);
	SECTreeNode* pNodeRet = pNode->GetPrevInDisplayOrder();
	if( pNodeRet == m_pNodeRoot )
		return (HTREEITEM)NULL;

	return (HTREEITEM)pNodeRet;
}



//@doc SEC_TREECLASS
//@mfunc Retrieves the attributes of the specified item.
//@syntax GetItem(TV_ITEM* pTVI, BOOL bCopyText, BOOL bGetDispInfo)
//@syntax GetItem(LV_ITEM* pLVI, BOOL bCopyText, BOOL bGetDispInfo)
//@rdesc Nonzero if successful; otherwise NULL.
//@parm TV_ITEM* | pTVI | A pointer to a TV_ITEM structure.
//@parm LV_ITEM* | pLVI | A pointer to an LV_ITEM structure.
//@parm  BOOL | bCopyText | If TRUE, the text for the item will be copied into your structure, otherwise the string pointer will be copied. Default is TRUE.
//@parm  BOOL | bGetDispInfo | If current item is a callback item, and this flag is set to TRUE, the tree view control sends the parent window a TVN_GETDISPINFO notification message.  By default, this flag is set to FALSE.
//@comm Call this function to retrieve the attributes of the specified tree view item.
// The first version of this function passes a pointer to a TV_ITEM structure.
// Note that the second version of this function is virtual, passing a pointer to an LV_ITEM structure.
// See the MFC documentation for complete information on these structures.
BOOL SEC_TREECLASS::GetItem(TV_ITEM* pTVI, BOOL bCopyText, BOOL bGetDispInfo) const
{
	Node *pNode = 0;
	UINT cbMask;

	pNode = GetNode(pTVI->hItem);
	if ( !pNode )
		return FALSE;

	cbMask = 0;

	if (pTVI->mask & TVIF_CHILDREN)
	{
		pTVI->cChildren = pNode->m_item.cChildren;
		if (pTVI->cChildren == I_CHILDRENCALLBACK)
			cbMask |= TVIF_CHILDREN;
	}

	if (pTVI->mask & TVIF_IMAGE)
	{
		pTVI->iImage = pNode->m_item.iImage;
		if (pTVI->iImage == I_IMAGECALLBACK)
			cbMask |= TVIF_IMAGE;
	}
	
	if (pTVI->mask & TVIF_PARAM)
		pTVI->lParam = pNode->m_item.lParam;

	if (pTVI->mask & TVIF_SELECTEDIMAGE)
	{
		pTVI->iSelectedImage = pNode->m_item.iSelectedImage;
		if (pTVI->iSelectedImage == I_IMAGECALLBACK)
			cbMask |= TVIF_SELECTEDIMAGE;
	}

	if (pTVI->mask & TVIF_STATE)
		pTVI->state = (pNode->m_item.state & pTVI->stateMask);

	if (pTVI->mask & TVIF_TEXT)
	{
		if (pNode->m_item.pszText == LPSTR_TEXTCALLBACK)
		{
			cbMask |= TVIF_TEXT;
			if (!bGetDispInfo)
				pTVI->pszText = pNode->m_item.pszText;
		}
		else if (bCopyText)
			_tcsncpy(pTVI->pszText, pNode->m_item.pszText, pTVI->cchTextMax);
		else
			pTVI->pszText = pNode->m_item.pszText;
	}

	if (bGetDispInfo && (cbMask != 0))
	{
		// Is it reasonable to say that if they have a callback mask,
		// they will want the LPARAM value?
		pTVI->lParam = pNode->m_item.lParam;
		pTVI->mask = cbMask;
		NotifyGetDispInfo(pTVI);
	}

	return TRUE;
}


BOOL SEC_TREECLASS::GetItem(LV_ITEM* pLVI, BOOL bCopyText, BOOL bGetDispInfo) const
{

	if (pLVI->iSubItem == 0)
	{
		if ( pLVI->iItem == -1)  // must have valid item spec
			return FALSE;

		TV_ITEM tvi;
		UINT stateMask;
		UINT mask;

		memset(&tvi, 0, sizeof(TV_ITEM));

		tvi.mask |= TVIF_HANDLE;
		tvi.hItem = GetItemAt(pLVI->iItem);

		//if there are no items in the tree, then return
		if(tvi.hItem == NULL)
			return FALSE;

		//ASSERT(tvi.hItem != NULL); // must have valid item

		// We're about to modify "lvi", so we need to save the
		// original masks so we can restore them before returning.
		mask = pLVI->mask;
		stateMask = pLVI->stateMask;

		if (pLVI->mask & LVIF_STATE)
		{
			pLVI->state = 0;
			tvi.mask |= TVIF_STATE;
			if (pLVI->stateMask & LVIS_CUT)
			{
				tvi.stateMask |= TVIS_CUT;
				pLVI->stateMask &= ~LVIS_CUT;
			}
			if (pLVI->stateMask & LVIS_DROPHILITED)
			{
				tvi.stateMask |= TVIS_DROPHILITED;
				pLVI->stateMask &= ~LVIS_DROPHILITED;
			}
			if (pLVI->stateMask & LVIS_OVERLAYMASK)
			{
				tvi.stateMask |= TVIS_OVERLAYMASK;
				pLVI->stateMask &= ~LVIS_OVERLAYMASK;
			}
			if (pLVI->stateMask & LVIS_SELECTED)
			{
				tvi.stateMask |= TVIS_SELECTED;
				pLVI->stateMask &= ~LVIS_SELECTED;
			}
			if (pLVI->stateMask & LVIS_STATEIMAGEMASK)
			{
				tvi.stateMask |= TVIS_STATEIMAGEMASK;
				pLVI->stateMask &= ~LVIS_STATEIMAGEMASK;
			}
			if (pLVI->stateMask == 0)
				pLVI->mask &= ~LVIF_STATE;
		}

		if (pLVI->mask & LVIF_TEXT)
		{
			tvi.mask |= TVIF_TEXT;
			tvi.pszText = pLVI->pszText;
			tvi.cchTextMax = pLVI->cchTextMax;
			pLVI->mask &= ~LVIF_TEXT;
		}

		if (pLVI->mask & LVIF_IMAGE)
		{
			tvi.mask |= TVIF_IMAGE;
			tvi.iImage = pLVI->iImage;
			pLVI->mask &= ~LVIF_IMAGE;
		}

		if (!GetItem(&tvi, bCopyText, bGetDispInfo))
			return FALSE;

		if (pLVI->mask != 0)
		{
			// If there are any fields or state flags present in LV_ITEM
			// but not in TV_ITEM (i.e. LVIS_FOCUSED), call the SEC_TREEBASE
			// class to fill in the list view only attributes.
			if (!SEC_TREEBASE::GetItem(pLVI, bCopyText, bGetDispInfo))
				return FALSE;
		}

		// Next, we translate TV_ITEM settings to LV_ITEM settings.
		// Some of the TV_ITEM fields have direct equivalents in the
		// LV_ITEM used by the SEC_TREEBASE class.  For example, the state
		// TV_ITEM member has a TVIS_SELECTED flag which corresponds to
		// LV_ITEM's LVIS_SELECTED flag. 

		pLVI->mask = mask;
		pLVI->stateMask = stateMask;
		if (tvi.mask & TVIF_STATE)
		{
			if (tvi.stateMask & TVIS_CUT)
				pLVI->state |= (tvi.state & TVIS_CUT) ? LVIS_CUT : 0;
			if (tvi.stateMask & TVIS_DROPHILITED)
				pLVI->state |= (tvi.state & TVIS_DROPHILITED) ? LVIS_DROPHILITED : 0;
			if (tvi.stateMask & TVIS_OVERLAYMASK)
				pLVI->state |= (tvi.state & TVIS_OVERLAYMASK) ? LVIS_OVERLAYMASK : 0;
			if (tvi.stateMask & TVIS_SELECTED)
				pLVI->state |= (tvi.state & TVIS_SELECTED) ? LVIS_SELECTED : 0;
			if (tvi.stateMask & TVIS_STATEIMAGEMASK)
				pLVI->state |= INDEXTOSTATEIMAGEMASK(GET_STATE_IMAGE(tvi.state));
		}

		if (tvi.mask & TVIF_TEXT)
			pLVI->pszText = tvi.pszText;

		if (tvi.mask & TVIF_IMAGE)
			pLVI->iImage = tvi.iImage;

		return TRUE;
	}
	else
		return SEC_TREEBASE::GetItem(pLVI, bCopyText, bGetDispInfo);
}




//@doc SEC_TREECLASS
//@mfunc Sets the extended attributes of the specified item.
//@syntax SetItemEx(TV_ITEM* pTVI, TV_ITEM_EX* pTVIX)
//@syntax SetItemEx(LV_ITEM* pLVI, const LV_ITEM_EX* pLVIX)
//@rdesc Nonzero if successful; otherwise NULL.
//@parm TV_ITEM* | pTVI | A pointer to a TV_ITEM structure. The hItem member must be set to the desired item before calling.
//@parm TV_ITEM_EX* | pTVIX | A pointer to a TV_ITEM_EX structure. Set the maskEx, stateExMask and stateEx members as desired before calling.
//@parm LV_ITEM* | pLVI | A pointer to an LV_ITEM structure. The iItem and iSubitem members must be set before calling.
//@parm const LV_ITEM_EX* | pLVIX | A pointer to an LV_ITEM_EX structure.Set the maskEx, stateExMask and stateEx members as desired before calling.
//@comm Call this function to set the extended attributes of the specified tree view item.
BOOL SEC_TREECLASS::SetItemEx(TV_ITEM* pTVI, TV_ITEM_EX* pTVIX)
{
	Node *pNode;
	UINT cbMask;

	pNode = GetNode(pTVI->hItem);
	cbMask = 0;

	if (pTVIX->maskEx & TVIF_EX_STATEEX)
	{
		pNode->stateEx &= ~(pTVIX->stateExMask);
		pNode->stateEx |= (pTVIX->stateEx & pTVIX->stateExMask);
	}

	if (cbMask != 0)
	{
		TV_ITEM_EX copy;
		memmove(&copy, pTVIX, sizeof(TV_ITEM_EX));
		copy.maskEx = cbMask;
		NotifySetDispInfoEx(&copy);
	}

	int iItem;
	iItem = ItemIndex(pTVI->hItem);
	if (iItem != -1)
	{
		LV_ITEM lvi;
		LV_ITEM_EX lvix;

		memset(&lvi, 0, sizeof(LV_ITEM));
		memset(&lvix, 0, sizeof(LV_ITEM_EX));

		lvi.iItem = iItem;
		lvi.iSubItem = 0;

		if (pTVIX->maskEx & TVIF_EX_STATEEX)
		{
			lvix.maskEx |= LVIF_EX_STATEEX;
			if( pTVIX->stateExMask & TVIS_EX_DISABLED )
			{
				lvix.stateEx |= (pTVIX->stateEx & TVIS_EX_DISABLED) ? LVIS_EX_DISABLED : 0;
				lvix.stateExMask |= LVIS_EX_DISABLED;
			}

			if( pTVIX->stateExMask & TVIS_EX_HIDDEN )
			{
				lvix.stateEx |= (pTVIX->stateEx & TVIS_EX_HIDDEN) ? LVIS_EX_HIDDEN : 0;
				lvix.stateExMask |= LVIS_EX_HIDDEN;
			}
		}

		if (!SEC_TREEBASE::SetItemEx(&lvi, &lvix))
			return FALSE;
	}

	return TRUE;
}


BOOL SEC_TREECLASS::SetItemEx(LV_ITEM* pLVI, const LV_ITEM_EX* pLVIX)
{
	if (pLVI->iSubItem == 0 && (pLVI->mask & LVIF_TEXT))
	{
		TV_ITEM tvi;
		TV_ITEM_EX tvix;

		tvi.mask = TVIF_HANDLE;
		tvi.hItem = GetItemAt(pLVI->iItem);
		tvix.maskEx = TVIF_EX_STATEEX;
		tvix.stateEx = pLVIX->stateEx & pLVIX->stateExMask;

		if (!SetItemEx(&tvi, &tvix))
			return FALSE;

		return TRUE;
	}
	else
		return SEC_TREEBASE::SetItemEx(pLVI, pLVIX);
}



//@doc SEC_TREECLASS
//@mfunc Retrieves the extended attributes of the specified item.
//@syntax GetItemEx(TV_ITEM* pTVI, TV_ITEM_EX* pTVIX, BOOL bGetDispInfoEx)
//@syntax GetItemEx(LV_ITEM* pLVI, LV_ITEM_EX* pLVIX, BOOL bGetDispInfoEx)
//@rdesc Nonzero if successful; otherwise NULL.
//@parm TV_ITEM* | pTVI | A pointer to a TV_ITEM structure. The hItem member must be set to the desired item before calling.
//@parm  TV_ITEM_EX* | pTVIX | A pointer to a TV_ITEM_EX structure that will hold the return values.
//@parm LV_ITEM* | pLVI | A pointer to an LV_ITEM structure. The iItem and iSubitem members must be set before calling.
//@parm  LV_ITEM_EX* | pLVIX | A pointer to an LV_ITEM_EX structure that will hold the return values.
//@parm  BOOL | bGetDispInfoEx | If current item is a callback item, and this flag is set to TRUE, the tree view control sends the parent window a TVN_GETDISPINFO notification message.  By default, this flag is set to FALSE.
//@comm Call this function to retrieve the extended attributes of the specified tree view item.
BOOL SEC_TREECLASS::GetItemEx(TV_ITEM* pTVI, TV_ITEM_EX* pTVIX, BOOL bGetDispInfoEx) const
{
	Node *pNode;
	UINT cbMask;

	pNode = GetNode(pTVI->hItem);
	cbMask = 0;

	if (pTVIX->maskEx & TVIF_EX_STATEEX)
	{
		pTVIX->stateEx = 0;
		if (GetCallbackMaskEx() != 0)
			cbMask |= TVIF_EX_STATEEX;
		else 
		{
			if ((pTVIX->stateExMask & TVIS_EX_DISABLED) && pNode->IsDisabled())
				pTVIX->stateEx |= TVIS_EX_DISABLED;
			if ((pTVIX->stateExMask & TVIS_EX_HIDDEN) && pNode->IsHidden())
				pTVIX->stateEx |= TVIS_EX_HIDDEN;
		}
	}

	if (bGetDispInfoEx && cbMask != 0)
	{
		pTVIX->maskEx = cbMask;
		NotifyGetDispInfoEx(pTVIX);
	}

	return TRUE;
}


BOOL SEC_TREECLASS::GetItemEx(LV_ITEM* pLVI, LV_ITEM_EX* pLVIX, BOOL bGetDispInfoEx) const
{
	if (pLVI->iSubItem == 0)
	{
		TV_ITEM tvi;
		TV_ITEM_EX tvix;

		memset(&tvi, 0, sizeof(TV_ITEM));
		memset(&tvix, 0, sizeof(TV_ITEM_EX));

		tvi.mask |= TVIF_HANDLE;
		ASSERT(pLVI->iItem >= 0);  // must have valid item spec
		tvi.hItem = GetItemAt(pLVI->iItem);
		ASSERT(tvi.hItem != NULL); // must have valid item

		if (pLVIX->maskEx & LVIF_EX_STATEEX)
		{
			tvix.maskEx = TVIF_EX_STATEEX;
			if (pLVIX->stateExMask & LVIS_EX_DISABLED)
				tvix.stateEx |= TVIS_EX_DISABLED;
			if (pLVIX->stateExMask & LVIS_EX_HIDDEN)
				tvix.stateEx |= TVIS_EX_HIDDEN;
		}

		if (!GetItemEx(&tvi, &tvix, bGetDispInfoEx))
			return FALSE;

		if (tvix.maskEx & TVIF_EX_STATEEX)
		{
			if (tvix.stateExMask & TVIS_EX_DISABLED)
				pLVIX->stateEx |= (tvix.stateEx & TVIS_EX_DISABLED) ? LVIS_EX_DISABLED : 0;
			if (tvix.stateExMask & TVIS_EX_HIDDEN)
				pLVIX->stateEx |= (tvix.stateEx & TVIS_EX_HIDDEN) ? LVIS_EX_HIDDEN : 0;
		}

		return TRUE;
	}
	else
		return SEC_TREEBASE::GetItemEx(pLVI, pLVIX);
}



//@doc SEC_TREECLASS
//@mfunc Retrieves the index of the item’s image and its selected image within the tree view control’s image list.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hItem | Handle of the item whose image is to be retrieved.
//@parm  int& | nImage | An integer that receives the index of the item’s image within the tree view control’s image list.
//@parm  int& | nSelectedImage | An integer that receives the index of the item’s selected image within the tree view control’s image list.
//@comm Each item in a tree view control can have a pair of bitmapped images associated with it.  The images appear on the left side of an item’s label.  One image is displayed when the item is selected, and the other is displayed when the item is not selected.  For example, an item might display an open folder when it is selected and a closed folder when it is not selected.  Call this function to retrieve the index of the item’s image and its selected image within the tree view control’s image list.
BOOL SEC_TREECLASS::GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage) const
{
	Node *pNode= GetNode(hItem);
	if( !pNode )
		return FALSE;
	
	nImage         = pNode->m_item.iImage;
	nSelectedImage = pNode->m_item.iSelectedImage;
	
	return TRUE;
}


//Used by SECListTip for displaying tooltip for items with TVIS_BOLD
UINT SEC_TREECLASS::GetStateFromItemIndex(int nItem, UINT nStateMask) const
{
	HTREEITEM hItem = GetItemAt(nItem);	
	if(!hItem)
		return FALSE;

	return GetItemState(hItem,nStateMask);
}


//@doc SEC_TREECLASS
//@mfunc Returns the state of the item specified.
//@rdesc A UINT specifying the item’s state. For information on possible values, see SEC_TREECLASS::GetItem.
//@parm HTREEITEM | hti | Handle of the item whose state is to be retrieved.
//@parm  UINT | nStateMask | Mask indicating which states are to be retrieved. For more information on possible values for nStateMask, see the discussion of the state and stateMask members of the TV_ITEM structure in SEC_TREECLASS::GetItem.
//@comm Returns the state of the item specified by hItem.
UINT SEC_TREECLASS::GetItemState(HTREEITEM hti, UINT nStateMask) const
{
	Node *pNode= GetNode(hti);
	if( !pNode )
		return FALSE;

	return (pNode->m_item.state & nStateMask);
}


//@doc SEC_TREECLASS
//@mfunc Gets the 32-bit application-specific value associated with the item specified.
//@rdesc The data for the item.
//@parm HTREEITEM | hti | Handle of the item whose data is to be retrieved.
//@comm Returns data associated with the item.
DWORD SEC_TREECLASS::GetItemData(HTREEITEM hti) const
{

	Node *pNode= GetNode(hti);
	if( !pNode )
		return FALSE;
	
	return pNode->m_item.lParam;
}


BOOL SEC_TREECLASS::BuildTreeNode(TV_ITEM* pTVI, Node* pNode, UINT* pMask /* = NULL */ )
{
	// this routine was separated out from SetItem for performance reasons.
	// When Inserting a new item, calling SetItem takes a lot of time because
	// it performs a linear search of the list control to see if the item exists.
	// We know it never exists when inserting a new item, and all SetItem was doing
	// is initializing the node. So we do the initialization here, and skip the
	// unnecessary SetItem overhead.
	BOOL bInvalid = FALSE;
	UINT cbMask = 0;

	if (pTVI->mask & TVIF_CHILDREN)
	{
		if (pNode->m_item.cChildren == I_CHILDRENCALLBACK)
			cbMask |= TVIF_CHILDREN;
		else
			pNode->m_item.cChildren = pTVI->cChildren;
	}

	if (pTVI->mask & TVIF_IMAGE)
	{
		if (pNode->m_item.iImage == I_IMAGECALLBACK)
			cbMask |= TVIF_IMAGE;
		else
			pNode->m_item.iImage = pTVI->iImage;
	}

	if (pTVI->mask & TVIF_PARAM)
		pNode->m_item.lParam = pTVI->lParam;

	if (pTVI->mask & TVIF_SELECTEDIMAGE)
	{
		if (pNode->m_item.iSelectedImage == I_IMAGECALLBACK)
			cbMask |= TVIF_SELECTEDIMAGE;
		else
			pNode->m_item.iSelectedImage = pTVI->iSelectedImage;
	}


	if (pTVI->mask & TVIF_STATE)
	{
		pNode->m_item.state &= ~(pTVI->stateMask);
		pNode->m_item.state |= pTVI->state;
	}

	if (pTVI->mask & TVIF_TEXT)
	{
		// If the user changes text we need to repaint
		bInvalid = TRUE ;
		if (pTVI->pszText == LPSTR_TEXTCALLBACK)
			pNode->m_item.pszText = pTVI->pszText;
		if (pNode->m_item.pszText == LPSTR_TEXTCALLBACK )
			cbMask |= TVIF_TEXT;
		else
		{
			if (pNode->m_item.pszText != NULL)
				delete[] pNode->m_item.pszText;
			pNode->m_item.pszText = new TCHAR[_tcslen(pTVI->pszText)+1];
			_tcscpy(pNode->m_item.pszText, pTVI->pszText);
		}
	}

	if (cbMask != 0)
	{
		pTVI->mask = cbMask;
		NotifySetDispInfo(pTVI);
	}

	if ( pMask )
		*pMask = cbMask;

	return bInvalid;
}




//@doc SEC_TREECLASS
//@mfunc Sets the attributes of the specified item.
//@syntax SetItem(TV_ITEM* pTVI)
//@syntax SetItem(const LV_ITEM* pLVI, BOOL bRedraw=TRUE)
//@syntax SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
//@rdesc Nonzero if successful; otherwise 0.
//@parm TV_ITEM* | pTVI | A pointer to a TV_ITEM structure that contains the new item attributes.
//@parm const LV_ITEM* | pLVI | A pointer to an LV_ITEM structure.
//@parm  BOOL | bRedraw | If the item will be redrawn with the new attributes.
//@parm HTREEITEM | hItem | Handle of the item whose attributes are to be set.
//@parm  UINT | nMask | Integer specifying which attributes to set.
//@parm  LPCTSTR | lpszItem | Pointer to a string containing the item’s text.
//@parm  int | nImage | Index of the item’s image in the tree view control’s image list.
//@parm  int | nSelectedImage | ndex of the item’s selected image in the tree view control’s image list.
//@parm  UINT | nState | Specifies values for the item’s states.
//@parm  UINT | nStateMask | Specifies which states are to be set.
//@parm  LPARAM | lParam | A 32-bit application-specific value associated with the item.
//@comm Call this function to set the attributes of the specified tree view item.
// In the TV_ITEM structure, the hItem member identifies the item, and the mask member specifies which attributes to set.
// If the mask member or the nMask parameter specifies the TVIF_TEXT value, the pszText member or the lpszItem is the address of a null-terminated string and the cchTextMax member is ignored. If mask (or nMask) specifies the TVIF_STATE value, the stateMask member or the nStateMask parameter specifies which item states to change and the state member or nState parameter contains the values for those states.
//
// Note that the second version of this function is virtual, passing a pointer to an LV_ITEM structure.
// See the MFC documentation for complete information on these structures.
BOOL SEC_TREECLASS::SetItem(TV_ITEM* pTVI)
{
	Node *pNode;
	UINT cbMask;
	BOOL bInvalid = FALSE ;
	BOOL bUpdateNow = FALSE;

	// The handle has to be valid.  Otherwise, we have
	// no way of retrieving the associated tree node.
	ASSERT(pTVI->mask & TVIF_HANDLE);

	pNode = GetNode(pTVI->hItem);
	ASSERT(pNode != NULL);
	cbMask = 0;

	// bInvalid and cbMask will be set by this.
	bInvalid = BuildTreeNode( pTVI, pNode, &cbMask);

	// Next, we translate TV_ITEM settings to LV_ITEM settings.
	// Some of the TV_ITEM fields have direct equivalents in the
	// LV_ITEM used by the SEC_TREEBASE class.  For example, the state
	// TV_ITEM member has a TVIS_SELECTED flag which corresponds to
	// LV_ITEM's LVIS_SELECTED flag. 

	int iItem;
	iItem = ItemIndex(pTVI->hItem);
	if (iItem != -1)
	{
		LV_ITEM lvi;

		memset(&lvi, 0, sizeof(LV_ITEM));
		lvi.iItem = iItem;
		lvi.iSubItem = 0;

		if (pTVI->mask & TVIF_STATE)
		{
			lvi.mask |= LVIF_STATE;
			if (pTVI->stateMask & TVIS_CUT)
			{
				lvi.state |= (pTVI->state & TVIS_CUT) ? LVIS_CUT : 0;
				lvi.stateMask |= LVIS_CUT;
			}
			if (pTVI->stateMask & TVIS_DROPHILITED)
			{
				lvi.state |= (pTVI->state & TVIS_DROPHILITED) ? LVIS_DROPHILITED : 0;
				lvi.stateMask |= LVIS_DROPHILITED;
				bInvalid = TRUE;
				bUpdateNow = TRUE;
			}
			if (pTVI->stateMask & TVIS_OVERLAYMASK)
			{
				lvi.state |= (pTVI->state & TVIS_OVERLAYMASK) ? LVIS_OVERLAYMASK : 0;
				lvi.stateMask |= LVIS_OVERLAYMASK;
				bInvalid = TRUE;
			}
			if (pTVI->stateMask & TVIS_SELECTED)
			{
				lvi.state |= (pTVI->state & TVIS_SELECTED) ? LVIS_SELECTED : 0;
				lvi.stateMask |= LVIS_SELECTED;
			}
			if (pTVI->stateMask & TVIS_STATEIMAGEMASK)
			{
				lvi.state |= INDEXTOSTATEIMAGEMASK(GET_STATE_IMAGE(pTVI->state));
				lvi.stateMask |= LVIS_STATEIMAGEMASK;
				bInvalid = TRUE ;
			}
		}

		if (pTVI->mask & TVIF_IMAGE)
		{
			lvi.mask |= LVIF_IMAGE;
			lvi.iImage = pTVI->iImage;
			bInvalid = TRUE ;
		}


		if (!SEC_TREEBASE::SetItem(&lvi,TRUE))
			return FALSE;

		// The text or state of a visible node has been changed,
		// so we must update the item.
		if (bInvalid && lvi.iItem != m_nEditItem)
		{
			ReMeasureItem(pTVI->hItem);
			Update(pTVI->hItem, FALSE, FALSE, FALSE, bUpdateNow);
		}
	}

	return TRUE;
}


BOOL SEC_TREECLASS::SetItem(const LV_ITEM* pLVI, BOOL bRedraw /*=TRUE*/ )
{
	if (pLVI->iSubItem == 0)
	{
		TV_ITEM tvi;
		LV_ITEM lviBase;

		memset(&tvi, 0, sizeof(TV_ITEM));
		memmove(&lviBase, pLVI, sizeof(LV_ITEM));

		tvi.mask |= TVIF_HANDLE;
		tvi.hItem = GetItemAt(pLVI->iItem);
		if ( tvi.hItem == NULL )
			return FALSE;

		// Map overlapping LV_ITEM attributes to identical TV_ITEM
		// attributes and set the attributes there instead.
		if (pLVI->mask & LVIF_STATE)
		{
			tvi.mask |= TVIF_STATE;
			if (pLVI->stateMask & LVIS_CUT)
			{
				tvi.state |= (pLVI->state & LVIS_CUT) ? TVIS_CUT : 0;
				tvi.stateMask |= TVIS_CUT;
				lviBase.stateMask &= ~LVIS_CUT;
			}
			if (pLVI->stateMask & LVIS_DROPHILITED)
			{
				tvi.state |= (pLVI->state & LVIS_DROPHILITED) ? TVIS_DROPHILITED : 0;
				tvi.stateMask |= TVIS_DROPHILITED;
				lviBase.stateMask &= ~LVIS_DROPHILITED;
			}
			if (pLVI->stateMask & LVIS_OVERLAYMASK)
			{
				tvi.state |= (pLVI->state & LVIS_OVERLAYMASK) ? TVIS_OVERLAYMASK : 0;
				tvi.stateMask |= TVIS_OVERLAYMASK;
				lviBase.stateMask &= ~LVIS_OVERLAYMASK;
			}
			if (pLVI->stateMask & LVIS_SELECTED)
			{
				tvi.state |= (pLVI->state & LVIS_SELECTED) ? TVIS_SELECTED : 0;
				tvi.stateMask |= TVIS_SELECTED;
				lviBase.stateMask &= ~LVIS_SELECTED;
			}
			if (pLVI->stateMask & LVIS_STATEIMAGEMASK)
			{
				tvi.state |= INDEXTOSTATEIMAGEMASK(GET_STATE_IMAGE(pLVI->state));
				tvi.stateMask |= TVIS_STATEIMAGEMASK;
				lviBase.stateMask &= ~LVIS_STATEIMAGEMASK;
			}
			if (lviBase.stateMask == 0)
				lviBase.mask &= ~LVIF_STATE;
		}

		if (pLVI->mask & LVIF_TEXT)
		{
			tvi.mask |= TVIF_TEXT;
			tvi.pszText = pLVI->pszText;
			tvi.cchTextMax = pLVI->cchTextMax;
			lviBase.mask &= ~LVIF_TEXT;
		}

		if (pLVI->mask & LVIF_IMAGE)
		{
			tvi.mask |= TVIF_IMAGE;
			tvi.iImage = pLVI->iImage;
			lviBase.mask &= ~LVIF_IMAGE;
		}


		// Some of the attributes present in LV_ITEM have equivalent
		// attributes in the TV_ITEM structure.  So, if the the
		// LVIS_SELECTED attribute is set via SECListCtrl::SetItem(),
		// we actually set TVIS_SELECTED.
		if (!SetItem(&tvi))
			return FALSE;

		if (lviBase.mask != 0)
		{
			// If there are any fields or state flags present in LV_ITEM
			// but not in TV_ITEM (i.e. LVIS_FOCUSED), call SECListCtrl
			// to set the list view only attributes.
			if (!SEC_TREEBASE::SetItem(&lviBase,bRedraw))
				return FALSE;
		}

		return TRUE;
	}
	else
		return SEC_TREEBASE::SetItem(pLVI,bRedraw);
}



BOOL SEC_TREECLASS::SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage,	UINT nState, UINT nStateMask, LPARAM lParam)
{
	TV_ITEM tvi;
	::ZeroMemory(&tvi, sizeof(TV_ITEM));

	tvi.hItem          = hItem;
	tvi.mask           = nMask;
	tvi.iImage         = nImage;
	tvi.iSelectedImage = nSelectedImage;
	tvi.state          = nState;
	tvi.stateMask      = nStateMask;
	tvi.lParam         = lParam;
	tvi.pszText        = (LPTSTR)lpszItem;
	
	return SetItem( &tvi );
}



//@doc SEC_TREECLASS
//@mfunc Sets the index of the item’s image and its selected image within the tree view control’s image list.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hItem | Handle of the item whose image is to be set.
//@parm  int | nImage | The zero based index of the image.
//@parm  int | nSelectedImage | The zero based index of the selected image.
//@comm Each item in a tree view control can have a pair of bitmapped images associated with it. The images appear on the left side of an item’s label. One image is displayed when the item is selected, and the other is displayed when the item is not selected. For example, an item might display an open folder when it is selected and a closed folder when it is not selected.
BOOL SEC_TREECLASS::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{
	TV_ITEM tvi;
	::ZeroMemory(&tvi, sizeof(TV_ITEM));

	tvi.hItem          = hItem;
	tvi.mask           = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_HANDLE;
	tvi.iImage         = nImage;
	tvi.iSelectedImage = nSelectedImage;

	return SetItem( &tvi );
}


//@doc SEC_TREECLASS
//@mfunc Sets the state of the item specified.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hItem | Handle of the item whose state is to be set.
//@parm  UINT | nState | Specifies new states for the item.
//@parm  UINT | nStateMask | Specifies which states are to be changed.
//@comm Sets the state of the item specified by hItem.
BOOL SEC_TREECLASS::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
	TV_ITEM tvi;
	::ZeroMemory( &tvi, sizeof(TV_ITEM) );

	tvi.hItem          = hItem;
	tvi.mask           = TVIF_STATE|TVIF_HANDLE;
	tvi.state          = nState;
	tvi.stateMask      = nStateMask;

	return SetItem( &tvi );
}


//@doc SEC_TREECLASS
//@mfunc Sets the 32-bit application-specific value associated with the item specified.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hItem | Handle of the item whose data is to be retrieved.
//@parm DWORD | dwData | A 32-bit application-specific value associated with the item specified by hItem.
//@comm Call this function to set the 32-bit application-specific value associated with the specified item.
BOOL SEC_TREECLASS::SetItemData(HTREEITEM hItem, DWORD dwData)
{
	TV_ITEM tvi;
	::ZeroMemory(&tvi, sizeof(TV_ITEM));

	tvi.hItem          = hItem;
	tvi.mask           = TVIF_PARAM|TVIF_HANDLE;
	tvi.lParam         = dwData;

	return SetItem( &tvi );
}


//@doc SEC_TREECLASS
//@mfunc Gets the stored pointer to a Node object.
//@syntax GetNode( HTREEITEM hti )
//@syntax GetNode( Item* pItem )
//@rdesc The pointer to the returned Node.
//@parm  HTREEITEM | hti | Handle of item.
//@parm  Item* | pItem  | Pointer to Item.
//@comm Gets the stored pointer of a node by passing either the handle of a tree item or a pointer to an item.
SEC_TREECLASS::Node* SEC_TREECLASS::GetNode( HTREEITEM hti ) const
{
	if( !hti || hti==TVI_ROOT )
		return m_pNodeRoot;

	Node *pNode = (Node*)hti;
	ASSERT_POINTER(pNode, Node);

	return pNode;
}


SEC_TREECLASS::Node*
SEC_TREECLASS::GetNode( Item* pItem ) const
{
	ASSERT_POINTER(pItem, Item);
	return GetNode(	(HTREEITEM)(pItem->GetItemData()) );
}


SEC_TREECLASS::Node*
//@doc SEC_TREECLASS
//@mfunc Gets the stored pointer of a node based on an item index.
//@rdesc A pointer to the node specified by item’s index.
//@parm  int | nIndex | The list index of item.
//@comm Gets the stored pointer of a node based on an item’s list index.
SEC_TREECLASS::GetNodeAt( int nIndex ) const
{
	Item* pItem;
	Node* pNode;
	
	pItem = SEC_TREEBASE::GetItemAt(nIndex);
	ASSERT_NULL_OR_POINTER(pItem, Item);
	if (pItem != NULL)
		pNode = GetNode(pItem);
	else
		pNode = NULL;

	return pNode;
}


//@doc SEC_TREECLASS
//@mfunc Gets an HTREEITEM based on a list index.
//@rdesc The handle of the item with the index specified.
//@parm  int | nIndex | The list index of item.
//@comm Gets an item based on its list index.
HTREEITEM SEC_TREECLASS::GetItemAt( int nIndex ) const
{
	return (HTREEITEM)GetNodeAt(nIndex);
}


//@doc SEC_TREECLASS
//@mfunc Returns the list index of an HTREEITEM.
//@rdesc The list index for the specified HTREEITEM if successful; otherwise -1. 
//@parm HTREEITEM | hti | Handle of the item to get the list index for.
//@parm  int | nStartFrom | The list index of the item to start searching from. Default is 0.
//@comm Use this function to retrieve the list index of an HTREEITEM.
int SEC_TREECLASS::ItemIndex(HTREEITEM hti, int nStartFrom) const
{
	return NodeIndex(GetNode(hti), nStartFrom);
}


//@doc SEC_TREECLASS
//@mfunc Returns the list index of a Node.
//@rdesc The list index for the specified HTREEITEM if successful; otherwise -1. 
//@parm Node* | pNode | Pointer to the Node to get the list index for.
//@parm  int | nStartFrom | The list index of the item to start searching from. Default is 0.
//@comm Use this function to retrieve the list index of an itemusing its Node*.
int SEC_TREECLASS::NodeIndex(Node* pNode, int nStartFrom) const
{
    for( int i=nStartFrom ; i < SEC_TREEBASE::GetItemCount() ; i++ )
    {
        if( pNode==GetNodeAt(i) )
             return i;
    }

    return -1;
}


//@doc SEC_TREECLASS
//@mfunc Determines whether a given item is expanded (i.e., displaying the list of child items, if any, associated with it).
//@rdesc Nonzero if the specified item is expanded; otherwise 0.
//@parm HTREEITEM | hti | Handle of item.
//@comm Determines whether a given item is expanded (i.e., displaying the list of child items, if any, associated with it).
BOOL SEC_TREECLASS::IsExpanded(HTREEITEM hti) const
{
	Node* pNode = GetNode(hti);
	return pNode->IsExpanded();
}


//@doc SEC_TREECLASS
//@mfunc Determines whether specified item uses text or image callbacks.
//@rdesc Nonzero if item is subject to callback function; otherwise 0.
//@parm int | nIndex | List index of item.
//@comm Determines whether specified item uses a callback to get its text or images
BOOL SEC_TREECLASS::IsCallbackItem(int nIndex) const
{
	Node *pNode;
	BOOL bCbItem;

	pNode = GetNodeAt(nIndex);
	bCbItem = FALSE;

	if (pNode->m_item.pszText == LPSTR_TEXTCALLBACK)
		bCbItem = TRUE;
	else if (pNode->m_item.cChildren == I_CHILDRENCALLBACK)
		bCbItem = TRUE;
	else if (pNode->m_item.iImage == I_IMAGECALLBACK)
		bCbItem = TRUE;
	else if (pNode->m_item.iSelectedImage == I_IMAGECALLBACK)
		bCbItem = TRUE;
	else
		bCbItem = SEC_TREEBASE::IsCallbackItem(nIndex);

	return bCbItem;
}


//@doc SECTreeCtrl
//@mfunc Returns the text of the specified item or subitem.
//@rdesc A CString object containing the item’s text.
//@parm HTREEITEM | hti | Handle of the item whose text is to be retrieved.
//@parm  int | iSubItem | The subitem index, indicating the column number. Default is 0.
CString SEC_TREECLASS::GetItemText(HTREEITEM hti, int iSubItem) const
{
	CString str;

	if (iSubItem == 0)
	{
		TV_ITEM tvi;

		tvi.hItem = hti;
		tvi.mask  = TVIF_TEXT|TVIF_HANDLE;
		tvi.pszText = m_pszInternalBuffer;
		tvi.cchTextMax = LVXC_INTERNALBUFFERSIZE;

		if (GetItem(&tvi, FALSE, TRUE))
			str = tvi.pszText;
	}
	else if ( IsStoringSubItemText() )
	{
		m_pSubItemTextStorage->GetString( hti, iSubItem, m_pszInternalBuffer, LVXC_INTERNALBUFFERSIZE );
		str = m_pszInternalBuffer;
	}
	else
	{
		int nItem = ItemIndex(hti);
		str = SEC_TREEBASE::GetItemText(nItem, iSubItem);
	}

	return str;
}



//@doc SEC_TREECLASS
//@mfunc Sets the text of the specified item. See remarks for subitems.
//@syntax SetItemText( HTREEITEM hItem, LPCTSTR lpszItem )
//@syntax SetItemText( HTREEITEM hItem, int nSubItem, LPCTSTR lpszItem )
//@rdesc Nonzero if successful; otherwise 0.
//@parm  HTREEITEM | hItem | Handle of the item whose text is to be set.
//@parm  int | nSubItem | The subitem index, indicating the column number. Default is 0.
//@parm  LPCTSTR | lpszItem | Address of a string containing the new text for the item.
//@comm If you want to use SetItemText() or SetItemString() for subitems and NOT use
//   LV_GETDISPINFO, you must call StoreSubItemText( TRUE ). This will enable internal
//   string storage for subitems.
//
//	 For large trees, it may be desireable to use the callback to conserve memory. By default,
//   the callback is used.
BOOL SEC_TREECLASS::SetItemText( HTREEITEM hItem, LPCTSTR lpszItem )
{
	TV_ITEM tvi;
	::ZeroMemory( &tvi, sizeof(TV_ITEM) );

	tvi.hItem = hItem;
	tvi.mask  = TVIF_TEXT|TVIF_HANDLE;
	tvi.pszText = (LPTSTR)lpszItem;

	return SetItem( &tvi );
}


BOOL SEC_TREECLASS::SetItemText( HTREEITEM hItem, int nSubItem, LPCTSTR lpszItem )
{
	if (nSubItem == 0)
		return SetItemText(hItem, lpszItem);
	else if ( IsStoringSubItemText() )
	{
		if ( nSubItem <= GetColumnCount() )
		{
			if ( !m_pSubItemTextStorage->AddOrModifyString( hItem, nSubItem, lpszItem) )
				return FALSE;
			// may be visible so update it.
			ReMeasureItem( hItem );
			Update(hItem, FALSE, TRUE, TRUE);
			return TRUE;
		}

	}
	else
	{
		int nItem = ItemIndex(hItem);
		return SEC_TREEBASE::SetItemText(nItem, nSubItem, lpszItem);
	}

	return FALSE;
}

void SEC_TREECLASS::InternalUpdateColumnText(int nOldSubIndex, int nNewSubIndex)
{
	// If sub-item storage is not enabled, then a buffer update is not required and execution can
	// be transferred back to the base class without having to perform any tree-specific processing.
	if( !IsStoringSubItemText() )
		return;

	for(int nIndex=0; nIndex < GetItemCount(); nIndex++)
	{
		HTREEITEM hItem = GetItemAt(nIndex);
		if( !GetParentItem(hItem) )
			RecIterateChildren(hItem, nOldSubIndex, nNewSubIndex);
	}
}

void SEC_TREECLASS::RecIterateChildren(HTREEITEM hItem, int nOldSubIndex, int nNewSubIndex)
{
	m_pSubItemTextStorage->ReorderItems(hItem, nOldSubIndex, nNewSubIndex);

	Node* pNode = GetNode(hItem);
	pNode = (Node*)pNode->GetFirstChild();
	while(pNode)
	{
		RecIterateChildren((HTREEITEM)pNode, nOldSubIndex, nNewSubIndex);
		pNode = (Node*)pNode->GetNextSibling();
	}
}


//@doc SEC_TREECLASS
//@mfunc Sets the text of the specified item or subitem.
//@rdesc Nonzero if successful; otherwise 0.	
//@parm HTREEITEM | hti | Handle of an item in the tree.
//@parm  int | nSubItem | index of the subitem column.
//@parm  const CString& | strItem | Reference to the new subitem text.
//@comm In multi-column operations, sets string of subitem associated with the specified item.
// This will only work as expected if StoreSubItemText( TRUE ) has been called
// previously. By default, text for subitems is provided by the LVN_GETDISPINFO
// callback.
BOOL SEC_TREECLASS::SetItemString(HTREEITEM hti, int nSubItem, const CString& strItem)
{
	return SetItemText(hti, nSubItem, strItem);
}


//@doc SEC_TREECLASS
//@mfunc Returns the text of the specified item or subitem.
//@rdesc BOOL Returns the text of the specified item or subitem.
//@parm HTREEITEM | hti | Handle of an item in the tree.
//@parm  int | nSubItem | index of the subitem column.
//@parm  CString& | strItem | Receives the returned string value.
//@comm In multi-column operations, retrieves string of subitem associated with the specified item.
BOOL SEC_TREECLASS::GetItemString(HTREEITEM hti, int nSubItem, CString& strItem )
{
	strItem = GetItemText(hti, nSubItem);
	return TRUE;
}



//@doc SEC_TREECLASS
//@mfunc SEC_TREECLASS
//@rdesc BOOL TRUE if the items were inserted; otherwise FALSE
//@parm TV_ITEM** | ppItems | an array of TV_ITEM pointers
//@parm  HTREEITEM | hParent | The common parent to insert all items under
//@parm  int | cItems | The number of items in ppItems
//@parmopt  BOOL | bInvalidate | TRUE | If TRUE, all items will be remeasured
// and the control will be invalidated after all items are inserted
//@comm This call has better performance for inserting large numbers of
// ordered items under a common parent than individual InsertItem calls.
// Each item will be inserted in array order, following the last existing
// child of the parent. The handles for the items will be placed in the
// individual hItem members of the TV_ITEM structures in the array, and the mask
// for each item will have the TVIF_HANDLE flag set
BOOL SEC_TREECLASS::InsertBatch(TV_ITEM** ppItems, HTREEITEM hParent, int cItems, BOOL bInvalidate)
{
	// inserts a batch of items at the end of a given parents child list
	if ( !ppItems || cItems < 1 )
		return FALSE;

	if( !m_pNodeRoot )
		InitRootNode();
	
	SECTreeNode*	pNodeParent = NULL;
	Node*			pNodeNew = NULL;

	TV_ITEM* pItem = 0;
	
	//Find the parent.
	pNodeParent = GetNode(hParent);
	ASSERT(pNodeParent);

	//insert a batch of nodes, all under the same parent
	for( int i = 0; i < cItems ; i++ )
	{
		pItem = ppItems[i];
		//allocate a new node, automatically deleted by root node destruction
		pNodeNew = NewRefTreeCtrlNode();
		pItem->hItem = pNodeNew->m_item.hItem = (HTREEITEM)pNodeNew;
		pItem->mask |= TVIF_HANDLE;
		BuildTreeNode(pItem , pNodeNew);

		//insert the new node into our internal tree
		if( !pNodeParent->AddChild( pNodeNew, TREENODE_LAST ) )
		{
			ASSERT(FALSE);
			return FALSE;
		}

		pNodeNew->Collapse();
		
	}

	// just check one item to see if all items should be in the list control, since they
	// all have the same parent
	if( ShouldBeInListBox( ppItems[0]->hItem ) )
	{
		Node* pNode = 0;
		Item* pItem = 0;
		// add the first item
		AddNodeToListBox( GetNode(ppItems[0]->hItem) );
		// get the index of the first item
		int nIndex = ItemIndex(ppItems[0]->hItem);
		// add the rest under it
		for( int i = 1; i < cItems; i++ )
		{
			pNode = GetNode(ppItems[i]->hItem);
			pItem = CreateItemForNode(pNode);
			VERIFY( SEC_TREEBASE::InsertItem(++nIndex, pItem) != -1);
		}
		
	}

	// GUI update
	if ( bInvalidate )
	{
		ReMeasureAllItems();
		Invalidate();
	}


	return TRUE;
}




//@doc SEC_TREECLASS
//@mfunc Inserts a new item in a tree view control.
//@syntax InsertItem(LPTV_INSERTSTRUCT lpTVIS)
//@syntax InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter)
//@syntax InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
//@syntax InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
//@rdesc Handle of the new item if successful; otherwise NULL.
//@parm  LPTV_INSERTSTRUCT | lpTVIS | A pointer to a TV_INSERTSTRUCT that specifies the attributes of the tree view item to be inserted.
//@parm  UINT | nMask | Integer specifying which attributes to set.
//@parm  LPCTSTR | lpszItem | 
//@parm  int | nImage | Index of the item’s image in the tree view control’s image list.
//@parm  int | nSelectedImage | Index of the item’s selected image in the tree view control’s image list.
//@parm  UINT | nState | Specifies values for the item’s states.
//@parm  UINT | nStateMask | Specifies which states are to be set.
//@parm  LPARAM | lParam | A 32-bit application-specific value associated with the item.
//@parm  HTREEITEM | hParent | Handle of the inserted item’s parent.
//@parm  HTREEITEM | hInsertAfter | Handle of the item after which the new item is to be inserted.
//@comm Call this function to insert a new item in a tree view control.
HTREEITEM SEC_TREECLASS::InsertItem(LPTV_INSERTSTRUCT lpTVIS)
{
	if( !m_pNodeRoot )
		InitRootNode();
	
	SECTreeNode      *pNodeParent,
	               *pNodeInsertAfter;
	Node *pNodeNew;

	//We make a copy of the insert struct because we will be changing some of the flags etc...
	TV_INSERTSTRUCT insertCopy;
	::ZeroMemory( &insertCopy, sizeof(TV_INSERTSTRUCT) );
	::CopyMemory( &insertCopy, lpTVIS, sizeof(TV_INSERTSTRUCT) );

	////
	//Insert into our internal storage of all nodes (visible or not)

	//Find the parent.
	pNodeParent = GetNode(insertCopy.hParent);
	ASSERT(pNodeParent);

	/////
	//allocate a new node, automatically deleted by root node destruction
	pNodeNew = NewRefTreeCtrlNode();
	insertCopy.item.hItem = pNodeNew->m_item.hItem = (HTREEITEM)pNodeNew;
	insertCopy.item.mask |= TVIF_HANDLE;

	BuildTreeNode(&(insertCopy.item), pNodeNew);

	pNodeInsertAfter = DetermineNodePosition( insertCopy.hInsertAfter, pNodeNew, pNodeParent );

	//insert the new node into our internal tree
	if( !pNodeParent->AddChild( pNodeNew, pNodeInsertAfter  ) )
	{
		ASSERT(FALSE);
		return NULL;
	}

	// initially expanded ?
	if ( (lpTVIS->item.mask & TVIF_STATE) && 
			(lpTVIS->item.stateMask & TVIS_EXPANDED) &&
			(lpTVIS->item.state & TVIS_EXPANDED))
		pNodeNew->Expand();
	else
		pNodeNew->Collapse();

	// add to list box if it is visible
	if( ShouldBeInListBox( (HTREEITEM)pNodeNew ) )
		AddNodeToListBox( pNodeNew );

		


	if ( IsRedrawAfterInsertEnabled() )
	{
		// even if it isn't visible in the list we must repaint
		// in case the parent now needs an expand button.
		ReMeasureAllItems();
		Invalidate();
	}


	return (HTREEITEM)pNodeNew;
}


HTREEITEM SEC_TREECLASS::InsertItem(UINT nMask,
							LPCTSTR lpszItem,
							int nImage,
							int nSelectedImage,
							UINT nState,
							UINT nStateMask,
							LPARAM lParam,
							HTREEITEM hParent,
							HTREEITEM hInsertAfter)
{
	TV_ITEM tvi;
	::ZeroMemory(&tvi, sizeof(tvi));
	tvi.mask           = nMask;
	tvi.iImage         = nImage;
	tvi.iSelectedImage = nSelectedImage;
	tvi.state          = nState;
	tvi.stateMask      = nStateMask;
	tvi.lParam         = lParam;

	if( lpszItem==LPSTR_TEXTCALLBACK )
		tvi.pszText=LPSTR_TEXTCALLBACK;
	else
	{
		tvi.pszText = (TCHAR*)lpszItem;
		tvi.cchTextMax = (int)_tcslen(lpszItem) + 1;
	}


	TV_INSERTSTRUCT  tvis;
	tvis.hParent      = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item         = tvi;
	
	HTREEITEM htiRet=InsertItem( &tvis );
	
	return htiRet;
}


HTREEITEM SEC_TREECLASS::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	TV_ITEM tvi;
	::ZeroMemory(&tvi, sizeof(tvi));

	tvi.mask    = TVIF_TEXT;
	
	if( lpszItem==LPSTR_TEXTCALLBACK )
	{
		tvi.pszText = LPSTR_TEXTCALLBACK;
	}
	else
	{
		tvi.pszText = (TCHAR*)lpszItem;
		tvi.cchTextMax = (int)_tcslen(lpszItem) + 1;
	}
	
	TV_INSERTSTRUCT tvis;
	tvis.hParent      = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item         = tvi;

	HTREEITEM htiRet=InsertItem( &tvis );
	
	return htiRet;
}


HTREEITEM SEC_TREECLASS::InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	TV_ITEM tvi;
	::ZeroMemory(&tvi, sizeof(tvi));

	tvi.mask           = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage         = nImage;
	tvi.iSelectedImage = nSelectedImage;
	
	if( lpszItem==LPSTR_TEXTCALLBACK )
	{
		tvi.pszText = LPSTR_TEXTCALLBACK;
	}
	else
	{
		tvi.pszText = (TCHAR*)lpszItem;
		tvi.cchTextMax = (int)_tcslen(lpszItem) + 1;
	}

	TV_INSERTSTRUCT tvis;
	tvis.hParent      = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item         = tvi;

	HTREEITEM htiRet=InsertItem( &tvis );

	return htiRet;
}





//@doc SEC_TREECLASS
//@mfunc Deletes an item from the tree view control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm  HTREEITEM | htiDelete | The handle of the item to delete.
//@comm Call this function to delete an item from the tree view control.
BOOL SEC_TREECLASS::DeleteItem( HTREEITEM htiDelete )
{
	if( !InternalDeleteItem( htiDelete ))
		return FALSE;

	if ( IsRedrawAfterInsertEnabled() )
	{
		// even if it isn't visible in the list we must repaint
		// in case the parent now does not need an expand button.
		// Also to update scroll bars.
		ReMeasureAllItems();
		Invalidate();
	}

	return TRUE;
}



BOOL SEC_TREECLASS::InternalDeleteItem( HTREEITEM htiDelete )
{
	if( m_pNodeRoot==NULL )
		return TRUE;

	BOOL       bDelInOneGo;
	SECTreeNode *pNodeDelete;
	
	if( htiDelete == TVI_ROOT )
	{
		pNodeDelete = m_pNodeRoot;
		bDelInOneGo = TRUE;
	}
	else
	{
		pNodeDelete = (SECTreeNode *)htiDelete;
		bDelInOneGo = FALSE;
	}

	ASSERT_POINTER(pNodeDelete, SECTreeNode);

	SECTreeNode *pBLC;
	while( 	(pBLC = pNodeDelete->GetBottomLeftChild()) != NULL )
	{
		if( !DeleteUnChildedItem( (HTREEITEM)pBLC, !bDelInOneGo ) )
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	if( pNodeDelete == m_pNodeRoot )
	{
		delete m_pNodeRoot;
		m_pNodeRoot=NULL;
		//if we successfully deleted everything...that means we'll have a NULL root node.
		// With a NULL root node, nothing works.  So we re-init the root node...
		BOOL bRet = SEC_TREEBASE::DeleteAllItems();
		if ( bRet )
			InitRootNode();
		return bRet;
	}
	else
	{
		if( !DeleteUnChildedItem( htiDelete, !bDelInOneGo ) )
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	return TRUE;
}



//@doc SEC_TREECLASS
//@mfunc Deletes all items from the tree view control.
//@rdesc Nonzero if successful; otherwise 0.
//@comm Call this function to delete all items from the tree view control.
BOOL SEC_TREECLASS::DeleteAllItems()
{
	return DeleteItem( TVI_ROOT );
}


//@doc SEC_TREECLASS
//@mfunc Recursively expands an item and all of its children.
//@rdesc void 
//@parm  HTREEITEM | htiExpand | The handle of the item to expand.
//@parmopt  BOOL | bRedraw | TRUE | If TRUE, the tree will be invalidated after expansion. Default is TRUE.
void SEC_TREECLASS::ExpandCompletely( HTREEITEM htiExpand, BOOL bRedraw /* TRUE */ )
{
	if ( Expand( htiExpand, TVE_EXPAND, FALSE ) )
	{
		Node* pNode = GetNode( htiExpand );
		pNode = (Node*)pNode->GetFirstChild();
		while ( pNode )
		{
			// recurse
			ExpandCompletely( (HTREEITEM)pNode, FALSE );
			pNode = (Node*)pNode->GetNextSibling();
		}
	}
	
	if ( bRedraw )
	{
		ReMeasureAllItems();
		Invalidate();
	}
}

//@doc SEC_TREECLASS
//@mfunc Recursively collapse an item and all of its children.
//@rdesc void 
//@parm  HTREEITEM | htiCollapse | The handle of the item to expand.
//@parmopt  BOOL | bRedraw | TRUE | If TRUE, the tree will be invalidated after collapsing. Default is TRUE.
void SEC_TREECLASS::CollapseCompletely(HTREEITEM htiCollapse, BOOL bRedraw /*= TRUE*/)
{
	CRect rcInside;
	if(bRedraw)
		GetInsideRect( rcInside );

	if(ItemHasChildren( htiCollapse ) && IsExpanded( htiCollapse ) )
	{
		_Node* pNode = GetNode(htiCollapse);
		pNode = (_Node*)pNode->GetFirstChild();                
		while ( pNode )
		{
			CollapseCompletely((HTREEITEM)pNode, FALSE);
			pNode = (_Node*)pNode->GetNextSibling();
		}
 
		Expand( htiCollapse, TVE_COLLAPSE, FALSE );
	}
	
	if ( bRedraw )
	{
		ReMeasureAllItems();
		CRect rcOld( rcInside );
		GetInsideRect( rcInside );

		if ( rcInside == rcOld )
			Update(htiCollapse, FALSE, TRUE, TRUE);
		else
			// repaint everything, since we could have lost or gained a scrollbar
			Invalidate();
	}
}


//@doc SEC_TREECLASS
//@mfunc Expands or collapses the list of child items, if any, associated with the given parent item.
//@rdesc Nonzero if successful; otherwise 0.
//@parm  HTREEITEM | htiExpand | Handle of the tree item being expanded.
//@parm  UINT | nCode | A flag indicating the type of action to be taken.  This flag can be one of the following values: TVE_COLLAPSE - collapses the list; TVE_COLLAPSERESET - collapses the list and removes the child items; TVE_EXPAND - expands the list; TVE_TOGGLE - collapses the list if it is currently expanded or expands it if it is currently collapsed.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the item; default is TRUE.
//@parm  BOOL | bForceExpand | Ignore any optimizations and force an expand action
//@comm Call this function to expand or collapse the child items, if any, associated with the given parent item.
BOOL SEC_TREECLASS::Expand( HTREEITEM htiExpand, UINT nCode, BOOL bRedraw, BOOL bForceExpand)
{
	Node *pNodeExpand = NULL;
	Node *pNodeFocus = NULL;
	UINT action = 0;
	int nIndex = -1;

	pNodeExpand = GetNode(htiExpand);
	ASSERT(pNodeExpand);

	//Quick Optimization...
	// If we don't have any children no reason to continue...just return TRUE and move on
	if( ! bForceExpand &&
		! pNodeExpand->HasChildren() && 
		! ItemHasChildrenOnDemand(htiExpand) )
		return TRUE;

	CRect rcInside;
	GetInsideRect( rcInside );

	nIndex = NodeIndex(pNodeExpand);

	if( nCode==TVE_TOGGLE )
	{
		if( pNodeExpand->IsExpanded() )
			action=TVE_COLLAPSE;
		else	
			action=TVE_EXPAND;
	}
	else
		action = nCode;

	if (NotifyItemExpanding(htiExpand, action) == TRUE)
		return FALSE;
    	
	switch( action )
	{
	case TVE_COLLAPSE:
	case TVE_COLLAPSERESET:
	case TVE_COLLAPSE | TVE_COLLAPSERESET:
			if( pNodeExpand->IsExpanded() )
			{
				// Make sure we don't still call a collapsed
				// item the focus item.
				pNodeFocus = (Node*)GetCaretItem();
				if (pNodeFocus && (pNodeExpand->IsDescendant(pNodeFocus)))
				{
					NotifySelChanging(nIndex, NodeIndex(pNodeFocus) ,ByMouse);
					SelectCaretItem((HTREEITEM)pNodeExpand);
					NotifySelChanged(nIndex, NodeIndex(pNodeFocus) ,ByMouse);
				}

				//collapse this branch of the tree
				pNodeExpand->Collapse();

				// Actually remove items from the display
				RemoveChildrenFromListCtrl(pNodeExpand, nIndex);
			}
			// if using I_CHILDRENCALLBACK, we may need to delete the items
			if ( action & TVE_COLLAPSERESET )
			{
				// remove children
				HTREEITEM hChild = GetChildItem( htiExpand );
				while( hChild != NULL)
				{
					HTREEITEM hDelete = hChild;
					hChild = GetNextSiblingItem( hDelete );
					InternalDeleteItem( hDelete );
				}
			}
			break;
	
		case TVE_EXPAND:
			if( pNodeExpand->IsExpanded() )
				return TRUE; //already expanded;

            if( !pNodeExpand->HasChildren() )
                return(FALSE);

			//expand this branch of the tree
    		pNodeExpand->Expand();

			// animation support
			if(m_dwTreeStyleEx & TVXS_ANIMATE)
				m_nAnimateCount = 0;
			
			// Add the children to the list control
			AddChildrenToListCtrl(pNodeExpand, nIndex);

			break;


		default:
			ASSERT(FALSE);
			break;
	}

	NotifyItemExpanded( htiExpand, action );

	if (bRedraw)
	{
		// The item just expanded or collapsed must
		// be visible.
		if ((action == TVE_EXPAND) && m_bScrollOnExpand)
		{
			if (pNodeExpand->GetLastChild() != NULL)
				EnsureVisible((HTREEITEM)(pNodeExpand->GetLastChild()), TRUE);

			int nExpanded = ItemIndex(htiExpand);
			if( nExpanded < GetTopIndex() && (nExpanded != -1))
				SelectSetFirstVisible(htiExpand);	

		}


		ReMeasureAllItems();

		CRect rcOld( rcInside );
		GetInsideRect( rcInside );

		if ( rcInside == rcOld )
			Update(htiExpand, FALSE, TRUE, TRUE);
		else
			// repaint everything, since we could have lost or gained a scrollbar
			Invalidate();
		
	}

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Selects the given tree view item, scrolls the item into view, or redraws the item in the style used to indicate the target of a drag-and-drop operation.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hti | Handle of a tree item.
//@parm  UINT | nCode | The type of action to take.  This parameter can be one of the following values: TVGN_CARET - sets the selection to the given item; TVGN_DROPHILITE - redraws the given item in the style used to indicate the target of a drag-and-drop operation; TVGN_FIRSTVISIBLE - scrolls the tree view vertically so that the given item is the first visible item.
//@comm Call this function to select the given tree view item, scroll the item into view, or redraw the item in the style used to indicate the target of a drag-and-drop operation.  If nCode contains the value TVGN_CARET, the parent window receives the TVN_SELCHANGING and TVN_SELCHANGED notification messages.  In addition, if the specified item is the child of a collapsed parent item, the parent’s list of child items is expanded to reveal the specified item.  In this case, the parent window receives the TVN_ITEMEXPANDING and TVN_ITEMEXPANDED notification messages.
BOOL SEC_TREECLASS::Select(HTREEITEM hti, UINT nCode)
{
	BOOL bRtn = FALSE;

	if (EnsureVisible(hti) == FALSE)
		return FALSE;

	switch ( nCode )
	{
	case TVGN_CARET:
		{
			bRtn = TRUE;

			int iNewIndex = ItemIndex(hti);
			int iOldIndex = GetFocusIndex();
			
			if( NotifySelChanging( iNewIndex, iOldIndex, UnKnown ) == TRUE )
				bRtn = FALSE;

			if( bRtn )
			{
				bRtn = SelectCaretItem(hti);

				if( bRtn )
					NotifySelChanged( iNewIndex, iOldIndex, UnKnown );
			}
		}
		break;
	case TVGN_DROPHILITE:
		bRtn = SelectDropTarget(hti);
		break;
	case TVGN_FIRSTVISIBLE:
		bRtn = SetFirstVisible(hti);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return bRtn;
}


//@doc SEC_TREECLASS
//@mfunc Selects the given tree view item.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm Call this function to select the given tree view item. This will
// result in TVN_SELCHANGING and TVN_SELCHANGED notifications being sent.
// If this the tree is in single select mode, the selected item will also
// get the caret.
BOOL SEC_TREECLASS::SelectItem(HTREEITEM hti)
{
	if (EnsureVisible(hti) == FALSE)
		return FALSE;

	int iNewIndex = ItemIndex(hti);
	ASSERT(iNewIndex != -1);

	BOOL bRet = FALSE;

	int iOldIndex = GetFocusIndex();
	if( NotifySelChanging( iNewIndex, iOldIndex, UnKnown ) == TRUE )
		return FALSE;

	bRet = SEC_TREEBASE::SelectItem(iNewIndex);
	if( bRet )
		NotifySelChanged( iNewIndex, iOldIndex, UnKnown );

	if( !(m_dwTreeStyleEx & TVXS_MULTISEL) )
		bRet =  Select( hti, TVGN_CARET );

	return bRet;
}


//@doc SEC_TREECLASS
//@mfunc Redraws the item in the style used to indicate the target of a drag-and-drop operation.
//@syntax SelectDropTarget(HTREEITEM htiDropHilightItem, BOOL bAutoScroll)
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | htiDropHilightItem | Handle of a tree item.
//@parm BOOL | bAutoScroll | If TRUE, the tree will be scrolled to ensure the item is visible. If FALSE, the call will fail on items that are not visible. Default is TRUE.
//@comm Call this function to redraw the item in the style used to indicate the target of a drag-and-drop operation.
BOOL SEC_TREECLASS::SelectDropTarget(HTREEITEM htiDropHilightItem, BOOL bAutoScroll /* TRUE */)
{
	if (htiDropHilightItem != NULL)
	{
		if ( bAutoScroll )
		{
			if (EnsureVisible(htiDropHilightItem) == FALSE)
				return FALSE;
		}

		int nIndex = ItemIndex(htiDropHilightItem);

		if ( nIndex == -1 )
			return FALSE;

		int nLastIndex = GetDropHilightIndex();
		BOOL bRet = SEC_TREEBASE::SelectDropTarget(nIndex);
		
		if(	(m_dwTreeStyleEx & TVXS_AUTOEXPAND) && 
				(nLastIndex != nIndex))
		{
			KillTimer(m_idAutoExpandTimer);
			m_idAutoExpandTimer = SetTimer( 3, m_nAutoExpandDelay, NULL);
		}

		return bRet;
	}
	else
	{
		SEC_TREEBASE::SelectDropTarget(-1);
		KillTimer(m_idAutoExpandTimer);
	}

	return FALSE;
}


//@doc SEC_TREECLASS
//@mfunc Scrolls the tree view vertically so that the given item is the first visible item and gives it the focus and selection.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hti | Handle of a tree item to be set as the first visible item.
//@comm Call this function to scroll the tree view vertically so that the given item is the first visible item.
BOOL SEC_TREECLASS::SelectSetFirstVisible(HTREEITEM hti)
{
	if ( !EnsureVisible( hti ) )
		return FALSE;

	int nIndex = ItemIndex(hti);

	if (nIndex == -1)
		return FALSE; // The item is not currently shown in the tree

	SEC_TREEBASE::SetFirstVisible(nIndex);

	if (SEC_TREEBASE::SelectFocusItem(nIndex, TRUE) == FALSE)
	{
		TRACE(_T("SelectSetFirstVisible failed.\n"));
		return FALSE;
	}

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Determines the location of the specified point relative to the client area of a tree view control.
//@syntax HitTest( CPoint pt, UINT* pFlags )
//@syntax HitTest(TV_HITTESTINFO* pHitInfo)
//@rdesc The handle of the tree view item that occupies the specified point or NULL if no item occupies the point.
//@parm  CPoint | pt | Client coordinates of the point to test.
//@parmopt  UINT* | pFlags | NULL | Pointer to an integer that receives information about the results of the hit test.  It can be one or more of the values listed under the flags member in the Remarks section.
//@parm TV_HITTESTINFO* | pHitInfo | Address of a TV_HITTESTINFO structure that contains the position to hit test and that receives information about the results of the hit test.
//@comm Call this function to determine the location of the specified point
// relative to the client area of a tree view control.  When this function is 
// called, the pt parameter specifies the coordinates of the point to test.
// The function returns the handle of the item at the specified point or NULL
// if no item occupies the point. In addition, the pFlags parameter contains a 
// value that indicates the location of the specified point and can be one of the following:
//
// TVHT_ABOVE   			Above the client area.
//	
// TVHT_BELOW   			Below the client area.
//
// TVHT_NOWHERE   			In the client area but below the last item.
// 
// TVHT_ONITEM   			On the bitmap or label associated with an item.
// 
// TVHT_ONITEMBUTTON   		On the button associated with an item.
// 
// TVHT_ONITEMICON   		On the bitmap associated with an item.
// 
// TVHT_ONITEMINDENT   		In the indentation associated with an item.
// 
// TVHT_ONITEMLABEL   		On the label (string) associated with an item.
// 
// TVHT_ONITEMRIGHT   		In the area to the right of an item.
// 
// TVHT_ONITEMSTATEICON 	On the state icon for a tree view item that is in a user-defined state.
// 
// TVHT_TOLEFT   			To the right of the client area.
// 
// TVHT_TORIGHT   			To the left of the client area.
// 
// TVHT_COLUMNSEP			On a column separator.
//
// TVHT_COLUMNHEADING		On a column header.
//
// TVHT_INSIDEITEMBOUNDS	Within the bounding rectangle of the item

HTREEITEM SEC_TREECLASS::HitTest( CPoint pt, UINT* pFlags )
{
	//only test those items that are shown in list box
	HTREEITEM htiHit;
    UINT iHitFlags;
	int nHitIndex;
	int nHitCol;
	UINT iRetFlags = 0;

    nHitIndex = SEC_TREEBASE::HitTest( pt, &iHitFlags, nHitCol );
	
	htiHit = NULL;

	if (iHitFlags & LVHT_INSIDEITEMBOUNDS)
	{
		htiHit = GetItemAt(nHitIndex);
		iRetFlags |= TVHT_INSIDEITEMBOUNDS;

		if (iHitFlags&LVHT_ONITEMLABEL)
			iRetFlags |= TVHT_ONITEMLABEL;
		else if (iHitFlags&LVHT_ONITEMICON)
			iRetFlags |= TVHT_ONITEMICON;
		else if (iHitFlags&LVHT_ONITEMRIGHT)
			iRetFlags |= TVHT_ONITEMRIGHT;
		else if (iHitFlags&LVHT_ONITEMSTATEICON )
			iRetFlags |=TVHT_ONITEMSTATEICON;
		else if (iHitFlags&LVHT_ONITEMLEFT)
		{
			CRect rectItem;
			VERIFY(SEC_TREEBASE::GetItemRect( nHitIndex, rectItem, LVIR_BOUNDS ));
			if( m_dwTreeStyle&TVS_HASBUTTONS )
			{
				CRect rectButton;
				CalcButtonRect( &rectButton, nHitIndex, rectItem );
				if( rectButton.PtInRect(pt) )
					iRetFlags |=TVHT_ONITEMBUTTON;
				else
					iRetFlags |= TVHT_ONITEMINDENT;
			}
			else
				iRetFlags |= TVHT_ONITEMINDENT;
		}
		else if ( !(GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) )
			htiHit = NULL;

	}
	else if (iHitFlags&LVHT_ABOVE)
		iRetFlags |= TVHT_ABOVE;
	else if (iHitFlags&LVHT_BELOW)
		iRetFlags |= TVHT_BELOW;
	else if (iHitFlags&LVHT_TOLEFT)
		iRetFlags |= TVHT_TOLEFT;
	else if (iHitFlags&LVHT_TORIGHT)
		iRetFlags |= TVHT_TORIGHT;
	else if (iHitFlags&LVHT_COLUMNSEP)
		iRetFlags |= TVHT_COLUMNSEP;
	else if (iHitFlags&LVHT_COLUMNHEADING)
		iRetFlags |= TVHT_COLUMNHEADING;
	else if (iHitFlags&LVHT_NOWHERE)
		iRetFlags |= TVHT_NOWHERE;

	if ( pFlags )
		*pFlags = iRetFlags;

	return htiHit;
}


HTREEITEM SEC_TREECLASS::HitTest(TV_HITTESTINFO* pHitInfo)
{
	pHitInfo->hItem = HitTest( pHitInfo->pt, &pHitInfo->flags );
	return pHitInfo->hItem;
}


//@doc SEC_TREECLASS
//@mfunc Creates the drag bitmap.
//@rdesc Pointer to the image list to which the dragging bitmap was added, if successful; otherwise NULL.
//@parm HTREEITEM | hti | Handle of the tree item to be dragged.
//@parm CPoint& | ptOffset | returns the offset subtracted from the item rectangles before placing them into the image
CImageList* SEC_TREECLASS::CreateDragImageEx(HTREEITEM hti, CPoint& ptOffset)
{
	//ASSERT -> If this ASSERT fires you are trying to create a 
	//  drag image off of a node that is not visible.  This not supported
	ASSERT(ItemIndex(hti) != -1);

	CDWordArray		*pArrSelected = GetSelectionArray();
	ASSERT(pArrSelected);
	int cCount = (int)pArrSelected->GetSize();
	// single selection
	if( cCount <= 1)
	{
		CRect rect;
		GetItemRect(hti, &rect, LVIR_ICON);
		ptOffset.x = rect.left;
		ptOffset.y = rect.top;
		return CreateDragImage(hti);
	}
	else
	{
		// multi select
		CRect rUnion;
		rUnion.SetRectEmpty();
		// build a rect to contain the entire selection
		for (int nArrIdx = 0; nArrIdx <= pArrSelected->GetUpperBound(); ++nArrIdx)
		{
			CRect rcBounds;
			DWORD dwIndex = pArrSelected->GetAt(nArrIdx);
			if(SEC_TREEBASE::GetItemRect(dwIndex, &rcBounds, LVIR_BOUNDS))
			{
				rcBounds.NormalizeRect();
				rUnion.UnionRect(rUnion, rcBounds);
			}
		}
		// zero based rect
		int xOffset = rUnion.left;
		int yOffset = rUnion.top;
		rUnion.OffsetRect(-xOffset, -yOffset);
		// image list creation
		CImageList* pImage = new CImageList;
		pImage->Create(rUnion.Width(), rUnion.Height(), ILC_MASK, 1, 1);
		CClientDC dc(this);
		CDC memDC;
		CBitmap bmp;
		memDC.CreateCompatibleDC( &dc );
		bmp.CreateCompatibleBitmap(&dc, rUnion.Width(), rUnion.Height());
		CBitmap* pBmpOld = memDC.SelectObject(&bmp);
		memDC.FillSolidRect(rUnion, GetBkColor());
		// draw each item
		RenderSelectionArray(&memDC, xOffset, yOffset);
		memDC.SelectObject(pBmpOld);
		pImage->Add(&bmp, GetBkColor());
		// return the offset, for calculating the hot spot
		ptOffset.x = xOffset;
		ptOffset.y = yOffset;
		return pImage;
	}

}

void SEC_TREECLASS::CreateDragArray(CPtrArray* pArray, HTREEITEM hti, CPoint& ptOffset, CRect& rect)
{
	hti;	// unused (release)

	ASSERT(ItemIndex(hti) != -1);

	CDWordArray		*pArrSelected = GetSelectionArray();
	ASSERT(pArrSelected);
	int cCount = (int)pArrSelected->GetSize();
	ASSERT(cCount > 1);
	rect.SetRectEmpty();
	BOOL bLinesAtRoot = m_dwTreeStyle&TVS_LINESATROOT && m_dwTreeStyle&(TVS_HASLINES|TVS_HASBUTTONS);

	// build a rect to contain the entire selection
	for (int nArrIdx = 0; nArrIdx <= pArrSelected->GetUpperBound(); ++nArrIdx)
	{
		CRect rcBounds;
		DWORD dwIndex = pArrSelected->GetAt(nArrIdx);
		if(SEC_TREEBASE::GetItemRect(dwIndex, &rcBounds, LVIR_BOUNDS))
		{
			rcBounds.NormalizeRect();
			// add to the array after offsetting for indent
			int cxIndent=(GetIndent()*(GetNodeAt(dwIndex)->GetDistanceFromRoot()-1));
			if( bLinesAtRoot )
				cxIndent += GetIndent();
			rcBounds.OffsetRect(cxIndent, 0);
			// increase the overall rect to contain this item
			rect.UnionRect(rect, rcBounds);
			SECCustomDragItem* pItem = new SECCustomDragItem;
			pItem->m_rect = rcBounds;
			pItem->m_indent = cxIndent;
			pArray->Add((void*)pItem);
		}
	}
	// zero based rect
	int xOffset = rect.left;
	int yOffset = rect.top;
	rect.OffsetRect(-xOffset, -yOffset);
	// offset all of the item rectangles
	for(nArrIdx = 0; nArrIdx < pArray->GetSize(); nArrIdx++)
	{
		SECCustomDragItem* pItem = (SECCustomDragItem*)pArray->GetAt(nArrIdx);
		pItem->m_rect.OffsetRect(-xOffset, -yOffset);
	}
	
	ptOffset.x = xOffset;
	ptOffset.y = yOffset;
}


void SEC_TREECLASS::RenderSelectionArray(CDC* pDC, int xOffset, int yOffset)
{
	CDWordArray		*pArrSelected = GetSelectionArray();
	for (int nArrIdx = 0; nArrIdx <= pArrSelected->GetUpperBound(); ++nArrIdx)
	{
		DWORD dwIndex = pArrSelected->GetAt(nArrIdx);
		HTREEITEM htiTmp = GetItemAt(dwIndex);
		if(htiTmp)	// handle to selected item
		{
			CImageList* pIml = InternalCreateDragImage(htiTmp);
			if(pIml)
			{
				CRect r;
				GetItemRect(htiTmp, &r, LVIR_BOUNDS);
				r.OffsetRect(-xOffset, -yOffset);
				// now indent
				int cxIndent=(GetIndent()*(GetNode(htiTmp)->GetDistanceFromRoot()-1));
				r.OffsetRect(cxIndent, 0);
				// draw it
				pIml->Draw(pDC, 0, r.TopLeft(), ILD_NORMAL);
				delete pIml;
				pIml = 0;
			}
		}
		
	}

}

//@doc SEC_TREECLASS
//@mfunc Creates a dragging bitmap for the given item in a tree view control, creates an image list for the bitmap, and adds the bitmap to the image list.  An application uses the image-list functions to display the image when an item is being dragged.
//@rdesc Pointer to the image list to which the dragging bitmap was added, if successful; otherwise NULL.
//@parm HTREEITEM | hti | Handle of the tree item to be dragged.
//@comm Call this function to create a dragging bitmap for the given item in a tree view control, create an image list for the bitmap, and add the bitmap to the image list.  An application uses the image-list functions to display the image when the item is being dragged.
CImageList* SEC_TREECLASS::CreateDragImage(HTREEITEM hti)
{
	//ASSERT -> If this ASSERT fires you are trying to create a 
	//  drag image off of a node that is not visible.  This not supported
	ASSERT(ItemIndex(hti) != -1);

	// use CreateDragImageEx instead for multiple selection
	ASSERT(GetSelectionArray()->GetSize() <= 1);
	return InternalCreateDragImage(hti);
}


CImageList* SEC_TREECLASS::InternalCreateDragImage(HTREEITEM hti)
{
	CImageList* pIml = GetImageList( TVSIL_NORMAL );
	if( pIml == NULL )
		return NULL;
	else
		ASSERT_VALID(pIml);

	TV_ITEM item;
	::ZeroMemory(&item, sizeof(item));
	item.mask = TVIF_HANDLE|TVIF_IMAGE|TVIF_TEXT;
	item.hItem = hti;
	item.pszText = m_pszInternalBuffer;
	item.cchTextMax = LVXC_INTERNALBUFFERSIZE;

	VERIFY(GetItem(&item,FALSE,TRUE));

	CClientDC dc(this);
	CDC memDC;

	if( memDC.CreateCompatibleDC( &dc ) )
	{
		// we need the font for the item, so we must create a paint context
		// and force a PickTextFont call ( via LoadPaintContext )
		LvPaintContext* pPC = 0;
		pPC = CreatePaintContext(&memDC);
		pPC->lvi.iItem = ItemIndex(hti);
		pPC->lvi.iSubItem = 0;
		pPC->pItem = SEC_TREEBASE::GetItemAt(pPC->lvi.iItem);
		LoadPaintContext(pPC);
		ASSERT(pPC->pFont);
		CFont* pOldFont = memDC.SelectObject(pPC->pFont);	
		// create the image for the text
		CImageList* pImlText  = ImageListForText( &memDC, item.pszText);
		memDC.SelectObject(pOldFont);
		if(pPC)
			delete pPC;
		if( pImlText == NULL )
		{
			ASSERT(FALSE);
			return NULL;
		}
			
		CImageList* pImlImage = ImageListForImage( pIml, item.iImage );
		if( pImlImage == NULL )
		{
			ASSERT(FALSE);
			return NULL;
		}

		CImageList* pImlCombo = CombineImageLists( pImlImage, pImlText );
		if( pImlCombo == NULL )
		{
			ASSERT(FALSE);
			return NULL;
		}

		if (pImlText)
			delete pImlText;
		if (pImlImage)
			delete pImlImage;

		return pImlCombo;
	}

	return NULL;
}	


//@doc SEC_TREECLASS
//@mfunc Ensures that a tree view item is visible.  If necessary, expands the parent item or scrolls the tree view control so that the item is visible.
//@rdesc Nonzero if successful; otherwise 0.
//@parm HTREEITEM | hti | Handle of the tree item being made visible.
//@parm  BOOL | bParentVisible | Flag indicating whether parent item is visible; default is FALSE.
//@comm Call this function to ensure that a tree view item is visible.  If necessary, the function expands the parent item or scrolls the tree view control so that the item is visible.
BOOL SEC_TREECLASS::EnsureVisible(HTREEITEM hti, BOOL bParentVisible)
{
	HTREEITEM hParent;
	if ( hti == NULL )
		return FALSE;
		
	int nIndex = ItemIndex(hti);
	if (nIndex == -1)
	{
		// Make sure all parents are expanded so that it is shown
		BOOL bSuccess = TRUE;
		hParent = GetParentItem(hti);
		while (hParent != NULL && bSuccess)
		{
			if (!IsExpanded(hParent))
				bSuccess = Expand(hParent, TVE_EXPAND, FALSE);
			hParent = GetParentItem(hParent);
		}

		if (!bSuccess)
			return FALSE;

		FilterListBox();
		nIndex = ItemIndex(hti);
		// Now, refresh everything
		RecalcScrollBars();
		Invalidate();
	}

	if (!bParentVisible)
	{
		if (!(SEC_TREEBASE::EnsureVisible(nIndex)))
			return FALSE;
	}
	else
	{
		int nParentIndex;
		hParent = GetParentItem(hti);
		nParentIndex = ItemIndex(hParent);
		// A child can only be visible (i.e. shown in list ctrl)
		// if its parent is visible too. 
		ASSERT(nParentIndex != -1);
		if (!(SEC_TREEBASE::EnsureRangeVisible(nParentIndex, nIndex)))
			return FALSE;
	}

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Compares the text of two items.
//@rdesc The result of CString::CompareNoCase( pNode1, pNode2 ) 
//@parm Node* | pNode1 | The first node to compare.
//@parm  Node* | pNode2 | The second node to compare.
int SEC_TREECLASS::CompareItem(Node* pNode1, Node* pNode2)
{
	return GetItemText((HTREEITEM)pNode1).CompareNoCase(GetItemText((HTREEITEM)pNode2));
}


//@doc SEC_TREECLASS
//@mfunc Sorts the child items of the given parent item in a tree view control.
//@rdesc Nonzero if successful; otherwise 0.	
//@parm HTREEITEM | hti | Handle of the parent item whose child items are to be sorted.
//@parm BOOL | bRecursive | If TRUE, sort all subtrees of this item. Default TRUE.
//@parm BOOL | bAscending | Sort Ascending. Default TRUE.
//@comm Call this function to sort the child items of the given parent item in a tree view control. If hItem
// is NULL, the entire tree control is sorted.
BOOL SEC_TREECLASS::SortChildren(HTREEITEM hti, BOOL bRecursive, BOOL bAscending)
{
	Node *pParent;
	Node *i, *j;
	int nResult;

	pParent = hti ? GetNode(hti) : m_pNodeRoot;

	HTREEITEM htiFocus = GetCaretItem();

	for( i=(Node*)pParent->GetFirstChild(); i && i->GetNextSibling(); i = (Node*)i->GetNextSibling())
	{
		for (j=(Node*)pParent->GetLastChild(); j && j != i;j = (Node*)j->GetPrevSibling())
		{
			nResult = CompareItem(j, (Node*)(j->GetPrevSibling()));
			nResult *= (bAscending ? 1 : -1);
			if( nResult < 0 )
			{
				if (i == (Node*)j->GetPrevSibling())
					i = (Node*)i->GetNextSibling();
				j->SwapPrev();
				j = (Node*)j->GetNextSibling();
			}
		}
	}

	// If we have composite children, sort them too
	if (bRecursive)
	{
		for( i=(Node*)pParent->GetFirstChild(); i; i = (Node*)i->GetNextSibling())
		{
			if (i->HasChildren())
				SortChildren((HTREEITEM)i, bRecursive, bAscending);
		}
	}

	// If we are expanded, then the sorted items are visible and
	// we must update the display.
	if (pParent->IsExpanded())
	{
		int nIndex = hti ? NodeIndex(pParent)+1 : 0;
		ASSERT(nIndex != -1);
		SEC_TREEBASE::Update(nIndex, FALSE, TRUE, TRUE);
		for( i=(Node *)pParent->GetFirstChild(); 
			 i && pParent->IsDescendant(i); 
			 i=(Node *)i->GetNextInDisplayOrder())
		{
			// For speed, we replace rather than remove and reinsert
			// the sorted items.
			if (i->IsVisible())
			{
				VERIFY(SEC_TREEBASE::SetItemData(nIndex++, (DWORD)i));
			}		
		}

		ReMeasureAllItems();
	}

	if( htiFocus != NULL )
		Select(htiFocus, TVGN_CARET);

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Sorts tree view items using an application-defined callback function that compares the items.
//@rdesc Nonzero if successful; otherwise 0.	
//@parm LPTV_SORTCB | pSort | Pointer to a TV_SORTCB structure.
//@parm  BOOL | bRecursive  | If TRUE, sort all subtrees of this item. Default TRUE.
//@parm  BOOL | bAscending | Sort Ascending. Default TRUE.
//@comm Call this function to sort tree view items using an application-defined callback function that compares the items.
BOOL SEC_TREECLASS::SortChildrenCB(LPTV_SORTCB pSort, BOOL bRecursive, BOOL bAscending)
{
	Node *pParent;
	Node *i, *j, *prev;
	int nResult;

	ASSERT(pSort);
	ASSERT(pSort->lpfnCompare);

	pParent = pSort->hParent ? GetNode(pSort->hParent) : m_pNodeRoot;

	HTREEITEM htiFocus = GetCaretItem();

	// Just a simple bubble sort.  This is an O(n^2) sorting
	// algorithm.  Ideally, I'd have used a quicksort or heap
	// sort since they are O(n Log n).  Problem is, the children
	// are ordered as a linked list rather than an array, and
	// sorting a linked list is a much harder problem.  We may
	// revisit (optimize) this later.
	for( i=(Node*)pParent->GetFirstChild(); i && i->GetNextSibling(); i = (Node*)i->GetNextSibling())
	{
		for (j=(Node*)pParent->GetLastChild(); j && j != i;j = (Node*)j->GetPrevSibling())
		{
			prev = (Node*)j->GetPrevSibling();
			//bk99 modified to match the parameters for the common control
			//nResult = (*pSort->lpfnCompare)(j->m_item.lParam, prev->m_item.lParam, pSort->lParam);
			//nResult *= (!bAscending ? 1 : -1);
			nResult = (*pSort->lpfnCompare)(prev->m_item.lParam, j->m_item.lParam, pSort->lParam);
			nResult *= (!bAscending ? 1 : -1);
			if (nResult < 0)
			{
				if (i == prev)
					i = (Node*)i->GetNextSibling();
				j->SwapPrev();
				j = (Node*)j->GetNextSibling();
			}
		}
	}

	// If we have composite children, sort them too
	if (bRecursive)
	{
		TV_SORTCB sortrec = *pSort;
		for( i=(Node*)pParent->GetFirstChild(); i; i = (Node*)i->GetNextSibling())
		{
			sortrec.hParent = (HTREEITEM)i;
			if (i->HasChildren())
				SortChildrenCB(&sortrec, bRecursive, bAscending);
		}
	}

	// If we are expanded, then the sorted items are visible and
	// we must update the display.
	if (pParent->IsExpanded())
	{
		int nIndex = pSort->hParent ? NodeIndex(pParent)+1 : 0;
		ASSERT(nIndex != -1);
		SEC_TREEBASE::Update(nIndex, FALSE, TRUE, TRUE);
		for( i=(Node *)pParent->GetFirstChild(); 
			 i && pParent->IsDescendant(i); 
			 i=(Node *)i->GetNextInDisplayOrder() )
		{
			// For speed, we replace rather than remove and reinsert
			// the sorted items.
			if (i->IsVisible())
				VERIFY(SEC_TREEBASE::SetItemData(nIndex++, (DWORD)i));
		}

		ReMeasureAllItems();
	}

	if( htiFocus != NULL )
		Select(htiFocus, TVGN_CARET);

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Returns the number of children an item has.
//@rdesc The number of children for this item.
//@parm HTREEITEM | hti | Handle of the item to count the children for.
//@parm  BOOL | bRecursive | If TRUE, all tree branches below hti will be recursed. Default is TRUE.
//@parm  BOOL | bExpandedOnly | If TRUE, only children that are expanded will be recursed. Only effective if bRecursive = TRUE. Default is FALSE.
UINT SEC_TREECLASS::GetChildCount(HTREEITEM hti, BOOL bRecursive, BOOL bExpandedOnly)
{
	Node *pParent;
	Node *pNode = 0;
	UINT nResult = 0;

	pParent = hti ? GetNode(hti) : m_pNodeRoot;

	pNode = (Node*)pParent->GetFirstChild();

	while( pNode )
	{
		nResult++;
		if (bRecursive && pNode->HasChildren() && (bExpandedOnly ? pNode->IsExpanded() : 1))
			nResult += GetChildCount( (HTREEITEM)pNode, bRecursive, bExpandedOnly );

		pNode = (Node*)pNode->GetNextSibling();
	}

	return nResult;
}



//@doc SEC_TREECLASS
//@mfunc Determines whether a given item is set as the first visible item in the tree view control.
//@rdesc Nonzero if the item specified is set as the first visible item; otherwise 0.
//@parm HTREEITEM | hti | Handle of the tree item.
//@comm Scrolls the item vertically to be the first visible item in the tree.
BOOL SEC_TREECLASS::SetFirstVisible(HTREEITEM hti)
{
	if ( !EnsureVisible( hti ) )
		return FALSE;

	int nIndex = ItemIndex(hti);

	if (nIndex == -1)
		return FALSE; // The item is not currently shown in the tree

	SEC_TREEBASE::SetFirstVisible(nIndex);

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Begins in-place editing of the specified item’s text.
//@rdesc If successful, a pointer to the Cedit object that is used to edit the item text; otherwise NULL.
//@parm HTREEITEM | hti | Handle of the tree item to be edited.
//@parm int | nCol | Specifies the column index for a multi-column tree. Default is 0.
//@comm Call this function to begin in-place editing of the specified item’s text.  The editing is accomplished by replacing the text of the item with a single-line edit control containing the text.
CEdit* SEC_TREECLASS::EditLabel(HTREEITEM hti, int nCol)
{
	if(GetListCtrlStyle() & LVS_EDITLABELS)
	{
		int nIndex = ItemIndex(hti);		
		if (nIndex >= 0)
		{
			m_nActiveColumn = nCol;
			return SEC_TREEBASE::EditLabel(nIndex, nCol);
		}
	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Selects all child nodes visible beneath a parent node.
//@rdesc Nonzero if all child nodes visible beneath a parent mode have been selected.
//@parm  HTREEITEM | hti | Handle of an item in the tree.
//@comm Selects all child nodes that are visible beneath a parent node.
BOOL SEC_TREECLASS::SelectAllVisibleChildren( HTREEITEM hti )
{
	SECTreeNode *pParent=(SECTreeNode *)hti;

	ASSERT_POINTER(pParent, SECTreeNode);
	
	if( !pParent->HasChildren() )
		return(FALSE);

	SECTreeNode *pNode = pParent;
	SECTreeNode *pNodeLast = NULL;

	while( TRUE )
	{
		pNodeLast = pNode;
		pNode=(SECTreeNode *)GetNextItem( (HTREEITEM)pNode, TVGN_NEXTVISIBLE);
		if( !pNode )
			break;

		if( !pNode->IsAncestor(pParent) )
			break;
	}
	
	return SelectItemRange( (HTREEITEM)pParent, (HTREEITEM)pNodeLast, TRUE ); 
}


//@doc SEC_TREECLASS
//@mfunc Selects/deselects a range of items.
//@rdesc Nonzero if the call was successful; otherwise 0.
//@parm  HTREEITEM | htiFirst | Handle of the first item in the range selected.
//@parm  HTREEITEM | htiLast | Handle of the last item in the range selected.
//@parm  BOOL | bSelect | Flag indicating whether given range is selected.
//@comm Selects or deselects a range of items.
BOOL SEC_TREECLASS::SelectItemRange( HTREEITEM htiFirst, HTREEITEM htiLast, BOOL bSelect )
{
	int iFirst, iLast;

	iFirst = ItemIndex(htiFirst);
	iLast = ItemIndex(htiLast);

	return SEC_TREEBASE::SelectItemRange(iFirst, iLast, bSelect);
}


//@doc SEC_TREECLASS
//@mfunc Indicates whether an item is selected.
//@rdesc Nonzero if item is selected; otherwise 0.
//@parm HTREEITEM | hti | Handle of an item in the tree.
BOOL SEC_TREECLASS::IsSelected(HTREEITEM hti) const
{
	int nIndex = ItemIndex(hti);
	if (nIndex >= 0)
		return SEC_TREEBASE::IsSelected(nIndex);
	else
		return FALSE;
}


//@doc SEC_TREECLASS
//@mfunc Hides/Shows a tree item.
//@rdesc Nonzero if the call was successful; otherwise 0.
//@parm  HTREEITEM | hti | Handle of an item in the tree.
//@parm  BOOL | bHide | TRUE to hide and item, FALSE to unhide an item.
BOOL SEC_TREECLASS::HideItem( HTREEITEM hti, BOOL bHide )
{
	if( hti==TVI_ROOT )
	{
		ASSERT(FALSE);
		return(FALSE);
	}
	
	Node *pNode = GetNode(hti);
	ASSERT(pNode);

	pNode->stateEx &= ~TVIS_EX_HIDDEN;
	if( bHide )
		pNode->stateEx |= TVIS_EX_HIDDEN;

    for( ; pNode ; pNode=(Node *)pNode->GetNextInDisplayOrder() )
    {
        if( pNode->IsVisible() )
        {
            if( !NodeInListBox(pNode) )
            {
                if(!AddNodeToListBox(pNode) )
                {
                    ASSERT(FALSE);
                    return(FALSE);
                }
            }
        }
        else
        {
            if( NodeInListBox(pNode) )
            {
				// If the node is no longer in the list ctrl, it
				// can't be selected.
				SetItemState((HTREEITEM)pNode, 0, TVIS_SELECTED);
                if( !DelNodeFromListBox(pNode) )
                {
                    ASSERT(FALSE);
                    return(FALSE);
                }
            }
        }
    }
	Invalidate();

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Indicates whether or not an item is hidden.
//@rdesc TRUE if the item is currently hidden.
//@parm  HTREEITEM | hti | Handle of an item in the tree.
//@comm Call this function to determine if an item has the TVIS_EX_HIDDEN style.
BOOL SEC_TREECLASS::IsHidden( HTREEITEM hti ) const
{
	if( hti==TVI_ROOT )
	{
		ASSERT(FALSE);
		return(FALSE);
	}
	Node *pNode = GetNode(hti);
	ASSERT(pNode);

	return pNode->IsHidden();
}


//@doc SEC_TREECLASS
//@mfunc Gets first hidden item.
//@rdesc The first hidden item in the tree.
HTREEITEM SEC_TREECLASS::GetFirstHiddenItem() const
{
	return GetNextHiddenItem( (HTREEITEM)m_pNodeRoot );
}


//@doc SEC_TREECLASS
//@mfunc Gets next hidden item.
//@rdesc The next hiden item in the tree.
//@parm HTREEITEM | hti | Handle to a tree item.
//@comm Use this function, in conjunction with GetFirstHiddenItem() to
// enumerate the hidden items in a tree.
HTREEITEM SEC_TREECLASS::GetNextHiddenItem(HTREEITEM hti) const
{
	Node *pNode = GetNode(hti);
	pNode=(Node *)pNode->GetNextInDisplayOrder();

	for( ; pNode ; pNode=(Node *)pNode->GetNextInDisplayOrder() )
	{
		if( IsHidden((HTREEITEM)pNode) )
        {
			return (HTREEITEM)pNode;
        }
	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Makes all hidden items visible.
//@rdesc Nonzero if all items that were hidden are now visible; otherwise 0.
BOOL SEC_TREECLASS::UnHideAllItems()
{
	for( HTREEITEM hti=GetFirstHiddenItem(); 
	     hti ;
		 hti = GetNextHiddenItem(hti) )
	{
		if( !HideItem(hti,FALSE) )
		{
			ASSERT(FALSE);
			return(FALSE);
		}
	}

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Disables all items (shown in disabled text color), or enables disabled items.
//@rdesc Nonzero if the call was successful; otherwise 0.
//@parm  BOOL | bDisable | TRUE to disable all items. FALSE to enable all items.
BOOL SEC_TREECLASS::DisableAllItems( BOOL bDisable )
{
	for( HTREEITEM hti = GetFirstVisibleItem();
	     hti ;
		 hti = GetNextItemInDisplayOrder(hti) ) 
	{
		if( !DisableItem(hti,bDisable) )
			return FALSE;
	}

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Disables/enables a particular item.
//@rdesc Nonzero if the call was successful; otherwise 0.
//@parm  HTREEITEM | hti | The item to disable or enable
//@parm  BOOL | bDisable | TRUE to disable the item, FALSE to enable.
//@comm This function will add or remove the TVIS_EX_DISABLED extended style and invalidate the items.
BOOL SEC_TREECLASS::DisableItem( HTREEITEM hti, BOOL bDisable )
{
	Node *pNodeDis=GetNode(hti);

	ASSERT(pNodeDis);
	
	pNodeDis->stateEx &= ~TVIS_EX_DISABLED;
	if (bDisable)
		pNodeDis->stateEx |= TVIS_EX_DISABLED;

	if (m_bHideDisabledItems)
		pNodeDis->stateEx |= TVIS_EX_HIDDEN;
	
	if( IsDisabled((HTREEITEM)pNodeDis) && m_bHideDisabledItems)
	{
		//if wants to be hidden then do this
		HideItem(hti, IsHidden((HTREEITEM)pNodeDis));
	}

	//Invalidate all child tree items
	for( SECTreeNode *pNode=pNodeDis ; 
	     pNode==pNodeDis || pNode->IsDescendant(pNodeDis) ;
		 pNode = pNode->GetNextInDisplayOrder() )
	{
		Update((HTREEITEM)pNode, FALSE, FALSE, TRUE); //need a redraw
	}

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Indicates whether or not an item is disabled.
//@rdesc Nonzero if the item is disabled; otherwise 0.
//@parm  HTREEITEM | hti | Handle of an item in the tree.
BOOL SEC_TREECLASS::IsDisabled( HTREEITEM hti) const
{
	if( hti==TVI_ROOT )
	{
		ASSERT(FALSE);
		return(FALSE);
	}

	Node* pNode = (Node*)hti;
	ASSERT(pNode);

	return (pNode->IsDisabled());
}


//@doc SEC_TREECLASS
//@mfunc Gets first disabled item.
//@rdesc The handle to the first disabled item, or NULL if on items are disabled.
//@comm This functnions can be used with GetNextDisabledItem to enumerate
// the disabled items in the tree.
HTREEITEM SEC_TREECLASS::GetFirstDisabledItem() const
{
	return GetNextDisabledItem( (HTREEITEM)m_pNodeRoot );
}


//@doc SEC_TREECLASS
//@mfunc Gets the next disabled item.
//@rdesc The handle to the next disabled item, or NULL if on items are disabled.
//@comm This functnions can be used with GeFirstDisabledItem to enumerate
// the disabled items in the tree.
HTREEITEM SEC_TREECLASS::GetNextDisabledItem( HTREEITEM hti ) const
{
	Node *pNode = GetNode(hti);
	pNode=(Node *)pNode->GetNextInDisplayOrder();

	for( ; pNode ; pNode=(Node *)pNode->GetNextInDisplayOrder() )
	{
		if( pNode->stateEx & TVIS_EX_DISABLED )
        {
			return (HTREEITEM)pNode;
        }
	}

	return NULL;
}


//@doc SEC_TREECLASS
//@mfunc Hides items that are disabled.
//@rdesc void 
//@parm BOOL | bHide | If TRUE all disabled item will be hidden. If FALSE, all disabled items will be made visible.
//@comm This call applies to all currently hidden items, and any items that
// are disabled in the future until another call to HideDisabledItems is made.
void SEC_TREECLASS::HideDisabledItems(BOOL bHide)
{
	m_bHideDisabledItems = bHide;
	for( HTREEITEM hti=GetFirstDisabledItem();
	     hti;
		 hti=GetNextDisabledItem(hti)  )
	{
		HideItem( hti, bHide );
	}
}


//@doc SEC_TREECLASS
//@mfunc Indicates whether or not disabled items are hidden.
//@rdesc TRUE if disabled items are hidden.
BOOL SEC_TREECLASS::IsHideDisabledItems() const
{
	return m_bHideDisabledItems;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves a count of the items in a tree view control.
//@rdesc The number of items in the tree.
UINT SEC_TREECLASS::GetCount() const
{
	return const_cast<SEC_TREECLASS*>(this)->GetChildCount( NULL );
}


//@doc SEC_TREECLASS
//@mfunc Determines whether the tree item specified has child items on demand.
//@rdesc Nonzero if the tree item specified by hItem has child items on demand; 0 if it does not.
//@parm HTREEITEM | hti | Handle of a tree item.
//@comm This function will return TRUE if the tvi.cChildren member for this
// item was initially set to I_CHILDRENCALLBACK and the cChildren member returned from
// the TVN_GETDISPINFO callback is > 0. See the Dynatree sample for a demonstration
// of children on demand.
BOOL SEC_TREECLASS::ItemHasChildrenOnDemand(HTREEITEM hti) const
{
	if( GetChildItem(hti) )
		return FALSE;

	TV_ITEM tvi;
	tvi.mask = TVIF_HANDLE|TVIF_CHILDREN;
	tvi.hItem = hti;
	if( !GetItem(&tvi, FALSE, TRUE) )
	{
		ASSERT(FALSE);
		return(FALSE);
	}

	if( tvi.mask&TVIF_CHILDREN && tvi.cChildren>0 )
		return TRUE;
	
	return FALSE;
}


//@doc SEC_TREECLASS
//@mfunc Determines whether the tree item specified has child items.
//@rdesc Nonzero if the tree item specified by hItem has child items; 0 if it does not.
//@parm HTREEITEM | hti | Handle of a tree item.
BOOL SEC_TREECLASS::ItemHasChildren(HTREEITEM hti) const
{
	if( GetChildItem(hti) )
		return TRUE;

	return FALSE;
}


//@doc SEC_TREECLASS
//@mfunc Indicates whether or not an item exists in the tree view control.
//@rdesc TRUE if the item exists in the tree. 
//@parm HTREEITEM | hti | Handle to a tree item.
BOOL SEC_TREECLASS::ItemExists(HTREEITEM hti) const
{
	// previous method only returned TRUE if the item was in the list control.
	//return ItemIndex(hti) >= 0;

	// see if this is in this tree
	Node* pNode = 0;

	if( !hti || hti==TVI_ROOT )
		pNode =  m_pNodeRoot;
	else
		pNode = (Node*)hti;

	if ( !AfxIsValidAddress(pNode, sizeof(Node) ) )
		return FALSE;

	// its the right type, but is it in our tree not another one?
	if ( pNode->GetRoot() != m_pNodeRoot )
		return FALSE;


	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Returns a pointer to the requested image list.
//@rdesc Pointer to the control’s image list if successful; otherwise NULL.
//@parm UINT | nImageListType | Type of image list to retrieve.  The image list can be one of the following values: TVSIL_NORMAL - retrieves the normal image list, which contains the selected and nonselected images for the tree view item; TVSIL_STATE - retrieves the state image list, which contains the images for tree view items that are in a user-defined state.
//@comm Call this function to retrieve the handle of the normal or state image list associated with the tree view control.  Each item in a tree view control can have a pair of bitmapped images associated with it.  One image is displayed when the item is selected, and the other is displayed when the item is not selected.  For example, an item might display an open folder when it is selected and a closed folder when it is not selected.
CImageList* SEC_TREECLASS::GetImageList(UINT nImageListType) const
{
	if (nImageListType == TVSIL_NORMAL)
		nImageListType = LVSIL_NORMAL;
	else if (nImageListType == TVSIL_STATE)
		nImageListType = LVSIL_STATE;

	return SEC_TREEBASE::GetImageList(nImageListType);
}


//@doc SEC_TREECLASS
//@mfunc Sets the normal or state image list.
//@rdesc Pointer to the previous image list, if any; otherwise NULL.
//@parm CImageList* | pList | Pointer to the image list to assign. If pImageList is NULL, all images are removed from the tree view control.
//@parm  int | nImageListType | Type of image list to set.  The image list can be one of the following values:TVSIL_NORMAL - sets the normal image list, which contains the selected and nonselected images for the tree view item; TVSIL_STATE - sets the state image list, which contains the images for tree view items that are in a user-defined state. LVSIL_HEADER - When the header control is enabled for the tree, this style sets the header image list.
//@comm Call this function to set the normal or state image list for a tree view control and redraw the control using the new images.
CImageList* SEC_TREECLASS::SetImageList(CImageList* pList, int nImageListType)
{
	if (nImageListType == TVSIL_NORMAL)
	{
		nImageListType = LVSIL_NORMAL;
		// make sure we have enough indent
		if ( pList )
		{
			IMAGEINFO imageInfo;
			if ( pList->GetImageInfo( 0, &imageInfo ) )
			{
				CRect r(imageInfo.rcImage);
				r.NormalizeRect();
				if ( (UINT)r.Width() > GetIndent() )
					SetIndent( r.Width());
			}
		}
	}
	else if (nImageListType == TVSIL_STATE)
		nImageListType = LVSIL_STATE;

	
	return SEC_TREEBASE::SetImageList(pList, nImageListType);
}


//@doc SEC_TREECLASS
//@mfunc Retrieves values for the style and extended style of a tree view control.
//@rdesc Nonzero if specified style and extended style are retrieved; otherwise 0.
//@parm DWORD& | dwStyle | Reference to a style value.
//@parm  DWORD& | dwExStyle | Reference to an extended style value.
BOOL SEC_TREECLASS::GetTreeCtrlStyles(DWORD& dwStyle, DWORD& dwExStyle) const
{
	dwStyle = GetTreeCtrlStyle();
	dwExStyle = GetTreeCtrlStyleEx();
	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Sets values for the style and extended style of a tree view control.
//@rdesc Nonzero if specified style and extended style are retrieved; otherwise 0.
//@parm DWORD | dwStyle | Reference to a style value.
//@parm  DWORD | dwExStyle | Reference to an extended style value.
//@parm  BOOL | bRedraw | TRUE to redraw with the new styles.
BOOL SEC_TREECLASS::SetTreeCtrlStyles(DWORD dwStyle, DWORD dwExStyle, BOOL bRedraw)
{
	if (!SetTreeCtrlStyle(dwStyle, bRedraw))
		return FALSE;

	if (!SetTreeCtrlStyleEx(dwExStyle, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the style of a tree view control.
//@rdesc The style’s value.
DWORD SEC_TREECLASS::GetTreeCtrlStyle() const
{
	DWORD dwStyle = m_dwTreeStyle;

	// Some tree and list styles overlap.  So,
	// the list styles become the tree styles
	// where there is overlap.
	if (m_dwListStyle&LVS_EDITLABELS)
		dwStyle |= TVS_EDITLABELS;
	if (m_dwListStyle&LVS_SHOWSELALWAYS)
		dwStyle |= TVS_SHOWSELALWAYS;

	return dwStyle;
}


//@doc SEC_TREECLASS
//@mfunc Retrieves the extended style of a tree view control.
//@rdesc The extended style’s value.
DWORD SEC_TREECLASS::GetTreeCtrlStyleEx() const
{
	return m_dwTreeStyleEx;
}


//@doc SEC_TREECLASS
//@mfunc Sets the style of a tree view control.
//@rdesc Nonzero if the style is successfully set; otherwise 0.
//@parm DWORD | dwStyle | Value of the style to set.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw tree control; default is TRUE.
BOOL SEC_TREECLASS::SetTreeCtrlStyle(DWORD dwStyle, BOOL bRedraw)
{
	DWORD dwListStyleAdd = 0;
	DWORD dwListStyleRemove = 0;

	if (dwStyle&TVS_EDITLABELS)
		dwListStyleAdd |= LVS_EDITLABELS;
	else
		dwListStyleRemove |= LVS_EDITLABELS;

	if (dwStyle&TVS_SHOWSELALWAYS)
		dwListStyleAdd |= LVS_SHOWSELALWAYS;
	else
		dwListStyleRemove |= LVS_SHOWSELALWAYS;

	m_dwTreeStyle = (dwStyle&(TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_DISABLEDRAGDROP|TVS_SINGLEEXPAND));
	return ModifyListCtrlStyle(dwListStyleRemove, dwListStyleAdd, bRedraw);
}


//@doc SEC_TREECLASS
//@mfunc Sets value for the extended style of a tree view control.
//@rdesc Nonzero if the extended style is successfully set; otherwise 0.
//@parm DWORD | dwStyleEx | Value of the extended style to set. Can be one or more of the folllowing:
//@flag TVXS_WORDWRAP | Enable the word wrap feature.
//@flag	TVXS_COLUMNHEADER | Enable the column header.
//@flag TVXS_SORTHEADER | Makes the header act like a push-button. 
//@flag TVXS_HEADERDRAGDROP | Enable column drag & drop. This style is available only if TVXS_SORTHEADER is present.
//@flag	TVXS_MULTISEL | Enable multiple selection.
//@flag	TVXS_FLYBYTOOLTIPS | Enable tooltips.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw tree control; default is TRUE.
BOOL SEC_TREECLASS::SetTreeCtrlStyleEx(DWORD dwStyleEx, BOOL bRedraw)
{
	m_dwTreeStyleEx = dwStyleEx;

	// Since some style bits overlap must make sure that the corresponding listctrl
	//  bits are turned on.  

	/////////////////////////////////////////////////////////
	// ** Begin
	DWORD dwListStyleAdd = 0;
	DWORD dwListStyleRemove = 0;
	DWORD dwListStyleExAdd = 0;
	DWORD dwListStyleExRemove = 0;

	if( m_dwTreeStyleEx & TVXS_WORDWRAP )
		dwListStyleRemove |= LVS_NOLABELWRAP;
	else
		dwListStyleAdd |= LVS_NOLABELWRAP;

	if( m_dwTreeStyleEx & TVXS_COLUMNHEADER )
		dwListStyleRemove |= LVS_NOCOLUMNHEADER;
	else
		dwListStyleAdd |= LVS_NOCOLUMNHEADER;

	//Set the TVXS_SORTHEADER style only if TVXS_COLUMNHEADER is present. This can be removed separately.
	if( (m_dwTreeStyleEx & TVXS_SORTHEADER) && (m_dwTreeStyleEx & TVXS_COLUMNHEADER) )
		dwListStyleRemove |= LVS_NOSORTHEADER;
	else
		dwListStyleAdd |= LVS_NOSORTHEADER;

	//Set TVXS_HEADERDRAGDROP only if TVXS_SORTHEADER is present.
	if( (m_dwTreeStyleEx & TVXS_SORTHEADER) && (m_dwTreeStyleEx & TVXS_HEADERDRAGDROP) )
		dwListStyleExAdd |= LVS_EX_HEADERDRAGDROP;
	else
		dwListStyleExRemove |= LVS_EX_HEADERDRAGDROP;

	if( m_dwTreeStyleEx & TVXS_MULTISEL )
		dwListStyleRemove |= LVS_SINGLESEL;
	else
		dwListStyleAdd |= LVS_SINGLESEL;

	if( m_dwTreeStyleEx & TVXS_FLYBYTOOLTIPS )
		dwListStyleExAdd |= LVXS_FLYBYTOOLTIPS;
	else
		dwListStyleExRemove |= LVXS_FLYBYTOOLTIPS;
	// ** End
	//////////////////////////////////////////////////////////

	return ModifyListCtrlStyles( dwListStyleRemove, dwListStyleAdd, dwListStyleExRemove, dwListStyleExAdd, bRedraw );
}






//@doc SEC_TREECLASS
//@mfunc Modifies a tree view control’s style by removal and addition of specified styles.
//@rdesc Nonzero if specified styles are removed and added; otherwise 0.
//@parm DWORD | dwRemove | The styles to remove.
//@parm  DWORD | dwAdd | The styles to add.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw tree control; default is TRUE.
//@comm Modifies (by removal and addition) the style of a tree view control. 
BOOL SEC_TREECLASS::ModifyTreeCtrlStyle(DWORD dwRemove, DWORD dwAdd, BOOL bRedraw)
{
	DWORD dwTreeStyle = GetTreeCtrlStyle();

	dwTreeStyle &= ~(dwRemove);
	dwTreeStyle |= dwAdd;

	if (!SetTreeCtrlStyle(dwTreeStyle, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Modifies a tree view control’s extended style by removal and addition of specified extended styles.
//@rdesc Nonzero if specified extended styles are removed and added; otherwise 0.
//@parm DWORD | dwRemoveEx | The extended styles to remove.
//@parm  DWORD | dwAddEx | The extended styles to add.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw tree control; default is TRUE.
//@comm Modifies (by removal and addition) the extended style of a tree view control. 
BOOL SEC_TREECLASS::ModifyTreeCtrlStyleEx(DWORD dwRemoveEx, DWORD dwAddEx, BOOL bRedraw)
{
	DWORD dwTreeStyleEx = GetTreeCtrlStyleEx();

	dwTreeStyleEx &= ~(dwRemoveEx);
	dwTreeStyleEx |= dwAddEx;

	if (!SetTreeCtrlStyleEx(dwTreeStyleEx, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Modifies a tree view control’s style ans extended style by removal and addition of specified styles and extended styles.
//@rdesc Nonzero if specified styles and extended styles are removed and added; otherwise 0.
//@parm DWORD | dwRemove | The styles to remove.
//@parm  DWORD | dwAdd | The styles to add.
//@parm  DWORD | dwRemoveEx | The extended styles to remove.
//@parm  DWORD | dwAddEx | The extended styles to add.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw tree control; default is TRUE.
//@comm Modifies (by removal and addition) the style and extended style of a tree view control. 
BOOL SEC_TREECLASS::ModifyTreeCtrlStyles(DWORD dwRemove, DWORD dwAdd, DWORD dwRemoveEx,
												DWORD dwAddEx, BOOL bRedraw)
{
	if (!ModifyTreeCtrlStyle(dwRemove, dwAdd, FALSE))
		return FALSE;

	if (!ModifyTreeCtrlStyleEx(dwRemoveEx, dwAddEx, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Enables/disables the column header control.
//@rdesc void 
//@parm BOOL | bEnable | Specifies whether header control should be enabled or disabled.
//@parm BOOL | bSortHeader | Setting to TRUE makes the header behave like a button.
void SEC_TREECLASS::EnableHeaderCtrl(BOOL bEnable, BOOL bSortHeader)
{
	//Sort header can be set only if header is enabled
	ASSERT( bSortHeader? (bEnable? TRUE:FALSE):TRUE );

	if ( bEnable )
		m_dwTreeStyleEx |= TVXS_COLUMNHEADER;
	else
		m_dwTreeStyleEx &= ~TVXS_COLUMNHEADER;

	if( bSortHeader && (m_dwTreeStyleEx|TVXS_COLUMNHEADER) )
		m_dwTreeStyleEx |= TVXS_SORTHEADER;
	else
		m_dwTreeStyleEx &= ~TVXS_SORTHEADER;

	SEC_TREEBASE::EnableHeaderCtrl( bEnable, bSortHeader );

}


//@doc SEC_TREECLASS
//@mfunc Enables/disables wrapping of item text
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable word wrap.
void SEC_TREECLASS::EnableWordWrap(BOOL bEnable)
{
	if ( bEnable )
		m_dwTreeStyleEx |= TVXS_WORDWRAP;
	else
		m_dwTreeStyleEx &= ~TVXS_WORDWRAP;

	SEC_TREEBASE::EnableWordWrap( bEnable );
}



//@doc SEC_TREECLASS
//@mfunc Enables/disables tooltips.
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable tooltips.
void SEC_TREECLASS::EnableToolTips(BOOL bEnable)
{
	// modify the tree style to match
	if ( bEnable )
		m_dwTreeStyleEx |= TVXS_FLYBYTOOLTIPS;
	else
		m_dwTreeStyleEx &= ~TVXS_FLYBYTOOLTIPS;

	SEC_TREEBASE::EnableToolTips( bEnable );

}


//@doc SEC_TREECLASS
//@mfunc Enables/disables multiple selection.
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable multiple selection.
void SEC_TREECLASS::EnableMultiSelect(BOOL bEnable)
{
		// modify the tree style to match
	if ( bEnable )
		m_dwTreeStyleEx |= TVXS_MULTISEL;
	else
		m_dwTreeStyleEx &= ~TVXS_MULTISEL;

	SEC_TREEBASE::EnableMultiSelect(bEnable);
}



//@doc SEC_TREECLASS
//@mfunc Filters out a complete level of the tree from visibility.
//@rdesc void 
//@parm WORD | wLevel | Value for level of filter to set.
//@comm Filters out a complete level of the tree.  0 = root, 1 = root’s children, etc.
void SEC_TREECLASS::SetFilterLevel(WORD wLevel)
{
	Node *pSelected=GetNode(GetSelectedItem());
    m_pNodeRoot->SetFilterLevel(wLevel);
    if( !pSelected )
    {
        //must have current selection in order to rebuild list+parent view mode
        //ASSERT(FALSE);
        return;
    }

	SEC_TREEBASE::DeleteAllItems();
    FilterListBox();
}


//@doc SEC_TREECLASS
//@mfunc Retrieves level of the tree isolated by the filter set by SetFilterLevel.
//@rdesc The current filter level.
//@comm Retrieves a complete level of the tree isolated by the filter set by the SetFilterLevel member function.   0 = root, 1 = root’s children, etc.
WORD SEC_TREECLASS::GetFilterLevel() const
{
    return m_pNodeRoot->GetFilterLevel();
}


//@doc SEC_TREECLASS
//@mfunc Updates the drawing of an item.
//@rdesc Nonzero if successful; otherwise 0.
//@parm  HTREEITEM | hti | The handle of the item to update.
//@parm  BOOL | bLabelOnly | Flag indicating whether to update only the label of the item; default is FALSE.
//@parm  BOOL | bEraseBkgnd | Flag indicating whether to erase background.  Default is TRUE.
//@parm  BOOL | bUpdateBelow | Flag indicating whether to update below the item.  Default is FALSE.
//@parm  BOOL | bUpdateNow | If TRUE, the item will be repainted immediately, bypassing the message queue. Default FALSE.
//@comm Call this function to redraw an item or part of an item. This is used internally by the control to invalidate items
// and to immediately redraw in some cases, such as drop highlighting, when messages may be locked.
BOOL SEC_TREECLASS::Update( HTREEITEM hti, BOOL bLabelOnly, BOOL bEraseBkgnd, BOOL bUpdateBelow, BOOL bUpdateNow )
{
	int nIndex;
	if ((nIndex = ItemIndex(hti)) != -1)
	    return SEC_TREEBASE::Update( nIndex, bLabelOnly, bEraseBkgnd, bUpdateBelow, bUpdateNow);
	else
		return FALSE;
}


BOOL SEC_TREECLASS::LoadPaintContext( LvPaintContext* pPC )
{
	if (pPC->lvi.iSubItem == 0)
	{
		TvPaintContext* pTvPC;
		UINT mask;
		UINT stateMask;
	
		pTvPC = (TvPaintContext*)pPC;
		pTvPC->pNode = GetNodeAt(pPC->lvi.iItem);
		pTvPC->tvi = pTvPC->pNode->m_item;
		pTvPC->tvi.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_SELECTEDIMAGE | TVIF_IMAGE;
		pTvPC->tvi.stateMask = 0xffff;
		pTvPC->tvi.pszText = m_pszInternalBuffer;
		pTvPC->tvi.cchTextMax = LVXC_INTERNALBUFFERSIZE;
	
		// The following GetItem call could modify the mask
		// and stateMask values, so we have to make our own
		// copy of their values.
		mask = pTvPC->tvi.mask;
		stateMask = pTvPC->tvi.stateMask;
	
		if (!GetItem(&(pTvPC->tvi), FALSE, TRUE))
			return FALSE;
	
		// Next, we load the state information which exists only for the SEC_TREEBASE
		// class.  For example, LVIS_FOCUSED has meaning in LV_ITEM, but there
		// is not corresponding state in TV_ITEM.
		pPC->lvi.mask = LVIF_STATE;
		pPC->lvi.stateMask = LVIS_FOCUSED;
		if (!GetItem(&(pPC->lvi)))
			return FALSE;
	
		// At this point, the TV_ITEM structure (pTvPC->tvi), has been loaded
		// with all information required to paint the node.  However, some of
		// the information (i.e. selection state TVIS_SELECTED), has an equivalent
		// value (LVIS_SELECTED) in the LV_ITEM structure (pPC->lvi).  Since
		// the SEC_TREEBASE class will rely on the latter, we have to translate TV_ITEM
		// settings to LV_ITEM settings which makes the item's state accessible
		// by the SEC_TREEBASE class.
		if (mask & TVIF_STATE)
		{
			if (stateMask & TVIS_CUT)
				pPC->lvi.state |= (pTvPC->tvi.state & TVIS_CUT) ? LVIS_CUT : 0;
			if (stateMask & TVIS_DROPHILITED)
				pPC->lvi.state |= (pTvPC->tvi.state & TVIS_DROPHILITED) ? LVIS_DROPHILITED : 0;
			if (stateMask & TVIS_OVERLAYMASK)
				pPC->lvi.state |= (pTvPC->tvi.state & TVIS_OVERLAYMASK) ? LVIS_OVERLAYMASK : 0;
			if (stateMask & TVIS_SELECTED)
				pPC->lvi.state |= (pTvPC->tvi.state & TVIS_SELECTED) ? LVIS_SELECTED : 0;
			if (stateMask & TVIS_STATEIMAGEMASK)
				pPC->lvi.state |= INDEXTOSTATEIMAGEMASK(GET_STATE_IMAGE(pTvPC->tvi.state));
		}
	
		if (mask & TVIF_TEXT)
			pPC->lvi.pszText = pTvPC->tvi.pszText;
	
		if (mask & TVIF_IMAGE)
			pPC->lvi.iImage = pTvPC->tvi.iImage;
	
		// Retrieve any extended flags
		pTvPC->tvix.maskEx = TVIF_EX_STATEEX;
		pTvPC->tvix.stateExMask = TVIS_EX_DISABLED|TVIS_EX_HIDDEN;
		pTvPC->lvix.maskEx = LVIF_EX_STATEEX;
		pTvPC->lvix.stateExMask = LVIS_EX_DISABLED|LVIS_EX_HIDDEN;
		pTvPC->lvix.stateEx = 0;
		if (!GetItemEx(&(pTvPC->tvi), &(pTvPC->tvix)))
			return FALSE;
	
		if (pTvPC->tvix.stateEx & TVIS_EX_DISABLED)
			pTvPC->lvix.stateEx |= LVIS_EX_DISABLED;
	
		if (pTvPC->tvix.stateEx & TVIS_EX_HIDDEN)
			pTvPC->lvix.stateEx |= LVIS_EX_HIDDEN;
	
		PickItemBkgnd(pPC);
		PickTextColors(pPC);
		PickTextFont(pPC);
		PickIconBkgnd(pPC);

		return TRUE;
	}
	else
		return SEC_TREEBASE::LoadPaintContext(pPC);
}


SEC_TREECLASS::LvPaintContext*
SEC_TREECLASS::CreatePaintContext( CDC* pDC )
{
	return new TvPaintContext(pDC);
}


void SEC_TREECLASS::MeasureText(CDC* pDC, Node* pNode, const CString& strText, CRect& rectText) const
{
	// measure using the attribute dc if possible
	HDC hdc = pDC->m_hAttribDC;
	if ( !hdc )
		hdc = pDC->m_hDC;

	if( IsWordWrapEnabled() )
	{
		//Search for hanging indent (maybe do other options later)
		int iFind = strText.Find( _T("\\{HI}") );
		if( iFind>=0 )
		{
			/////
			//!! NB. Theres room for a new class here
			//!! ie text.GetFormated(), text.GetBeforeIndent(), etc...
			/////

			//Found hanging indent
			pNode->m_bHangingIndent       = TRUE;
			pNode->m_strBeforeIndent      = strText.Left(iFind+1);
			pNode->m_strAfterIndent       = strText.Right(strText.GetLength()-iFind-5);
			pNode->m_cxTextBeforeIndent   = pDC->GetTextExtent(
			                                  pNode->m_strBeforeIndent, 
			                                  pNode->m_strBeforeIndent.GetLength()-1)
			                                     .cx;

			CRect rect = rectText;
			rect.top = rect.bottom = 0;
			rect.left = rectText.left + pNode->m_cxTextBeforeIndent;
			::DrawText( hdc, pNode->m_strAfterIndent,
						   -1,
						   &rect,
						   DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX|DT_EXPANDTABS );
			
			//rectText is the TOTAL rect!
			rectText = rect;
		}
		else
		{		
			//no formatting options
			pNode->m_bHangingIndent=FALSE;
			
			rectText.top = rectText.bottom = 0;
			::DrawText( hdc, strText, 
			               -1, 
			               &rectText, 
			               DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX|DT_EXPANDTABS );


		}

		//rectText.Height() now contains height of word-wrapped text
		rectText.bottom += 2;

	}
	else
	{
		//Single line
		::DrawText(hdc, strText, 
					   strText.Find('\n'), 
					   &rectText,
					   DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_NOCLIP|DT_CALCRECT|DT_EXPANDTABS);

#ifndef UNDER_CE
		// DrawText does not measure properly with tabbed text
		DWORD dwSize = ::GetTabbedTextExtent( hdc, strText,	(int)_tcslen(strText), 0, NULL );
		CSize sExtent(dwSize);
		
		// adjust the rect for the max measurement
		int cx = rectText.Width();
		rectText.right = rectText.left + otmax(cx, sExtent.cx);			
#endif //UNDER_CE
		rectText.InflateRect(0, 1, 0, 1);

	}
}

//////////////////////////////////////////////////////////////
//                                                          //
//  FUNCTION: DrawHierarchyLines()                          //
//                                                          //
//  PURPOSE:  Draw lines connecting the node on this        //
//            row to its parent(s) and next and             //
//            previous siblings.                            //
//                                                          //
// ........................................................ //
//                                                          //
// How this function works:                                 //
//                                                          //
//  This function paints the hierarchy lines for each row   //
//  individually.  For example, for a node 5 levels deep    //
//  in the hierarchy, it will paint:                        //
//                                                          //
//   | | | | '-  [NODE]                                     //
//                                                          //
//  The above diagram assumes the node on this row has      //
//  no siblings below itself (i.e. no next sibling).  In    //
//  this case, an upward L shape is painted.  If the        //
//  node does have a next sibling, the drawing becomes:     //
//                                                          //
//   | | | | |-  [NODE]                                     //
//                                                          //
//  The painting starts by positioning the drawing point    //
//  at the far right of the picture above (i.e. the horz    //
//  line that connects to the node itself.  Next, it draws  //
//  the upward L shape.  Lastly, it enters a for loop       //
//  where it draws 'n' vertical lines where 'n' is the      //
//  hierarchical depth of the node.  Note, a vertical line  //
//  is drawn over the upward L shape iff the node has a     //
//  next sibling.                                           //
//                                                          //
//  There is only one exception to the above painting       //
//  algorithm.  The very first node in the tree doesn't     //
//  have the vertical line segment of the upward L shape.   //
//  This is because the very first node has no parent or    //
//  previous sibling to connect to.                         //
//                                                          //
//////////////////////////////////////////////////////////////


void SEC_TREECLASS::DrawHierarchyLines(LvPaintContext* pPC)
{
	SECTreeNode *pParent;
	CPoint pt;
	int w;

	TvPaintContext* pTvPC = (TvPaintContext*)pPC;
	int  iOrigDist = pTvPC->pNode->GetDistanceFromRoot()-1;
	BOOL bLinesAtRoot = m_dwTreeStyle&TVS_LINESATROOT && m_dwTreeStyle&(TVS_HASLINES|TVS_HASBUTTONS);

	
	// Position drawing point to far right
	pt.y = pPC->rect.top;

	BOOL bSelected = (pTvPC->tvi.state & TVIS_SELECTED) != 0;
	BOOL bHasImage = HasImage(&(pTvPC->tvi));

	if (m_pImageList && bHasImage)
	{
		pt.x = pPC->rect.left + pTvPC->pItem->rcIcon.Width()/2 + GetIndent()*(iOrigDist-1);
		pt.y += pPC->pItem->rcIcon.top + pPC->pItem->rcIcon.Height()/2;
	}
	else
	{
		pt.x = pPC->rect.left + 8 + GetIndent()*(iOrigDist-1);
		pt.y += pPC->pItem->rcText.top + pPC->pItem->rcText.Height()/2;
	}

	if (m_pStateImageList && GET_STATE_IMAGE(pTvPC->tvi.state) )
		w = pPC->rect.left + pPC->pItem->rcStateIcon.left - pt.x - 1;
	else if (m_pImageList && bHasImage)
		w = pPC->rect.left + pPC->pItem->rcIcon.left - pt.x - 1;
	else
		w = pPC->rect.left + pPC->pItem->rcText.left - pt.x - 1;


	if ( ((pt.y-m_ptViewportOrg.y)%2) != 0 )
		pt.y++;

	if ( ((pt.x+m_ptViewportOrg.x)%2) != 0 )
		pt.x++;

	if( bLinesAtRoot )
	{
		pt.x += GetIndent();
		w -= GetIndent();
	}


	// Draw the short horizontal line that connects to the
	// node icon (or text).
	if ( bLinesAtRoot || (iOrigDist > 0) )
		DrawHorzHierarchyLine(pPC->pdc, bSelected, pt.x, pt.x + w, pt.y);

	// Draw the upward line segment of the upward L shape.  This
	// is the line connecting this node to its parent or previous
	// sibling.  (NOTE: this line segment should not be drawn for
	// the very first node in the tree.)
	if (pTvPC->lvi.iItem > 0)
	{
		DrawVertHierarchyLine(pPC->pdc, bSelected, pt.x, pt.y, pPC->rect.top);
	}

	// Draw the line under the icon that connects this item to
	// its children.
	if ( (pTvPC->pNode->HasChildren() || ItemHasChildrenOnDemand( (HTREEITEM)(pTvPC->pNode) )) &&
		pTvPC->pNode->IsExpanded())
	{
		int cyOff;
		if (m_pImageList && bHasImage)
			cyOff = pPC->pItem->rcIcon.Height()/2;
		else if (m_pStateImageList && GET_STATE_IMAGE(pTvPC->tvi.state) )
			cyOff = pPC->pItem->rcStateIcon.Height()/2;
		else
			cyOff = pTvPC->pItem->rcText.Height()/2;
		DrawVertHierarchyLine(pPC->pdc, bSelected, pt.x+GetIndent(), pt.y + cyOff, pPC->rect.bottom);
	}

	// Draw the vertical lines to the left of the upward L shape.
	// Note, if the node has a next sibling, we do not draw an
	// upward L shape.  The hiearchy lines for the node described
	// above that is five level deep but has a next child is:
	//
	//     | | | | |-
	//
	// 
	pParent = pTvPC->pNode;
	for (int i = iOrigDist; i>=(bLinesAtRoot ? 0 : 1); i--)
	{
		if (pParent->GetNextSibling() != NULL)
			DrawVertHierarchyLine(pPC->pdc, bSelected, pt.x, pt.y, pPC->rect.bottom);
		pParent = (Node*)pParent->GetParent();
		pt.x -= GetIndent();
		pt.y = pPC->rect.top;
	}
}


void SEC_TREECLASS::DrawButton(LvPaintContext* pPC)
{
	static CPen   penSolid;
	static CPen   penShaded;
	static BOOL bCreated=FALSE;

	if( !bCreated )
	{
		bCreated=TRUE;
		BOOL bOk;

		bOk=penSolid.CreatePen( PS_SOLID, 1, GetSysColor(COLOR_WINDOWTEXT) );
		ASSERT(bOk);
		bOk=penShaded.CreatePen( PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW) );
		ASSERT(bOk);
	}

	TvPaintContext* pTvPC = (TvPaintContext*)pPC;
	if (pTvPC->pNode->HasChildren() || ItemHasChildrenOnDemand( (HTREEITEM)(pTvPC->pNode) ))
	{
		CRect rectButton;

		CalcButtonRect( &rectButton, pPC->lvi.iItem, pPC->rect );
		
		COLORREF rgbBack;
		// set the background color for the button.
		if ( IsWindowEnabled() )
			rgbBack = RGB( 255, 255, 255 );
		else
			rgbBack = GetBkColor();

		COLORREF rgbPrevBk = pPC->pdc->SetBkColor( rgbBack );

		//draw button box
		CPen *pPenOld = (CPen *)pPC->pdc->SelectObject(&penShaded);

		CBrush brush(rgbBack);
		CBrush* pBrushOld = (CBrush *)pPC->pdc->SelectObject(&brush);

		pPC->pdc->Rectangle( rectButton );

		pPC->pdc->SelectObject(pBrushOld);
		pPC->pdc->SelectObject(pPenOld);

		CPen* pOldPen = pPC->pdc->SelectObject(&penSolid);

		CPoint pt = rectButton.CenterPoint();
		if (((pt.x+m_ptViewportOrg.x)%2) != 0)
			pt.x--;
		int w = rectButton.Width()/3;

		//put '-' sign in box
		pPC->pdc->MoveTo((pt.x - w) + 1, pt.y);
		pPC->pdc->LineTo(pt.x + w, pt.y);
		
		if( !pTvPC->pNode->IsExpanded() )
		{
			//put '+' into box
			pPC->pdc->MoveTo(pt.x, (pt.y - w) + 1 );
			pPC->pdc->LineTo(pt.x, pt.y + w );
		}
		
		pPC->pdc->SelectObject( pPenOld );
		pPC->pdc->SetBkColor( rgbPrevBk );
		pPC->pdc->SelectObject( pOldPen );
	}
}


void SEC_TREECLASS::MeasureSubItem(LvPaintContext* pPC, UINT& cySubItem)
{

	if (pPC->lvi.iSubItem == 0)
	{
		CString strText;
		WORD    leftText = 0;

		// Tree items will be indented according to distance down the tree
		int cxIndent;
		TvPaintContext* pTvPC = (TvPaintContext*)pPC;
		cxIndent=(GetIndent()*(pTvPC->pNode->GetDistanceFromRoot()-1));

		if( m_dwTreeStyle&TVS_LINESATROOT && m_dwTreeStyle&(TVS_HASLINES|TVS_HASBUTTONS) )
			cxIndent += GetIndent();
		

		// test for state image
		BOOL bHasStateImage = FALSE;
		IMAGEINFO ii;
		::ZeroMemory( &ii, sizeof(IMAGEINFO));

		if( m_pStateImageList && 
			GET_STATE_IMAGE(pTvPC->tvi.state) && 
			m_pStateImageList->GetImageInfo( GET_STATE_IMAGE(pTvPC->tvi.state), &ii))
			bHasStateImage = TRUE;

		if( bHasStateImage )
		{

			if (!m_pImageList) {	// Make sure the icon is valid.
				pPC->pItem->rcIcon.left   = cxIndent;
				pPC->pItem->rcIcon.top    = 0;
				pPC->pItem->rcIcon.right  = pPC->pItem->rcIcon.left ;
				pPC->pItem->rcIcon.bottom = ii.rcImage.bottom - ii.rcImage.top;
			}
			
			pPC->pItem->rcStateIcon.left   = cxIndent;
			pPC->pItem->rcStateIcon.top    = 0;
			pPC->pItem->rcStateIcon.right  = pPC->pItem->rcStateIcon.left + (ii.rcImage.right-ii.rcImage.left) ; //WIDTH_BITMAP;
			pPC->pItem->rcStateIcon.bottom = ii.rcImage.bottom - ii.rcImage.top;

			//figure out where text starts
			leftText = (WORD)(pPC->pItem->rcStateIcon.right + (WIDTH_GAP));
		}


		// test for normal image
		BOOL bHasNormalImage = FALSE;
		if( m_pImageList && m_pImageList->GetImageInfo( pTvPC->tvi.iImage, &ii ))
			bHasNormalImage = TRUE;

		if( bHasNormalImage )
		{
			//Set icon rectangle
			if (!(m_pStateImageList && GET_STATE_IMAGE(pTvPC->tvi.state))) {	// Make sure the icon is valid.
				pPC->pItem->rcStateIcon.left   = cxIndent;
				pPC->pItem->rcStateIcon.top    = 0;
				pPC->pItem->rcStateIcon.right  = pPC->pItem->rcStateIcon.left ;
				pPC->pItem->rcStateIcon.bottom = ii.rcImage.bottom - ii.rcImage.top;
			}

			pPC->pItem->rcIcon.left   = pPC->pItem->rcStateIcon.right;
			pPC->pItem->rcIcon.top    = 0;
			pPC->pItem->rcIcon.right  = pPC->pItem->rcIcon.left + (ii.rcImage.right-ii.rcImage.left); //WIDTH_BITMAP;
			pPC->pItem->rcIcon.bottom = ii.rcImage.bottom - ii.rcImage.top;

			//figure out where text starts
			leftText = (WORD)(pPC->pItem->rcIcon.right + (WIDTH_GAP));
		}

		// no images
		if (!bHasNormalImage && !bHasStateImage)
		{
			pPC->pItem->rcIcon.left   = cxIndent;
			pPC->pItem->rcIcon.top    = 0;
			pPC->pItem->rcIcon.right  = cxIndent;
			pPC->pItem->rcIcon.bottom = 0;		
			leftText = (WORD)(cxIndent + (WIDTH_GAP));

			// Make sure the state is valid.
			pPC->pItem->rcStateIcon.left = pPC->pItem->rcStateIcon.right = pPC->pItem->rcIcon.left ;
			pPC->pItem->rcStateIcon.top = pPC->pItem->rcIcon.top ;
			pPC->pItem->rcStateIcon.bottom = pPC->pItem->rcIcon.bottom ;
		}

		//measure and SAVE the text RECTANGLE

		int w = 0;
		if ( GetColumnCount() > 1 )
			w = GetColumnWidth(pPC->iCol) - SEC_RIGHT_OFFSET;
		else if (  IsWordWrapEnabled() )
		{
			CRect r;
			GetInsideRect( &r );
			w = r.Width() - SEC_RIGHT_OFFSET;
		}
		else
			w = SECLIST_MAX_LOGICAL_WIDTH;


		pPC->pItem->rcText.SetRect(leftText, 0, w, 0);

		CFont* pOldFont=pPC->pdc->SelectObject(pPC->pFont);
		MeasureText(pPC->pdc, pTvPC->pNode, pTvPC->tvi.pszText, pPC->pItem->rcText);
		pPC->pdc->SelectObject(pOldFont);

		// Save the item's measurements
		UINT cy = otmax(pPC->pItem->rcIcon.Height(), pPC->pItem->rcStateIcon.Height());
		cy = otmax((UINT)pPC->pItem->rcText.Height(), cy);
		cySubItem = otmax(cy, cySubItem);

		// Since we're using a dotted pen in drawing the hierarchy
		// lines, we have to make sure each row is an even number of
		// pixels in height.  If we don't do this, the hierarchy lines
		// will appear blotchy.
		if ( (cySubItem%2) != 0 )
			cySubItem++;
		
		// Center the text vertically
		if (cySubItem > (UINT)pPC->pItem->rcText.Height())
			pPC->pItem->rcText.OffsetRect(0, (cySubItem - pPC->pItem->rcText.Height())/2);
	}
	else
		SEC_TREEBASE::MeasureSubItem(pPC, cySubItem);  
}


void SEC_TREECLASS::DrawSubItem(LvPaintContext* pPC)
{
	if (pPC->lvi.iSubItem == 0)
	{
		//Get our owner drawn data
		CString  strText;
		int      iImage;
		CRect    rectText;
		CRect rcIndent;

		BOOL bEditing = (pPC->lvi.iItem == m_nEditItem) && (pPC->iCol == m_nActiveColumn);

		TvPaintContext* pTvPC = (TvPaintContext*)pPC;
		BOOL bSelected = (pTvPC->tvi.state & TVIS_SELECTED) != 0;
		iImage = bSelected ? pTvPC->tvi.iSelectedImage : pTvPC->tvi.iImage;
		BOOL bHasImage = HasImage(&(pTvPC->tvi));

		strText = pTvPC->tvi.pszText;

		// Fill with background color
		CRect  rectFill;
		CBrush brush(pPC->rgbItemBkgnd);
		rectFill = pPC->rect;

		int xOrigin = pPC->rect.left;

		// Compute text size and position
		rectText = pPC->pItem->rcText;
		rectText.left += xOrigin;
		rectText.top += pPC->rect.top;
		rectText.right += xOrigin;
		rectText.bottom += pPC->rect.top;
		
		// If we have multiple columns, next columns clip the text of previous columns
		if ( GetColumnCount() > 1 || pPC->pdc->IsPrinting() )
			rectText.right = pPC->rect.right - SEC_RIGHT_OFFSET;


		// Unless we have an icon, the hierarchy lines end
		// where the label begins.
		rcIndent = pPC->rect;
		rcIndent.right = rectText.left;

		if( pPC->da == DrawEntire && m_pStateImageList)
		{
			//Then draw icons/bitmaps/whatever
			// If we are using a state image then select it!
			int iOurImage = GET_STATE_IMAGE(pTvPC->tvi.state);

			if( iOurImage >= m_pStateImageList->GetImageCount() )
			{
				TRACE( _T("Warning: Image does not exist\n") );
				iOurImage=0;
			}

			if ( iOurImage > 0 )
			{
			
	 			CPoint pt(  xOrigin + pPC->pItem->rcStateIcon.left,
	 					pPC->rect.top  + pPC->pItem->rcStateIcon.top  );

				m_pStateImageList->SetBkColor(pPC->rgbIconBkgnd);
				m_pStateImageList->Draw( pPC->pdc, iOurImage, pt, ILD_NORMAL);

				// Erase background to left of icon.
				// Don't erase icon or it will flicker.
				IMAGEINFO ii;
				m_pStateImageList->GetImageInfo( iOurImage, &ii );
				rectFill.right = pt.x;
				pPC->pdc->FillRect(rectFill, &brush);

				// Move the fill rect over for next fill between icon and label
				// Erase background between icon and label
				rectFill.left = rectFill.right + (ii.rcImage.right-ii.rcImage.left);
				rectFill.right = rectText.left;
				pPC->pdc->FillRect(rectFill, &brush);

				// Since we have an icon, the hierarchy lines end where
				// the icon begins
				rcIndent.right = pt.x;
			}
		}


		if( pPC->da == DrawEntire && bHasImage)
		{
			//Then draw icons/bitmaps/whatever

			if( iImage >= m_pImageList->GetImageCount() )
			{
				TRACE( _T("Warning: Image does not exist\n") );
				iImage=0;
			}
			
	 		CPoint pt(  xOrigin + pPC->pItem->rcIcon.left,
	 					pPC->rect.top  + pPC->pItem->rcIcon.top  );

			m_pImageList->SetBkColor(pPC->rgbIconBkgnd);

			// blend the icon image with the selection color if selected or drop highlighted
			// and the LVXS_BLENDIMAGE style is present
			BOOL bBlend = ((bSelected && GetFocus() == this)  || 
							(pPC->lvi.iItem == GetDropHilightIndex())) &&
							(GetListCtrlStyleEx() & LVXS_BLENDIMAGE);

			UINT nStyle = bBlend ? ILD_BLEND50 : ILD_NORMAL;
			// check for overlay image
			int iOimage =  GET_OVERLAY_IMAGE(pTvPC->tvi.state);
			if(iOimage > 0 )
				nStyle |= INDEXTOOVERLAYMASK(iOimage);
			
			// draw the image
			m_pImageList->Draw( pPC->pdc, iImage, pt, nStyle );

			// Erase background to left of icon.
			// Don't erase icon or it will flicker.
			IMAGEINFO ii;
			m_pImageList->GetImageInfo( iImage, &ii );
			rectFill.right = pt.x;
			pPC->pdc->FillRect(rectFill, &brush);


			// Move the fill rect over for next fill between icon and label
			// Erase background between icon and label
			rectFill.left = rectFill.right + (ii.rcImage.right-ii.rcImage.left);
			rectFill.right = rectText.left;
			pPC->pdc->FillRect(rectFill, &brush);

			// Since we have an icon, the hierarchy lines end where
			// the icon begins
			rcIndent.right = pt.x;
		}

		// Now erase everything to the right of the text string.
		// This is necessary because the last string painted here
		// may have been longer than the current, so we have to
		// erase everything to the right of the current text.
		rectFill.left = rectText.right;
		rectFill.right = pPC->rect.right;
		if ( rectFill.left < rectFill.right )
			pPC->pdc->FillRect(rectFill, &brush);

		// Now we can draw the lines and button since we've erased background
		if (pPC->pdc->RectVisible(rcIndent))
		{
			if (m_dwTreeStyle&TVS_HASLINES)
				DrawHierarchyLines(pPC);
			if (m_dwTreeStyle&TVS_HASBUTTONS)
				DrawButton(pPC);
		}

		// Compute fill rect
		rectFill.left = rectText.left - (WIDTH_GAP/2);
		rectFill.top = pPC->rect.top;
		rectFill.bottom = pPC->rect.bottom;

		if ((GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) && GetColumnCount() >= 1)
			rectFill.right = pPC->rect.right;	// full row select
		else if ( GetColumnCount() > 1 )
		{
			// NOT full row select, but multiple columns. Use the minimum of
			// the right side of the logical text rectangle, or the right side of the column.
			int iTextRight = xOrigin + pPC->pItem->rcText.right + (WIDTH_GAP/2);
			int iColumnRight = xOrigin + GetColumnWidth(pPC->iCol) - (SEC_RIGHT_OFFSET/2);
			rectFill.right = otmin( iTextRight, iColumnRight);
		}
		else	// plain vanilla single column and not full row select
			rectFill.right = xOrigin + pPC->pItem->rcText.right + (WIDTH_GAP/2);


		// Fill background
		BOOL bFullRow = GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS;
		// if we are editing and NOT in full row select, do not show
		// any highlighting or focus rect
		if ( !(bEditing && !bFullRow) )
		{
			CFont* pOldFont;
			COLORREF oldFore, oldBack;

			// Pick appropriate colors and select them into the DC
			pOldFont = pPC->pdc->SelectObject(pPC->pFont);
			oldFore = pPC->pdc->SetTextColor( pPC->rgbText );
			oldBack = pPC->pdc->SetBkColor( pPC->rgbTextBkgnd );

			// Fill background (shows selection)
			CBrush brush( pPC->rgbTextBkgnd );
			pPC->pdc->FillRect( rectFill, &brush );

			rectText.OffsetRect(0, 1);

			if( IsWordWrapEnabled() && !bEditing)
			{
				if( pTvPC->pNode->m_bHangingIndent )
				{
					//Hanging indent. Draw text before and after indent seperately
					CRect rectBefore,
						  rectAfter;

					rectBefore = rectText;
					rectBefore.right = rectBefore.left + pTvPC->pNode->m_cxTextBeforeIndent;
					pPC->pdc->DrawText( pTvPC->pNode->m_strBeforeIndent,
								   -1,
								   rectBefore,
								   DT_SINGLELINE|DT_NOPREFIX|DT_EXPANDTABS );
					
					rectAfter = rectText;
					rectAfter.left += pTvPC->pNode->m_cxTextBeforeIndent;
					pPC->pdc->DrawText( pTvPC->pNode->m_strAfterIndent,
								   -1,
								   rectAfter,
								   DT_WORDBREAK|DT_NOPREFIX|DT_EXPANDTABS );
				}
				else
				{

					// draw
					pPC->pdc->DrawText( strText,
						   -1,
						   rectText,
						   DT_VCENTER |DT_WORDBREAK|DT_NOPREFIX|DT_EXPANDTABS );
					



				}
			}
			else if ( !bEditing )
			{
				//no wordwrap
#ifndef UNDER_CE
				pPC->pdc->DrawText( strText, strText.Find('\n'), rectText,
					   DT_SINGLELINE|DT_LEFT| DT_VCENTER |
					   DT_NOPREFIX|DT_EXPANDTABS|DT_END_ELLIPSIS );
#else
				pPC->pdc->DrawText( strText, strText.Find('\n'), rectText,
					   DT_SINGLELINE|DT_LEFT| DT_VCENTER | DT_NOPREFIX|DT_EXPANDTABS);

#endif //UNDER_CE


			}

			pPC->pdc->SelectObject(pOldFont);
			pPC->pdc->SetTextColor( oldFore );
			pPC->pdc->SetBkColor( oldBack );

			//If we are in full row select, we will wait till we completely draw the row,
			// before drawing the focus rect. Unless there is only one column
			if( ( ((GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) == 0) || (GetColumnCount() == 1)) 
				&& pPC->lvi.iItem == SEC_TREEBASE::GetFocusIndex() 
				&& GetFocus() == this 
				&& !bEditing
				)
			{
				//Draw a selected rectangle
				pPC->pdc->DrawFocusRect( &rectFill );
			}
		}
	}
	else
		SEC_TREEBASE::DrawSubItem(pPC);
}


void SEC_TREECLASS::PickTextFont( LvPaintContext* pPC )
{
	TvPaintContext* ptvpc = (TvPaintContext*)pPC;
	ptvpc->pFont = GetLabelFont();

	if( ptvpc->tvi.state & TVIS_BOLD )
	{
		static CFont boldFont;
		
		LOGFONT lf;
		if( ptvpc->pFont->GetLogFont( &lf ) )
		{
			lf.lfWeight = 700;

			if( boldFont.GetSafeHandle() ) 
				boldFont.DeleteObject();

			if( boldFont.CreateFontIndirect( &lf ) )
				ptvpc->pFont = &boldFont;
		}
	}
}


void SEC_TREECLASS::DrawHorzHierarchyLine(CDC* pDC, BOOL bSelected, int x1, int x2, int y)
{
	COLORREF rgbTbl[2];
	int swap;
	int i;
	bSelected;	// not used

	// colors
	rgbTbl[0] = ::GetSysColor(COLOR_BTNSHADOW);
	rgbTbl[1] = GetBkColor();

	if (x2 < x1)
	{
		swap = x1;
		x1 = x2;
		x2 = swap;
	}

	if ( ((x1+m_ptViewportOrg.x)%2) != 0 )
		x1--;

	if ( pDC->IsPrinting() )
	{
		pDC->MoveTo(x1, y);
		pDC->LineTo(x2, y);
	}
	else
	{
		for (i=x1; i<x2; i+=2)
		{
			pDC->SetPixel(i, y, rgbTbl[0]);
			pDC->SetPixel(i+1, y, rgbTbl[1]);
		}
	}
}


void SEC_TREECLASS::DrawVertHierarchyLine(CDC* pDC, BOOL bSelected, int x, int y1, int y2)
{
	COLORREF rgbTbl[2];
	int swap;
	int i;
	bSelected;	// not used

	// colors
	rgbTbl[0] = ::GetSysColor(COLOR_BTNSHADOW);
	rgbTbl[1] = GetBkColor();

	if (y2 < y1)
	{
		swap = y1;
		y1 = y2;
		y2 = swap;
	}

	if ( ((y1+m_ptViewportOrg.y)%2) != 0 )
	  	y1--;

	if ( pDC->IsPrinting() )
	{
		pDC->MoveTo(x, y1);
		pDC->LineTo(x, y2);
	}
	else
	{
		for (i=y1; i<y2; i+=2)
		{
			pDC->SetPixel(x, i, rgbTbl[0]);
			pDC->SetPixel(x, i+1, rgbTbl[1]);
		}
	}
}


void SEC_TREECLASS::OnReturnKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	NotifyNMHDR( NM_RETURN );
	// this is not normal tree control behavior and will cause problems
	// for users defeating the expansion of items via notifications
	//OnRightKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
	nItem;
	nSubItem;
	bShift;
	bCtrl;
	nRepCnt;
	nFlags;
}


void SEC_TREECLASS::OnHomeKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	SEC_TREEBASE::OnHomeKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
}


void SEC_TREECLASS::OnEndKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	SEC_TREEBASE::OnEndKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
}


void SEC_TREECLASS::OnLeftKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	HTREEITEM htiSel;
	htiSel = GetItemAt(nItem);

	if( !htiSel || htiSel == TVI_ROOT )
		return;
	if ((ItemHasChildren(htiSel) || ItemHasChildrenOnDemand(htiSel)) && IsExpanded(htiSel))
		VERIFY(Expand( htiSel, TVE_COLLAPSE));
	else
	{
		HTREEITEM htiParent = GetParentItem(htiSel);
		if( !htiParent || htiParent==TVI_ROOT )
			return;
		DeselectAllItems();
		VERIFY(Select(htiParent, TVGN_CARET));
	}
	
	m_pToolTipWnd->Hide();
}


void SEC_TREECLASS::OnBackKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	OnLeftKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
}


void SEC_TREECLASS::OnMultKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	HTREEITEM htiSel = GetItemAt(nItem);
	if( !htiSel )
		return;
	
	// expand the item and all subitems
	ExpandCompletely( htiSel, TRUE );
}


void SEC_TREECLASS::OnRightKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	HTREEITEM htiSel = GetItemAt(nItem);

	if( !htiSel )
		return;
	if (ItemHasChildren(htiSel) && IsExpanded(htiSel))
	{
		HTREEITEM htiChild = GetChildItem(htiSel);
		DeselectAllItems();
		Select(htiChild, TVGN_CARET);
	}
	else if (ItemHasChildren(htiSel) || ItemHasChildrenOnDemand(htiSel))
		Expand(htiSel, TVE_EXPAND);
	
	m_pToolTipWnd->Hide();
}


void SEC_TREECLASS::NotifyBeginDrag(int nDragItem, POINT ptDrag)
{
	if ( m_dwTreeStyle & TVS_DISABLEDRAGDROP )
	{
		m_bDragging = FALSE;
		return;
	}

	NM_TREEVIEW nmtv;
	Node *pDragNode;

	// Validate params
	ASSERT(nDragItem >= 0 && nDragItem < GetItemCount());

	// fill a NM_TREEVIEW
	memset( &nmtv, 0, sizeof(NM_TREEVIEW) );
	
	// fill a NMHDR
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code	  = TVN_BEGINDRAG;

	// New Item
	pDragNode = GetNodeAt(nDragItem);
	ASSERT_POINTER(pDragNode, Node);
	nmtv.itemNew = pDragNode->m_item;
	nmtv.ptDrag = ptDrag;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv );
}


BOOL SEC_TREECLASS::NotifyBeginLabelEdit(int nEditItem, int iSubItem) const
{
	if ( iSubItem == 0 )
	{
		TV_DISPINFO tvdi;

		// Validate params
		ASSERT(nEditItem >= 0 && nEditItem < GetItemCount());

		// fill a NMHDR
		tvdi.hdr.hwndFrom = GetSafeHwnd();
		tvdi.hdr.idFrom   = GetDlgCtrlID();
		tvdi.hdr.code	  = TVN_BEGINLABELEDIT;
		
		//fill out specific to a treectrl
		Node* pNode = GetNodeAt(nEditItem);
		ASSERT_POINTER(pNode, Node);
		tvdi.item = pNode->m_item;
		tvdi.item.mask |= TVIF_TEXT;

		//Send notification message to parent window
		CWnd *pwndParent = NotifyWnd();
		return (BOOL)(pwndParent->SendMessage( WM_NOTIFY,
								 GetDlgCtrlID(), //TVN_BEGINLABELEDIT
								 (LPARAM)&tvdi ));
	}
	else
		return SEC_TREEBASE::NotifyBeginLabelEdit(nEditItem, iSubItem);
}


void SEC_TREECLASS::NotifyBeginRDrag(int nDragItem, POINT ptDrag)
{
	if ( m_dwTreeStyle & TVS_DISABLEDRAGDROP )
	{
		m_bDragging = FALSE;
		return;
	}

	NM_TREEVIEW nmtv;
	Node *pDragNode;

	// Validate params
	ASSERT(nDragItem >= 0 && nDragItem < GetItemCount());

	// fill a NM_TREEVIEW
	memset( &nmtv, 0, sizeof(NM_TREEVIEW) );
	
	// fill a NMHDR
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code	  = TVN_BEGINRDRAG;

	// Drag Item
	pDragNode = GetNodeAt(nDragItem);
	ASSERT_POINTER(pDragNode, Node);
	nmtv.itemNew = pDragNode->m_item;
	nmtv.ptDrag = ptDrag;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv );
}


void SEC_TREECLASS::NotifyDeleteItem(int nDelItem) const
{
	//this only gets called from the base class. We don't want to notify when
	// items are removed from the list control, such as when a node is collapsed.
	// Our notifications are sent from NotifyParent_DeleteItem.

	SEC_TREEBASE::NotifyDeleteItem(nDelItem);
}


BOOL SEC_TREECLASS::NotifyEndLabelEdit(int nEditItem, int iSubItem, CString& strEdit, BOOL bCommit) const
{
	TV_DISPINFO tvdi;
	
	BOOL bRet = FALSE;
	if ( iSubItem == 0 )
	{
		// Validate params
		ASSERT(nEditItem >= 0 && nEditItem < GetItemCount());

		// fill a NMHDR
		tvdi.hdr.hwndFrom = GetSafeHwnd();
		tvdi.hdr.idFrom   = GetDlgCtrlID();
		tvdi.hdr.code	  = TVN_ENDLABELEDIT;

		//fill out specific to a treectrl
		Node* pNode = GetNodeAt(nEditItem);
		ASSERT_POINTER(pNode, Node);
		tvdi.item = pNode->m_item;
		tvdi.item.mask |= TVIF_TEXT;

		LPTSTR pszInternalBuffer = NULL;
		// Make a copy of the edited string so the handler can alter it too
		if (bCommit)
		{
			pszInternalBuffer = new TCHAR[LVXC_INTERNALBUFFERSIZE];
			_tcscpy(pszInternalBuffer, (LPCTSTR)strEdit);
			tvdi.item.pszText = pszInternalBuffer;
		}
		else
			tvdi.item.pszText = NULL;

		//Send notification message to parent window
		CWnd *pwndParent = NotifyWnd();
		LRESULT result = pwndParent->SendMessage( WM_NOTIFY,
								 GetDlgCtrlID(), //TVN_ENDLABELEDIT
								 (LPARAM)&tvdi );

		// Store changes to edited string made by handler
		if (result != 0 && bCommit)
		{
			strEdit = tvdi.item.pszText;
			bRet = TRUE;
		}
		else
			bRet = FALSE;

		if ( pszInternalBuffer )
			delete [] pszInternalBuffer;
	}
	else
		bRet = SEC_TREEBASE::NotifyEndLabelEdit(nEditItem, iSubItem, strEdit, bCommit);

	return bRet;
}

// This notification sent AFTER a successful label edit change has occurred.
// This is the last notification for a label edit.
// To receive in your app, you must call 
// RegisterWindowMessage(SEC_TREECTRL_NOTIFYENDLABELEDITCHG_REGMSG) 
// to fetch the appropriate unique message id.
void SEC_TREECLASS::NotifyEndLabelEditChangeComplete(int nEditItem,int iSubItem) const {
	if(iSubItem!=0) {
		SEC_TREEBASE::NotifyEndLabelEditChangeComplete(nEditItem,iSubItem);
		return;
		}

	// Send our registered message to the notify window, pass the HTREEITEM changed
	HTREEITEM hti=GetItemAt(nEditItem);

	CWnd *pwndParent = NotifyWnd();
	if(!pwndParent) return;

	LRESULT result;
	result = pwndParent->SendMessage( sec_rm_tree_notify_end_label_edit_change,
								 GetDlgCtrlID(), 
								 (LPARAM)(hti));
}

void SEC_TREECLASS::NotifyItemExpanded(HTREEITEM hti, UINT nCode) const
{
	NM_TREEVIEW nmtv;
	Node *pNode;

	// fill a NM_TREEVIEW
	memset( &nmtv, 0, sizeof(NM_TREEVIEW) );
	
	// fill a NMHDR
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code	  = TVN_ITEMEXPANDED;

	// Drag Item
	pNode = GetNode(hti);
	ASSERT_POINTER(pNode, Node);
	nmtv.itemNew = pNode->m_item;
	nmtv.itemNew.state = pNode->IsExpanded() ? TVIS_EXPANDED : 0;
	nmtv.action = nCode;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv );
}


BOOL SEC_TREECLASS::NotifyItemExpanding(HTREEITEM hti, UINT nCode) const
{
	NM_TREEVIEW nmtv;
	Node *pNode;

	// fill a NM_TREEVIEW
	memset( &nmtv, 0, sizeof(NM_TREEVIEW) );
	
	// fill a NMHDR
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code	  = TVN_ITEMEXPANDING;

	// Drag Item
	pNode = GetNode(hti);
	ASSERT_POINTER(pNode, Node);
	nmtv.itemNew = pNode->m_item;
	nmtv.action = nCode;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	return (BOOL)(pwndParent->SendMessage( WM_NOTIFY,
						GetDlgCtrlID(), (LPARAM)&nmtv ));
}


void SEC_TREECLASS::NotifySelChanged(int nNewItem, int nOldItem, SelectAction sa) const
{
	NM_TREEVIEW nmtv;
	Node *pNodeNew, *pNodeOld;

	// fill a NM_TREEVIEW
	memset( &nmtv, 0, sizeof(NM_TREEVIEW) );
	
	// fill a NMHDR
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code	  = TVN_SELCHANGED;

	// Action
	if (sa == ByKeyboard)
		nmtv.action = TVC_BYKEYBOARD;
	else if (sa == ByMouse)
		nmtv.action = TVC_BYMOUSE;
	else
		nmtv.action = TVC_UNKNOWN;

	// Old Item
	if (nOldItem != -1)
	{
		pNodeOld = GetNodeAt(nOldItem);
		ASSERT_POINTER(pNodeOld, Node);
		nmtv.itemOld = pNodeOld->m_item;
	}

	// New Item
	if (nNewItem != -1)
	{
		pNodeNew = GetNodeAt(nNewItem);
		ASSERT_POINTER(pNodeNew, Node);
		nmtv.itemNew = pNodeNew->m_item;
	}

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv );
}


BOOL SEC_TREECLASS::NotifySelChanging(int nNewItem, int nOldItem, SelectAction sa) const
{
	NM_TREEVIEW nmtv;
	Node *pNodeNew, *pNodeOld;

	// fill a NM_TREEVIEW
	memset( &nmtv, 0, sizeof(NM_TREEVIEW) );
	
	// fill a NMHDR
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code	  = TVN_SELCHANGING;

	// Action
	if (sa == ByKeyboard)
		nmtv.action = TVC_BYKEYBOARD;
	else if (sa == ByMouse)
		nmtv.action = TVC_BYMOUSE;
	else
		nmtv.action = TVC_UNKNOWN;

	// Old Item
	if (nOldItem != -1)
	{
		pNodeOld = GetNodeAt(nOldItem);
		ASSERT_POINTER(pNodeOld, Node);
		nmtv.itemOld = pNodeOld->m_item;
	}

	// New Item
	if (nNewItem != -1)
	{
		pNodeNew = GetNodeAt(nNewItem);
		ASSERT_POINTER(pNodeNew, Node);
		nmtv.itemNew = pNodeNew->m_item;
	}

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	LRESULT r = pwndParent->SendMessage( WM_NOTIFY,
								GetDlgCtrlID(), (LPARAM)&nmtv );

	return (BOOL)r;
}


void SEC_TREECLASS::NotifySetDispInfo(const LV_ITEM* lvi)
{
	// grab the text if we are handling storage.
	if ( (lvi->iSubItem > 0) && IsStoringSubItemText() && ( lvi->mask & LVIF_TEXT ) )
	{
		HTREEITEM hItem = GetItemAt( lvi->iItem );
		m_pSubItemTextStorage->AddOrModifyString( hItem, lvi->iSubItem, lvi->pszText );
	}
	
	SEC_TREEBASE::NotifySetDispInfo(lvi);	// let it pass through
}




void SEC_TREECLASS::NotifySetDispInfo(TV_ITEM* pTVI) const
{
	TV_DISPINFO tvdi;

	// fill a NMHDR
	tvdi.hdr.hwndFrom = GetSafeHwnd();
	tvdi.hdr.idFrom   = GetDlgCtrlID();
	tvdi.hdr.code	  = TVN_SETDISPINFO;
	
	//fill out specific to a treectrl
	tvdi.item = *pTVI;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //TVN_SETDISPINFO
							 (LPARAM)&tvdi );
}


// this is overridden from the list control, to give correct lParam in NotifyGetDispInfo.

LPARAM SEC_TREECLASS::GetUserParam(Item* pItem) const
{
	Node* pNode = GetNode( pItem );		
	if ( pNode )
		return pNode->m_item.lParam;

	return 0;
}


void SEC_TREECLASS::NotifyGetDispInfo(LV_ITEM* lvi) const
{
	// retrieve subitem text from internal storage if needed.
	if ( (lvi->iSubItem > 0) &&  IsStoringSubItemText() && ( lvi->mask & LVIF_TEXT ) )
	{
		HTREEITEM hItem = GetItemAt( lvi->iItem );
		m_pSubItemTextStorage->GetString( hItem, lvi->iSubItem, lvi->pszText, lvi->cchTextMax);
	}
	else
		SEC_TREEBASE::NotifyGetDispInfo(lvi);
}




void SEC_TREECLASS::NotifyGetDispInfo(TV_ITEM* pTVI) const
{
	TV_DISPINFO tvdi;

	// fill a NMHDR
	tvdi.hdr.hwndFrom = GetSafeHwnd();
	tvdi.hdr.idFrom   = GetDlgCtrlID();
	tvdi.hdr.code	  = TVN_GETDISPINFO;
	
	//fill out specific to a treectrl
	tvdi.item = *pTVI;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //TVN_GETDISPINFO
							 (LPARAM)&tvdi );
	*pTVI = tvdi.item;
}


void SEC_TREECLASS::NotifySetDispInfoEx(const TV_ITEM_EX* pTVIX) const
{
	pTVIX; // UNUSED
}


void SEC_TREECLASS::NotifyGetDispInfoEx(TV_ITEM_EX* pTVIX) const
{
	pTVIX; // UNUSED
}


void SEC_TREECLASS::NotifyKeyDown(UINT nChar) const
{
	TV_KEYDOWN tvkd;

	// fill a NMHDR
	tvkd.hdr.hwndFrom = GetSafeHwnd();
	tvkd.hdr.idFrom   = GetDlgCtrlID();
	tvkd.hdr.code	  = TVN_KEYDOWN;
	
	//fill out specific to a treectrl
	tvkd.wVKey = (WORD)nChar;
	tvkd.flags = 0;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //TVN_KEYDOWN
							 (LPARAM)&tvkd );
}


BOOL SEC_TREECLASS::AddNodeToListBox( Node* pNodeAdd )
{
	int nIndex;
	Node *pNodeAbove;
	
	ASSERT( pNodeAdd->IsVisible() );
	
	if(     pNodeAdd->GetParent()==m_pNodeRoot 
	    &&  pNodeAdd->GetPrevSibling()==NULL )
	{
		//then item to be shown is the first child item
		nIndex=0;
	}
	else
	{
        pNodeAbove = GetNode(GetPrevVisibleItem((HTREEITEM)pNodeAdd));
        if( !pNodeAbove )
        {
        	ASSERT(FALSE);
        	return FALSE;
        }

        nIndex = NodeIndex(pNodeAbove);
        nIndex++;
	}

	Item* pItem = CreateItemForNode(pNodeAdd);


	RETURN_ON_ERROR(SEC_TREEBASE::InsertItem(nIndex, pItem) != -1, NULL);

	return TRUE;
}


BOOL SEC_TREECLASS::DelNodeFromListBox( Node *pNodeDel )
{
	int nIndex;

	RETURN_ON_ERROR((nIndex=NodeIndex(pNodeDel)) != -1, FALSE);
	RETURN_ON_ERROR(SEC_TREEBASE::DeleteItem(nIndex) != FALSE, FALSE);

	return TRUE;
}


//@doc SEC_TREECLASS
//@mfunc Changes the parent of an existing Node object.
//@syntax SetNodeParent( HTREEITEM hti, HTREEITEM htiNewParent, BOOL bInvalidate=TRUE, HTREEITEM hInsertAfter=TVI_LAST)
//@syntax SetNodeParent( SECTreeNode *pNode, SECTreeNode *pNodeParent, BOOL bInvalidate=TRUE, HTREEITEM hInsertAfter=TVI_LAST)
//@rdesc Returns TRUE if successful; otherwise 0.
//@parm  SECTreeNode | *pNode | A pointer to the node to re-parent.
//@parm  SECTreeNode | *pNodeParent | A pointer to the new parent.
//@parm  BOOL | bInvalidate | If TRUE the control will be redrawn after re-parenting. Otherwise you must handle invalidating the control yourself. Default is TRUE.
//@parm  HTREEITEM | hInsertAfter | Specifies where under the new parent the node should be inserted. Default is TVI_LAST.
//@comm Call this function to re-parent a tree node. This can be used
// effectively as a move operator for drag and drop. All children of
// the node being reparented will be moved as well, recursing until all
// child levels are moved.
BOOL SEC_TREECLASS::SetNodeParent( HTREEITEM hti, HTREEITEM htiNewParent, BOOL bInvalidate /*TRUE*/, HTREEITEM hInsertAfter /*TVI_LAST*/)
{
	return SetNodeParent( GetNode( hti ), GetNode( htiNewParent), bInvalidate, hInsertAfter );
}


BOOL SEC_TREECLASS::SetNodeParent( SECTreeNode *pNode, SECTreeNode *pNodeParent, BOOL bInvalidate /*TRUE*/, HTREEITEM hInsertAfter /*TVI_LAST*/)
{
	ASSERT( pNode && pNodeParent );
	if ( (pNode == pNodeParent) || pNode->IsDescendant( pNodeParent ) )
		return FALSE;

	HTREEITEM hSelection = GetCaretItem();
	SetItemState((HTREEITEM)pNode, 0, TVIS_SELECTED);
	DeselectAllItems();

	Node* pNodeX = (Node*)pNode;
	Node* pNodeParentX = (Node*)pNodeParent;

	Node* pCurrentParent = (Node*)(pNode->GetParent());

	BOOL bExpanded = pNodeX->IsExpanded();
	// collapse and remove the node form the tree and list
	if ( NodeInListBox( pNodeX ) )
	{
		if ( bExpanded )
			Expand( (HTREEITEM)pNodeX, TVE_COLLAPSE, FALSE );
		pNodeX->DetachFromTree();
		DelNodeFromListBox( pNodeX );
	}
    else
		pNodeX->DetachFromTree();

	// reload the previous parent of the node
	if ( pCurrentParent && NodeInListBox( pCurrentParent ) && pCurrentParent->IsExpanded())
	{
		Expand( (HTREEITEM)pCurrentParent, TVE_COLLAPSE, FALSE );
		Expand( (HTREEITEM)pCurrentParent, TVE_EXPAND, FALSE );
	}

	// reparent the node
	SECTreeNode* pNodeInsertAfter = DetermineNodePosition( hInsertAfter, pNodeX, pNodeParent );
	if (!pNodeParent->AddChild( pNodeX, pNodeInsertAfter) )
		return FALSE;

	// add to list
	if( ShouldBeInListBox( (HTREEITEM)pNodeX ) )
		AddNodeToListBox( pNodeX );

	// reload the new parent
	if ( pNodeParentX && NodeInListBox( pNodeParentX ) && pNodeParentX->IsExpanded())
	{
		Expand( (HTREEITEM)pNodeParentX, TVE_COLLAPSE, FALSE );
		Expand( (HTREEITEM)pNodeParentX, TVE_EXPAND, FALSE );
	}

	// expand the node if it was expanded before re-parenting
	if ( bExpanded && NodeInListBox( pNodeX ) )
		Expand( (HTREEITEM)pNodeX, TVE_EXPAND, FALSE );

	if ( bInvalidate )
	{
		ReMeasureAllItems(); 
		Invalidate();
	}

	// re-select the old selection, if it is still visible,
	// otherwise select the top level item
	DeselectAllItems();
	if ( ItemIndex(hSelection) != -1 )
	{
		SelectCaretItem(hSelection);
	}
	else
	{
		HTREEITEM hItem = GetRootItem( TVI_ROOT );
		EnsureVisible( hItem );
		SelectCaretItem(hItem);
	}

	return TRUE;		
}



void SEC_TREECLASS::InitRootNode(void)
{
	if( !m_pNodeRoot )
	{
		m_pNodeRoot = NewRefTreeCtrlNode(); 
		ASSERT( m_pNodeRoot );
		m_pNodeRoot->Expand();
	}
}



BOOL SEC_TREECLASS::DeleteUnChildedItem( HTREEITEM htiDelete, BOOL bDelFromListBox )
{
	Node *pNodeDelete = GetNode(htiDelete);

	if( pNodeDelete->GetFirstChild() )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pNodeDelete->DetachFromTree();
	if( GetSafeHwnd() )
	{
		
		if( bDelFromListBox && NodeInListBox( pNodeDelete ) )
			DelNodeFromListBox( pNodeDelete );

		NotifyParent_DeleteItem( htiDelete );
	}

	delete pNodeDelete;
	return TRUE;
}


BOOL SEC_TREECLASS::AddChildNodesToListBox( Node*, BOOL )
{
	return FALSE;
}


BOOL SEC_TREECLASS::NodeInListBox( Node *pNode ) const
{
	ASSERT_POINTER(pNode, Node);

	if (NodeIndex(pNode) != -1)
		return TRUE;
	else
		return FALSE;
}


BOOL SEC_TREECLASS::GetIndexSelItems( CWordArray* ) const
{
	return FALSE;
}


void SEC_TREECLASS::DelNonSiblingsFromListBox( Node *)
{
}


HTREEITEM SEC_TREECLASS::ExpandSelectedItem( UINT nCode )
{
    HTREEITEM hti=GetSelectedItem();
    if( !Expand( hti, nCode ) )
        return NULL;

    return hti;        
}


void SEC_TREECLASS::CollapseSelectedItemThenSelectParent()
{
}


void SEC_TREECLASS::AddSiblingsToListBox( Node * )
{
}


BOOL SEC_TREECLASS::FilterListBox()
{

    Node *pNode;
    for( pNode=(Node *)m_pNodeRoot->GetNextInDisplayOrder(); 
         pNode ; 
         pNode=(Node *)pNode->GetNextInDisplayOrder() )
    {
        if( pNode->IsVisible() )
        {
            if( !NodeInListBox(pNode) )
            {
                if(!AddNodeToListBox(pNode) )
                {
                    ASSERT(FALSE);
                    return(FALSE);
                }
            }
        }
        else
        {
            if( NodeInListBox(pNode) )
            {
				// If the node is no longer in the list ctrl, it
				// can't be selected.
				SetItemState((HTREEITEM)pNode, 0, TVIS_SELECTED);
                if( !DelNodeFromListBox(pNode) )
                {
                    ASSERT(FALSE);
                    return(FALSE);
                }
            }
        }
    }

    return TRUE;
}


void SEC_TREECLASS::AddChildrenToListCtrl(Node* pParent, int& nIndex)
{
    Node *pNode;
	Item* pItem;
	// If we're adding children, then the parent must already
	// be present in the list ctrl.  Otherwise, we don't know
	// which index to insert below.
	if(nIndex == -1) return;


    for( pNode=(Node *)pParent->GetFirstChild(); 
         pNode ; 
         pNode=(Node *)pNode->GetNextSibling() )
    {
		if( pNode->IsHidden() )
			continue;

		pItem = CreateItemForNode(pNode);

		VERIFY(SEC_TREEBASE::InsertItem(++nIndex, pItem) != -1);

		// animate expansion
		if(m_dwTreeStyleEx & TVXS_ANIMATE)
		{
			if(++m_nAnimateCount <= m_nMaxAnimations)
				SEC_TREEBASE::Update(nIndex, FALSE, TRUE, TRUE, TRUE);
		}

		if (pNode->IsExpanded())
			AddChildrenToListCtrl(pNode, nIndex);
	}
}


void SEC_TREECLASS::RemoveChildrenFromListCtrl(Node* pParent, int nParent)
{
    Node *pNode;
	int nIndex, cCount;
	int i;

	// If we're removing children, then the parent must 
	// be present in the list ctrl.  Otherwise, we don't know
	// which index to delete below.
	ASSERT(nParent != -1);
	cCount = 0;
	for (nIndex = nParent+1; nIndex < GetItemCount(); nIndex++, cCount++)
	{
		pNode = GetNodeAt(nIndex);
		if (!pParent->IsDescendant(pNode))
			break;
	}

	// Reset selection state for the nodes being collapsed.
	int nLast = nParent + 1 + cCount;
	int val;
	for (i=(int)m_arrSelItems.GetUpperBound(); i>=0; i--)
	{
		val = m_arrSelItems.GetAt(i);
		if (val > nParent && val < nLast)
		{
			pNode = GetNodeAt(val);
			SetItemState((HTREEITEM)pNode, 0, TVIS_SELECTED);
		}
	}

	// Now actually remove the item from the display
	if (cCount > 0)
	{
		ASSERT(m_nMaxAnimations > 0 );
		int nMod = cCount / m_nMaxAnimations;
		if(nMod == 0)
			nMod++;
		if(m_dwTreeStyleEx & TVXS_ANIMATE)
		{
			// animate collapse
			for(int i = 0; i < cCount; i++)
			{
				SEC_TREEBASE::DeleteItem(nParent+1, 1);	// delete one at a time
				if(i % nMod == 0)
					UpdateWindow();
			}
			UpdateWindow();
		}
		else	// remove all at once
			SEC_TREEBASE::DeleteItem(nParent+1, cCount);

	}
}


void SEC_TREECLASS::CalcButtonRect
	( 
		CRect *pRectButton, 
		int iItem,
		const CRect & rcItem  
	) const
//
// This is the button the user clicks on to expand/contract
// a tree node. Need to know to test for hits.
//
{
	Node* pNode = GetNodeAt(iItem);
	Item* pItem = SEC_TREEBASE::GetItemAt(iItem);


	int iDist  = pNode->GetDistanceFromRoot()-1;
	int yMid;
	int xMid;
	int w;

	BOOL bHasImage = HasImage(&(pNode->m_item));

	yMid = rcItem.top;
	if (m_pImageList && bHasImage)
	{
		xMid = rcItem.left + pItem->rcIcon.Width()/2 + GetIndent()*(iDist-1);// - (bLinesAtRoot ? 0 : GetIndent());
		yMid += pItem->rcIcon.top + pItem->rcIcon.Height()/2;
		w = otmin(15, (int)(pItem->rcIcon.Height()/1.65));
	}
	else if (m_pStateImageList && GET_STATE_IMAGE(pNode->m_item.state) )
	{
		xMid = rcItem.left + pItem->rcStateIcon.Width()/2 + GetIndent()*(iDist-1);// - (bLinesAtRoot ? 0 : GetIndent());
		yMid += pItem->rcStateIcon.top + pItem->rcStateIcon.Height()/2;
		w = otmin(15, (int)(pItem->rcIcon.Height()/1.65));
	}
	else
	{
		xMid = rcItem.left + 8 + GetIndent()*(iDist-1);// - (bLinesAtRoot ? 0 : GetIndent());
		yMid += pItem->rcText.top + pItem->rcText.Height()/2;
		w = 8;
	}

	if ( ((yMid-m_ptViewportOrg.y)%2) != 0 )
		yMid++;

	if ( ((xMid+m_ptViewportOrg.x)%2) != 0 )
		xMid++;

	if( m_dwTreeStyle&TVS_LINESATROOT && m_dwTreeStyle&(TVS_HASLINES|TVS_HASBUTTONS) )
		xMid+=GetIndent();



	pRectButton->left   = xMid-w/2;
	pRectButton->top    = yMid-w/2;
	pRectButton->right  = pRectButton->left + w;
	pRectButton->bottom = pRectButton->top  + w;

	if ( (pRectButton->Height()%2) != 1 )
		pRectButton->bottom++;

	if ( (pRectButton->Width()%2) != 1 )
		pRectButton->right++;
}


BOOL SEC_TREECLASS::SelectCaretItem( HTREEITEM hti )
{
	int iFind;

	RETURN_ON_ERROR((iFind = ItemIndex(hti)) != -1, FALSE);
	
	return SelectFocusItem(iFind);
}


BOOL SEC_TREECLASS::ToolTipManager( CPoint )
{
	return FALSE;
}


BOOL SEC_TREECLASS::ShouldBeInListBox( HTREEITEM hti ) const
{
	Node *pNode = GetNode(hti);
	return pNode->IsVisible();
}


void SEC_TREECLASS::NotifyParent_DeleteItem(HTREEITEM htiDelete)
{

    TV_ITEM tvi;
	tvi.mask = TVIF_HANDLE | TVIF_PARAM;
	tvi.hItem = htiDelete;
    if( !GetItem(&tvi) )
    {
        ASSERT(FALSE);
        return;
    }


	NM_TREEVIEW nmtv;
	memset( &nmtv, 0, sizeof(nmtv) );

	nmtv.itemOld = tvi;
	nmtv.action  = TVN_DELETEITEM;
	nmtv.hdr.hwndFrom = GetSafeHwnd();
	nmtv.hdr.idFrom   = GetDlgCtrlID();
	nmtv.hdr.code = TVN_DELETEITEM;
    
	CWnd *pwndParent=NotifyWnd();
    if( pwndParent )
        pwndParent->SendMessage( WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtv);

	if ( IsStoringSubItemText() )
		m_pSubItemTextStorage->DeleteItem( htiDelete );

}


void SEC_TREECLASS::OnDestroy()
{
	DeleteItem( TVI_ROOT );
	SEC_TREEBASE::OnDestroy();
}


void SEC_TREECLASS::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	//We call the SEC_TREEBASE class first because it sends the notification messages for us...
	SEC_TREEBASE::OnLButtonDblClk(nFlags, point);
    
	if ( m_lastNotifyResult == 0 )
	{
		UINT      iHitFlags;
		HTREEITEM htiHit;
		htiHit=HitTest( point, &iHitFlags );

		if( nFlags&MK_CONTROL || nFlags&MK_SHIFT )
			return;

		if( iHitFlags&(TVHT_ONITEMICON|TVHT_ONITEMBUTTON|TVHT_ONITEMLABEL))
			Expand(htiHit, TVE_TOGGLE);

		if(m_pToolTipWnd->GetSafeHwnd() && m_pToolTipWnd->IsWindowVisible())
			m_pToolTipWnd->Hide();
	}
}


void SEC_TREECLASS::OnLButtonDown( UINT nFlags, CPoint point )
{
#ifndef UNDER_CE
	//In case of MDI clicking on tooltip, fails to activate the parent window
	if(m_pToolTipWnd->GetSafeHwnd() && m_pToolTipWnd->IsWindowVisible())
	{
		CWnd* pWndTop = GetParentFrame();		
		SendMessage(WM_MOUSEACTIVATE, WPARAM(pWndTop->m_hWnd), MAKELPARAM(HTCLIENT, WM_LBUTTONDOWN));
	}	
#endif //UNDER_CE	
	if (GetFocus() != this)
		SetFocus();


	// cache the current focus item
	HTREEITEM htiPrev =	GetSelectedItem();

	// lets see whats been hit
    UINT iHitFlags;
    HTREEITEM   htiHit;
    htiHit=HitTest( point, &iHitFlags );

	// TVS_SINGLEEXPAND support
	if( (m_dwTreeStyle & TVS_SINGLEEXPAND) && htiPrev && htiHit )
	{
		SEC_TREEBASE::OnLButtonDown(nFlags, point);
		HTREEITEM hti =	GetSelectedItem();
		// check for click on same item, toggle expand
		if((hti == htiPrev) && (hti == htiHit))
			Expand(htiHit, TVE_TOGGLE );
		else if((hti != htiPrev) && (hti == htiHit))
		{
			if(IsExpanded(htiPrev))		// close previous focus item
			{
				Node* pNodePrev = GetNode(htiPrev);
				Node* pNode = GetNode(hti);
				if(!pNodePrev->IsDescendant(pNode))
					Expand(htiPrev, TVE_COLLAPSE);
			}
			
			Expand(hti, TVE_EXPAND);	// expand the new selection

		}

	}
	else if( iHitFlags&TVHT_ONITEMBUTTON )
		Expand(htiHit, TVE_TOGGLE );
	else
		SEC_TREEBASE::OnLButtonDown(nFlags, point);


}


CBitmap* SEC_TREECLASS::BitmapFromText( CDC* pdc, const CString& rStr)
{
	CRect rectText(0,0,0,0);
	pdc->DrawText(rStr,&rectText,DT_CALCRECT|DT_EXPANDTABS|DT_NOPREFIX);	// must use DrawText, not GetTextExtent, for accurate estimate

	CBitmap* pTextBmp = new CBitmap, *pOldBmp = NULL;
	pTextBmp->CreateCompatibleBitmap( pdc, rectText.Width(), rectText.Height() );

	pOldBmp = pdc->SelectObject( pTextBmp );
	pdc->DrawText( rStr, rectText, DT_EXPANDTABS|DT_NOPREFIX );

	pdc->SelectObject( pOldBmp );
	
	return pTextBmp;
}


CImageList* SEC_TREECLASS::ImageListForText( CDC* pdc, const CString& rStr)
{
	ASSERT_VALID(pdc);

	CImageList* pIml = NULL;
	
	CBitmap* pTextBmp = BitmapFromText( pdc, rStr);
	if( pTextBmp )
	{
		BITMAP bm;
		VERIFY(pTextBmp->GetBitmap( &bm ));

		pIml = new CImageList;
		VERIFY(pIml->Create( bm.bmWidth, bm.bmHeight, ILC_COLOR|ILC_MASK, 1, 1 ));
		pIml->Add( pTextBmp, 0xFFFFFFFF );

		delete pTextBmp;
	}

	return pIml;
}


CImageList* SEC_TREECLASS::ImageListForImage( CImageList* pImlFrom, int iImageIndex )
{
	ASSERT_VALID(pImlFrom);
	ASSERT( iImageIndex < pImlFrom->GetImageCount() );

	HICON hIcon = pImlFrom->ExtractIcon( iImageIndex );

	IMAGEINFO ii;
	::ZeroMemory( &ii, sizeof(IMAGEINFO) );

	if( hIcon != NULL && pImlFrom->GetImageInfo( iImageIndex, &ii ))
	{
		CImageList* pImlImage = new CImageList;
		pImlImage->Create( (ii.rcImage.right - ii.rcImage.left), 
			               (ii.rcImage.bottom - ii.rcImage.top), 
						   ILC_COLOR|ILC_MASK, 1, 1 );

		pImlImage->Add( hIcon );

		return pImlImage;
	}

	return NULL;
}


CImageList* SEC_TREECLASS::CombineImageLists( CImageList* pImlImage, CImageList* pImlText )
{

	if (!pImlImage && !pImlText)
		return NULL;

	HIMAGELIST hImlImage = NULL,
			   hImlText = NULL;

	IMAGEINFO imageInfo;
	::ZeroMemory(&imageInfo, sizeof(imageInfo));

	CRect rcImage;
	rcImage.SetRectEmpty();

	if (pImlImage)
	{
		pImlImage->GetImageInfo( 0, &imageInfo );
		rcImage = imageInfo.rcImage;
		hImlImage = pImlImage->GetSafeHandle();
	}

	if ( pImlText )
		hImlText  = pImlText->GetSafeHandle();

	int dx = rcImage.Width() + 6;
	int dy = 0;

	CImageList* pImlCombo = NULL;
	HIMAGELIST hImlCombo = NULL;

	if ( hImlImage && hImlText )
		hImlCombo = ImageList_Merge( hImlImage, 0, hImlText, 0, dx, dy );


	if( hImlCombo != NULL )
	{
		pImlCombo = new CImageList;
		pImlCombo->Attach( hImlCombo );
	}


	return pImlCombo;
}


//@doc SEC_TREECLASS
//@mfunc Causes the item's width and height to be measured. The item will then be invalidated.
//@rdesc void 
//@parm  HTREEITEM | hti | The handle of the item to remeasure.
//@comm Certain operations can change the item metrics, such as a change
// in image size, or font size. Usually the tree control will remeasure items
// as needed. This function is available in case you ever need to force a
// remeasure on an item.
void SEC_TREECLASS::ReMeasureItem( HTREEITEM hti )
{
	//ASSERT->Can't remeasure an item that isn't in the tree control...
	if ( ItemIndex(hti) != -1 )
		SEC_TREEBASE::ReMeasureItem( ItemIndex(hti) );
	RecalcScrollBars();
}


//@doc SEC_TREECLASS
//@mfunc Sets the height of a single tree item
//@rdesc The previous height of the item 
//@parm  HTREEITEM | hti | The item to set the height for
//@parm  UINT | cyItemHeight | The height of the item, in pixels
//@comm This method sets the preferred height of the item. If the image or text height is larger
// than this value, the larger value will be used instead.

UINT SEC_TREECLASS::SetItemHeight(HTREEITEM hti, UINT cyItemHeight) const
{
	UINT retval = 0;
	Node* pNode = GetNode(hti);
	if(pNode)
	{
		retval = pNode->m_cyPreferred;
		pNode->m_cyPreferred = cyItemHeight;
		int nItem = ItemIndex(hti);
		if (nItem >= 0)
			SEC_TREEBASE::SetItemHeight(nItem, cyItemHeight);
	}
	return retval;
}


//@doc SEC_TREECLASS
//@mfunc Enables or disables subitem text storage for multi-column trees.
//@rdesc void 
//@parm  BOOL | bEnable | TRUE to begin storing subitem text. FALSE to revert back to the callback functions.
//@comm By default, the tree control does not store subitem text internally.
// When a sub-item is redrawn, an LVN_GETDISPINFO notification is sent to request the text
// for the subitem. Calling StoreSubItemText( TRUE ) will signal the control to begin
// storing subitem text internally, so subsequent calls to SetItemText( hti, 1, _T("Column 1 text")) will
// work as expected. SetItemString would have the same result as SetItemText. This function can be
// called at any time after construction.
void SEC_TREECLASS::StoreSubItemText( BOOL bEnable )
{
	if ( bEnable && !IsStoringSubItemText() )
		m_pSubItemTextStorage = new SECSubItemTextStorage;
	else if ( !bEnable && IsStoringSubItemText() )
	{
		delete m_pSubItemTextStorage;
		m_pSubItemTextStorage = NULL;
	}
}


BOOL SEC_TREECLASS::HasImage(TV_ITEM* pTVI) const
{
	if ( pTVI && m_pImageList)
	{
		BOOL bSelected = (pTVI->state & TVIS_SELECTED) != 0;
		int iImage = bSelected ? pTVI->iSelectedImage : pTVI->iImage;
		if ( iImage != SEC_NO_IMAGE )
			return TRUE;
	}
	return FALSE;
	
}



SECTreeNode *SEC_TREECLASS::DetermineNodePosition
	(
	HTREEITEM hInsertAfter,
	Node* pNodeNew,
	SECTreeNode* pNodeParent
	)
{
	Node *pChild;
	SECTreeNode *pNodeInsertAfter = 0;

	//position relative to sibling
	switch( (DWORD)hInsertAfter )
	{
		case (DWORD)TVI_FIRST:
			pNodeInsertAfter = TREENODE_FIRST;
			break;

		case (DWORD)0:
		case (DWORD)TVI_ROOT:
		case (DWORD)TVI_LAST:
			pNodeInsertAfter = TREENODE_LAST;
			break;

		case (DWORD)TVI_SORT:
			{			
				// Let's find the node to put it after		
				pNodeInsertAfter = TREENODE_FIRST;
				for( pChild = (Node *)pNodeParent->GetFirstChild(); 
					 pChild; 
					 pChild = (Node *)pChild->GetNextSibling() )
				{
					if( CompareItem(pChild, pNodeNew) > 0 )
						break;
					pNodeInsertAfter = pChild ;
				}
			}
			break;
		
		case (DWORD)TVI_SORT_DESCENDING:
			{			
				// Let's find the node to put it before		
				pNodeInsertAfter = TREENODE_FIRST;
				for( pChild = (Node *)pNodeParent->GetFirstChild(); 
					 pChild; 
					 pChild = (Node *)pChild->GetNextSibling() )
				{
					if( CompareItem(pChild, pNodeNew) < 0 )
						break;
					pNodeInsertAfter = pChild ;
				}
			}
			break;
		
		default:
			if( !GetChildItem((HTREEITEM)pNodeParent) )
			{
				//Parent has no children so user should have set
				//hInsertAfter to one of the above cases. (MFCTREE
				//sample code makes this mistake).So lets set it
				//to TVI_LAST.
				pNodeInsertAfter = TREENODE_LAST;
			}
			else
				pNodeInsertAfter = GetNode(hInsertAfter);
			break;
	}

	return pNodeInsertAfter;
}


void SEC_TREECLASS::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_idAutoExpandTimer)
	{
		KillTimer( m_idAutoExpandTimer );
		m_idAutoExpandTimer=0;

		if(m_dwTreeStyleEx & TVXS_AUTOEXPAND)
		{
			// expand the current drop highlight item
			HTREEITEM hti = GetDropHilightItem();
			if(hti && !IsExpanded(hti))
			{
				Expand(hti, TVE_EXPAND, TRUE);				
				UpdateWindow();
			}
		}

	}

	SEC_TREEBASE::OnTimer(nIDEvent);
}

void SEC_TREECLASS::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Since already handled + and - via OnKeyDown to expand and collapse, 
	// let us not pass it on to the base class (otherwise it might beep).
	if((nChar == 45) || (nChar == 95) || (nChar == 61) || (nChar == 43) || (nChar == 42))
		return;

	SEC_TREEBASE::OnChar(nChar, nRepCnt, nFlags);
}

SEC_TREEBASE::Item* SEC_TREECLASS::CreateItemForNode(Node* pNode)
{

#if (_MFC_VER >= 0x0600)
	ASSERT(pNode);
#else
	ASSERT(pNode);
#endif


	Item* pItem = CreateNewItem();

#if (_MFC_VER >= 0x0600)
	ASSERT(pItem);
#else
	ASSERT(pItem);
#endif

	pItem->SetItemData( (DWORD)pNode );
	pItem->m_cy = pNode->m_cyPreferred;
	return pItem;
}

//@doc SEC_TREECLASS
//@mfunc Enables/disables scrolling after an item is expanded
//@rdesc void 
//@parm  BOOL | bEnable | TRUE to enable auto scrolling, FALSE to disable. Default TRUE.
//@comm By default, the tree control will scroll when an item is expanded.
void SEC_TREECLASS::EnableScrollOnExpand(BOOL bEnable)
{
	m_bScrollOnExpand = bEnable;
}


#ifdef _DEBUG
	#undef new
#endif


#endif // defined(WIN32)
