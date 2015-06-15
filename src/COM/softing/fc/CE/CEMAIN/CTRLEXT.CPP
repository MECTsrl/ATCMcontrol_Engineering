
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "ctrlext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#define _AFXCTL_INLINE inline


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlNode


const CTreeCursor& CTreeCursor::operator =(const CTreeCursor& posSrc)
{
	if(&posSrc != this){
		m_hTreeItem = posSrc.m_hTreeItem;
		m_pTree = posSrc.m_pTree;
	}
	return *this;
}


CTreeCursor CTreeCursor::_Insert(LPCTSTR strItem,int nImageIndex,HTREEITEM hAfter)
{
	TV_INSERTSTRUCT ins;
	ins.hParent = m_hTreeItem;
	ins.hInsertAfter = hAfter;
	ins.item.mask = TVIF_TEXT;
	ins.item.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		ins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		ins.item.iImage = nImageIndex;
		ins.item.iSelectedImage = nImageIndex;
	}
	return CTreeCursor(m_pTree->InsertItem(&ins), m_pTree);
}

int CTreeCursor::GetImageID()
{
	TV_ITEM item;
	item.mask = TVIF_HANDLE | TVIF_IMAGE;
	item.hItem = m_hTreeItem;
	m_pTree->GetItem(&item);
	return item.iImage;
}

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx

// All handled by inlines

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

BOOL CListCtrlEx::AddColumn(LPCTSTR strItem,int nItem,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = GetStringWidth(lvc.pszText) + 15;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return InsertColumn(nItem,&lvc);
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}
	if(nSubItem == 0)
		return InsertItem(&lvItem);
    return CListCtrl::SetItem(&lvItem);
}


//------------------------------------------------------------------*
/**
 * set item.
 *
 *  change icon and / or text of item
 *
 * @param           [in] iItem: item number = line number in list ctrl
 * @param           [in] iSubItem: subitem number = column number in list ctrl
 * @param           [in] strItem: new item text
 * @param           [in] iImageIndex: new index of image in image list (default -1)
 * @return          successful?
*/
BOOL CListCtrlEx::SetItem(int iItem, int iSubItem, LPCTSTR strItem, int iImageIndex /*=-1*/)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = iItem;
	lvItem.iSubItem = iSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(iImageIndex != -1)
    {
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = iImageIndex;
	}
    return CListCtrl::SetItem(&lvItem);
}

/////////////////////////////////////////////////////////////////////////////