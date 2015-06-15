// FcNavTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CpNavigator.h"
#include "CpObjectList.h"
#include "CpNavTreeCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpNavTreeCtrl

//*****************************************************************************
CCpNavTreeCtrl::CCpNavTreeCtrl()
//*****************************************************************************
{
	m_bCtrlKey = FALSE;
	m_ClassFilter = 0;
}

//*****************************************************************************
CCpNavTreeCtrl::~CCpNavTreeCtrl()
//*****************************************************************************
{
	ASSERT_VALID(this);
	ASSERT(m_ImageList.m_hImageList == NULL);
}


//*****************************************************************************
BOOL CCpNavTreeCtrl::Create(
	const RECT& rect, 
	CWnd* pParentWnd, 
	UINT nID )
//*****************************************************************************
{
	return CTreeCtrl::Create(TVS_SHOWSELALWAYS | TVS_HASLINES | TVS_HASBUTTONS, rect, pParentWnd, nID);
}

//*****************************************************************************
BOOL CCpNavTreeCtrl::ShowObjectList(
	CCpObjectList* pObjectList, 
	unsigned InitialDisplayLevel /*=99*/,
	unsigned ClassFilter/*=0*/)
//*****************************************************************************
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));

	if (IsWindow(m_hWnd))
	{
		CTreeCtrl::DeleteAllItems();
		m_ClassFilter = ClassFilter;

		if ((pObjectList->GetObjectCount() == 1 &&  pObjectList->GetTypeId() == FCNAV_ITEMTYPE_PROJECT) ||
			pObjectList->GetObjectCount() > 1)
		{
			HTREEITEM hRootItem = InsertItem(NULL, pObjectList);
			ASSERT(hRootItem != NULL);
			InsertSubTree(hRootItem, pObjectList, InitialDisplayLevel);
		}

		return TRUE;
	}
	return FALSE;
}

//*****************************************************************************
BOOL CCpNavTreeCtrl::InsertSubTree(
	HTREEITEM hParentItem,
	CCpObjectList* pParent,
	unsigned InitialDisplayLevel)
//*****************************************************************************
{
	CCpObjectList* pChild = NULL;
	HTREEITEM hItem = NULL;
	if (GetItemLevel(hParentItem) <= InitialDisplayLevel)
	{
		for (unsigned ii = 0; ii < pParent->GetChildCount(); ii++)
		{
			pChild = pParent->GetChild(ii);
			if (pChild->Filter(m_ClassFilter, NULL))
				hItem = InsertItem(hParentItem, pChild);
			else
				hItem = hParentItem;

			InsertSubTree(hItem, pChild, InitialDisplayLevel);
		}
	}

	if (pParent->GetLevel() < InitialDisplayLevel)
		Expand(hParentItem, TVE_EXPAND);
	return TRUE;
}


//*****************************************************************************
unsigned CCpNavTreeCtrl::GetItemLevel(
	HTREEITEM hItem)
//*****************************************************************************
{
	HTREEITEM hParentItem = GetParentItem(hItem);
	if (hParentItem == NULL)
		return 0;
	else
		return GetItemLevel(hParentItem) + 1;
}

//*****************************************************************************
HTREEITEM CCpNavTreeCtrl::InsertItem(
	HTREEITEM hParentItem,
	CCpObjectList* pObject)
//*****************************************************************************
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	if (IsWindow(m_hWnd))
	{
		unsigned ImageId;
		switch (pObject->GetTypeId())
		{
		case FCNAV_ITEMTYPE_PROJECT	:
			ImageId = FCNAV_IMAGE_PROJECT + 1;
			break;
		case FCNAV_ITEMTYPE_CONFIGURATION :
			ImageId = FCNAV_IMAGE_CONFIGURATION + 1;
			break;
		case FCNAV_ITEMTYPE_WEBRESSOURCE :
			ImageId = FCNAV_IMAGE_WEBRESSOURCE + 1;
			break;
		case FCNAV_ITEMTYPE_CONTROLRESSOURCE :
			ImageId = FCNAV_IMAGE_CONTROLRESSOURCE + 1;
			break;
		case FCNAV_ITEMTYPE_TASK :
			ImageId = FCNAV_IMAGE_TASK + 1;
			break;
		case FCNAV_ITEMTYPE_PROGRAM :
			ImageId = FCNAV_IMAGE_PROGRAM + 1;
			break;
		case FCNAV_ITEMTYPE_FUNCTIONBLOCK :
			ImageId = FCNAV_IMAGE_FUNCTIONBLOCK + 1;
			break;
		case FCNAV_ITEMTYPE_VARIABLE :
			ImageId = FCNAV_IMAGE_VARIABLE + 1;
			break;
		case FCNAV_ITEMTYPE_LIST :
			ImageId = FCNAV_IMAGE_LIST + 1;
			break;
		default :
			ImageId = FCNAV_IMAGE_INVALIDENTRY + 1;
			break;
		}
		
		CString EntryText = pObject->GetName();
		if (pObject->GetClassName().GetLength() > 0)
			EntryText += " (" + pObject->GetClassName() + ")";
		if (pObject->GetErrorText().GetLength() > 0)
			EntryText += "  " + pObject->GetErrorText();

		HTREEITEM hItem = 
			CTreeCtrl::InsertItem(EntryText, ImageId, ImageId, hParentItem);

		pObject->SetUserData((unsigned)hItem);
		SetItemData(hItem, (DWORD)pObject);		

		return hItem;
	}
	return NULL;
}

BEGIN_MESSAGE_MAP(CCpNavTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CCpNavTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemExpanded)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//*****************************************************************************	
#ifdef _DEBUG
void CCpNavTreeCtrl::AssertValid() const
//*****************************************************************************	
{
    // call inherited AssertValid first
    CTreeCtrl::AssertValid();

} 
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpNavTreeCtrl message handlers
//*****************************************************************************
void CCpNavTreeCtrl::OnPaint() 
//*****************************************************************************
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));

	CPaintDC dc(this); // device context for painting

	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)dc.m_hDC, 0 );

	HTREEITEM hItem = GetFirstVisibleItem();	
	int n = GetVisibleCount() + 1;
	UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
	while (hItem && n--)	
	{		
		CRect rect;
		BOOL bIsSelected = (GetItemState(hItem, selflag ) & selflag) != 0;
		
		if (!bIsSelected)
		{
			CCpObjectList* pObject = GetObject(hItem);
			ASSERT(pObject != NULL);
			if ((pObject->GetAspFile().GetLength() > 0) ||
				(!pObject->ChildrenLoaded() && pObject->GetChildrenFile().GetLength() > 0))
			{
				CFont *pFont = GetFont();
				CFont *pFontOld = dc.SelectObject(pFont);			

				dc.SetTextColor(RGB(0,0,255));			

				CString sItem = GetItemText( hItem);
				GetItemRect(hItem, &rect, TRUE);
				dc.SetBkColor(GetSysColor(COLOR_WINDOW));
				dc.TextOut(rect.left + 2, rect.top + 1, sItem);

				CPen Pen(PS_SOLID, 1, RGB(0,0,255));
				CPen *pOldPen = dc.SelectObject(&Pen);
				dc.MoveTo(rect.left+2, rect.bottom-3);
				dc.LineTo(rect.right-2, rect.bottom-3);
				dc.SelectObject(pOldPen);

				dc.SelectObject( pFontOld );
			}
		}
		hItem = GetNextVisibleItem(hItem);
	}
}

int CCpNavTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create image list for tree control
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	if (IsWindow(m_hWnd))
	{
		m_ImageList.Create(IDB_TREEICONS, 16, 0, RGB(255, 0, 255));
		ASSERT_VALID(&m_ImageList);
		ASSERT(m_ImageList.m_hImageList != NULL);
		SetImageList(&m_ImageList, TVSIL_NORMAL);
	}
	return 0;
}

void CCpNavTreeCtrl::OnDestroy() 
{
	CTreeCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	ASSERT_VALID(&m_ImageList);
	ASSERT(m_ImageList.m_hImageList != NULL);
	if (m_ImageList.m_hImageList != NULL)
		m_ImageList.DeleteImageList( );	
}

void CCpNavTreeCtrl::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));

	if (IsWindow(m_hWnd))
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		if (pNMTreeView->action & TVE_EXPAND)
		{
			HTREEITEM hItem = pNMTreeView->itemNew.hItem;
			HTREEITEM hChildItem = GetChildItem(hItem);
			while (hChildItem != NULL)
			{
				CCpObjectList* pChild = GetObject(hChildItem);
				if (pChild->GetChildCount() > 0 && !ItemHasChildren(hChildItem))
				{
					InsertSubTree(hChildItem, pChild, GetItemLevel(hItem)+1);
				}
				hChildItem = GetNextSiblingItem(hChildItem); 
			}
		}
		if (pNMTreeView->action & TVE_COLLAPSE)
		{
			HTREEITEM hItem = pNMTreeView->itemNew.hItem;
			HTREEITEM hChildItem = GetChildItem(hItem);
			while (hChildItem != NULL)
			{
				CCpObjectList* pChild = GetObject(hChildItem);
				Expand(hChildItem, TVE_COLLAPSE);
				hChildItem = GetNextSiblingItem(hChildItem); 
			}
		}
	}
	
	*pResult = 0;
}


void CCpNavTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_CONTROL)
		m_bCtrlKey = TRUE;		
	else
		m_bCtrlKey = FALSE;		
	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CCpNavTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));

	if (IsWindow(m_hWnd))
	{
		POINT pp;
		GetCursorPos(&pp);
		ScreenToClient(&pp); 
		CPoint p(pp);

		UINT flags;
		HTREEITEM hItem = HitTest(pp, &flags);

		if (hItem && (flags & TVHT_ONITEM))
		{
			if (m_bCtrlKey)
				GetParent()->SendMessage(WM_FCNAV_NEWURL, PARAM_NEW_WINDOW, (LPARAM)hItem);
			else
				GetParent()->SendMessage(WM_FCNAV_NEWURL, 0, (LPARAM)hItem);
			
			m_bCtrlKey = FALSE;			
		}
	}
	*pResult = 0;
}

void CCpNavTreeCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	HTREEITEM hItem = GetSelectedItem();
	if (hItem != NULL)
	{
		switch (nChar)
		{
		case '+' :
			Expand(hItem, TVE_EXPAND);
			break;

		case '-' :
			Expand(hItem, TVE_COLLAPSE );
			break;

		case VK_RETURN :
			if (m_bCtrlKey)
				GetParent()->SendMessage(WM_FCNAV_NEWURL, PARAM_NEW_WINDOW, (LPARAM)hItem);
			else
				GetParent()->SendMessage(WM_FCNAV_NEWURL, 0, (LPARAM)hItem);
			m_bCtrlKey = FALSE;			
			break;

		}
	}
	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}
