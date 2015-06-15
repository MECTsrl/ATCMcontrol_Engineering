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
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif //UNDER_CE

#ifndef _SECRES_H_
#include "toolkit\secres.h"
#endif

#ifndef __SLSTEDIT_H__
#include "toolkit\slstedit.h"
#endif

#ifndef __SLSTCTIP_H__
#include "toolkit\slstctip.h"
#endif

#ifndef _SECDC_H_
#include "Foundation\Graphics\MFC\secdc.h"
#endif

#ifndef __SECAUX_H__
#include "toolkit\secaux.h"
#endif

#ifndef __TREEUTIL_H__
#include "toolkit\treeutil.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Offset of image from header
#define HeaderImgOffset			10

///////////////////////////////////////////////////////////////////////
//
//  SECListCtrl Construction / Destruction
//


//@doc SEC_LISTCLASS
//@mfunc Construct an SEC_LISTCLASS object.
SEC_LISTCLASS::SEC_LISTCLASS()
{
	m_nAnchor = -1;
	m_nTopIndex = 0;
	m_nFocusIndex = -1;
	m_nDropHilite = -1;
	m_cyHeader = SECLISTCTRL_HDRHEIGHT_CLISTCTRL;
	m_nPendingSelect = -1;	// not used
	m_nPendingEdit = -1;	
	m_ptViewportOrg.x = 0;
	m_ptViewportOrg.y = 0;
	m_hcurSplit = NULL;
	m_bColSepHit = FALSE;
	m_dwListStyle = 0;
	m_dwListStyleEx = 0;
	m_nEditItem = -1;
	m_nActiveColumn = 0;
	m_pLabelFont = NULL;
	m_pHeaderFont = NULL;
	
	m_pImageList = NULL;
	m_pStateImageList = NULL;
	m_pSmallImageList = NULL;
	m_pHeaderImageList = NULL;
	m_pHdrDragImageList = NULL;

	m_nBtnHdr = -1;
	m_nColInsert = -1;	// used for header drag drop

	m_bTrackSystemColors = TRUE;
	OnSysColorChange();				// initialize the default colors
	m_bTrackSystemColors = FALSE;

	m_bAbortPendingSelect = FALSE;
	m_bDidProcessAndDestroyEditLabel=FALSE;
	m_uiCallbackMask = 0;
	m_uiCallbackMaskEx = 0;
	m_nSubItemCount = 0;
	m_pszInternalBuffer = new TCHAR[LVXC_INTERNALBUFFERSIZE];
	m_idEditLabelTimer=0;
	m_idAutoScrollTimer=0;
	m_idAutoKeySelectTimer = 0;
	m_dwScrollStyle = 0;

	m_pToolTipWnd = new SECListTip( this ); 
	m_pEditCtrl = new SECListEdit( this );

	m_pNotifyWnd = NULL;

	m_bRedrawAfterEveryInsert = FALSE;
	m_bDragging = FALSE;
	m_bHdrDragging = FALSE;
	m_bLookForBeginDrag = FALSE;
	m_iDeltaAccum = 0;

	m_uiAutoKeySelectionMode = 0;	// AutoKeySelection enabled by default.
	m_nAutoKeySelectionTimerInterval = 250;
}


SEC_LISTCLASS::~SEC_LISTCLASS()
{
	delete m_pToolTipWnd;
	delete m_pEditCtrl;
	delete[] m_pszInternalBuffer;
}

//////////////////////////////////////////////////////////////////////
//
//  SEC_LISTCLASS Creation / Initialization
//


//@doc SEC_LISTCLASS
//@mfunc Creates a child window and attaches it to the SEC_LISTCLASS object.
//@syntax Create(DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, 	UINT nID,CCreateContext* pContext)
//@syntax Create(DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//@syntax Create(LPCTSTR lpszClassName, LPCTSTR lpszWndName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT uID, CCreateContext* pContext)
//@rdesc Nonzero if successful; otherwise 0.	
//@parm DWORD | dwStyle | Specifies the window style attributes.
//@parm const RECT& | rect | The size and position of the window, in client coordinates of pParentWnd.
//@parm CWnd* | pParentWnd | The parent window.
//@parm UINT | nID | The ID of the child window.
//@parm CCreateContext* | pContext | The creation context. Default is NULL.
//@parm DWORD | dwExStyle | Specifies the window extended style attributes.
//@parm LPCTSTR | lpszClassName | Points to a null-terminated character string that names the Windows class (a WNDCLASS structure). The class name can be any name registered with the global AfxRegisterWndClass function or any of the predefined control-class names. If NULL, uses the default CWnd attributes.
//@parm LPCTSTR | lpszWndName | Points to a null-terminated character string that contains the window name.
BOOL SEC_LISTCLASS::Create(DWORD dwStyle,const RECT& rect, CWnd* pParentWnd, 	UINT nID,CCreateContext* pContext /* =NULL */	)
{
	return Create(dwStyle, 0, rect, pParentWnd, nID, pContext);
}


BOOL SEC_LISTCLASS::Create( DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/)
{
	//Assume Failure
	BOOL bRtn = FALSE;

	bRtn = CreateEx( dwExStyle, NULL, NULL, dwStyle, 
		             rect.left, rect.top, 
				     rect.right - rect.left, 
					 rect.bottom - rect.top, 
					 pParentWnd->GetSafeHwnd(), (HMENU)nID, (LPVOID)pContext );

	return bRtn;
}


BOOL SEC_LISTCLASS::Create(	 LPCTSTR lpszClassName,
										 LPCTSTR lpszWndName, 
										 DWORD dwStyle,
										 const RECT& rect,
										 CWnd* pParentWnd,
										 UINT uID,
										 CCreateContext* pContext /* = NULL */	)
{
	lpszClassName;
	lpszWndName;
	
	return Create( dwStyle, 0, rect, pParentWnd, uID, pContext );
}

extern void AFXAPI AfxHookWindowCreate(CWnd* pWnd);
extern BOOL AFXAPI AfxUnhookWindowCreate();


BOOL SEC_LISTCLASS::CreateEx(DWORD dwExStyle,
		LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		int x,
		int y,
		int cx,
		int cy,
		HWND hWndParent,
		HMENU nIDorHMenu,
		LPVOID lpParam // = NULL
	)
{
	CREATESTRUCT cs;
	cs.dwExStyle		= dwExStyle;
	cs.lpszClass		= lpszClassName;
	cs.lpszName			= lpszWindowName;
	cs.style			= dwStyle;
	cs.x				= x;
	cs.y				= y;
	cs.cx				= cx;
	cs.cy				= cy;
	cs.hwndParent		= hWndParent;
	cs.hMenu			= nIDorHMenu;
	cs.hInstance		= AfxGetInstanceHandle();
	cs.lpCreateParams	= lpParam;
#ifdef UNDER_CE
	if (!PreCreateWindow(cs) || !wce_PreCreateWindow(cs))
#else
	if (!PreCreateWindow(cs))
#endif //UNDER_CE
	{
		PostNcDestroy();
		return FALSE;
	}
	else
	{
		ParseCreateStruct(cs);
	}

	AfxHookWindowCreate(this);
	HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
			cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
			cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

#ifdef _DEBUG
	if (hWnd == NULL)
	{
		TRACE1("Warning: Window creation failed: GetLastError returns 0x%8.8X\n",
			GetLastError());
	}
#endif

	if (!AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd); // should have been set in send msg hook
	return TRUE;
}


int SEC_LISTCLASS::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   BOOL bOK = (SEC_LISTBASE::OnCreate(lpCreateStruct) == 0);

   if (bOK)
   {
	   m_hcurSplit = AfxGetApp()->LoadCursor(IDC_TOOLBAR_HORZDRAG);

	   // If the following ASSERT fires, you probably didn't include
	   // the Objective Toolkit resources (secres.rc and secres.h). Please
	   // consult the User's Guide for the proper method of including
	   // these 2 files into your application.
	    ASSERT(m_hcurSplit != NULL);

	   //This might seem odd to insert a column, when we aren't in report mode, but we
	   // are going to use the width of the column to represent the width of the item.
	   // By default this width is just 20 pixels, but by the time it is finished measuring
	   // all the items it will set the width of the column to be as wide as the widest string
	   if( (lpCreateStruct->style & LVS_TYPEMASK) != LVS_REPORT &&
		   (GetListCtrlStyle() & LVS_TYPEMASK) != LVS_REPORT )
	   {
		   ModifyListCtrlStyle( 0, LVS_NOCOLUMNHEADER, FALSE );
		   InsertColumn( 0, _T(""), LVCFMT_LEFT, 20, 0, FALSE );
	   }
   }

   return bOK ? 0 : -1;
}



void SEC_LISTCLASS::ParseCreateStruct(CREATESTRUCT& cs 	)
{
	DWORD dwViewStyle = cs.style & LVS_TYPEMASK;
	cs.style &= ~LVS_TYPEMASK;

	DWORD dwListStyle = cs.style & LVS_STYLES;
	cs.style &= ~LVS_STYLES;

	DWORD dwListStyleEx = cs.dwExStyle & LVXS_STYLES;
	cs.dwExStyle &= ~LVXS_STYLES;

	ModifyListCtrlStyle( 0, dwListStyle|dwViewStyle, FALSE );
	ModifyListCtrlStyleEx( 0, dwListStyleEx, FALSE );
}


BOOL SEC_LISTCLASS::AttachListCtrl( UINT uID,				//ID of the List Control to Attach To
		CWnd* pParentWnd		//Parent of the list control which is being attached to
	)
{
	CWnd* pWnd = pParentWnd->GetDlgItem( uID );
		
	DWORD dwStyle = pWnd->GetStyle();
	dwStyle |= WS_BORDER;

	DWORD dwExStyle = pWnd->GetExStyle();

	CRect rect;
	pWnd->GetWindowRect(&rect);
	pParentWnd->ScreenToClient(&rect);
	pWnd->DestroyWindow();

	return Create( dwStyle, dwExStyle, rect, pParentWnd, uID );
}


void SEC_LISTCLASS::OnDestroy(void) 
{
	StopAutoScroll();

	if( (GetListCtrlStyle() & LVS_SHAREIMAGELISTS) == 0 )
	{
		if(m_pImageList)		{ delete (m_pImageList);       }
		if(m_pSmallImageList) 	{ delete (m_pSmallImageList);  }
		if(m_pStateImageList)	{ delete (m_pStateImageList);  }
		if(m_pHeaderImageList)  { delete (m_pHeaderImageList); }
	}
	
	if( m_pToolTipWnd && m_pToolTipWnd->GetSafeHwnd() )
		m_pToolTipWnd->DestroyWindow();

	DeleteAllItems();
	DeleteAllColumns();
	SEC_LISTBASE::OnDestroy();
}


BOOL SEC_LISTCLASS::AllocateSubItems(Item* pItem 
	) const
{
	ASSERT_POINTER(pItem, Item);

    int c = GetSubItemCount();
    for( int w=0; w<c ; w++ )
    {
        pItem->AddSubItem();
    }

    return TRUE;
}


BOOL SEC_LISTCLASS::DeallocateSubItems
	( 
		Item* pItem 
	) const
{
	ASSERT_POINTER(pItem, Item);

    while( pItem->GetSubItemCount() )
	{
        VERIFY( pItem->DeleteSubItem(0) );
    }

    return TRUE;
}


int SEC_LISTCLASS::InitNewItem
	(
		Item* pItem
	) const
{
	if (AllocateSubItems( pItem ) == FALSE)
	{
		ASSERT(FALSE);
		return -1;
	}
	else
		return 0;
}


SEC_LISTCLASS::Item*
SEC_LISTCLASS::CreateNewItem()
{
	Item* pItem = 0;
	pItem = new Item;

	ASSERT_POINTER(pItem, Item);

	VERIFY(InitNewItem(pItem) != -1);

	return pItem;
}

///////////////////////////////////////////////////////////////////////
//
//  SEC_LISTCLASS Color/Font Pick Members
//
//		These members are virtual so that you can override them
//		to pick fonts, colors etc. programmatically using any
//		app-specific criteria required.
//


void SEC_LISTCLASS::PickTextFont(LvPaintContext* pPC)
{
	pPC->pFont = GetLabelFont();
}


void SEC_LISTCLASS::PickTextColors(LvPaintContext* pPC)
{
	// responsible for setting pPC->rgbText and pPC->rgbTextBkgnd.

	CWnd* pFocusWnd = GetFocus();
	if ( !IsWindowEnabled() )
	{
		// window is disabled
		pPC->rgbText = m_rgbDisabledText;
		pPC->rgbTextBkgnd = rgbNoFocusBk;
		return;
	}
	
	// window is enabled
    if( !((GetListCtrlStyle()&LVS_SHOWSELALWAYS) == 0 && pFocusWnd != this) // have showselalways OR we have the focus
		&& (pPC->lvi.state & LVIS_SELECTED)
		&& (pPC->lvi.iSubItem == 0 || GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS))
    {
		// item is selected
		if ( (pFocusWnd == this) || (m_nEditItem != -1) )
		{
			// control has focus, or we are editing a label
			if( pPC->lvix.stateEx & LVIS_EX_DISABLED )
				// item is disabled and selected
				pPC->rgbText = m_rgbDisabledText;
			else
				// normal and selected
				pPC->rgbText = m_rgbSelText;

    		pPC->rgbTextBkgnd = m_rgbSelTextBk;
		}
		else	// item selected, but the control does not have focus
		{
			pPC->rgbText = rgbNoFocusText;
    		pPC->rgbTextBkgnd = rgbNoFocusBk;
		}
    }
    else	// not selected. normal.
    {
    	if( pPC->lvix.stateEx & LVIS_EX_DISABLED )
			pPC->rgbText = m_rgbDisabledText;
		else
	    	pPC->rgbText = m_rgbText;

		pPC->rgbTextBkgnd = m_rgbTextBk;
    }

	if( (pPC->lvi.iItem == GetDropHilightIndex()) && 
		( (pPC->lvi.iSubItem == 0 ) || (GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) ) )   
   {
		pPC->rgbText = m_rgbSelText;
   		pPC->rgbTextBkgnd = m_rgbSelTextBk;
   }
}


void SEC_LISTCLASS::PickItemBkgnd(LvPaintContext* pPC)
{
	pPC->rgbItemBkgnd = GetBkColor();
}


void SEC_LISTCLASS::PickIconBkgnd(LvPaintContext* pPC)
{
	if ( !IsWindowEnabled() )
	{
		pPC->rgbIconBkgnd = GetBkColor();
		return;
	}

	CWnd* pFocusWnd = GetFocus();

	// selected and we have the focus
    if( (pPC->lvi.state & LVIS_SELECTED) && (pFocusWnd == this))
			pPC->rgbIconBkgnd = m_rgbSelIconBk;
    else
		pPC->rgbIconBkgnd = m_rgbIconBk;
}

///////////////////////////////////////////////////////////////////////
//
//  SEC_LISTCLASS Paint Context Members
//


SEC_LISTCLASS::LvPaintContext* 
SEC_LISTCLASS::CreatePaintContext(CDC* pDC)
{
	return new LvPaintContext(pDC);
}


BOOL SEC_LISTCLASS::LoadPaintContext(LvPaintContext* pPC)
{
	pPC->lvi.mask = LVIF_TEXT|LVIF_STATE;
	pPC->lvi.stateMask = LVIS_SELECTED|LVIS_FOCUSED;
	pPC->lvi.pszText = m_pszInternalBuffer;
	pPC->lvi.cchTextMax = LVXC_INTERNALBUFFERSIZE;

	pPC->dwViewStyle = (GetListCtrlStyle() & LVS_TYPEMASK);

	if (GetItem(&(pPC->lvi), FALSE, TRUE))
	{
		// If the LVN_GETDISPINFO handler sets the LVIF_DI_SETITEM flag,
		// this tells us not to ask for the information again.  So, we
		// remove the LPSTR_TEXTCALLBACK or I_IMAGECALLBACK setting.
		if (pPC->lvi.mask & LVIF_DI_SETITEM)
			VERIFY(SetItem( &(pPC->lvi), FALSE ));

		PickItemBkgnd(pPC);
		PickIconBkgnd(pPC);
		PickTextColors(pPC);
		PickTextFont(pPC);
		return TRUE;
	}
	else
		return FALSE;
}

///////////////////////////////////////////////////////////////////////
//
//  SEC_LISTCLASS Owner Draw Members
//


void SEC_LISTCLASS::DrawSubItem(LvPaintContext* pPC)
{
	// Fill with background color
	CBrush brush(pPC->rgbItemBkgnd);
	pPC->pdc->FillRect(pPC->rect, &brush);

	if( pPC->lvi.pszText && _tcslen(pPC->lvi.pszText) >= 0 )
	{	
		if( pPC->pCol->iSubItem == 0 )
			DrawSubItemIcon( pPC );

		DrawSubItemText( pPC );
	}
}


void SEC_LISTCLASS::DrawSubItemIcon( LvPaintContext* pPC )
{
	CImageList* pIml = NULL;

	switch( GetListCtrlStyle() & LVS_TYPEMASK )
	{
	case LVS_ICON:
		pIml = GetImageList( LVSIL_NORMAL );
		break;

	case LVS_REPORT:
	case LVS_SMALLICON:
	case LVS_LIST:
		pIml = GetImageList( LVSIL_SMALL );
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	if( !pIml )
		return;

	ASSERT_VALID(pIml);

	CPoint ptDraw;
	
	switch( GetListCtrlStyle() & LVS_TYPEMASK )
	{
	case LVS_REPORT:
		ptDraw = CPoint( pPC->rect.left + pPC->pItem->rcIcon.left,
						 pPC->rect.top  + pPC->pItem->rcIcon.top );
		break;

	case LVS_ICON:
		{
			CRect* prcText = &(pPC->pItem->rcText);
			int x = (prcText->Width() - 16)/2;
			ptDraw = CPoint( x,
							 pPC->rect.top + pPC->pItem->rcIcon.top );
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	
	pIml->Draw( pPC->pdc, pPC->lvi.iImage, ptDraw, ILD_NORMAL );
}


void SEC_LISTCLASS::DrawSubItemText( LvPaintContext* pPC )
{
	CFont *pOldFont;
	COLORREF oldFore, oldBack;
	int oldBkMode;
	UINT nFlags;
	CRect rectText, rectTextSel, rectRowSel;

	rectTextSel = pPC->rect;
	rectTextSel.right = rectTextSel.left + pPC->pItem->rcText.Width();
	
	rectText = pPC->rect;
	rectText.left += SEC_LEFT_OFFSET;
	rectText.right -= SEC_RIGHT_OFFSET;

	if( pPC->pItem->rcIcon.IsRectEmpty() == FALSE && 
		pPC->lvi.iSubItem == 0 )
	{
		CRect* prcIcon = &(pPC->pItem->rcIcon);
		CRect* prcText = &(pPC->pItem->rcText);

		switch( GetListCtrlStyle() & LVS_TYPEMASK )
		{
		case LVS_REPORT:
		case LVS_SMALLICON:
		case LVS_LIST:
			{
				int width = prcText->Width();
				rectText.left  = prcIcon->right + 4;
				rectText.top  += 3;
				rectText.right = rectText.left + width;

				rectTextSel.top    = pPC->rect.top + prcIcon->top;
				rectTextSel.left   = rectText.left - 2;
				rectTextSel.right  = rectText.right + 2;
				rectTextSel.bottom = pPC->rect.bottom + prcIcon->bottom;
			}
			break;

		case LVS_ICON:
			{
				rectRowSel.SetRectEmpty();

				rectText.top    = pPC->rect.top  + prcText->top;
				rectText.left   = pPC->rect.left + prcText->left;
				rectText.right  = rectText.left  + prcText->Width();
				rectText.bottom = rectText.top   + prcText->Height();

				rectTextSel = rectText;
				rectTextSel.InflateRect( 2, 0, 2, 0 );
			}
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}

	if( (GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS))
	{
		rectRowSel = rectTextSel;
		rectRowSel.right = pPC->rect.right;
	}

	nFlags = DT_WORDBREAK | DT_VCENTER|DT_NOPREFIX|DT_EXPANDTABS;

	// Select DC attributes
	pOldFont = pPC->pdc->SelectObject(pPC->pFont);
	oldFore = pPC->pdc->SetTextColor(pPC->rgbText);
	oldBack = pPC->pdc->SetBkColor(pPC->rgbIconBkgnd);
	oldBkMode = pPC->pdc->SetBkMode(TRANSPARENT);

	if( (GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) && 
		((pPC->lvi.state & LVIS_SELECTED) || (GetDropHilightIndex() == pPC->lvi.iItem)) )
	{
		CBrush selbrush(pPC->rgbTextBkgnd);
		pPC->pdc->FillRect(rectRowSel, &selbrush);
	}
	else if( (pPC->lvi.iSubItem == 0))
	{
		CBrush selbrush(pPC->rgbTextBkgnd);
		pPC->pdc->FillRect(rectTextSel, &selbrush);
	}
	

	// Map the format constants
	switch (pPC->pCol->fmt & ~LVCFMT_BITMAP_ON_RIGHT)
	{
	case LVCFMT_LEFT:
		nFlags |= DT_LEFT;
		break;
	case LVCFMT_RIGHT:
		nFlags |= DT_RIGHT;
		break;
	case LVCFMT_CENTER:
		nFlags |= DT_CENTER;
		break;
	default:
		// Invalid Format
		ASSERT(FALSE);
	}

	int pos = -1;
#ifndef UNDER_CE
	if (GetListCtrlStyle() & LVS_NOLABELWRAP)
		nFlags |= DT_SINGLELINE|DT_END_ELLIPSIS;
#else
	if (GetListCtrlStyle() & LVS_NOLABELWRAP)
		nFlags |= DT_SINGLELINE;
#endif //UNDER_CE

	if ( !( pPC->lvi.iItem == m_nEditItem && pPC->iCol == m_nActiveColumn ) )
		pPC->pdc->DrawText( pPC->lvi.pszText, pos, &rectText, nFlags );

	// Restore DC
	pPC->pdc->SelectObject(pOldFont);
	pPC->pdc->SetTextColor(oldFore);
	pPC->pdc->SetBkColor(oldBack);
	pPC->pdc->SetBkMode(oldBkMode);
}


void SEC_LISTCLASS::PickItemLinePen(CPen*& pPen)
{
	static CPen penDivider;
	static BOOL bCreated = FALSE;

	if (!bCreated)
	{
		penDivider.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
		bCreated = TRUE;
	}

	pPen = &penDivider;
}


void SEC_LISTCLASS::PickColumnLinePen(CPen*& pPen)
{
	static CPen penDivider;
	static BOOL bCreated = FALSE;

	if (!bCreated)
	{
		penDivider.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
		bCreated = TRUE;
	}

	pPen = &penDivider;
}


void SEC_LISTCLASS::DrawColumnLines(CDC* pDC, int nItem, const CRect& rcRow)
{
	CPen* pPen;
	CPen* pOldPen;
	CRect rcCell;
	Column* pCol;
	int nColCount;
	int iCol;

	PickColumnLinePen(pPen);
	pOldPen = pDC->SelectObject(pPen);

	// Initialize cell bounding rectangle
	rcCell.top = rcRow.top;
	rcCell.bottom = rcRow.bottom;
	rcCell.left = -m_ptViewportOrg.x;
	rcCell.right = rcCell.left;

	// Draw column dividers
	nColCount = GetColumnCount();
	nColCount -= (GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE) ? 1 : 0;
	for(iCol=0; iCol<nColCount; iCol++)
	{
		pCol = GetColumnAt(iCol);
		if ( pCol )
		{
			rcCell.right = rcCell.left + pCol->cx;
			rcCell.left = rcCell.right;
			pDC->MoveTo(rcCell.right, rcCell.top);
			pDC->LineTo(rcCell.right, rcCell.bottom);
		}
	}

	PickItemLinePen(pPen);
	pDC->SelectObject(pPen);

	if (nItem == (GetItemCount()-1))
	{
		pDC->MoveTo(rcRow.left, rcRow.bottom-1);
		pDC->LineTo(rcRow.right, rcRow.bottom-1);
	}

	// Draw column dividers
 	rcCell.left = -m_ptViewportOrg.x;
	rcCell.right = rcCell.left;
	nColCount = GetColumnCount();
	nColCount -= (GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE) ? 1 : 0;
	for(iCol=0; iCol<nColCount; iCol++)
	{
		pCol = GetColumnAt(iCol);
		rcCell.right = rcCell.left + pCol->cx;
		rcCell.left = rcCell.right;
		if (pCol->iSubItem == 0 || iCol == (GetColumnCount()-1))
		{
			pDC->MoveTo(rcCell.right, rcCell.top);
			pDC->LineTo(rcCell.right, rcCell.bottom);
		}
	}

	pDC->SelectObject(pOldPen);
}


void SEC_LISTCLASS::DrawItemLines(CDC* pDC, int nItem, const CRect& rcRow)
{
	CPen* pPen;
	CPen* pOldPen;

	nItem; // UNUSED

	PickItemLinePen(pPen);
	pOldPen = pDC->SelectObject(pPen);
	pDC->MoveTo(rcRow.left, rcRow.bottom-1);
	pDC->LineTo(rcRow.right, rcRow.bottom-1);
	pDC->SelectObject(pOldPen);

	pDC->SelectObject(pOldPen);
}


void SEC_LISTCLASS::DrawItem(LvPaintContext* pPC)
{
	CRect rcCell;
	int cColumns;
	CRect rcRow;

	rcRow = pPC->rect;

	// Initialize cell bounding rectangle
	rcCell.top = rcRow.top;
	rcCell.bottom = rcRow.bottom;
	rcCell.left = rcRow.left;
	rcCell.right = rcCell.left;

	// If the text of the item is dynamic, we need to remeasure the
	// item because the text may be larger now than it was the last
	// time a redraw was done.
	if (IsCallbackItem(pPC->lvi.iItem))
		MeasureItem(pPC);

	cColumns = GetColumnCount();
	for(pPC->iCol=0; pPC->iCol<cColumns; pPC->iCol++)
    {
		pPC->pCol = GetColumnAt(pPC->iCol);

		
		if (cColumns > 1)
			rcCell.right = rcCell.left + pPC->pCol->cx;
		else	// use the entire logical width if we need to.
			rcCell.right = rcCell.left + otmax(pPC->pCol->cx, pPC->pItem->rcText.right);

		if (pPC->pdc->RectVisible(rcCell))
		{
			pPC->lvi.iSubItem = pPC->pCol->iSubItem;
			if (pPC->pCol->iSubItem != 0)
				pPC->pSubItem = pPC->pItem->GetSubItem(pPC->pCol->iSubItem);
			pPC->rect = rcCell;
			VERIFY(LoadPaintContext(pPC));
			DrawSubItem(pPC);
		}
		rcCell.left = rcCell.right;
	}

	if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		if( GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS 
			&& pPC->lvi.iItem == GetFocusIndex() 
			&& GetFocus() == this
			&& GetColumnCount() > 1)
		{
			CRect temp = rcRow;
			temp.left = pPC->pItem->rcText.left - (WIDTH_GAP/2) - m_ptViewportOrg.x;
			//Draw a selected rectangle
			pPC->pdc->DrawFocusRect( &temp );
		}

		if (GetListCtrlStyleEx() & LVXS_LINESBETWEENCOLUMNS)
			DrawColumnLines(pPC->pdc, pPC->lvi.iItem, rcRow);

		if (GetListCtrlStyleEx() & LVXS_LINESBETWEENITEMS)
			DrawItemLines(pPC->pdc, pPC->lvi.iItem, rcRow);
	}
}


void SEC_LISTCLASS::DrawInvalidItems(LvPaintContext* pPC)
{
	CRect rcInside;
	CRect rcRow;
	CSize szLog;
	int cItemsThisPage = 0;
	int cItemCount;

	if ( pPC->pdc->IsPrinting() )
	{
		cItemCount = GetLastFullyVisibleItem(pPC);

		if ( cItemCount < GetItemCount() )
			cItemCount++;
	}
	else
		cItemCount = GetItemCount();

	// Initialize the row bounding rectangle
	GetInsideRect(rcInside);

	if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		szLog = GetLogicalSize( pPC );
		rcRow.top = rcInside.top;
		rcRow.left = -m_ptViewportOrg.x;
		rcRow.right = rcRow.left + szLog.cx;
		rcRow.bottom = rcRow.top;

		// Iterate through all rows and draw on the visible and
		// invalid ones
		// Sometimes GetTopIndex return -1 and so included the check (QA3100)
		for (pPC->lvi.iItem = GetTopIndex();
			 pPC->lvi.iItem < cItemCount && pPC->lvi.iItem != -1;
			 pPC->lvi.iItem++, cItemsThisPage++)
		{
			pPC->pItem = GetItemAt(pPC->lvi.iItem);
			if (pPC->pItem->m_bDirty)
				MeasureItem(pPC);
			
			rcRow.top += GetMargin();
			rcRow.bottom += pPC->pItem->m_cy + GetMargin();

			if( pPC->pdc->RectVisible(rcRow) || pPC->pdc->IsPrinting())
			{
				pPC->rect = rcRow;
				DrawItem(pPC);
			}

			// If the LVN_GETDISPINFO handler sets the LVIF_DI_SETITEM flag,
			// this tells us not to ask for the information again.  So, we
			// remove the LPSTR_TEXTCALLBACK or I_IMAGECALLBACK setting.
			if (pPC->lvi.mask & LVIF_DI_SETITEM)
				VERIFY(SetItem(&(pPC->lvi), FALSE));

			rcRow.top = rcRow.bottom;


			// Conditionally exit loop here so cItemsThisPage isn't
			// incremented on last item
			if (rcRow.top > rcInside.bottom)
				break;
		}
	}
	else if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_ICON )
	{
		rcRow.top = rcInside.top;
		rcRow.left = -m_ptViewportOrg.x;
		rcRow.right = rcRow.left + GetColumnAt(0)->cx;
		rcRow.bottom = rcRow.top;

		// Sometimes GetTopIndex return -1 and so included the check (QA3100)
		for (pPC->lvi.iItem = GetTopIndex();
			 pPC->lvi.iItem < cItemCount && pPC->lvi.iItem != -1;
			 pPC->lvi.iItem++, cItemsThisPage++)
		{
			pPC->pItem = GetItemAt(pPC->lvi.iItem);
			
			if (pPC->pItem->m_bDirty)
				MeasureItem(pPC);

			rcRow.top += GetMargin();
			rcRow.bottom += (pPC->pItem->m_cy) + GetMargin();

			if (pPC->pdc->RectVisible(rcRow))
			{
				pPC->rect = rcRow;
				DrawItem(pPC);
			}

			// If the LVN_GETDISPINFO handler sets the LVIF_DI_SETITEM flag,
			// this tells us not to ask for the information again.  So, we
			// remove the LPSTR_TEXTCALLBACK or I_IMAGECALLBACK setting.
			if (pPC->lvi.mask & LVIF_DI_SETITEM)
				VERIFY(SetItem(&(pPC->lvi)));

			rcRow.left = rcRow.right;
			rcRow.right = rcRow.left + GetColumnAt(0)->cx;

			if( rcRow.right > rcInside.right )
			{
				rcRow.top = rcRow.bottom;
				rcRow.left = rcInside.left;
				rcRow.right = rcRow.left + GetColumnAt(0)->cx;
			}

			// Conditionally exit loop here so cItemsThisPage isn't
			// incremented on last item
			if (rcRow.top > rcInside.bottom)
				break;
		}
	}
	else
	{
		//ASSERT->All other viewing styles are not supported at this time...
		ASSERT(FALSE);
	}


	if ( pPC && !(pPC->pdc->IsPrinting()) )
	{
		// Update scroll bar thumb size and position
		SCROLLINFO sbi;
		sbi.fMask = SIF_POS | SIF_PAGE;
		sbi.nPos = GetTopIndex();
		sbi.nPage = cItemsThisPage;
		SetScrollInfo(SB_VERT, &sbi, TRUE);
	}
}

void SEC_LISTCLASS::DrawHeader(LvPaintContext* pPC)
{
	Column* pCol;
	CRect rcHeader, rcClient;
	CSize szLog;
	int i;
	UINT fAdditionalFlags(0);

	// If there's no column header, this function shouldn't be called.
	ASSERT(IsHeaderCtrlEnabled() == TRUE);

	SECGetClientRect(rcClient);
	szLog = GetLogicalSize(pPC);

	// Compute Header Rect
	rcHeader.top = rcClient.top;
	rcHeader.bottom = (rcHeader.top + m_cyHeader);
	rcHeader.left = -m_ptViewportOrg.x;
	rcHeader.right = otmax(szLog.cx, rcClient.Width()+1);

	if (!pPC->pdc->RectVisible(rcHeader))
		return;  // Early return if header isn't in need of repaint

	CFont* pOldFont;
	pOldFont = pPC->pdc->SelectObject(GetHeaderFont());

	static CBrush br(rgbBtnFace);
	CBrush* pOldBrush = pPC->pdc->SelectObject(&br);
	int oldMode = pPC->pdc->SetBkMode(TRANSPARENT);
	
	// Draw the Header Background
	pPC->pdc->FillRect(rcHeader, &br);

	// Draw the columns
	if (GetColumnCount() >= 1)
	{
		int nColCount;
		nColCount = GetColumnCount();
		for (i=0; i<nColCount; i++)
		{
			pCol = GetColumnAt(i);
			rcHeader.right = rcHeader.left + GetColumnWidth(i);

			// Drawing a frame control with zero width sometimes corrupts image, guard
			if(rcHeader.right>rcHeader.left)
				pPC->pdc->DrawFrameControl(&rcHeader, DFC_BUTTON, DFCS_BUTTONPUSH );				
			rcHeader.left = rcHeader.right;
		}
	
		//fill in whats left over...
		//   make sure it always hangs over...just like the Win32 Header
		if( rcHeader.right < rcClient.right )
			rcHeader.right = rcClient.right + 2;
		else
			rcHeader.right += 2;

		pPC->pdc->DrawFrameControl(&rcHeader, DFC_BUTTON, DFCS_BUTTONPUSH );
	}

	rcHeader.left = -m_ptViewportOrg.x;

	//use another rectangle because we might have to offset text on the 
	//		left, right or both, and we don't want to corrupt rcHeader...
	CRect rectText(0,0,0,0);

	// Draw the column header text
	for (i=0; i<GetColumnCount(); i++)
	{
		pCol = GetColumnAt(i);
		rcHeader.right = rcHeader.left + GetColumnWidth(i) - 1;

		rectText = rcHeader;
		rectText.left  += SEC_LEFT_OFFSET;
		rectText.right -= SEC_RIGHT_OFFSET;

		ColumnFormatHelper(pCol->fmt, fAdditionalFlags);

		CSize size = pPC->pdc->GetTextExtent( pCol->strText );
	
		//Draw the header icon if the header width permits. Or else, just draw the text.
		if( (m_pHeaderImageList) && (pCol->iImage >= 0) && (rcHeader.Width()>22))
		{
			ASSERT_VALID(m_pHeaderImageList);
			DrawHeaderImage(pPC->pdc, pCol, rectText, fAdditionalFlags);
		}
#ifndef UNDER_CE
		if( size.cx > rectText.Width() )
			fAdditionalFlags |= DT_END_ELLIPSIS;
#endif //UNDER_CE
		pPC->pdc->DrawText( pCol->strText, 
					   -1,
					   rectText,
					   DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_EXPANDTABS|fAdditionalFlags);

		rcHeader.left = rcHeader.right + 1;
	}

	pPC->pdc->SetBkMode(oldMode);
	pPC->pdc->SelectObject(pOldBrush);
	pPC->pdc->SelectObject(pOldFont);
}


void SEC_LISTCLASS::DrawBtnHeader(int nColHit, BOOL bPush)
{
	// If there's no column header & header sort is not enabled, then this function should not be called.
	ASSERT(!(GetListCtrlStyle() & LVS_NOSORTHEADER));

	Column* pCol;
	CRect rcHeader;	
	
	// Compute Header Rect
	rcHeader.top = 0;
	rcHeader.bottom = m_cyHeader;
	rcHeader.left = GetColumnLeft(nColHit);
	rcHeader.right = GetColumnRight(nColHit);

	CClientDC dc(this);

	//If the vertical scrollbar is present, then determine the intersection region and exclude this rect from the 
	//dc's clipping region
	CScrollBar* pVertSB = GetScrollBarCtrl(SB_VERT);
	if(pVertSB)
	{
		CRect rcScroll;
		pVertSB->GetWindowRect(&rcScroll);
		ScreenToClient(&rcScroll);
		CRect rcIntSect = rcHeader & rcScroll;
		if(!rcIntSect.IsRectEmpty())
			dc.ExcludeClipRect(&rcScroll);
	}	

	CFont* pOldFont;
	pOldFont = dc.SelectObject(GetHeaderFont());

	CBrush br(::GetSysColor(COLOR_BTNFACE));
	CBrush* pOldBrush = dc.SelectObject(&br);
	int oldMode = dc.SetBkMode(TRANSPARENT);
	
	// Draw the Header Background
	dc.FillRect(rcHeader, &br);

	// Draw the column in the pushed state
	pCol = GetColumnAt(nColHit);
	rcHeader.right = rcHeader.left + GetColumnWidth(nColHit);
	if(bPush)
#ifndef UNDER_CE
		dc.DrawFrameControl(&rcHeader, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED|DFCS_FLAT  );
#else
		dc.DrawFrameControl(&rcHeader, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED );
#endif
	else
		dc.DrawFrameControl(&rcHeader, DFC_BUTTON, DFCS_BUTTONPUSH );

	UINT fAdditionalFlags(0);
	
	CRect rectText(0,0,0,0);

	// Draw the column header text
	rcHeader.right -= 1;
	rectText = rcHeader;
	rectText.left  += SEC_LEFT_OFFSET;
	rectText.right -= SEC_RIGHT_OFFSET;
	
	ColumnFormatHelper(pCol->fmt, fAdditionalFlags);

	CSize size = dc.GetTextExtent( pCol->strText );

	if(bPush)
		rectText.OffsetRect(1,1);

	//Draw the header icon if the header width permits. Or else, just draw the text.
	if( (m_pHeaderImageList) && (pCol->iImage >= 0) && (rcHeader.Width()>22))
	{
		ASSERT_VALID(m_pHeaderImageList);
		DrawHeaderImage(&dc, pCol, rectText, fAdditionalFlags);
	}
#ifndef UNDER_CE
	if( size.cx > rectText.Width() )
		fAdditionalFlags |= DT_END_ELLIPSIS;
#endif //UNDER_CE
	dc.DrawText( pCol->strText, 
				   -1,
				   rectText,
				   DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_EXPANDTABS|fAdditionalFlags);

	dc.SetBkMode(oldMode);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldFont);
}

void SEC_LISTCLASS::DrawHeaderImage(CDC* pDC, Column* pCol, CRect& rectText, UINT& nFlags)
{
	CSize size = pDC->GetTextExtent( pCol->strText );

	HICON hHdrIcon = m_pHeaderImageList->ExtractIcon(pCol->iImage);
	if(hHdrIcon)
	{
		//Determine the x position for the icon relative to the text
		int nOffset = 0;
		if(pCol->fmt & LVCFMT_CENTER)
			nOffset = rectText.left + (rectText.Width()-(size.cx+16+HeaderImgOffset))/2;
		else if(pCol->fmt & LVCFMT_RIGHT)
			nOffset = rectText.right - size.cx - 16 - HeaderImgOffset;
		else	//LVCFMT_LEFT
			nOffset = rectText.left;

		if( (pCol->fmt & LVCFMT_BITMAP_ON_RIGHT) )
		{
			nOffset += (size.cx + HeaderImgOffset);					
			if(nOffset > rectText.right-16)
				nOffset = rectText.right-16;
		}

		if(nOffset < rectText.left)
			nOffset = rectText.left;
	
		::DrawIconEx(pDC->m_hDC,nOffset,rectText.top+1,hHdrIcon,16,16,
			NULL,NULL,DI_NORMAL);

		if( !(pCol->fmt & LVCFMT_BITMAP_ON_RIGHT) )
		{
			rectText.left = nOffset+16+HeaderImgOffset;
			nFlags = DT_LEFT;
		}
		else 
		{
			rectText.right = nOffset-HeaderImgOffset; 
			nFlags = DT_RIGHT;
		}
	}
}


CImageList* SEC_LISTCLASS::CreateHeaderDragImage( const int nColumn ) 
{
	// If there's no column header & header sort is not enabled, then this function should not be called.
	ASSERT( (GetListCtrlStyleEx() & LVS_EX_HEADERDRAGDROP) );

	//MemDC header rect
	CRect rcMem(0,0,0,0);
	rcMem.right = GetColumnRight(nColumn) - GetColumnLeft(nColumn);
	rcMem.bottom = m_cyHeader+1;
	rcMem.OffsetRect(0,2);

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	VERIFY( bitmap.CreateCompatibleBitmap( &dc, rcMem.Width(), rcMem.Height() ) );
	CBitmap* pOldBmp = memDC.SelectObject( &bitmap );

	//Draw the header into the logical bitmap and use this to generate the drag image list
	int oldMode = memDC.SetBkMode(TRANSPARENT);
	CFont* pOldFont = memDC.SelectObject(GetHeaderFont());
	//Specify a dark grey background for the drag image 
	CBrush br(RGB(110,110,110));		
	CBrush* pOldBrush = memDC.SelectObject(&br);			
	memDC.FillRect(rcMem, &br);

	Column* pCol = GetColumnAt(nColumn);
	
	UINT fAdditionalFlags(0);
	
	// Draw the column header text		
	rcMem.left  += SEC_LEFT_OFFSET;
	rcMem.right -= SEC_RIGHT_OFFSET;
	
	ColumnFormatHelper(pCol->fmt, fAdditionalFlags);

	CSize size = memDC.GetTextExtent( pCol->strText );
	
	//Draw the header icon if the header width permits. Or else, just draw the text.
	if( (m_pHeaderImageList) && (pCol->iImage >= 0) && (rcMem.Width()>22))
	{
		ASSERT_VALID(m_pHeaderImageList);
		DrawHeaderImage(&memDC, pCol, rcMem, fAdditionalFlags);
	}
#ifndef UNDER_CE
	if( size.cx > rcMem.Width() )
		fAdditionalFlags |= DT_END_ELLIPSIS;
#endif //UNDER_CE
	memDC.DrawText( pCol->strText, 
				   -1,
				   rcMem,
				   DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_EXPANDTABS|fAdditionalFlags);

	memDC.SetBkMode(oldMode);
	memDC.SelectObject(pOldBrush);
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldBmp);

	//Now that we have our bitmap, use this to create the image list. A COLOR_BTNFACE mask is used
	//on the assumption that this RGB will have min interference with the text or icon images in the header.
	BITMAP bm;
	memset(&bm,0,sizeof(BITMAP));
	bitmap.GetBitmap(&bm);
	CImageList* pDragImage = new CImageList;
	VERIFY(pDragImage->Create( bm.bmWidth, bm.bmHeight, ILC_COLOR|ILC_MASK, 0, 1 ));
	pDragImage->Add( &bitmap, ::GetSysColor(COLOR_BTNFACE) );		

	return pDragImage;	
}


void SEC_LISTCLASS::ColumnFormatHelper(int& nColFmt, UINT& flags)
{
	switch (nColFmt & ~LVCFMT_BITMAP_ON_RIGHT)
	{
	case LVCFMT_LEFT:
		flags = DT_LEFT;
		break;
	case LVCFMT_RIGHT:
		flags = DT_RIGHT;
		break;
	case LVCFMT_CENTER:
		flags = DT_CENTER;
		break;
	default:
		// Invalid Format
		ASSERT(FALSE);
	}
}


BOOL SEC_LISTCLASS::RedrawItems( int nFirst, int nLast )
{
	Item* pItem = 0;
	RECT rcInvalid;
	RECT rcInside;
	int i;

	GetInsideRect(&rcInside);
	rcInvalid.top = rcInside.top;
	rcInvalid.left = rcInside.left - m_ptViewportOrg.x;
	rcInvalid.right = rcInside.right;

	if (nFirst > nLast)
	{
		int nSwap;
		nSwap = nFirst;
		nFirst = nLast;
		nLast = nSwap;
	}

	nFirst = otmax(nFirst, GetTopIndex());

	for (i=GetTopIndex(); i<nFirst; i++)
	{
		pItem = GetItemAt(i);
		rcInvalid.top += pItem->m_cy + GetMargin();
	}

	rcInvalid.bottom = rcInvalid.top;

	for (; i<=nLast; i++)
	{
		pItem = GetItemAt(i);
		rcInvalid.bottom += pItem->m_cy + GetMargin();
	}

	InvalidateRect(&rcInvalid, TRUE);

	return TRUE;
}


BOOL SEC_LISTCLASS::Update( int nItem, BOOL bLabelOnly, BOOL bEraseBkgnd, BOOL bUpdateBelow, BOOL bUpdateNow)
{
	if (nItem >= GetTopIndex())
	{
		CRect rcItem;
		Item* pItem = 0;
		CRect rcInside;
		CSize szLog;
		int i;

		GetInsideRect(&rcInside);
		szLog = GetLogicalSize();
		rcItem.top = rcInside.top + GetMargin();
		rcItem.left = rcInside.left - m_ptViewportOrg.x;
		rcItem.right = rcItem.left + otmax(szLog.cx, rcInside.Width());
		rcItem.bottom = rcItem.top;

		for (i=GetTopIndex(); i<=nItem; i++)
		{
			pItem = GetItemAt(i);
			rcItem.top = rcItem.bottom;
			if ( pItem )
				rcItem.bottom += pItem->m_cy + GetMargin();
		}

		if (bUpdateBelow)
		{
			// Make sure we not only invalidate, but erase
			// below the updated item.  This is necessary
			// because items may have been deleted in which
			// case no fills will be done on the next repaint
			// of this region.
			if (bEraseBkgnd)
				rcItem.bottom = rcInside.bottom;
			else
			{
				CClientDC dc(this);
				int top = rcItem.top;
				rcItem.top = rcItem.bottom;
				rcItem.bottom = rcInside.bottom;
				dc.FillSolidRect(rcItem, GetBkColor());
				rcItem.top = top;
			}
		}

		if (pItem && bLabelOnly && ((GetListCtrlStyleEx()&LVXS_HILIGHTSUBITEMS) == 0))
		{
			// State has changed so make sure the item is redrawn
			rcItem.left += pItem->rcText.left - WIDTH_GAP/2;
			rcItem.right = rcItem.left + pItem->rcText.Width() + WIDTH_GAP;
		}

		if ( bUpdateNow )
		{
			// invalidate does not work if we are dragging an image.
			// the imagelist locks up the window so it can't draw, except during
			// CImageList::DragLeave -> CImageList::DragEnter sections,(typically in OnMouseMove).
			// If these sections are in the same function, the paint message will not
			// be handled in time, before the window is locked again.
#ifndef UNDER_CE
			UINT flags = RDW_INVALIDATE | RDW_UPDATENOW;

			if ( bEraseBkgnd )
				flags |= RDW_ERASE;
			RedrawWindow( &rcItem, NULL, flags);
#else
			RedrawWindow( &rcItem, NULL);
#endif //UNDER_CE
		}
		else
			InvalidateRect(&rcItem, bEraseBkgnd);

	}

	return TRUE;
}


void SEC_LISTCLASS::OnPaint()
{
	//The paint DC...
	CPaintDC paintDC(this);
	DoPaint(&paintDC);
}


void SEC_LISTCLASS::DoPaint(CDC* pDC)
{
	LvPaintContext* pPC;
	CRect rectInside(0,0,0,0),	//Inside Rectangle  = "client rect" - "width of vertical scroll" - "height of horizontal scroll" - "height of header"
		  rectClient(0,0,0,0),  //Complete client area
		  rectFill(0,0,0,0),    //Area to fill with background color.
		  rectErase(0,0,0,0);   //small corner intersection of vertical and horizontal scroll bar ( lower right hand corner )

	GetInsideRect(rectInside);
	SECGetClientRect(rectClient);

	if ( pDC->IsPrinting() )
	{
		// Initialize Paint Context
		pPC = CreatePaintContext(pDC);
		pPC->da = DrawEntire;
		pPC->pdc = pDC;
		pPC->dwViewStyle = GetListCtrlStyle() & LVS_TYPEMASK;


		//Draw the header control...
		if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT 
			&& IsHeaderCtrlEnabled())
		{
			DrawHeader( pPC );				
		}


		DrawInvalidItems(pPC);

	}
	else	// not printing
	{
		CDC* pTempDC = NULL; // scratch pad dc
		
		BOOL bDelete = TRUE;
		BOOL bHScroll = !!(m_dwScrollStyle & WS_HSCROLL);
		BOOL bVScroll = !!(m_dwScrollStyle & WS_VSCROLL);


		//We only fill in the item area...normally we would want to fill the header as well,
		// but since the header can extend off the screen we don't want to confine it to our normal clipping rect...
		rectFill = rectInside;
		rectFill.top = rectClient.top;

		//If I don't have a Horizontal scroll bar, I want my rectFill to extend all
		// the way to the bottom
		if( !bHScroll )
		{
			rectFill.bottom = rectClient.bottom;
		}

		//If I don't have a Vertical scroll bar, I want my rectFill to extend all
		// the way to the right
		if( !bVScroll )
		{
			rectFill.right = rectClient.right;
		}

		// If I have both a hscroll and vscroll, setup the rectangular area 
		//  where the scrollbars meet.
		if( bHScroll && bVScroll )
		{
			rectErase.left = rectInside.right + 1;
			rectErase.right = rectClient.right;
			rectErase.top = rectInside.bottom + 1;
			rectErase.bottom = rectClient.bottom;
		}

		//Create the memdc SEC_LISTBASEd on the fill rectangle.  That way there will be no flicker behind
		//  the scroll bars when they redraw ( hopefully :-)
		bDelete = CreateMemDC( pDC, pTempDC, rectFill );

		pTempDC->FillSolidRect( rectFill, GetBkColor() );

		// Initialize Paint Context
		pPC = CreatePaintContext(pTempDC);
		pPC->da = DrawEntire;
		pPC->pdc = pTempDC;
		pPC->dwViewStyle = GetListCtrlStyle() & LVS_TYPEMASK;

		//Draw the header control...
		if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT 
			&& IsHeaderCtrlEnabled())
		{
			DrawHeader(pPC);
			
		}

		// We have to do this so drawn text doesn't overwrite the
		// vertical scrollbar which makes it flicker.
		pTempDC->IntersectClipRect(rectInside);

		// And finally, draw the contents of the list control
		DrawInvalidItems(pPC);

		if( bDelete ) 
			delete pTempDC;

		//All non-memdc drawing goes here...

		// Erase the rectangular area where the horizontal and vertical
		// scrollbars meet.  Must Erase with the screen dc and not mem dc because mem dc is created
		// SEC_LISTBASEd on rectFill which may not include this area.
		if ( bHScroll && bVScroll )
		{
			pDC->FillSolidRect(rectErase, rgbBtnFace);
		}
	}
	// Delete the paint context
	delete pPC;
}


BOOL SEC_LISTCLASS::OnEraseBkgnd(CDC* pDC)
{
	pDC;		// unreferenced
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Layout Members & Position/Dimension Accessors
//


void SEC_LISTCLASS::RecalcLayout()
{
	CScrollBar* pHorzSB = GetScrollBarCtrl(SB_HORZ);
	CScrollBar* pVertSB = GetScrollBarCtrl(SB_VERT);

	// give the hint for the maximum number of HWNDs
	if (pHorzSB || pVertSB)
	{
		CRect rectInside;
		CRect rectClient;

		HDWP hDWP = ::BeginDeferWindowPos(2);

		GetInsideRect(rectInside);
		SECGetClientRect(rectClient);

		// reposition scroll bars
		if (pHorzSB)
		{
			::DeferWindowPos(hDWP, pHorzSB->m_hWnd, NULL, rectInside.left,
				rectInside.bottom, rectInside.Width()+1, cyHorzScroll, 0);
			pHorzSB->Invalidate();
		}

		if (pVertSB)
		{
			int cy = rectInside.Height()+1;
			if ( IsHeaderCtrlEnabled() )
				cy += m_cyHeader;

			::DeferWindowPos(hDWP, pVertSB->m_hWnd, NULL, rectInside.right,
				rectClient.top, cxVertScroll, cy, 0);

			pVertSB->Invalidate();
		}

		// move and resize all the windows at once!
		if (hDWP == NULL || !::EndDeferWindowPos(hDWP))
			TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
	}
}


void SEC_LISTCLASS::MeasureSubItem(LvPaintContext* pPC, UINT& cySubItem)
{
	CRect rcText(0,0,pPC->pCol->cx,0);
	
	CRect rcIcon; 
	rcIcon.SetRectEmpty();

	if( pPC->lvi.pszText && _tcslen(pPC->lvi.pszText) >= 0 )
	{
		if( pPC->pCol->iSubItem == 0 )
		{
			MeasureSubItemIcon( pPC, rcIcon );
			pPC->pItem->rcIcon = rcIcon;
		}
		
		MeasureSubItemText( pPC, rcText );
	}

	switch( GetListCtrlStyle() & LVS_TYPEMASK )
	{
	case LVS_REPORT:
		cySubItem = otmax(rcText.Height(), rcIcon.Height());
		break;

	case LVS_ICON:
		cySubItem = rcIcon.Height() + 5 + rcText.Height();
		break;

	case LVS_LIST:
	case LVS_SMALLICON:
	default:
		ASSERT(FALSE);
		break;
	}
}


void SEC_LISTCLASS::MeasureSubItemIcon( LvPaintContext* pPC, CRect& rcIcon )
{
	DWORD dwView = GetListCtrlStyle() & LVS_TYPEMASK;
	
	rcIcon.SetRectEmpty();

	CImageList* pIml = NULL;

	switch( dwView )
	{
	case LVS_ICON:
		pIml = GetImageList( LVSIL_NORMAL );
		break;

	case LVS_SMALLICON:
	case LVS_LIST:
	case LVS_REPORT:
		pIml = GetImageList( LVSIL_SMALL );
		break;

	default:
		//ASSERT -> Unknown style
		ASSERT(FALSE);
		return;
	}

	//If we don't have an image list, odds are they don't want one...just return
	if( !pIml )
		return;

	//If we do have an image list, better be a valid image list...
	ASSERT_VALID( pIml );
		
	IMAGEINFO ii;
	::ZeroMemory( &ii, sizeof(ii) );

	pIml->GetImageInfo( pPC->pItem->iImage, &ii );

	int xIndentFromLeft = 2;

	rcIcon.left = xIndentFromLeft;
	rcIcon.top  = 2;
	rcIcon.right = rcIcon.left + (ii.rcImage.right - ii.rcImage.left);
	rcIcon.bottom = rcIcon.top + (ii.rcImage.bottom - ii.rcImage.top);
}


void SEC_LISTCLASS::MeasureSubItemText( LvPaintContext* pPC, CRect& rcItem )
{
	CRect rcText( 0, 0, pPC->pCol->cx, 0 );
	
	UINT nFlags = DT_WORDBREAK|DT_NOPREFIX|DT_CALCRECT|DT_EXPANDTABS;
	
	CFont* pOldFont = pPC->pdc->SelectObject( pPC->pFont );

	switch( pPC->pCol->fmt & ~LVCFMT_BITMAP_ON_RIGHT )
	{
	case LVCFMT_LEFT:
		nFlags |= DT_LEFT;
		break;

	case LVCFMT_RIGHT:
		nFlags |= DT_RIGHT;
		break;

	case LVCFMT_CENTER:
		nFlags |= DT_CENTER;
		break;

	default:
		ASSERT(FALSE);
		nFlags |= DT_LEFT;
		break;
	}

	int pos = -1;
			
	if (GetListCtrlStyle() & LVS_NOLABELWRAP)
	{
		TCHAR* pdest;
		pdest = _tcschr(pPC->lvi.pszText, '\n');
		if (pdest != NULL)
			pos = (int)(pdest - pPC->lvi.pszText + 1);
		nFlags |= DT_SINGLELINE;
	}

	// measure using the attribute dc if possible
	HDC hdc = pPC->pdc->m_hAttribDC;
	if ( !hdc )
		hdc = pPC->pdc->m_hDC;

	::DrawText( hdc, pPC->lvi.pszText, pos, &rcText, nFlags);
		
	//int xIndent = 2;
	int xIndent = SEC_LEFT_OFFSET;
	int yIndent = 2;

	if( (pPC->pItem->rcIcon.IsRectEmpty() == FALSE) && 
		(pPC->lvi.iSubItem == 0) )
	{
		switch( GetListCtrlStyle() & LVS_TYPEMASK )
		{
		case LVS_REPORT:
		case LVS_SMALLICON:
		case LVS_LIST:
			xIndent = pPC->pItem->rcIcon.right + 5;
			yIndent = (pPC->pItem->rcIcon.Height() - rcText.Height()) / 2;
			break;

		case LVS_ICON:
			xIndent = 8;
			yIndent = pPC->pItem->rcIcon.bottom + 2;
			break;
		}
	}

	rcItem.left   = xIndent; 
	rcItem.top    = yIndent; 
	rcItem.right  = rcItem.left + rcText.Width();
	rcItem.bottom = rcItem.top + rcText.Height();

	if( pPC->lvi.iSubItem == 0 )
		pPC->pItem->rcText = rcItem;
	else
		pPC->pSubItem->rcText = rcItem;

	pPC->pdc->SelectObject(pOldFont);
}


void SEC_LISTCLASS::MeasureItem(LvPaintContext* pPC) 
{
	if(!pPC->pItem->m_bDirty)
		return;

	if( pPC->pItem->m_cy == LVXC_UNINITIALIZED )
		pPC->pItem->m_cy = 0;

	UINT cySubItem = pPC->pItem->m_cy;

	int cColumns = GetColumnCount();
	for( pPC->iCol=0 ; pPC->iCol<cColumns; pPC->iCol++ )
	{
		pPC->pCol = GetColumnAt(pPC->iCol);
		pPC->lvi.iSubItem = pPC->pCol->iSubItem;
		if (pPC->pCol->iSubItem > 0)
		{
			ASSERT_POINTER(pPC->pItem, Item);
			pPC->pSubItem = pPC->pItem->GetSubItem(pPC->pCol->iSubItem);
		}
		// Select the font into the DC
		VERIFY(LoadPaintContext(pPC));
		MeasureSubItem(pPC, cySubItem);
		pPC->pItem->m_cy = otmax(pPC->pItem->m_cy, cySubItem);
	}
	pPC->pItem->m_bDirty = FALSE;
}


CDC* SEC_LISTCLASS::GetActiveDC( BOOL& bDelete )
{
	bDelete = TRUE;
	return new CClientDC(this);
}



void
SEC_LISTCLASS::MeasureItem(int nItem, LvPaintContext* pPC)
{

	if ( pPC == NULL )
	{
		BOOL bDelete = FALSE;
		CDC* pDC = GetActiveDC( bDelete );
		// Init Paint Context
		LvPaintContext* pPC2 = CreatePaintContext(pDC);
		pPC2->pdc = pDC;

		pPC2->lvi.iItem = nItem;
		pPC2->pItem = GetItemAt(pPC2->lvi.iItem);
		LoadPaintContext(pPC2);
		MeasureItem(pPC2);

		// Delete the paint context
		if ( pDC && bDelete)
			delete pDC;
		if ( pPC2 )
			delete pPC2;
	}
	else	// a pPC was passed in so use it
	{
		pPC->lvi.iItem = nItem;
		pPC->pItem = GetItemAt(pPC->lvi.iItem);
		LoadPaintContext(pPC);

		MeasureItem(pPC);
	}


}


//@doc SEC_LISTCLASS
//@mfunc Marks all items as needing a remeasurement when the next WM_PAINT occurs.
//@rdesc void 
//@comm  This call does not invalidate or redraw the control.
void SEC_LISTCLASS::ReMeasureAllItems()
{
	ClearMeasurements();
	RecalcScrollBars();
}


void SEC_LISTCLASS::ClearMeasurements()
{
	Item* pItem = NULL;
	int i, c;

	c = GetItemCount();
	for (i=0; i<c; i++)
	{
		pItem = GetItemAt(i);
		if ( pItem )
		{
			pItem->m_bDirty = TRUE;
			if(IsWordWrapEnabled())
				pItem->m_cy = LVXC_UNINITIALIZED;
		}
	}
}



void SEC_LISTCLASS::ReMeasureItem( int iItem )
{
	ASSERT( iItem >= 0 && iItem < GetItemCount() );

	Item* pItem = NULL;
	pItem = GetItemAt(iItem);
	if(pItem)
		pItem->m_bDirty = TRUE;	

	Update( iItem, FALSE, TRUE, FALSE );
}


BOOL SEC_LISTCLASS::GetItemRect( int nItem, LPRECT lpRect, UINT nCode ) const
{
	Column *pCol;
	int nCol;

	for (nCol=0; nCol<GetColumnCount(); nCol++)
	{
		pCol = GetColumnAt(nCol);
		if (pCol->iSubItem == 0)
			break;
	}
	
	if (nCol < GetColumnCount())
		return GetItemRect(nItem, nCol, lpRect, nCode);
	else
		return FALSE;
}


BOOL SEC_LISTCLASS::GetItemRect( int nItem, int nCol, LPRECT lpRect, UINT nCode ) const
{
	if (nItem >= GetTopIndex())
	{
		Item* pItem;
		RECT rcInside;
		Column* pCol;
		int iSubItem;
		int i;

		GetInsideRect(&rcInside);
		pCol = GetColumnAt(nCol);
		lpRect->top = rcInside.top;
		lpRect->left = GetColumnLeft(nCol);
		lpRect->right =  lpRect->left + pCol->cx;
		lpRect->bottom = lpRect->top;

		for (i=GetTopIndex(); i<=nItem; i++)
		{
			pItem = GetItemAt(i);
			lpRect->top = lpRect->bottom + GetMargin();
			lpRect->bottom += pItem->m_cy + GetMargin();
		}

		if (nCode == LVIR_BOUNDS)
		{
			// we don't want to bound to the column width if only 1 column is being used.
			if ( GetColumnCount() <= 1 )
				lpRect->right = rcInside.right;

			return TRUE;
		}
		else
		{
			iSubItem = pCol->iSubItem;
			if (iSubItem == 0)
			{
				Item* pItem = GetItemAt(nItem);

				if (nCode == LVIR_LABEL)
				{
					lpRect->left += pItem->rcText.left;
					lpRect->right = lpRect->left + pItem->rcText.Width();
					return TRUE;
				}
				else if (nCode == LVIR_ICON)
				{
					lpRect->top += pItem->rcIcon.top;
					lpRect->bottom = lpRect->top + pItem->rcIcon.Height();
					lpRect->left += pItem->rcIcon.left;
					lpRect->right = lpRect->left + pItem->rcIcon.Width();
					return TRUE;
				}
			}
			else
			{
				Item* pItem;
				SubItem* pSubItem;

				lpRect->left = GetColumnLeft(nCol);
				lpRect->right = lpRect->left + GetColumnWidth(nCol);

				pItem = GetItemAt(nItem);
				pSubItem = pItem->GetSubItem(iSubItem);

				if (nCode == LVIR_LABEL)
				{
					// pSubItem->rcText has the correct size, but must be adjusted in the x direction,
					// depending on the alignment of the column. Our drawing functions don't have to
					// worry about this: they just use the entire item rect and tell DrawText to do
					// the alignment for us. However, any functions that rely on knowing the true text
					// position and extents, such as the tooltip decision, will need this.
					int xCenter;
					switch (pCol->fmt & ~LVCFMT_BITMAP_ON_RIGHT)
					{
					case LVCFMT_RIGHT|LVCFMT_CENTER:
						xCenter = lpRect->left + ( pCol->cx / 2 );
						lpRect->left = xCenter - ( pSubItem->rcText.Width() / 2 );
						lpRect->right = lpRect->left + pSubItem->rcText.Width();
						break;
					case LVCFMT_LEFT:
					default:
						lpRect->left += pSubItem->rcText.left;
						lpRect->right = lpRect->left + pSubItem->rcText.Width();
						break;

					}

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


BOOL SEC_LISTCLASS::AutoSizeColumns()
{
	if( GetColumnCount() == 1 || GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE )
	{
		ResizeColumnsToFit();
		return TRUE;
	}

	return FALSE;
}


void SEC_LISTCLASS::OnSize(UINT nType, int cx, int cy)
{
	RecalcLayout();

	if( GetColumnCount() > 0 )
	{
		AutoSizeColumns();
		ReMeasureAllItems();
	}

	SEC_LISTBASE::OnSize(nType, cx, cy);
}


int SEC_LISTCLASS::GetItemAtPoint(const CPoint& pt)
{
	Item* pItem;
	int y;
	int i;

	y = 0;
	for (i=0; i<GetItemCount() && y < pt.y; i++)
	{
		pItem = GetItemAt(i);
		y += pItem->m_cy + GetMargin();
	}

	return i;
}


int SEC_LISTCLASS::GetItemAbovePoint( const CPoint& pt )
{
	int y = 0;
	for( int i = 0; i < GetItemCount() && pt.y > y; i++ )
	{
		y += GetItemAt(i)->m_cy + GetMargin();
	}

	return i;
}


int SEC_LISTCLASS::GetItemBelowPoint( const CPoint& pt )
{
	return GetItemAtPoint(pt);
}


int SEC_LISTCLASS::CalcRangeHeight(int nFirst, int nLast, BOOL bInclusive)
{
	Item* pItem;
	int cyRange;
	int i;
	BOOL bNeg = FALSE;

	if (nFirst > nLast)
	{
		int nSwap;
		nSwap = nFirst;
		nFirst = nLast;
		nLast = nSwap;
		bNeg = TRUE;
	}

	cyRange = 0;
	for (i=nFirst; i<nLast || (bInclusive && i<=nLast); i++)
	{
		pItem = GetItemAt(i);
		if (pItem->m_bDirty)
			MeasureItem(i);
		cyRange += pItem->m_cy + GetMargin();
	}

	if (bNeg)
		cyRange *= -1;

	return cyRange;
}


//
// CalcItemsInRange()
// 
//	This is function will tell you how many items will fit in a given height.
//	Since the list control allows variable height items, we must explicit
//	sum the heights of items to determine how many will fit within a pixel
//	range.
//
//   cyRange    - The pixel height to count the number of items fitting.
//   nStartItem - The index of the item to start the computation with.
//   bDown      - Indicates whether we should scan downward or upward from
//                the starting item in computing item count within range.
//

int SEC_LISTCLASS::CalcItemsInRange(int cyRange, int nStartItem, BOOL bDown)
{
	Item* pItem;
	int cy;
	int nItems;
	int i;

	cy = 0;
	nItems = 0;
	if (bDown)
	{
		for (i=nStartItem; i<GetItemCount(); i++)
		{
			pItem = GetItemAt(i);
			cy += pItem->m_cy + GetMargin();
			nItems++;
			if (cy >= cyRange)
				break;
		}
	}
	else // up
	{
		for (i=nStartItem; i>=0; i--)
		{
			pItem = GetItemAt(i);
			cy += pItem->m_cy + GetMargin();
			nItems++;
			if (cy >= cyRange)
				break;
		}
	}

	return nItems;
}


int SEC_LISTCLASS::GetLogicalHeight()
{
	return GetItemCount();
}


int SEC_LISTCLASS::GetLogicalWidth(LvPaintContext* pPC)
{
	int cx = 0;
	if (GetColumnCount() > 1)
	{
		int i, c;
		c = GetColumnCount();
		for (i=0; i<c; i++)
			cx += GetColumnWidth(i);
	}
	else
	{
		CRect rcInside;
		Item* pItem;
		int i, c;
		int cyRange;

		c = GetItemCount();
		int cxTextMax = 0;
		cyRange = 0;
		GetInsideRect(rcInside);
		
		//////////////////////////////////
		if ( IsWordWrapEnabled() || ( GetColumnCount() > 1 ) )
		{
			// need to measure just the visible items, because all have the same width
			for (i=GetTopIndex(); (i < c) && cyRange < rcInside.Height(); i++)
			{
				pItem = GetItemAt(i);
				if (pItem->m_bDirty)
					MeasureItem(i, pPC);

				cxTextMax = otmax(cxTextMax, pItem->rcText.right + SEC_RIGHT_OFFSET);
				cyRange += pItem->m_cy + GetMargin();
			}

		}
		else
		{

			// need to measure all items, not just the visible ones
			for (i=GetTopIndex(); i < c ; i++)
			{
				pItem = GetItemAt(i);
				if (pItem->m_bDirty)
					MeasureItem(i, pPC);

				cxTextMax = otmax(cxTextMax, pItem->rcText.right + SEC_RIGHT_OFFSET);
			}
		}

		if( (( this -> GetListCtrlStyle() & LVS_TYPEMASK ) == LVS_REPORT ) )
			cx = otmax(cxTextMax, rcInside.Width());
		else
			cx = cxTextMax;
	}
	
	return cx;
}



//@doc SEC_LISTCLASS
//@mfunc Retrieves the logical size of the list view control.
//@rdesc The current size of the control item bounds.
//@comm Retrieves the logical size of the list view control.  Use this member to retrieve the minimum bounding size necessary to display all items and columns of your list control.  Note: size returned may be larger than the available visible size.
CSize SEC_LISTCLASS::GetLogicalSize(LvPaintContext* pPC)
{
	CSize szLog(0, 0);
	szLog.cx = GetLogicalWidth(pPC);
	szLog.cy = GetLogicalHeight();
	return szLog;

}


int SEC_LISTCLASS::GetFirstItemLastPage() const
{
	CRect rcInside;
	Item* pItem;
	int cyRange;
	int i;

	i = GetItemCount();
	if (i > 0)
	{
		cyRange = 0;
		i = GetItemCount();
		GetInsideRect(rcInside);
		for (;;)
		{
			pItem = GetItemAt(i-1);
			cyRange += pItem->m_cy + GetMargin();
			if (cyRange > rcInside.Height())
				break;
			i--;
			if (i <= 0)
				break;
		}
	}

	return i;
}


int SEC_LISTCLASS::GetFirstItemPrevPage()
{
	Item* pItem;
	int cyRange;
	int i;

	CRect rcInside;
	GetInsideRect(rcInside);

	cyRange = 0;
	i = GetTopIndex();
	if (i > 0)
	{
		for (; i>=0 && cyRange < rcInside.Height(); i--)
		{
			pItem = GetItemAt(i);
			if (pItem->m_bDirty)
				MeasureItem(i);
			cyRange += pItem->m_cy + GetMargin();
		}
	}

	return i;
}


int SEC_LISTCLASS::GetLastFullyVisibleItem(LvPaintContext* pPC)
{
	Item* pItem;
	int cyRange;
	int i;

	CRect rcInside;
	GetInsideRect(rcInside);


	cyRange = 0;
	for (i=GetTopIndex(); i<GetItemCount(); i++)
	{
		pItem = GetItemAt(i);
		if (pItem->m_bDirty)
		{
			MeasureItem(i, pPC);
		}
		cyRange += pItem->m_cy + GetMargin();

		if (cyRange > rcInside.Height())
			break;
	}

	return i-1;
}


int SEC_LISTCLASS::GetFirstItemNextPage() const
{
	Item* pItem;
	int cyRange;
	int i;

	CRect rcInside;
	GetInsideRect(rcInside);

	cyRange = 0;
	for (i=GetTopIndex(); i<GetItemCount() && cyRange < rcInside.Height(); i++)
	{
		pItem = GetItemAt(i);
		cyRange += pItem->m_cy + GetMargin();
	}

	return i;
}

// Returns the number of fully visible items currently displayed

int SEC_LISTCLASS::ItemsThisPage()
{
	Item* pItem;
	int cyRange;
	int cCount, i;

	CRect rcInside;
	GetInsideRect(rcInside);

	cyRange = 0;
	cCount = 0;
	for (i=GetTopIndex(); i<GetItemCount(); i++, cCount++)
	{
		pItem = GetItemAt(i);
		if (pItem->m_bDirty)
			MeasureItem(i);
		cyRange += pItem->m_cy + GetMargin();
		if (cyRange > rcInside.Height())
			break;
	}

	if (cyRange < rcInside.Height())
	{
		i = GetTopIndex()-1;
		if (i >=0)
		{
			for (; i>=0; i--, cCount++)
			{
				pItem = GetItemAt(i);
				if (pItem->m_bDirty)
					MeasureItem(i);
				cyRange += pItem->m_cy + GetMargin();
				if (cyRange > rcInside.Height())
					break;
			}
		}
	}

	return cCount;
}

// Returns the number of fully visible items on the previous page.

int SEC_LISTCLASS::ItemsPrevPage()
{
	Item* pItem;
	int cyRange;
	int cCount, i;

	CRect rcInside;
	GetInsideRect(rcInside);

	cyRange = 0;
	cCount = 0;
	i = GetTopIndex()-1;
	if (i >= 0)
	{
		for (; i>=0; i--, cCount++)
		{
			pItem = GetItemAt(i);
			if (pItem->m_bDirty)
				MeasureItem(i);
			cyRange += pItem->m_cy + GetMargin();
			if (cyRange > rcInside.Height())
				break;
		}
	}

	return cCount;
}


int SEC_LISTCLASS::GetTopIndex() const
{
	return m_nTopIndex;
}


BOOL SEC_LISTCLASS::GetOrigin(LPPOINT lpPoint) const
{
	lpPoint->x = m_ptViewportOrg.x;
	lpPoint->y = m_ptViewportOrg.y;

	return TRUE;
}


BOOL SEC_LISTCLASS::SetItemHeight(int nIndex, UINT cyItemHeight) const
{
	if (cyItemHeight >= 0)
	{
		Item* pItem = GetItemAt(nIndex);
		pItem->m_cy = cyItemHeight;
		return TRUE;
	}

	return FALSE;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the useable client area of the control.
//@rdesc void 
//@parm LPRECT | lpRect | Receives the client rectangle.
//@comm  Use this function instead of GetClientRect() to get the client drawing area.
void SEC_LISTCLASS::GetInsideRect(LPRECT lpRect) const
{
	if ( !m_hWnd )
		return;

	SECGetClientRect(lpRect);
	if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT 
		&& IsHeaderCtrlEnabled())
	{
		lpRect->top += m_cyHeader;
	}

	// Subtract scrollbar clearance
	if (m_dwScrollStyle & WS_VSCROLL)
		lpRect->right -= cxVertScroll - 1;
	if (m_dwScrollStyle & WS_HSCROLL)
		lpRect->bottom -= cyHorzScroll - 1;
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Column Management & Accessors
//


//@doc SEC_LISTCLASS
//@mfunc Returns the Column object at a given index.
//@rdesc a pointer to the specified column object if successful, otherwise NULL.
//@parm int | nCol | The index of the column.
SEC_LISTCLASS::Column* SEC_LISTCLASS::GetColumnAt(int nCol) const
{
	Column* pCol = NULL;
	if ( nCol >= 0 && nCol < GetColumnCount() )
		pCol = (Column*)m_arrColumns[nCol];

	return pCol;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the number of columns.
//@rdesc The number of columns in the control.
int SEC_LISTCLASS::GetColumnCount() const
{
	return (int)m_arrColumns.GetSize();
}


SEC_LISTCLASS::Item* 
SEC_LISTCLASS::GetItemAt(int nIndex) const
{
	Item* pItem = NULL;
	//ASSERT(nIndex >= 0 && nIndex < GetItemCount());
	if (nIndex >= 0 && nIndex < GetItemCount())
		pItem = (Item*)m_arrItems[nIndex];

	ASSERT_NULL_OR_POINTER(pItem, Item);

	return pItem;
}



//@doc SEC_LISTCLASS
//@mfunc Inserts a column in the list view control.
//@syntax InsertColumn( int nCol, Column* pColNew, BOOL bUpdate)
//@syntax InsertColumn(int nCol, const CString& strHeader, int fmt, int widthCol, int iSubItem, BOOL bUpdate)
//@syntax InsertColumn( int nCol, const LV_COLUMN* lvc, BOOL bUpdate )
//@rdesc Nonzero if successful; otherwise 0.
//@parm  int | nCol | The index of the new column.
//@parm  Column* | pColNew | Address of a Column structure that contains the attributes of the new column.
//@parm  BOOL | bUpdate | If TRUE and redraw after insert is enabled, the control will be invalidated.
//@parm  const CString& | strHeader | Refernce to a string containing the column’s header.
//@parm  int | fmt | Integer specifying the alignment of the column.  It can be one of these values: LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.  Default is LVCFMT_LEFT.
//@parm  int | widthCol | Width of the column, in pixels.  If this parameter is -1, the column width is not set.
//@parm  int | iSubItem | Index of the subitem associated with the column.  If this parameter is -1, no subitem is associated with the column.
//@parm  int | iImage   | Zero-based index of the column header image. When using header images the LVCFMT_BITMAP_ON_RIGHT can be specified to display the image to the right of text. 
//@parm  const LV_COLUMN* | lvc | Address of an LV_COLUMN structure that contains the attributes of the new column.
BOOL SEC_LISTCLASS::InsertColumn( int nCol, Column* pColNew, BOOL bUpdate)
{
	//According to the documentation...column 0 in a list control with report view set,
	// must be formatted to LVCFMT_LEFT.  So I make sure that if nCol == 0, 
	// to set Column::fmt to LVCFMT_LEFT

	ProcessAndDestroyEditLabel();
	m_pToolTipWnd->Hide();

	if( nCol == 0 )
	{
		pColNew->fmt = LVCFMT_LEFT;
		pColNew->iSubItem = 0;
	}

	Item* pItem;
	int cSubItems;
	int nIndex;

	ASSERT_POINTER(pColNew, Column);

	// Record the new column
	m_arrColumns.InsertAt(nCol, pColNew);

	cSubItems = GetSubItemCount();

	// Set the subitem id
	if (pColNew->iSubItem == -1 || pColNew->iSubItem > cSubItems)
	{
		// We must create a new sub item
		pColNew->iSubItem = nCol;

		// Insert sub items for all existing items
		for (nIndex=0; nIndex < GetItemCount(); nIndex++)
		{
			pItem = GetItemAt(nIndex);
			pItem->InsertSubItem(nCol - 1); 
		}

		// If the column width was unspecified, choose a default
		if (pColNew->cx == -1)
		{
#if 0
			CRect rcInside;
			GetInsideRect(rcInside);
			pColNew->cx = rcInside.Width()/4;
#else
			pColNew->cx = 10;
#endif
		}

		m_nSubItemCount++;
	}

	//OK...new column is already in the array of columns...
	//  we have to increase the sub-item count for each column after this one or
	//  we get a copy of the text and we don't want that...
	if( nCol < GetColumnCount() )
	{
		Column* pCol = NULL;
		Item* pItem = NULL;
		BOOL bAdded = FALSE;
		int i = 0;

		for( i = nCol + 1; i < GetColumnCount(); i++ )
		{
			pCol = GetColumnAt(i);
			if ( pCol )
			{
				pCol->iSubItem++;
				bAdded = TRUE;
			}
		}

		if( bAdded )
			for( i = 0; i < GetItemCount(); i++ )
			{
				pItem = GetItemAt(i);
				pItem->InsertSubItem( nCol );
			}
		
		if( IsKindOf(RUNTIME_CLASS(SECTreeView)) )
		{
			class FriendSECTreeView : public SECTreeView
			{
				friend class SEC_LISTCLASS;
			};
			FriendSECTreeView* pThis = (FriendSECTreeView*)(this);
			if( pThis->IsStoringSubItemText() )
				pThis->m_pSubItemTextStorage->InsertItemAll(nCol);
		}
	}

	if ( bUpdate && IsRedrawAfterInsertEnabled() )
	{
		if( GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE )
			ResizeColumnsToFit();

		ReMeasureAllItems();
		Invalidate();
	}

	return TRUE;
}


BOOL SEC_LISTCLASS::InsertColumn(int nCol, const CString& strHeader, int fmt, int widthCol, int iSubItem, int iImage, BOOL bUpdate)
{
    //insert Item info
	Column* pCol = new Column;
	pCol->cx = widthCol;
	pCol->strText = strHeader;
	pCol->fmt = fmt;
	pCol->iSubItem = iSubItem;
	pCol->iImage = iImage;

	return InsertColumn(nCol, pCol, bUpdate);
}


BOOL SEC_LISTCLASS::InsertColumn( int nCol, const LV_COLUMN* lvc, BOOL bUpdate )
{
	CString strHeader;
	int fmt = LVCFMT_LEFT;
	int cx = -1;
	int iSubItem = -1;
	int iImage = -1;

	if( lvc->mask & LVCF_TEXT )
		strHeader = lvc->pszText;
	if( lvc->mask & LVCF_FMT )
		fmt = lvc->fmt;
	if( lvc->mask & LVCF_WIDTH )
		cx = lvc->cx;
	if( lvc->mask & LVCF_SUBITEM )
		iSubItem = lvc->iSubItem;
	if( lvc->mask & LVCF_IMAGE )
		iImage = lvc->iImage;

	return InsertColumn( nCol, strHeader, fmt, cx, iSubItem, iImage, bUpdate );
}


//@doc SEC_LISTCLASS
//@mfunc Adds a column in the list view control.
//@syntax AddColumn( const CString& strHeader, int fmt, int widthCol, int iSubItem)
//@syntax AddColumn( const LV_COLUMN* lvc )
//@syntax AddColumn( Column* pCol )
//@rdesc Nonzero if successful; otherwise 0.
//@parm  const CString& | strHeader | Reference to a string containing the column’s header.
//@parm  int | fmt | Integer specifying the alignment of the column.  It can be one of these values: LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.  Default is LVCFMT_LEFT.
//@parm  int | widthCol | Width of the column, in pixels.  If this parameter is -1, the column width is not set.
//@parm  int | iSubItem | Index of the subitem associated with the column.  If this parameter is -1, no subitem is associated with the column.
//@parm  int | iImage   | Zero-based index of the column header image. When using header images the LVCFMT_BITMAP_ON_RIGHT can be specified to display the image to the right of text. 
//@parm  const LV_COLUMN* | lvc | Address of an LV_COLUMN structure that contains the attributes of the new column.
//@parm  Column* | pCol | Address of a Column structure that contains the attributes of the new column.
BOOL SEC_LISTCLASS::AddColumn( const CString& strHeader, int fmt, int widthCol, int iSubItem, int iImage)

//
// Add a column to the whole SEC_LISTCLASS
//
{
    return  InsertColumn( GetColumnCount(), strHeader, fmt, widthCol, iSubItem, iImage );
}


BOOL SEC_LISTCLASS::AddColumn( Column* pCol )

//
// Add a column to the whole SEC_LISTCLASS
//
{
    return  InsertColumn( GetColumnCount(), pCol );
}


BOOL SEC_LISTCLASS::AddColumn( const LV_COLUMN* lvc )
{
	return InsertColumn( GetColumnCount(), lvc );
}


//@doc SEC_LISTCLASS
//@mfunc Deletes a column.
//@syntax DeleteColumn( int nCol )
//@syntax DeleteColumn( const CString& strColumnHeading )
//@rdesc Nonzero if successful; otherwise 0.
//@parm  int | nCol | The index of the column to delete.
//@parm  CString& | strColumnHeading | Reference to a string containing column’s heading.
BOOL SEC_LISTCLASS::DeleteColumn( int nCol )
//
// Delete a column from the whole SEC_LISTCLASS
//
{
	ProcessAndDestroyEditLabel();
	m_pToolTipWnd->Hide();

	Column* pCol;
	Column* pColDelete;
	Item* pItem;
	int nIndex;
	int nDeleteIndex;

    // Get column to delete
	pColDelete = GetColumnAt(nCol);
	if ( !pColDelete )
		return FALSE;

	// Delete subitem for all existing items
	nDeleteIndex = pColDelete->iSubItem;
	for (nIndex=0; nIndex < GetItemCount(); nIndex++)
	{
		pItem = GetItemAt(nIndex);
		pItem->DeleteSubItem(nDeleteIndex);
	}

	// Reset the width of the item column so the
	// new column is initially visible
	if( (GetListCtrlStyleEx() & LVXS_NOGROWCOLUMNONDELETE) == 0 )
	{
		for(nIndex=0; nIndex<GetColumnCount(); nIndex++)
		{
			pCol = GetColumnAt(nIndex);
			if (pCol && ( pCol->iSubItem == 0 ) )
			{
				pCol->cx += pColDelete->cx;
				break;
			}
		}
	}

	// We just deleted a subitem, so decrement subitem count
	m_nSubItemCount--;

	// Delete the column
	delete pColDelete;
	m_arrColumns.RemoveAt(nCol);
	
	// All remaining columns may now be pointing to the
	// wrong subitem.  This is because deleting a subitem
	// moves those following it down by one index. So,
	// update all columns to  make sure their subitem
	// index is adjusted.
	for (nIndex = 0; nIndex < GetColumnCount(); nIndex++)
	{
		pCol = GetColumnAt(nIndex);
		if (pCol && (pCol->iSubItem > nDeleteIndex) )
			pCol->iSubItem--;
	}

	return TRUE;
}


BOOL
SEC_LISTCLASS::DeleteColumn( const CString& strColumnHeading )
//
// Delete a column from the whole SEC_LISTCLASS
//
{
	int nCol;
	int iSubItem;

	if (FindColumn(strColumnHeading, nCol, iSubItem))
		return DeleteColumn(nCol);
	else
		return FALSE;
}


//@doc SEC_LISTCLASS
//@mfunc Deletes all columns.
//@rdesc Nonzero if successful; otherwise 0.
BOOL SEC_LISTCLASS::DeleteAllColumns()
{
	Column* pCol;
	int nCol;

    // Delete column info
    for (nCol = GetColumnCount()-1; nCol >= 0; nCol--)
	{
		// Get column to delete
		pCol = GetColumnAt(nCol);
		// Delete the column
		if ( pCol )
			delete pCol;
	}

	m_arrColumns.RemoveAll();
	RecalcScrollBars();

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Retrieves information about a column.
//@rdesc Nonzero if successful; otherwise 0.
//@parm  int | nCol | The index of the column.
//@parm  LV_COLUMN* | lvc | A pointer to an LV_COLUMN structure to receive the column information.
BOOL SEC_LISTCLASS::GetColumn( int nCol, LV_COLUMN* lvc )
{
	Column* col = NULL;

	col = GetColumnAt(nCol);
	if( !col )
		return FALSE;

	if( lvc->mask & LVCF_TEXT )
		_tcsncpy( lvc->pszText, col->strText, lvc->cchTextMax );
	if( lvc->mask & LVCF_FMT )
		lvc->fmt = col->fmt;
	if( lvc->mask & LVCF_SUBITEM )
		lvc->iSubItem = col->iSubItem;
	if( lvc->mask & LVCF_WIDTH )
		lvc->cx = col->cx;
	if( lvc->mask & LVCF_IMAGE )
		lvc->iImage = col->iImage;

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the properties of a column.
//@rdesc Nonzero if successful; otherwise 0.
//@parm  int | nCol | The index of the column.
//@parm  const LV_COLUMN* | lvc | A pointer to an LV_COLUMN structure that contains the column settings.
//@parm  BOOL | bUpdate | If TRUE, the control will be invalidated.
BOOL SEC_LISTCLASS::SetColumn( int nCol, const LV_COLUMN* lvc, BOOL bUpdate )
{
	Column* col = NULL;
	
	col = GetColumnAt(nCol);
	if( !col )
		return FALSE;

	if( lvc->mask & LVCF_TEXT )
		col->strText = lvc->pszText;
	if( lvc->mask & LVCF_FMT )
		col->fmt = lvc->fmt;
	if( lvc->mask & LVCF_SUBITEM )
		col->iSubItem = lvc->iSubItem;
	if( lvc->mask & LVCF_WIDTH )
		col->cx = lvc->cx;

	if( bUpdate )
	{
		ReMeasureAllItems();
		if( this -> GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE )
			ResizeColumnsToFit();
		Invalidate();
	}

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the width of a column in pixels.
//@rdesc void 
//@parm  int | nCol | The index of the column.
//@parm  int | width | Width in pixels.
void SEC_LISTCLASS::SetColumnWidth( int nCol, int width )
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
	{
		pCol->cx = width;
		if(IsWordWrapEnabled())
			ClearAllCy();
	}
}


//@doc SEC_LISTCLASS
//@mfunc Returns the width of a column in pixels.
//@rdesc Column width in pixels.
//@parm  int | nCol | The index of the column.
int SEC_LISTCLASS::GetColumnWidth( int nCol ) const
{
	if (GetColumnCount() >= 1)
	{
		Column* pCol = GetColumnAt(nCol);
		if ( pCol )
			return pCol->cx;
		return 0;
	}
	else
	{
		CRect rcInside;
		GetInsideRect(rcInside);
		return rcInside.Width();
	}
}


//@doc SEC_LISTCLASS
//@mfunc Sets the alignment of a column.
//@rdesc void 
//@parm  int | nCol | The index of the column.
//@parm  int | fmt | Integer specifying the alignment of the column.  It can be one of these values: LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
void SEC_LISTCLASS::SetColumnFormat( int nCol, int fmt )
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
		pCol->fmt = fmt;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the alignment of a column.
//@rdesc Integer specifying the alignment of the column.  It can be one of these values: LVCFMT_LEFT, LVCFMT_RIGHT, or LVCFMT_CENTER.
//@parm  int | nCol | The column index.
int SEC_LISTCLASS::GetColumnFormat( int nCol ) const
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
		return pCol->fmt;
	return 0;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the text of a column header.
//@rdesc void 
//@parm  int | nCol | The index of the column.
//@parm  const CString& | strHeading | A reference to the new column heading text.
void SEC_LISTCLASS::SetColumnHeading( int nCol, const CString& strHeading )
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
		pCol->strText = strHeading;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the text of a column header.
//@rdesc The column heading text.
//@parm  int | nCol | The index of the column.
CString SEC_LISTCLASS::GetColumnHeading( int nCol ) const
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
		return pCol->strText;
	return _T("");
}

//@doc SEC_LISTCLASS
//@mfunc Sets the image for the column header.
//@rdesc void 
//@parm  int | nCol | The index of the column.
//@parm  int | nImage | The zero-based index of the image in the control's header image list.
void SEC_LISTCLASS::SetColumnImage( int nCol, int nImage )
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
		pCol->iImage = nImage;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the index of the column image.
//@rdesc The zero-based index of the image in the control's header image list.
//@parm  int | nCol | The index of the column.
int SEC_LISTCLASS::GetColumnImage( int nCol ) const
{
	Column* pCol = GetColumnAt(nCol);
	if ( pCol )
		return pCol->iImage;
	return -1;
}



int SEC_LISTCLASS::GetColumnLeft(int nCol) const
//
// Get x at left-hand Column edge
//
{
    int width=-m_ptViewportOrg.x;

	ASSERT(nCol <= GetColumnCount());

    for( int w=0; w<nCol ; w++ )
        width += GetColumnWidth(w);

    return width;
}

//
// Get x at right-hand Column edge
//
int SEC_LISTCLASS::GetColumnRight(int nCol) const
{
    return GetColumnLeft(nCol) + GetColumnWidth(nCol);
}


//@doc SEC_LISTCLASS
//@mfunc Returns whether a column with the given text exists or not.
//@rdesc TRUE if the specified column exists.
//@parm  const CString | &strColTitle | The column heading text to search for.
BOOL SEC_LISTCLASS::ColumnExists( const CString &strColTitle ) const
{
	int nCol;
	int iSubItem;

    return FindColumn(strColTitle, nCol, iSubItem);
}


//@doc SEC_LISTCLASS
//@mfunc Searches for a column by string title.
//@rdesc TRUE if the specified column exists.
//@parm const CString | &strColumnHeading | The column heading text to search for.
//@parm  int& | nCol | The returned column index.
//@parm  int& | iSubItem | The returned column associated subitem.
BOOL SEC_LISTCLASS::FindColumn(const CString &strColumnHeading, int& nCol, int& iSubItem) const
{
	Column* pCol;
	int i;

	for (i=0; i<GetColumnCount(); i++)
	{
		pCol = GetColumnAt(i);
		ASSERT_POINTER(pCol, Column);
		if (strColumnHeading == pCol->strText)
		{
			nCol = i;
			iSubItem = pCol->iSubItem;
			return TRUE;
		}
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Feature and Viewing Mode Enable/Disable Members
//


//@doc SEC_LISTCLASS
//@mfunc Enables/disables wrapping of item text
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable word wrap.
void SEC_LISTCLASS::EnableWordWrap(BOOL bEnable)
{
	ModifyListCtrlStyle(bEnable*LVS_NOLABELWRAP, !bEnable*LVS_NOLABELWRAP);
}


//@doc SEC_LISTCLASS
//@mfunc Returns whether word wrap is enabled or not.
//@rdesc TRUE if word wrap is enabled.
BOOL SEC_LISTCLASS::IsWordWrapEnabled() const
{
	return ((this -> GetListCtrlStyle()&LVS_NOLABELWRAP) == 0);
}


//@doc SEC_LISTCLASS
//@mfunc Enables/disables tooltips.
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable tooltips.
void SEC_LISTCLASS::EnableToolTips(BOOL bEnable)
{
	ModifyListCtrlStyleEx(!bEnable*LVXS_FLYBYTOOLTIPS, bEnable*LVXS_FLYBYTOOLTIPS, FALSE);
}


//@doc SEC_LISTCLASS
//@mfunc Returns whether tooltips is enabled or not.
//@rdesc TRUE if tooltips are enabled.
BOOL SEC_LISTCLASS::AreToolTipsEnabled() const
{
	return ((this -> GetListCtrlStyleEx()&LVXS_FLYBYTOOLTIPS) != 0);
}


//@doc SEC_LISTCLASS
//@mfunc Enables/disables the column header control.
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable the column header control.
void SEC_LISTCLASS::EnableHeaderCtrl(BOOL bEnable, BOOL bSortHeader)
{
	//Sort header can be set only if header is enabled
	ASSERT( bSortHeader? (bEnable? TRUE:FALSE):TRUE );

	ModifyListCtrlStyle( (bEnable*LVS_NOCOLUMNHEADER)|(bSortHeader*LVS_NOSORTHEADER),
		(!bEnable*LVS_NOCOLUMNHEADER)|(!bEnable*LVS_NOSORTHEADER) );	
}


//@doc SEC_LISTCLASS
//@mfunc Returns whether the column header is enabled or not.
//@rdesc TRUE if the column header is enabled.
BOOL SEC_LISTCLASS::IsHeaderCtrlEnabled() const
{
	return ( (this -> GetListCtrlStyle() & LVS_NOCOLUMNHEADER) == 0 );
}


//@doc SEC_LISTCLASS
//@mfunc Enables/disables multiple selection.
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable multiple selection.
void SEC_LISTCLASS::EnableMultiSelect(BOOL bEnable)
{
	if ( !bEnable && ( GetSelectedCount() > 1 ) ) 
		DeselectAllItems();

	ModifyListCtrlStyle(bEnable*LVS_SINGLESEL, !bEnable*LVS_SINGLESEL);
}


//@doc SEC_LISTCLASS
//@mfunc Returns whether multiple selection is enabled or not.
//@rdesc TRUE if multiple selection is enabled.
BOOL SEC_LISTCLASS::IsMultiSelectEnabled() const
{
	return ((this -> GetListCtrlStyle()&LVS_SINGLESEL) == 0);
}


//@doc SEC_LISTCLASS
//@mfunc Enables/disables redraw after insert.
//@rdesc void 
//@parm BOOL | bEnable | TRUE to enable redraw after insert.
//@comm  By default, the control is not invalidated after every insert/delete. This allows 
// for efficient addition and removal of multiple items, followed by ReMeasureAllItems() and
// Invalidate() calls. If EnableRedrawAfterInsert( TRUE ) is called, the control will
// remeasure and invalidate after each insert/delete.
void SEC_LISTCLASS::EnableRedrawAfterInsert( BOOL bEnable )
{
	m_bRedrawAfterEveryInsert = bEnable;
}


//@doc SEC_LISTCLASS
//@mfunc Returns whether redraw after insert is enabled or not.
//@rdesc TRUE if redraw after insert is enabled.
BOOL SEC_LISTCLASS::IsRedrawAfterInsertEnabled() const
{
	return m_bRedrawAfterEveryInsert;
}
/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Item/SubItem Management & Accessors
//


int
SEC_LISTCLASS::GetSubItemCount() const
{
	return m_nSubItemCount;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the number of selected items.
//@rdesc The number of selected items.
UINT SEC_LISTCLASS::GetSelectedCount( ) const
{
	return (UINT)m_arrSelItems.GetSize();
}


int
SEC_LISTCLASS::GetNextItem(int nItem, int nFlags) const
{
	int nNextItem = -1;
	UINT uState = 0;

	ASSERT(nItem >= -1 && nItem < GetItemCount());

	if (nFlags & LVNI_SELECTED)
		uState |= LVIS_SELECTED;
	if (nFlags & LVNI_FOCUSED)
		uState |= LVIS_FOCUSED;
	if (nFlags & LVNI_DROPHILITED)
		uState |= LVIS_DROPHILITED;
	if (nFlags & LVNI_CUT)
		uState |= LVIS_CUT;

	nFlags &= ~(LVNI_SELECTED|LVNI_FOCUSED|LVNI_DROPHILITED|LVNI_CUT);

	// this does not work, because LVNI_ALL is defined as 0
	//if (nFlags == LVNI_ALL)	
	if ( uState == 0 )
	{
		if (nFlags&LVNI_TOLEFT || nFlags&LVNI_ABOVE)
		{
			if (nItem > 0)
				nNextItem = nItem - 1;
		}
		else if ((nFlags == LVNI_ALL) || nFlags&LVNI_TORIGHT || nFlags&LVNI_BELOW)
		{
			if (nItem < (GetItemCount()-1))
				nNextItem = nItem + 1;
		}
	}
	else
	{
		if (nFlags&LVNI_TOLEFT || nFlags&LVNI_ABOVE)
		{
			if (nItem > 0)
			{
				int i;
				for (i = nItem - 1; i >= 0; i--)
					if (GetItemState(i, uState))
					{
						nNextItem = i;
						break;
					}
			}
		}
		else if ((nFlags == LVNI_ALL) || nFlags&LVNI_TORIGHT || nFlags&LVNI_BELOW)
		{
			if (nItem < (GetItemCount()-1))
			{
				int i;
				for (i = nItem + 1; i < GetItemCount(); i++)
					if (GetItemState(i, uState))
					{
						nNextItem = i;
						break;
					}
			}
		}
		else
			ASSERT(FALSE); // Unrecognized flag passed
	}

	return nNextItem;
}


BOOL SEC_LISTCLASS::SetCallbackMask(UINT nMask)
{
	m_uiCallbackMask = nMask;
	return TRUE;
}


BOOL SEC_LISTCLASS::SetCallbackMaskEx(UINT nMaskEx)
{
	m_uiCallbackMaskEx = nMaskEx;
	return TRUE;
}


UINT SEC_LISTCLASS::GetCallbackMask() const
{
	return m_uiCallbackMask;
}


UINT SEC_LISTCLASS::GetCallbackMaskEx() const
{
	return m_uiCallbackMaskEx;
}


BOOL SEC_LISTCLASS::SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam)
{
	LV_ITEM item;

	item.mask = nMask;
	item.iItem = nItem;
	item.iSubItem = nSubItem;
	item.state = nState;
	item.pszText = (LPTSTR)lpszItem;
	item.stateMask = nStateMask;
	item.lParam = lParam;
	item.iImage = nImage;

	return SetItem(&item);
}


BOOL SEC_LISTCLASS::AddToSelItemsArray(int iItem)
{
	int nIndex;

	nIndex = (int)m_arrSelItems.GetSize();
	if (nIndex == 0 || iItem > (int)m_arrSelItems.GetAt(nIndex-1))
	{
		m_arrSelItems.Add(iItem);
		return TRUE;
	}
	else if (!FindInSelItemsArray(iItem, nIndex))
	{
		m_arrSelItems.InsertAt(nIndex, iItem);
		return TRUE;
	}

	return FALSE;
}


BOOL SEC_LISTCLASS::RemoveFromSelItemsArray(int iItem)
{
	int nIndex;
	if (FindInSelItemsArray(iItem, nIndex))
	{
		m_arrSelItems.RemoveAt(nIndex);
		return TRUE;
	}
	else
		return FALSE;
}


BOOL SEC_LISTCLASS::FindInSelItemsArray(int iItem, int& nIndex) const
{
	int lb, ub;

	lb = 0;
	nIndex = ub = (int)m_arrSelItems.GetSize();

	if (ub > 0)
	{
		int val;
		ub--;
		if ( (ub-lb) > 5 )
		{
			do
			{
				nIndex = lb + (ub - lb)/2;
				val = m_arrSelItems.GetAt(nIndex);
				if (val < iItem)
					lb = nIndex;
				else if (val > iItem)
					ub = nIndex;
			} while ( (ub-lb) > 1 && iItem != val );
		}

		nIndex = lb;
		do
		{
			val = m_arrSelItems.GetAt(nIndex);
		} while ( val < iItem && ++nIndex <= ub );

		return (val == iItem);
	}
	else
		return FALSE;
}


BOOL SEC_LISTCLASS::SetItem
	(
		const LV_ITEM* lvi,
		BOOL bRedraw // = TRUE
	)
{
	Item* pItem = 0;
	UINT cbMask;
	BOOL bInvalid, bReMeasure, bUpdateNow;
	
	pItem = GetItemAt(lvi->iItem);
	// this assert is because the item is not in the list control.
	ASSERT( pItem );
	if ( !pItem )
		return FALSE;

	cbMask = 0;
	bInvalid = FALSE;
	bReMeasure = FALSE;
	bUpdateNow = FALSE;

	if (lvi->mask & LVIF_TEXT)
	{
		bReMeasure = TRUE;
		if (lvi->pszText == LPSTR_TEXTCALLBACK)
		{
			pItem->pszText = lvi->pszText;
			cbMask |= LVIF_TEXT;
		}
		else if (lvi->iSubItem == 0)
		{
			if (pItem->pszText == LPSTR_TEXTCALLBACK && (lvi->mask&LVIF_DI_SETITEM) == 0)
				cbMask |= LVIF_TEXT;
			else
			{
				if (pItem->pszText != NULL && pItem->pszText != LPSTR_TEXTCALLBACK)
					delete[] pItem->pszText;
				pItem->pszText = new TCHAR[_tcslen(lvi->pszText)+1];
				_tcscpy(pItem->pszText, lvi->pszText);
			}
		}
		else
		{
			SubItem* pSubItem = pItem->GetSubItem(lvi->iSubItem);
			if (pSubItem->pszText == LPSTR_TEXTCALLBACK && (lvi->mask&LVIF_DI_SETITEM) == 0)
				cbMask |= LVIF_TEXT;
			else
			{
				if (pSubItem->pszText != NULL && pSubItem->pszText != LPSTR_TEXTCALLBACK)
					delete[] pSubItem->pszText;
				pSubItem->pszText = new TCHAR[_tcslen(lvi->pszText)+1];
				_tcscpy(pSubItem->pszText, lvi->pszText);
			}
		}
	}

	if (lvi->mask & LVIF_IMAGE)
	{
		bInvalid = TRUE;
		if (pItem->iImage == I_IMAGECALLBACK && (lvi->mask&LVIF_DI_SETITEM) == 0)
			cbMask |= LVIF_IMAGE;
		else
		{
			pItem->iImage = lvi->iImage;
		}
	}

	if (lvi->mask & LVIF_STATE)
	{
		UINT stateMask = lvi->stateMask;
		if (stateMask & LVIS_SELECTED)
		{
			if (lvi->state & LVIS_SELECTED)
				bInvalid |= AddToSelItemsArray(lvi->iItem);
			else
				bInvalid |= RemoveFromSelItemsArray(lvi->iItem);
			stateMask &= ~LVIS_SELECTED;
		}
		if (stateMask & LVIS_FOCUSED)
		{
			int nFocusBefore = m_nFocusIndex;
			if (lvi->state & LVIS_FOCUSED)
				m_nFocusIndex = lvi->iItem;
			else if (m_nFocusIndex == lvi->iItem)
				m_nFocusIndex = LVXC_UNINITIALIZED;
			stateMask &= ~LVIS_FOCUSED;
			if (nFocusBefore != m_nFocusIndex)
				bInvalid = TRUE;
		}
		if (stateMask & LVIS_DROPHILITED)
		{
			int nDropBefore = m_nDropHilite;
			if (lvi->state & LVIS_DROPHILITED)	// highlight the item
				m_nDropHilite = lvi->iItem;
			else if (m_nDropHilite == lvi->iItem) // unhighlight the item
				m_nDropHilite = LVXC_UNINITIALIZED;

			stateMask &= ~LVIS_DROPHILITED;
			if (nDropBefore != m_nDropHilite)
			{
				bInvalid = TRUE;
				bUpdateNow = TRUE;
			}


		}
		UINT prevState = pItem->state;
		pItem->state &= ~(stateMask);
		pItem->state |= (lvi->state & stateMask);
		if (pItem->state != prevState)
			bInvalid = TRUE;
	}

	if (lvi->mask & LVIF_PARAM)
		pItem->lParam = lvi->lParam;

	if (cbMask != 0)
	{
		LV_ITEM cblvi;
		memmove(&cblvi, lvi, sizeof(LV_ITEM));
		cblvi.mask = cbMask;
		NotifySetDispInfo(&cblvi);
	}

	if (lvi->mask & (LVIF_TEXT|LVIF_STATE))
	{
		if (bReMeasure)
			//bk99 MeasureItem( lvi->iItem );	// causes recursion on subitems with callbacks DI_
			pItem->m_bDirty = TRUE;


		if( bRedraw )
		{
			if (bInvalid && lvi->iItem != m_nEditItem)
				Update(lvi->iItem, FALSE, FALSE, FALSE, bUpdateNow);
		}
	}

	return TRUE;
}


BOOL SEC_LISTCLASS::SetItemEx(LV_ITEM* lvi, const LV_ITEM_EX* lvix)
{
	Item* pItem;
	UINT cbMask;

	pItem = GetItemAt(lvi->iItem);
	cbMask = 0;

	if (lvix->maskEx & LVIF_EX_STATEEX)
	{
		UINT stateBefore = pItem->stateEx;
		pItem->stateEx &= ~(lvix->stateExMask);
		pItem->stateEx |= (lvix->stateEx & lvix->stateExMask);
		if (stateBefore != pItem->stateEx)
			Update(lvi->iItem, (GetListCtrlStyleEx()&LVXS_HILIGHTSUBITEMS) ? FALSE : TRUE, FALSE, FALSE);
	}

	if (cbMask != 0)
	{
		LV_ITEM_EX cblvix;
		memmove(&cblvix, lvix, sizeof(LV_ITEM_EX));
		cblvix.maskEx = cbMask;
		NotifySetDispInfoEx(&cblvix);
	}

	if (lvix->maskEx & (LVIF_EX_STATEEX))
	{
		// The text or state of a visible node has been changed,
		// so we must remeasure and update the item.
		if (lvix->maskEx & LVIF_EX_STATEEX)
			MeasureItem(lvi->iItem);
		if (lvi->iItem != m_nEditItem)
			Update(lvi->iItem, TRUE, FALSE);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS::GetItem
//
//	This function mirrors the CListCtrl::GetItem function but adds
//  two additional paramaters:
//
//	  bCopyText		If TRUE (default), a new copy of the item text
//					is created.  If FALSE, the tvi.pszText pointer
//					is set equal to the item text buffer.  The latter
//					avoids a strcpy and the need to preallocate
//					a buffer.  However, you must be careful not
//					to use the pointer after the item is removed
//					from the list control since the buffer will be
//					freed at that point.
//
//	  bGetDispInfo	If FALSE (default), the TVN_GETDISPINFO message
//					is not sent for callback items.  Therefore,
//					LPSTR_TEXTCALLBACK will be returned in tvi.pszText
//					rather than the text stored on the application
//					side.  If TRUE, the callback is called if the
//					item text is maintained by the application,
//					and the value returned in tvi.pszText will be the
//					actual item text reported by the application.
//

BOOL SEC_LISTCLASS::GetItem(LV_ITEM* lvi, BOOL bCopyText, BOOL bGetDispInfo) const
{
	Item* pItem = 0;
	UINT cbMask;

	pItem = GetItemAt(lvi->iItem);
	// item is not present in the list control
	if ( !pItem )
		return FALSE;

	cbMask = 0;

	if (lvi->mask & LVIF_TEXT)
	{
		if (lvi->iSubItem == 0)
		{
			if (pItem->pszText == LPSTR_TEXTCALLBACK)
			{
				cbMask |= LVIF_TEXT;
				if (!bGetDispInfo)
					lvi->pszText = pItem->pszText;
			}
			else if (bCopyText)
				_tcsncpy(lvi->pszText, pItem->pszText, lvi->cchTextMax);
			else
				lvi->pszText = pItem->pszText;
		}
		else
		{
			SubItem* pSubItem = pItem->GetSubItem(lvi->iSubItem);
			if (pSubItem->pszText == LPSTR_TEXTCALLBACK)
			{
				cbMask |= TVIF_TEXT;
				if (!bGetDispInfo)
					lvi->pszText = pSubItem->pszText;
			}
			else if (bCopyText)
				_tcsncpy(lvi->pszText, pSubItem->pszText, lvi->cchTextMax);
			else
				lvi->pszText = pSubItem->pszText;
		}
	}

	if (lvi->mask & LVIF_IMAGE)
	{
		lvi->iImage = pItem->iImage;
		if (lvi->iImage == I_IMAGECALLBACK)
			cbMask |= LVIF_IMAGE;
	}

	if (lvi->mask & LVIF_STATE)
	{
		if (GetCallbackMask() != 0)
			cbMask |= LVIF_STATE;
		else
		{
			int nIndex;
			lvi->state = (pItem->state & lvi->stateMask);
			if (lvi->stateMask & LVIS_SELECTED && FindInSelItemsArray(lvi->iItem, nIndex))
				lvi->state |= LVIS_SELECTED;
			if (lvi->stateMask & LVIS_FOCUSED && lvi->iItem == m_nFocusIndex)
				lvi->state |= LVIS_FOCUSED;
			if (lvi->stateMask & LVIS_DROPHILITED && lvi->iItem == m_nDropHilite)
				lvi->state |= LVIS_DROPHILITED;
		}
	}
	
	if ( (lvi->mask & LVIF_PARAM) || ( cbMask != 0))
		lvi->lParam = GetUserParam( pItem );

	if (bGetDispInfo && cbMask != 0)
	{
		lvi->mask = cbMask;
		NotifyGetDispInfo(lvi);
	}

	return TRUE;
}


BOOL SEC_LISTCLASS::GetItemEx(LV_ITEM* lvi, LV_ITEM_EX* lvix, BOOL bGetDispInfoEx) const
{
	Item* pItem;
	UINT cbMask;

	pItem = GetItemAt(lvi->iItem);
	cbMask = 0;

	// item is not present in the list control
	if(!pItem)                                             
		return FALSE; 

	if (lvix->maskEx & LVIF_EX_STATEEX)
	{
		if (GetCallbackMaskEx() != 0)
			cbMask |= LVIF_EX_STATEEX;
		else
			lvix->stateEx = (pItem->stateEx & lvix->stateExMask);
	}

	if (bGetDispInfoEx && cbMask != 0)
	{
		lvix->maskEx = cbMask;
		NotifyGetDispInfoEx(lvix);
	}

	return TRUE;
}


BOOL SEC_LISTCLASS::SetItemText(int iItem, int iSubItem, LPCTSTR pszText)
{
	LV_ITEM lvi;

	lvi.iItem = iItem;
	lvi.iSubItem = iSubItem;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = (LPTSTR)pszText;
	if (pszText != LPSTR_TEXTCALLBACK)
		lvi.cchTextMax = (int)_tcslen(pszText);
	
	return SetItem(&lvi);
}


int SEC_LISTCLASS::GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const
{
	LV_ITEM lvi;

	lvi.iItem = nItem;
	lvi.iSubItem = nSubItem;
	lvi.mask = LVIF_TEXT;
	lvi.stateMask = 0;
	lvi.pszText = lpszText;
	lvi.cchTextMax = nLen;
	
	if (GetItem(&lvi, TRUE, TRUE))
		return (int)_tcslen(lvi.pszText);
	else
		return 0;
}


CString SEC_LISTCLASS::GetItemText(int nItem, int nSubItem) const
{
	CString str;

	if (GetItemText(nItem, nSubItem, m_pszInternalBuffer, LVXC_INTERNALBUFFERSIZE))
		str = m_pszInternalBuffer;

	return str;
}


BOOL SEC_LISTCLASS::SetItemState(int nItem, UINT nState, UINT nMask)
{
	LV_ITEM lvi;

	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_STATE;
	lvi.state = nState;
	lvi.stateMask = nMask;
	
	return SetItem(&lvi);
}


BOOL SEC_LISTCLASS::SetItemData(int nItem, DWORD dwData)
{
	LV_ITEM lvi;

	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;
	lvi.lParam = dwData;
	
	return SetItem(&lvi);
}


DWORD SEC_LISTCLASS::GetItemData(int nItem) const
{
	LV_ITEM lvi;

	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;
	
	if (GetItem(&lvi))
		return lvi.lParam;
	else
		return 0;
}


int SEC_LISTCLASS::GetFocusIndex() const
{
	return m_nFocusIndex;
}


BOOL SEC_LISTCLASS::DeleteItem( Item* pItemToDelete )
{
	int count = GetItemCount();

	for( int i = 0; i < count; i++ )
	{
		if( GetItemAt(i) == pItemToDelete )
			break;
	}

	return DeleteItem(i);
}


BOOL SEC_LISTCLASS::DeleteItem( int nItem, int cCount )
{
	Item* pItem;
	int nIndex;
	int val;
	int nLast, i;

	pItem = GetItemAt(nItem);                   
	
	// item is not present in the list control
	if(!pItem)                                             
		return FALSE;   
	
	ASSERT(nItem >= 0 && nItem < GetItemCount());
	Update(nItem > 0 ? nItem-1 : 0, FALSE, TRUE, TRUE);
	NotifyDeleteItem(nItem);

	nLast = nItem + cCount - 1;
	for (i=nItem; i<=nLast; i++)
	{
		pItem = GetItemAt(i);
		delete pItem;
	}

	m_arrItems.RemoveAt(nItem, cCount);

	// Item count has changed, make sure top index
	// does not exceed allowable range
	m_nTopIndex=otmin(m_nTopIndex,GetItemCount());

	// The selection, focus and drop hilite indexes may
	// be wrong after the delete.  For example, if the
	// focus item was just deleted, we have to reset
	// m_nFocusIndex accordingly.  Also, if items
	// preceeding the focus index where deleted, the
	// focus index must be adjusted to account for the
	// fewer items.  The same logic applies to selected
	// items and the drop hilite index.
	if (m_nFocusIndex > nItem || m_nFocusIndex >= GetItemCount())
	{
		if (m_nFocusIndex > nLast)
			m_nFocusIndex -= cCount;
		else
			m_nFocusIndex = nItem-1;
	}
	if (m_nDropHilite > nItem)
	{
		if (m_nDropHilite > nLast)
			m_nDropHilite -= cCount;
		else
			m_nDropHilite = nItem-1;
	}
	FindInSelItemsArray(nItem, nIndex);
	for (i=(int)m_arrSelItems.GetUpperBound(); i>=nIndex; i--)
	{
		val = m_arrSelItems.GetAt(i);
		if (val >= nItem && val <= nLast)
			// Item was just deleted so remove its
			// selection flag.
			m_arrSelItems.RemoveAt(i);
		else
		{
			val -= cCount;
			m_arrSelItems.SetAt(i, val);
		}
	}

	if ( m_arrSelItems.GetSize() == 0 )
		SelectFocusItem( GetFocusIndex() );

	RecalcScrollBars();
	return TRUE;
}


BOOL SEC_LISTCLASS::DeleteAllItems()
{
	Item* pItem;
	int nIndex;

	// If all items are deleted, we can't be
	// editing anything.
	VERIFY(KillEditControl());

   // delete Item data
    for (nIndex=0; nIndex < GetItemCount(); nIndex++)
    {
		pItem = GetItemAt(nIndex);
		delete pItem;
    }
	m_arrItems.RemoveAll();
	m_arrSelItems.RemoveAll();

	// Now that we've deleted all items, our top index
	// cannot be greater than 0
	m_nTopIndex = 0;
	m_nFocusIndex = -1;

	RecalcScrollBars();
	Invalidate();

	return TRUE;
}


BOOL SEC_LISTCLASS::SelectItem(int nIndex, BOOL bSelect)
{
	if (( GetListCtrlStyle() & LVS_SINGLESEL ))
		DeselectAllItems(nIndex);

	UINT nState(0), nStateMask(LVIS_SELECTED);

	if( bSelect )
		nState = nStateMask;
	else
		nState = 0;
	
	return SetItemState(nIndex, nState, nStateMask);
}


BOOL SEC_LISTCLASS::SelectItemRange(int nFirst, int nLast, BOOL bSelect)
{
	int i;

	if (nFirst > nLast)
	{
		int nSwap;
		nSwap = nFirst;
		nFirst = nLast;
		nLast = nSwap;
	}

	for (i=nFirst; i<=nLast; i++)
		SelectItem(i, bSelect);

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Clears the selection array.
//@rdesc Nonzero if successful; otherwise 0.
//@parm int | iExclude | The list index of an item to exclude from deselection, i.e. leave that one item selected.
BOOL SEC_LISTCLASS::DeselectAllItems(int iExclude)
{
	int iItem;
	int i;

	for (i=(int)m_arrSelItems.GetUpperBound(); i>=0; i--)
	{
		iItem = m_arrSelItems.GetAt(i);
		if (iItem != iExclude)
			SetItemState(iItem, 0, LVIS_SELECTED);
	}

	return TRUE;
}

// sets the focus, and sets or clears the selection based on bSelected

BOOL SEC_LISTCLASS::SelectFocusItem(int nIndex, BOOL bSelected)
{
	int nCurFocus = GetFocusIndex();

	if (GetListCtrlStyle() & LVS_SINGLESEL)
		DeselectAllItems(nIndex);

	if (nCurFocus != -1 && nCurFocus != nIndex)
		SetItemState(nCurFocus, 0, LVIS_FOCUSED);

	if ( nIndex >= 0 )
		SetItemState(nIndex, LVIS_FOCUSED|(bSelected*LVIS_SELECTED), LVIS_FOCUSED|LVIS_SELECTED);

	m_nAnchor = GetFocusIndex();

	return TRUE;
}

// sets the focus, and leaves the selection unchanged

BOOL SEC_LISTCLASS::SetFocusItem(int nIndex)
{
	int nCurFocus = GetFocusIndex();

	if (nCurFocus != -1 && nCurFocus != nIndex)
		SetItemState(nCurFocus, 0, LVIS_FOCUSED);

	SetItemState(nIndex, LVIS_FOCUSED, LVIS_FOCUSED);

	m_nAnchor = GetFocusIndex();

	return TRUE;
}



int SEC_LISTCLASS::GetItemCount() const
{
	return (int)m_arrItems.GetSize();
}


UINT SEC_LISTCLASS::GetItemState(int nItem, UINT nMask) const
{
	LV_ITEM lvi;

	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_STATE;
	lvi.stateMask = nMask;
	
	if (GetItem(&lvi))
		return lvi.state;
	else
		return 0;
}


BOOL SEC_LISTCLASS::IsSelected(int nIndex) const
{
	if ( nIndex == -1 )
		return FALSE;
	return GetItemState(nIndex, LVIS_SELECTED) > 0;
}


BOOL SEC_LISTCLASS::IsCallbackItem(int nIndex) const
{
	Item* pItem;
	BOOL bCbItem;

	pItem = GetItemAt(nIndex);
	bCbItem = FALSE;

	if (pItem->pszText == LPSTR_TEXTCALLBACK)
		bCbItem = TRUE;
	else if (pItem->iImage == I_IMAGECALLBACK)
		bCbItem = TRUE;
	else if (GetCallbackMask() != 0)
		bCbItem = TRUE;
	else if (GetCallbackMaskEx() != 0)
		bCbItem = TRUE;
	else
	{
		SubItem* pSubItem;
		int nSubItem;
		Column* pCol;
		int iCol;

		for(iCol=0; iCol<GetColumnCount(); iCol++)
		{
			pCol = GetColumnAt(iCol);
			if ( pCol )
			{
				nSubItem = pCol->iSubItem;
				if (nSubItem != 0)
				{
					pSubItem = pItem->GetSubItem(pCol->iSubItem);
					if (pSubItem->pszText == LPSTR_TEXTCALLBACK)
						bCbItem = TRUE;
				}
			}
		}
	}

	return bCbItem;
}


int SEC_LISTCLASS::InsertItem
	(
		int nItem, 
		LPCTSTR lpszItem
	)
{
	return InsertItem( LVIF_TEXT, nItem, lpszItem, 0, 0, 0, 0 );
}


int SEC_LISTCLASS::InsertItem
	(
		int nItem, 
		LPCTSTR lpszItem, 
		int nImage
	)
{
	return InsertItem( LVIF_TEXT|LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0 );
}


int SEC_LISTCLASS::InsertItem
	(
		UINT nMask, 
		int nItem, 
		LPCTSTR lpszItem, 
		UINT nState, 
		UINT nStateMask, 
		int nImage, 
		LPARAM lParam
	)
{
	LV_ITEM item;
	::ZeroMemory(&item,sizeof(item));

	item.mask = nMask;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)lpszItem;
	item.state = nState;
	item.stateMask = nStateMask;
	item.iImage = nImage;
	item.lParam = lParam;

	return InsertItem( &item );
}


int SEC_LISTCLASS::InsertItem
	( 
		const LV_ITEM* pItem
	)
{
	int nItem = pItem->iItem;
	//Item* pNewItem = new Item;
	Item* pNewItem = CreateNewItem();


	InsertItem( nItem, pNewItem );
	
	VERIFY(SetItem( pItem, IsRedrawAfterInsertEnabled() ));

	if ( IsRedrawAfterInsertEnabled() )
		RecalcScrollBars();

	return nItem;
}


int SEC_LISTCLASS::InsertItem(int nItem, const Item* pItem)
{
	int nIndex;
	int colCount;
	DWORD val;

	m_arrItems.InsertAt(nItem, (void*)pItem);

	if (m_nFocusIndex >= nItem)
		m_nFocusIndex++;
	if (m_nDropHilite >= nItem)
		m_nDropHilite++;
	FindInSelItemsArray(nItem, nIndex);
	for (;nIndex < m_arrSelItems.GetSize(); nIndex++)
	{
		val = m_arrSelItems.GetAt(nIndex);
		val++;
		m_arrSelItems.SetAt(nIndex, val);
	}

	// Make sure we have a focus item.  We should always
	// have a focus item.  We have to guarantee this before
	// the first painting and this seems like the best place
	// to do it.
	if (GetFocusIndex() == -1)
		SelectFocusItem(0);

	// Make sure we have enough sub-items for columns...
	if( pItem->GetSubItemCount() < GetColumnCount() - 1 )
	{
		for( colCount = 1; colCount < GetColumnCount(); colCount++ )
		{
			VERIFY(((Item*)pItem)->AddSubItem() == colCount - 1);
		}
	}
	
	return nItem;
}


int SEC_LISTCLASS::InsertItemAfter( Item* pInsertAfter, LV_ITEM* pLVI )
{
	//Special Case...if pInsertAfter is NULL...insert at item 0
	if( pInsertAfter == NULL )
	{
		pLVI->iItem = 0;
		return InsertItem( pLVI );
	}

	int count = GetItemCount();
	for( int i = 0; i < count; i++ )
	{
		Item* pItem = GetItemAt(i);
		if( pItem == pInsertAfter )
		{
			pLVI->iItem = i + 1;
			break;
		}
	}

	return InsertItem( pLVI );
}


int SEC_LISTCLASS::InsertItemBefore( Item* pInsertBefore, LV_ITEM* pLVI )
{
	//Special Case...if pInsertBefore is NULL...insert at item 0
	if( pInsertBefore == NULL )
	{
		pLVI->iItem = 0;
		return InsertItem( pLVI );
	}

	int count = GetItemCount();
	for( int i = 0; i < count; i++ )
	{
		Item* pItem = GetItemAt(i);
		if( pItem == pInsertBefore )
		{
			pLVI->iItem = i - 1;
			break;
		}
	}

	return InsertItem( pLVI );
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Style Accessors
//

BOOL SEC_LISTCLASS::IsStyleSet( DWORD dwStyle ) const
{
	if( (GetListCtrlStyle() & dwStyle) == dwStyle )
		return TRUE;

	return FALSE;
}


BOOL SEC_LISTCLASS::IsExStyleSet( DWORD dwExStyle ) const
{
	if( (GetListCtrlStyleEx() & dwExStyle) == dwExStyle )
		return TRUE;

	return FALSE;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the list control style.
//@rdesc the list control style.
DWORD SEC_LISTCLASS::GetListCtrlStyle() const
{
	return m_dwListStyle;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the list control extended style.
//@rdesc the list control extended style. See SetlistCtrlStyleEx for possible values.
DWORD SEC_LISTCLASS::GetListCtrlStyleEx() const
{
	return m_dwListStyleEx;
}


void SEC_LISTCLASS::ResizeColumnsToFit()
{
	int iColumnCount = GetColumnCount();
	//Only resize if there are some columns available...
	if( iColumnCount != 0 )
	{
		Column* pCol = NULL;
		CRect rcInside(0,0,0,0);
		int cxSubItems(0);
		int iItemCol(0);
		int nCol(0);

		cxSubItems = 0;
		for (nCol=0; nCol<iColumnCount; nCol++)
		{
			pCol = GetColumnAt(nCol);

			if ( pCol && (pCol->iSubItem != 0))
				cxSubItems += pCol->cx;
			else
				iItemCol = nCol;
		}

		// The item column takes up the remaining width
		// not occupied by subitem columns.
		pCol = GetColumnAt(iItemCol);
		if ( pCol )
		{
			GetInsideRect(rcInside);
			pCol->cx = rcInside.Width() - cxSubItems;
		}
	}
}


//@doc SEC_LISTCLASS
//@mfunc Sets the list control style.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD | dwStyle | Value for the style.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the tree control; default is TRUE.
BOOL SEC_LISTCLASS::SetListCtrlStyle(DWORD dwStyle, BOOL bRedraw)
{
	DWORD dwOldStyle = m_dwListStyle;
	m_dwListStyle = dwStyle;
	ProcessAndDestroyEditLabel();
	KillEditControl();
	m_pToolTipWnd->Hide();
	
	// word wrap removed or added then must recalc all item heights
	if((m_dwListStyle & LVS_NOLABELWRAP) != (dwOldStyle & LVS_NOLABELWRAP))
		ClearAllCy();

	if (bRedraw)
	{
		ReMeasureAllItems();
		Invalidate();
	}

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the list control extended styles.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD | dwStyleEx | The new extended style.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the tree control; default is TRUE.
//@comm  The extended styles can be a combination of the following:
//	LVXS_FLYBYTOOLTIPS			Enable tooltips.
//	LVXS_OWNERDRAWVARIABLE		Reserved.
//	LVXS_LINESBETWEENITEMS		Paints horizontal lines between items.
//	LVXS_LINESBETWEENCOLUMNS	Paints vertical lines between columns.
//	LVXS_WORDWRAP				Enables wrapping item text if column is too narrow.
//	LVXS_HILIGHTSUBITEMS		Allows full row selection. 
//								This is required for LVS_EDITLABELS to work for subitems.
//
//	LVXS_FITCOLUMNSONSIZE		The item column takes up the remaining width not occupied by subitem columns.
//
//	LVXS_NOGROWCOLUMNONDELETE	Prevents automatic resizing of column 0 when a column is deleted.
BOOL SEC_LISTCLASS::SetListCtrlStyleEx(DWORD dwStyleEx, BOOL bRedraw)
{
	ProcessAndDestroyEditLabel();
	m_pToolTipWnd->Hide();
	if ((this -> GetListCtrlStyleEx()&LVXS_FITCOLUMNSONSIZE) == 0 &&
		(dwStyleEx&LVXS_FITCOLUMNSONSIZE) != 0)
	{
		ResizeColumnsToFit();
	}
	m_dwListStyleEx = dwStyleEx;

	if (bRedraw)
	{
		ReMeasureAllItems();
		Invalidate();
	}

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the list control style and extended style.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD& | dwStyle | The list control style.
//@parm  DWORD& | dwStyleEx | The list control extended style.
BOOL SEC_LISTCLASS::GetListCtrlStyles(DWORD& dwStyle, DWORD& dwStyleEx) const
{
	dwStyle = GetListCtrlStyle();
	dwStyleEx = GetListCtrlStyleEx();
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the list control styles and extended styles.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD | dwStyle | The list control style.
//@parm  DWORD | dwStyleEx | The list control extended style.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the tree control; default is TRUE.
BOOL SEC_LISTCLASS::SetListCtrlStyles(DWORD dwStyle, DWORD dwStyleEx, BOOL bRedraw)
{
	if (!SetListCtrlStyle(dwStyle, FALSE))
		return FALSE;

	if (!SetListCtrlStyleEx(dwStyleEx, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Adds and/or removes list control styles.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD | dwRemove | The styles to remove, or 0 for none.
//@parm  DWORD | dwAdd | The styles to add, or 0 for none.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the tree control; default is TRUE.
BOOL SEC_LISTCLASS::ModifyListCtrlStyle(DWORD dwRemove, DWORD dwAdd, BOOL bRedraw)
{
	DWORD dwListStyle = m_dwListStyle;

	dwListStyle &= ~(dwRemove);
	dwListStyle |= dwAdd;

	if (!SetListCtrlStyle(dwListStyle, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Adds and/or removes list control extended styles.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD | dwRemoveEx | The extended styles to remove, or 0 for none.
//@parm  DWORD | dwAddEx | The extended styles to add, or 0 for none.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the tree control; default is TRUE.
BOOL SEC_LISTCLASS::ModifyListCtrlStyleEx(DWORD dwRemoveEx, DWORD dwAddEx, BOOL bRedraw)
{
	DWORD dwListStyleEx = m_dwListStyleEx;

	dwListStyleEx &= ~(dwRemoveEx);
	dwListStyleEx |= dwAddEx;

	if (!SetListCtrlStyleEx(dwListStyleEx, bRedraw))
		return FALSE;

	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Adds and/or removes list control styles and extended styles.
//@rdesc Nonzero if successful; otherwise 0.
//@parm DWORD | dwRemove | The styles to remove, or 0 for none.
//@parm  DWORD | dwAdd | The styles to add, or 0 for none.
//@parm DWORD | dwRemoveEx | The extended styles to remove, or 0 for none.
//@parm  DWORD | dwAddEx | The extended styles to add, or 0 for none.
//@parm  BOOL | bRedraw | Flag indicating whether to redraw the tree control; default is TRUE.
BOOL SEC_LISTCLASS::ModifyListCtrlStyles(DWORD dwRemove, DWORD dwAdd, DWORD dwRemoveEx, DWORD dwAddEx, BOOL bRedraw)
{
	if (!ModifyListCtrlStyle(dwRemove, dwAdd, FALSE))
		return FALSE;

	if (!ModifyListCtrlStyleEx(dwRemoveEx, dwAddEx, bRedraw))
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Drag&Drop Members
//


BOOL SEC_LISTCLASS::SelectDropTarget(int nIndex)
{
	int nPrevDrop = GetDropHilightIndex();

	if (nIndex != nPrevDrop)
	{
		if (nPrevDrop != -1)
			SetItemState(nPrevDrop, 0, LVIS_DROPHILITED);

		if (nIndex != -1)
			SetItemState(nIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
	}

	return TRUE;
}


int SEC_LISTCLASS::GetDropHilightIndex() const
{
	return m_nDropHilite;
}

//@doc SEC_LISTCLASS
//@mfunc starts automatic scrolling during drag drop operation.
//@parm UINT | nInterval | the number of mill-seconds to hover before scrolling
void SEC_LISTCLASS::StartAutoScroll(UINT nInterval)
{
	StopAutoScroll();
	if(nInterval > 0)
		m_idAutoScrollTimer = SetTimer( 2, nInterval, NULL );

}

//@doc SEC_LISTCLASS
//@mfunc stops automatic scrolling after a drag drop operation.
void SEC_LISTCLASS::StopAutoScroll()
{
	if(m_idAutoScrollTimer)
	{
		KillTimer( m_idAutoScrollTimer );
		m_idAutoScrollTimer = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS In-Place Editing Members
//


void SEC_LISTCLASS::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_idEditLabelTimer)
	{
		KillTimer( m_idEditLabelTimer );
		m_idEditLabelTimer=0;
		if (m_nPendingEdit == GetFocusIndex())
			EditLabel(m_nPendingEdit, m_nActiveColumn);
	}
	else if((nIDEvent == m_idAutoScrollTimer) && m_bDragging)
	{
		// hit test for the current item
		CPoint ptHit;
		GetCursorPos(&ptHit);
		ScreenToClient(&ptHit);
		UINT iHitFlags;
		int nHitIndex;
		int nHitCol;
		nHitIndex = HitTest( ptHit, &iHitFlags, nHitCol );
		if(nHitIndex != -1 && (GetItemCount() >= 3))
		{
			if((GetTopIndex() > 0) && (nHitIndex < GetTopIndex() + 1))
			{
				SelectDropTarget(-1);
				ScrollVert(-1);
			}
			else if( (nHitIndex > (GetLastFullyVisibleItem() - 1)) &&
					(GetLastFullyVisibleItem() < GetItemCount() - 1) )
			{
				SelectDropTarget(-1);
				ScrollVert(1);
			}
		}
	}
	else if (nIDEvent == m_idAutoKeySelectTimer)
	{
		if(!(m_uiAutoKeySelectionMode & AutoKeySelectionOn) || !(m_uiAutoKeySelectionMode & AutoKeySelecting))
			// Mode is not on anymore or the user did not enter a key within the timer interval
		{
			// Kill timer
			if(m_idAutoKeySelectTimer)
			{
				KillTimer(m_idAutoKeySelectTimer);
				m_idAutoKeySelectTimer = 0;
			}
			m_uiAutoKeySelectionMode = 0;	// Clear all modes
			m_strAutoKeySelectString.Empty();
		}
		// If mode is still on, remove AutoKeySelecting 
		// and see if it gets set before the next timer event.
		m_uiAutoKeySelectionMode &= ~AutoKeySelecting;
	}

	SEC_LISTBASE::OnTimer(nIDEvent);
}


CEdit* SEC_LISTCLASS::EditLabel( int iItem, int iCol )
{
	if( m_pEditCtrl->GetSafeHwnd() )
	{
		//ASSERT --> You can't call EditLabel twice.  
		//	That would cause a memory leak.
		ASSERT(FALSE);
		return NULL;
	}

	Column* pCol = 0;
	int iSubItem = 0;

	m_pToolTipWnd->Hide();

	// If we are already editing a label, reset the old one before
	// creating the new
	if (m_nEditItem != -1)
		ProcessAndDestroyEditLabel(TRUE);

	pCol = GetColumnAt(iCol);
	if ( pCol )
		iSubItem = pCol->iSubItem;
	
	ASSERT_VALID(m_pEditCtrl);

	m_nPendingEdit = -1;
	m_nEditItem = iItem;

	CRect rect;
	RETURN_ON_ERROR( GetItemRect( iItem, iCol, rect, LVIR_LABEL ), NULL );

	GetItemAt(iItem);	// may need to force a callback

	rect.top -= 1;
	rect.left  -= 4;
	rect.bottom += 1;

	CRect rcInside;
	GetInsideRect( &rcInside );

	int width = rect.Width() + 10;	// a little extra padding
	rect.left = otmax(0, rect.left);
	rect.right = otmin( rect.left + width, rcInside.right );

	DWORD dwStyle = WS_BORDER|WS_CHILD;
	if (this -> GetListCtrlStyle() & LVS_NOLABELWRAP)
		// we are not multiline, but have to use the style to get tab support.
		dwStyle |= ES_MULTILINE | ES_AUTOHSCROLL | ES_WANTRETURN;
	else
		dwStyle |= ES_AUTOVSCROLL|ES_MULTILINE;

	RETURN_ON_ERROR(m_pEditCtrl->Create( dwStyle, rect, this, ID_EDIT_REFTREE ), NULL);
	// enable tab expansion
	m_pEditCtrl->SetTabStops();

	CClientDC dc(this);
	LvPaintContext* pPC;
	pPC = CreatePaintContext(&dc);
	CString str = GetItemText( iItem, iSubItem );
	m_pEditCtrl->SetWindowText( str );

	pPC->lvi.iItem = iItem;
	pPC->lvi.iSubItem = iSubItem;
	pPC->lvi.state = 0;
	VERIFY(LoadPaintContext(pPC));
	m_pEditCtrl->SetFont( pPC->pFont );
	m_pEditCtrl->SetSel(0, -1);

	if ( NotifyBeginLabelEdit(iItem, iSubItem) == TRUE )
	{
		m_pEditCtrl->DestroyWindow();
		m_nEditItem = -1;
		m_nPendingEdit = -1;
	}
	else
	{
		m_pEditCtrl->ShowWindow(SW_SHOW);
		m_pEditCtrl->SetFocus();
		m_strOldEditText=str;
		Update( m_nEditItem, FALSE, TRUE, FALSE );
	}
	m_bDidProcessAndDestroyEditLabel=FALSE;
	delete pPC;

	return m_pEditCtrl;
}


//@doc SEC_LISTCLASS
//@mfunc Returns a pointer to the edit control used in EditLabel.
//@rdesc a pointer to the control.
//@comm  This control will only have a valid hWnd during a label editing operation.
CEdit * SEC_LISTCLASS::GetEditControl()
{
	return m_pEditCtrl;
}


BOOL SEC_LISTCLASS::KillEditControl()
{
 	if( m_idEditLabelTimer )
	{
		KillTimer( m_idEditLabelTimer );
		m_idEditLabelTimer=0;
	}

	return TRUE;
}


BOOL SEC_LISTCLASS::ProcessAndDestroyEditLabel(BOOL bSaveLabel)
{
	if( !m_pEditCtrl->GetSafeHwnd() )
		return FALSE;

	// Focus loss from DestroyWindow may result in reentrancy
	// in some scenarios - protect against this.
	if(m_bDidProcessAndDestroyEditLabel) return FALSE;
	m_bDidProcessAndDestroyEditLabel=TRUE;

	CString strEdit;
	m_pEditCtrl->GetWindowText(strEdit);

	Column* pCol;
	int iSubItem = 0;
	pCol = GetColumnAt(m_nActiveColumn);
	if ( pCol )
		iSubItem = pCol->iSubItem;

	// item # needs to go on the stack in case they call EditLabel again from the handler.
	int nEditItem = m_nEditItem;

	if (NotifyEndLabelEdit(nEditItem, iSubItem, strEdit, bSaveLabel)) {
		SetItemText(nEditItem, iSubItem, strEdit);

		// After the fact notification that a label edit change has completed.
		// (but before the change is applied to screen)
		NotifyEndLabelEditChangeComplete(nEditItem,iSubItem);
		}
	else
		SetItemText(nEditItem, iSubItem, m_strOldEditText);

	m_pEditCtrl->DestroyWindow();
	m_nEditItem = -1;

	ReMeasureAllItems();
	Invalidate();

	return TRUE;
}


void
SEC_LISTCLASS::EditLabel_OnKillFocus(void)
{
	ProcessAndDestroyEditLabel();
}


void 
SEC_LISTCLASS::EditLabel_OnUpdate(void)
{
	ASSERT_VALID(m_pEditCtrl);

	CClientDC dc(m_pEditCtrl);
	CString strEdit;

	Column* pCol;
	pCol = GetColumnAt(m_nActiveColumn);

	// Save old text rect
	CRect rectBefore;
	VERIFY( GetItemRect( m_nEditItem, m_nActiveColumn, rectBefore, LVIR_LABEL ) );

	// Store edits to item text
	m_pEditCtrl->GetWindowText(strEdit);
	SetItemText(m_nEditItem, pCol->iSubItem, strEdit);
	MeasureItem(m_nEditItem);

	CRect rectAfter;
	VERIFY( GetItemRect( m_nEditItem, m_nActiveColumn, rectAfter, LVIR_LABEL ) );

	// If a new Item is required, we must remeasure all Items.
	if (rectAfter.Height() != rectBefore.Height())
	{
		ReMeasureAllItems();
		Update(m_nEditItem, FALSE, TRUE, TRUE);
	}

	// Modify dimensions of edit control
	int width;
	rectAfter.top -= 1;
	rectAfter.left  -= 4;
	rectAfter.right += 4;
	width = rectAfter.Width();
	rectAfter.left = otmax(0, rectAfter.left);
	rectAfter.right = rectAfter.left + width;
	if (GetColumnCount() > 1)
		rectAfter.right = otmin(rectAfter.right + 10, GetColumnRight(m_nActiveColumn));
	else
	{
		CRect rcInside;
		GetInsideRect(rcInside);
		rectAfter.right = otmin(rectAfter.right + 10, rcInside.right);
	}

	rectAfter.bottom += 1;

	m_pEditCtrl->SetWindowPos(  NULL, 0, 0,
								rectAfter.Width(),
								rectAfter.Height(),
								SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );
}

/////////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Scrolling Members
//



BOOL SEC_LISTCLASS::CreateScrollBarCtrl(DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	return (::CreateWindow(_T("SCROLLBAR"), NULL,
		dwStyle | WS_VISIBLE | WS_CHILD,
		0, 0, 1, 1, m_hWnd, (HMENU)nID,
		AfxGetInstanceHandle(), NULL) != NULL);
}


CScrollBar* SEC_LISTCLASS::GetScrollBarCtrl(int nBar) const
{
	UINT nIDScroll;

	if (nBar == SB_HORZ)
		nIDScroll = AFX_IDW_HSCROLL_FIRST;
	else
		nIDScroll = AFX_IDW_VSCROLL_FIRST;

	return (CScrollBar*)GetDlgItem(nIDScroll);
}


void SEC_LISTCLASS::ShowScrollBarCtrl(int nBar, BOOL bShow, LPSCROLLINFO lpScrollInfo)
{
	//If they don't want scroll bars...shut them off...
	if( GetListCtrlStyle() & LVS_NOSCROLL )
		bShow = FALSE;

	if (nBar == SB_BOTH)
	{
		ShowScrollBarCtrl(SB_HORZ, bShow);
		ShowScrollBarCtrl(SB_VERT, bShow);
	}
	else
	{
		CScrollBar* pScrollBar = GetScrollBarCtrl(nBar);
		if (pScrollBar == NULL)
		{
			// No scroll bar control has been created yet.  Since we're
			// told to show a scroll bar, we have to create one first.
			if (nBar == SB_HORZ)
			{
				if (!CreateScrollBarCtrl(SBS_HORZ, AFX_IDW_HSCROLL_FIRST))
					AfxThrowResourceException();
			}
			else if (nBar == SB_VERT)
			{
				if (!CreateScrollBarCtrl(SBS_VERT, AFX_IDW_VSCROLL_FIRST))
					AfxThrowResourceException();
			}
			pScrollBar = GetScrollBarCtrl(nBar);
		}
		// Since we created a scroll bar if one didn't exists, we'd
		// better have a non-NULL scroll bar pointer by now.
		ASSERT(pScrollBar != NULL);

		// When showing a scroll bar, we may want to first set the min,
		// max info just before making the scrollbar visible.  Setting
		// the scroll bar info just before rendering reduces flicker.
		if (lpScrollInfo != NULL)
			pScrollBar->SetScrollInfo(lpScrollInfo, FALSE);

		DWORD dwPrevScrollStyle = m_dwScrollStyle;

		if (bShow)
			m_dwScrollStyle |= (nBar == SB_VERT) ? WS_VSCROLL : WS_HSCROLL;
		else
			m_dwScrollStyle &= ~((nBar == SB_VERT) ? WS_VSCROLL : WS_HSCROLL);

		if (m_dwScrollStyle != dwPrevScrollStyle)
		{
			pScrollBar->ShowScrollBar(bShow);
			pScrollBar->Invalidate();
			pScrollBar->UpdateWindow();
		}
	}
}


//@doc SEC_LISTCLASS
//@mfunc determines if the horizontal and/or vertical scroll bars should be shown or not.
//@rdesc void 
void SEC_LISTCLASS::RecalcScrollBars()
{
	
	// 1st time
	RecalcVertScrollBar( GetLogicalHeight() );
	
	if ( IsWordWrapEnabled() )
		ClearMeasurements();	

	RecalcHorzScrollBar( GetLogicalWidth() );
	// 2nd time - may have changed with horz scroll bar recalc
	RecalcVertScrollBar( GetLogicalHeight() );

	RecalcLayout();
}


void SEC_LISTCLASS::RecalcVertScrollBar(int cyLog)
{
	CRect rcInside;
	SCROLLINFO sbi;

	int cItemsThisPage = ItemsThisPage();

	GetInsideRect(rcInside);

	if (cyLog > cItemsThisPage)
	{
		if (m_nTopIndex > (cyLog - cItemsThisPage))
			ScrollVert((cyLog - cItemsThisPage) - m_nTopIndex);
		sbi.fMask = SIF_ALL;
		sbi.nMin = 0;
		sbi.nPage = cItemsThisPage;
		sbi.nMax = GetItemCount()-1;
		sbi.nPos = GetTopIndex();
		ShowScrollBarCtrl(SB_VERT, TRUE, &sbi);
	}
	else
	{
		if (m_nTopIndex > 0)
			ScrollVert(-m_nTopIndex);
		ShowScrollBarCtrl(SB_VERT, FALSE);
	}

	// rect may have changed because of vertical scroll bar
	int cx = rcInside.Width();
	GetInsideRect(rcInside);
	if ( rcInside.Width() != cx )
	{
		if ( AutoSizeColumns() )
			Invalidate();
	}

}


void SEC_LISTCLASS::RecalcHorzScrollBar( int cxLog )
{
	CRect rcInside;
	GetInsideRect(rcInside);
	SCROLLINFO sbi;

	if (cxLog > rcInside.Width())
	{
		if (m_ptViewportOrg.x > (cxLog - rcInside.Width()))
			ScrollHorz((cxLog - rcInside.Width()) - m_ptViewportOrg.x);
		sbi.fMask = SIF_ALL;
		sbi.nMin = 0;
		sbi.nPage = rcInside.Width();
		sbi.nMax = cxLog;
		sbi.nPos = m_ptViewportOrg.x;
		ShowScrollBarCtrl(SB_HORZ, TRUE, &sbi);
	}
	else
	{
		m_ptViewportOrg.x = 0;
		ShowScrollBarCtrl(SB_HORZ, FALSE);
	}

}


void SEC_LISTCLASS::SetFirstVisible(int nIndex)
{
	ScrollVert(nIndex - GetTopIndex());
	RecalcScrollBars();
}


void SEC_LISTCLASS::ScrollHorz(int nPixels)
{
	CRect rcInside;
	CSize szLog;

	GetInsideRect(rcInside);
	szLog = GetLogicalSize();

	int xOrig = m_ptViewportOrg.x;
	m_ptViewportOrg.x -= nPixels;
	m_ptViewportOrg.x = otmax(0, m_ptViewportOrg.x);
	m_ptViewportOrg.x = otmin((szLog.cx - rcInside.Width()), m_ptViewportOrg.x);
	nPixels = xOrig - m_ptViewportOrg.x;

	// Scroll the header control too
	if (IsHeaderCtrlEnabled())
	{
		rcInside.top -= m_cyHeader;
	}

	SetScrollPos(SB_HORZ, m_ptViewportOrg.x, TRUE);
	Invalidate(FALSE);
}

//
// ScrollVert()
//
//  This function scrolls the list up or down.  The nScrollItems
//  argument indicates how many items to scroll the list by.
//  There is a complication:  each item in the list can have a
//	different height.  Therefore, it becomes ambiguous as to which
//  item's you mean to scroll.  For example, you can scroll three
//	items off of the top of the list or three items into view from
//  the bottom of the list.  These two cases may scroll the list
//  by different pixel deltas since the items at the top of the
//  list may have different heights than the items at the bottom.
//  This is what the bFromBottom flag is all about.  If true, the
//  height of the items just off the bottom of the display are
//  summed in computing the total distance to scroll.  If false,
//  the items at the top of the display are summed instead.
//

void SEC_LISTCLASS::ScrollVert(int nScrollItems, BOOL bFromBottom)
{
	int nFirst, nLast;

	if (!bFromBottom)
	{
		nFirst = GetTopIndex();
		nLast = nFirst + nScrollItems;
		nLast = otmax(nLast, 0);
		nLast = otmin(nLast, GetItemCount()-1);
		nScrollItems = nLast - nFirst;

		if (nScrollItems != 0)
		{
			// Since we scroll by items (i.e. item granularity), all
			// we really have to do to scroll is increment/decrement
			// the value of m_nTopIndex.
			m_nTopIndex += nScrollItems;
			if(m_nTopIndex < 0)	//m_nTopIndex should never be negative
				m_nTopIndex = 0;
			// removed ScrollWindow optimization because it did not behave
			// well with word wrap and subitems with callback text.
			Invalidate();
		}
	}
	else
	{
		nFirst = GetLastFullyVisibleItem();
		nLast = nFirst + nScrollItems;
		int cyScroll = CalcRangeHeight(nFirst, nLast, TRUE);
		nScrollItems = CalcItemsInRange(cyScroll, GetTopIndex(), TRUE);
		ScrollVert(nScrollItems, FALSE);
	}
}


BOOL SEC_LISTCLASS::EnsureVisible(int nItem, BOOL bPartialOk)
{
	// Scroll the item into view iff neccessary
	int nLastFullyVisible = GetLastFullyVisibleItem();
	int nTopIndex = GetTopIndex();

	if((nItem == nLastFullyVisible)&&(nLastFullyVisible == GetItemCount()-1))
		return TRUE;

	if (nItem >= nLastFullyVisible && nLastFullyVisible != -1)
	{
		ScrollVert(nItem - nLastFullyVisible, TRUE);
	}
	else if (nItem < (nTopIndex + 1))
	{
		ScrollVert(nItem - (nTopIndex + 1));
	}

	return TRUE;
}


BOOL SEC_LISTCLASS::EnsureRangeVisible(int nFirst, int nLast, BOOL bPartialOk)
{
	bPartialOk; // UNUSED
	
	// validate the range
	if ( (nFirst < 0 )|| ( nLast > GetItemCount()) || (nFirst > nLast) )
		return FALSE;

	if((nFirst >= GetTopIndex()) &&
		(nLast <= GetLastFullyVisibleItem()) )
		return TRUE;

	CRect rcInside;
	Item* pItem;
	int i;
	int cyRange;
	int cyRemaining;

	GetInsideRect(rcInside);

	cyRange = 0;
	cyRemaining = rcInside.Height();
	for (i=nFirst; i<=nLast && cyRange < cyRemaining; i++)
	{
		pItem = GetItemAt(i);
		if ( !pItem )
			return FALSE;
		if (pItem->m_bDirty)
			MeasureItem(i);
		cyRange += pItem->m_cy + GetMargin();
	}

	cyRemaining = rcInside.Height() - cyRange;
	if (cyRemaining > 0 && nFirst > 0)
	{
		for (nFirst--; nFirst >= 0;nFirst--)
		{
			pItem = GetItemAt(nFirst);
			if (!pItem )
				return FALSE;
			if (pItem->m_bDirty)
				MeasureItem(nFirst);
			if (cyRemaining < (int)pItem->m_cy)
			{
				nFirst++;
				break;
			}
			cyRemaining -= pItem->m_cy + GetMargin();
		}
		nFirst = otmax(nFirst, 0);
	}


	ScrollVert(nFirst - GetTopIndex());

	return TRUE;
}


void SEC_LISTCLASS::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	pScrollBar;  // UNUSED
	CRect rcInside;

	// Cleanup any open edits or tooltips
	ProcessAndDestroyEditLabel();
	m_pToolTipWnd->Hide();

	GetInsideRect(rcInside);
	switch ( nSBCode )
	{
	case SB_LINELEFT:
		ScrollHorz(+15);
		break;
	case SB_LINERIGHT:
		ScrollHorz(-15);
		break;
	case SB_PAGELEFT:
		ScrollHorz(rcInside.Width());
		break;
	case SB_PAGERIGHT:
		ScrollHorz(-rcInside.Width());
		break;
	case SB_RIGHT:
		ScrollHorz(-m_ptViewportOrg.x);
		break;
	case SB_LEFT:
		ScrollHorz(m_ptViewportOrg.x);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		ScrollHorz(m_ptViewportOrg.x - nPos);
		break;
	default:
		break;
	}
}


void SEC_LISTCLASS::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nNextPage;

	// Cleanup any open edits or tooltips
	ProcessAndDestroyEditLabel();
	m_pToolTipWnd->Hide();	

	if(pScrollBar == NULL)
		pScrollBar = GetScrollBarCtrl(SB_VERT);
	ASSERT_VALID(pScrollBar);

	SCROLLINFO sbi;	
	pScrollBar->GetScrollInfo(&sbi);

	pScrollBar;  // UNUSED
	switch ( nSBCode )
	{
	case SB_LINEUP:
		if (sbi.nPos > 0)
			ScrollVert(-1);
		break;
	case SB_LINEDOWN:
		if ( (sbi.nPos + (int)sbi.nPage) < (sbi.nMax+1))
			ScrollVert(+1);
		break;
	case SB_PAGEUP:
		ScrollVert(-ItemsPrevPage());
		break;
	case SB_PAGEDOWN:
		nNextPage = GetTopIndex() + sbi.nPage;
		// The maximum scroll position is that which places
		// the very last item at the bottom of the display.
		// This check makes sure the page down doesn't go
		// past that maximum position.
		if (nNextPage > (sbi.nMax + 1 - (int)sbi.nPage))
			nNextPage = sbi.nMax + 1 - sbi.nPage;
		ScrollVert(nNextPage - GetTopIndex());
		break;
	case SB_BOTTOM:
		if (!IsWordWrapEnabled())
			ScrollVert(sbi.nMax - sbi.nPos - sbi.nPage + 1);
		else
		{
			CRect rcInside;
			GetInsideRect(rcInside);
			const int nHeight   = rcInside.Height();
			const int nMargin   = GetMargin();
			const int nLastItem = GetItemCount()-1;

			int cyRange = 0;
			for (int i=nLastItem; i>=0; i--)
			{
				Item * pItem = GetItemAt(i);
				if (pItem->m_bDirty)
					MeasureItem(i);
				cyRange += pItem->m_cy + nMargin;
				if (cyRange > nHeight)
					break;
			}

			if (i != nLastItem)
				i++;
			ScrollVert(i-GetTopIndex());
		} 
		break;
	case SB_TOP:
		ScrollVert(-sbi.nPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		ScrollVert(nPos - sbi.nPos);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Colors / Fonts / Brushes / Pens etc.
//


//@doc SEC_LISTCLASS
//@mfunc Returns the background color of the control. Default is ::GetSysColor(COLOR_WINDOW).
//@rdesc COLORREF 
//@comm  If the window is disabled, this function will return the disabled background color.
COLORREF SEC_LISTCLASS::GetBkColor() const
{
	if ( IsWindowEnabled() )
		return m_rgbBk;
	return rgbNoFocusBk;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the normal text color. Default is ::GetSysColor( COLOR_WINDOWTEXT );
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetTextColor() const
{
	return m_rgbText;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the current normal text background color. This defaults to the window background color.
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetTextBkColor() const
{
	return m_rgbTextBk;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the selected text color. Default is ::GetSysColor( COLOR_HIGHLIGHTTEXT ).
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetSelTextColor() const
{
	return m_rgbSelText;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the selected text background color. Default is ::GetSysColor( COLOR_HIGHLIGHT ).
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetSelTextBkColor() const
{
	return m_rgbSelTextBk;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the disabled text color. Default is ::GetSysColor( COLOR_GRAYTEXT ).
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetDisabledTextColor() const
{
	return m_rgbDisabledText;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the icon background color. Default is ::GetSysColor( COLOR_WINDOW );
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetIconBkColor() const
{
	return m_rgbIconBk;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the selected icon background color. Default is ::GetSysColor( COLOR_WINDOW ).
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetSelIconBkColor() const
{
	return m_rgbSelIconBk;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the tooltip text background color. Default is ::GetSysColor( COLOR_INFOBK ).
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetToolTipBkColor() const
{
	return m_rgbToolTipBk;
}


//@doc SEC_LISTCLASS
//@mfunc Returns the tooltip text color. Default is ::GetSysColor( COLOR_INFOTEXT ).
//@rdesc COLORREF 
COLORREF SEC_LISTCLASS::GetToolTipTextColor() const
{
	return m_rgbToolTipText;
}


//@doc SEC_LISTCLASS
//@mfunc Returns a pointer to the label font used to draw all item text. By default, the stock DEFAULT_GUI_FONT is used.
//@rdesc CFont* 
CFont* SEC_LISTCLASS::GetLabelFont() const
{
	if (m_pLabelFont)
		return m_pLabelFont;
	else
#ifndef UNDER_CE
		return CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
#else
		return CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FONT));
#endif
}


//@doc SEC_LISTCLASS
//@mfunc Returns a pointer to the header control text font. By default, the stock DEFAULT_GUI_FONT is used.
//@rdesc CFont* 
CFont* SEC_LISTCLASS::GetHeaderFont() const
{
	if (m_pHeaderFont)
		return m_pHeaderFont;
	else
#ifndef UNDER_CE
		return CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
#else
		return CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FONT));
#endif
}


//@doc SEC_LISTCLASS
//@mfunc Returns a pointer to the window that will receive control notifications. The default is the parent of the control.
//@rdesc CWnd* 
CWnd* SEC_LISTCLASS::GetNotifyWnd() const
{
	return m_pNotifyWnd;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the background color of the control. Also will set the text background color to match.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbBk | RGB value for background color.
BOOL SEC_LISTCLASS::SetBkColor(COLORREF rgbBk)
{
	m_rgbBk = rgbBk;
	SetTextBkColor( rgbBk );
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the text color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbText | RGB value for text color.
BOOL SEC_LISTCLASS::SetTextColor(COLORREF rgbText)
{
	m_rgbText = rgbText;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the text background color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbTextBk | RGB value for text background color.
BOOL SEC_LISTCLASS::SetTextBkColor(COLORREF rgbTextBk)
{
	m_rgbTextBk = rgbTextBk;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the selected text color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbSelText | RGB value for selected text color.
BOOL SEC_LISTCLASS::SetSelTextColor(COLORREF rgbSelText)
{
	m_rgbSelText = rgbSelText;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the selected text background color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbSelTextBk | RGB value for selected text background color.
BOOL SEC_LISTCLASS::SetSelTextBkColor(COLORREF rgbSelTextBk)
{
	m_rgbSelTextBk = rgbSelTextBk;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the disabled text color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbDisabledText | RGB value for disabled text color.
BOOL SEC_LISTCLASS::SetDisabledTextColor(COLORREF rgbDisabledText)
{
	m_rgbDisabledText = rgbDisabledText;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the icon background color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbIconBk | RGB value for icon background color.
BOOL SEC_LISTCLASS::SetIconBkColor(COLORREF rgbIconBk)
{
	m_rgbIconBk = rgbIconBk;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the selected icon background color of the control.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbSelIconBk | RGB value for selected icon background color.
BOOL SEC_LISTCLASS::SetSelIconBkColor(COLORREF rgbSelIconBk)
{
	m_rgbSelIconBk = rgbSelIconBk;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the tooltip background color.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbToolTipBk | RGB value for tooltip background color.
BOOL SEC_LISTCLASS::SetToolTipBkColor(COLORREF rgbToolTipBk)
{
	m_rgbToolTipBk = rgbToolTipBk;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the text color of the tooltip.
//@rdesc Nonzero if successful; otherwise 0.
//@parm COLORREF | rgbToolTipText | RGB value for tooltip text color.
BOOL SEC_LISTCLASS::SetToolTipTextColor(COLORREF rgbToolTipText)
{
	m_rgbToolTipText = rgbToolTipText;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the font to be used when drawing label text.
//@rdesc Nonzero if successful; otherwise 0.
//@parm CFont* | pFont | A pointer to the new font to be used.
BOOL SEC_LISTCLASS::SetLabelFont(CFont* pFont)
{
	m_pLabelFont = pFont;
	return TRUE;
}


//@doc SEC_LISTCLASS
//@mfunc Sets the font to be used when drawing column header text.
//@rdesc Nonzero if successful; otherwise 0.
//@parm CFont* | pFont | A pointer to the new font to be used.
BOOL SEC_LISTCLASS::SetHeaderFont(CFont* pFont)
{
	m_pHeaderFont = pFont;
	return TRUE;
}

//You can pass in a NULL CWnd to let default routing be used...

//@doc SEC_LISTCLASS
//@mfunc Sets the window that will receive notifications.
//@rdesc Nonzero if successful; otherwise 0.
//@parm  CWnd* | pNotifyWnd | A pointer to the window that will receieve control notifications.
BOOL SEC_LISTCLASS::SetNotifyWnd( CWnd* pNotifyWnd )
{
	if( pNotifyWnd )
		ASSERT_VALID(pNotifyWnd);

	m_pNotifyWnd = pNotifyWnd;

	return TRUE;
}


CImageList* SEC_LISTCLASS::SetImageList(CImageList *pImageList, int nImageListType)
{
	CImageList* pOldList = NULL;
	if( GetListCtrlStyle() & LVS_SHAREIMAGELISTS )
	{
		if (nImageListType == LVSIL_NORMAL)
		{
			pOldList = m_pImageList;
			m_pImageList = pImageList;
		}
		else if (nImageListType == LVSIL_SMALL)
		{
			pOldList = m_pSmallImageList;
			m_pSmallImageList = pImageList;
		}
		else if (nImageListType == LVSIL_STATE)
		{
			pOldList = m_pStateImageList;
			m_pStateImageList = pImageList;
		}
		else if (nImageListType == LVSIL_HEADER)
		{
			pOldList = m_pHeaderImageList;
			m_pHeaderImageList = pImageList;
		}
		else
		{
			ASSERT(FALSE) ; // Unknown type!
			pOldList = NULL;
		}
	}
	
	//we take responsibility for destroying them...so make a copy and lets begin...
	else 
	{
		if (nImageListType == LVSIL_NORMAL)
		{
			pOldList = m_pImageList;
			CopyImageList( pImageList, m_pImageList );
		}
		else if (nImageListType == LVSIL_SMALL)
		{
			pOldList = m_pSmallImageList;
			CopyImageList( pImageList, m_pSmallImageList );
		}
		else if (nImageListType == LVSIL_STATE)
		{
			pOldList = m_pStateImageList;
			CopyImageList( pImageList, m_pStateImageList );
		}
		else if (nImageListType == LVSIL_HEADER)
		{
			pOldList = m_pHeaderImageList;
			CopyImageList( pImageList, m_pHeaderImageList);
		}
		else
		{
			ASSERT(FALSE);
			pOldList = NULL;
		}
	}

	return pOldList;
}


CImageList* SEC_LISTCLASS::GetImageList(UINT nImageListType) const
{
	CImageList* pIml = NULL;
	if (nImageListType == LVSIL_NORMAL)
	{
		if( m_pImageList )
		{
			ASSERT_VALID(m_pImageList);
			pIml = m_pImageList;
		}
	}
	else if (nImageListType == LVSIL_SMALL)
	{
		if( m_pSmallImageList )
		{
			ASSERT_VALID( m_pSmallImageList );
			pIml = m_pSmallImageList;
		}
	}
	else if (nImageListType == LVSIL_STATE) 
	{
		if( m_pStateImageList )
		{
			ASSERT_VALID( m_pStateImageList );
			pIml = m_pStateImageList;
		}
	}
	else if (nImageListType == LVSIL_HEADER)
	{
		if(m_pHeaderImageList)
		{
			ASSERT_VALID( m_pHeaderImageList);
			pIml = m_pHeaderImageList;
		}
	}
	else
	{
		ASSERT(FALSE) ; // Unknown type!
	}

	return pIml;
}

//////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Focus
//


void SEC_LISTCLASS::OnSetFocus(CWnd* pOldWnd) 
{
	SEC_LISTBASE::OnSetFocus(pOldWnd);
	
	//If we are in single select mode, we only want to 
	// redraw the item that is selected if there is one.	
	if( (GetListCtrlStyle() & LVS_SINGLESEL) && GetFocusIndex() != -1 )
		Update(GetFocusIndex(), TRUE, FALSE);
	else
		//Redraw the entire control.  Might have multiple items selected...
		Invalidate();

	NotifyNMHDR( NM_SETFOCUS );
}


void SEC_LISTCLASS::OnKillFocus( CWnd *pwndNew )
{
	if( pwndNew == m_pEditCtrl )
		return;

	if( m_pEditCtrl->GetSafeHwnd() )
		ProcessAndDestroyEditLabel();


	//If we are in single select mode, we only want to 
	// redraw the item that is selected if there is one.	
	if( (GetListCtrlStyle() & LVS_SINGLESEL) && GetFocusIndex() != -1 )
		Update(GetFocusIndex(), FALSE, FALSE);
	else
		//Redraw the entire control.  Might have multiple items selected...
		Invalidate();

	NotifyNMHDR( NM_KILLFOCUS );
}


void SEC_LISTCLASS::OnEnable( BOOL bEnable )
{
	if ( !bEnable )
		m_pToolTipWnd->Hide();

	Invalidate();

}

//////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Hit Testing & Mouse Handling
//

#define VACINITY_FACTOR 4


//@doc SEC_LISTCLASS
//@mfunc Determines what item is under a given point.
//@rdesc he index of the item, if any, that occupies the specified point; otherwise -1.
//@parm CPoint | pt | Client coordinates of the point to test.
//@parm  UINT* | pFlags | Pointer to an integer that receives information about the results of the hit test. See remarks.
//@parm  int& | nHitCol | The returned column thet the point is contained in; otherwise -1.
//@comm  The pFlags member will contain one of more of the following:
//	LVHT_ABOVE The position is above the client area of the control.
//
//	LVHT_BELOW The position is below the client area of the control.
//
//	LVHT_NOWHERE The position is inside the list view control's client window but is not over a list item.
//
//	LVHT_ONITEMICON The position is over a list view item's icon.
//
//	LVHT_ONITEMLABEL The position is over a list view item's text.
//
//	LVHT_ONITEMSTATEICON The position is over the state image of a list view item.
//
//	LVHT_TOLEFT The position is to the left of the list view control's client area.
//
//	LVHT_TORIGHT The position is to the right of the list view control's client area.

int SEC_LISTCLASS::HitTest(CPoint pt, UINT* pFlags, int& nHitCol) const
{
	Item* pItem = 0;
	Column* pCol = 0;
	int xColLeft, xColRight;
	CRect rcItem;
	RECT rcInside, rcClient;
	int nTopIndex;
	int nHitItem;
	int iSubItem;
	int i;

	*pFlags = 0;
	nHitItem = -1;
	nHitCol = -1;

	SECGetClientRect(&rcClient);


	GetInsideRect(&rcInside);


	// Hit outside of this window?
	if (pt.y < rcClient.top)
		*pFlags |= LVHT_ABOVE;
	if (pt.y > rcClient.bottom)
		*pFlags |= LVHT_BELOW;
	if (pt.x < rcClient.left)
		*pFlags |= LVHT_TOLEFT;
	if (pt.x > rcClient.right)
		*pFlags |= LVHT_TORIGHT;

	if (*pFlags != 0)
		return -1;

	nTopIndex = GetTopIndex();

	// At this point, the hit was definitely inside the window.
	// So, which column was hit?
	xColLeft = -m_ptViewportOrg.x;
	xColRight = xColLeft;

	if( GetColumnCount() != 0 )
	{
		if ( GetColumnCount() > 1)
		{
			for (i=0; i<GetColumnCount(); i++)
			{
				pCol = GetColumnAt(i);
				xColRight += GetColumnWidth(i);
				if (pt.x >= xColLeft && pt.x < xColRight)
				{
					nHitCol = i;
					break;
				}
				else
					xColLeft = xColLeft;
			}
		}
		else
		{
			pCol = GetColumnAt(0);
			nHitCol = 0;
		}
	}


	// Did the user click on a column header?
	if (pt.y >= rcClient.top && pt.y <= rcInside.top)
	{

		CRect rcCol;
		int nColCount;

		rcCol.top = rcClient.top;
		rcCol.left = -m_ptViewportOrg.x;
		rcCol.bottom = rcInside.top;
		rcCol.right = rcCol.left;

		nColCount = GetColumnCount();
		nColCount -= (GetListCtrlStyleEx()&LVXS_FITCOLUMNSONSIZE) ? 1 : 0;

		for (i=0; i<=nColCount; i++)
		{
			pCol = GetColumnAt(i);
			rcCol.right += GetColumnWidth(i);
			if (GetColumnCount() >= 1
				&& pt.x >= (rcCol.right - VACINITY_FACTOR)
				&& pt.x <= (rcCol.right + VACINITY_FACTOR))
			{
				*pFlags = LVHT_COLUMNSEP;
				nHitItem = i;
				break;
			}
			else if (rcCol.PtInRect(pt))
			{
				*pFlags = LVHT_COLUMNHEADING;
				nHitItem = i;
				break;
			}
			else
				rcCol.left = rcCol.right;
		}
	}
	else  // An item was definitely hit..  So which one?
	{

		rcItem.top = rcInside.top;
		rcItem.left = -m_ptViewportOrg.x;
		rcItem.right = rcInside.right;
		rcItem.bottom = rcItem.top;

		for (i=nTopIndex; rcItem.top < rcInside.bottom && i<GetItemCount(); i++)
		{
			pItem = GetItemAt(i);
			if (pItem->m_bDirty)
				const_cast<SEC_LISTCLASS*>(this)->MeasureItem(i);

			rcItem.top += GetMargin();
			rcItem.bottom += pItem->m_cy + GetMargin();


			if (rcItem.PtInRect(pt))
			{
				nHitItem = i;
				iSubItem = pCol->iSubItem;
				*pFlags |= LVHT_INSIDEITEMBOUNDS;
				if (iSubItem == 0)
				{
					CRect rcText, rcIcon, rcStateIcon;

					// Text rect
					rcText.top = rcItem.top;
					rcText.bottom = rcItem.bottom;
					rcText.left = xColLeft + pItem->rcText.left;
					rcText.right = rcText.left + pItem->rcText.Width();

					// State icon rect
					rcStateIcon.top = rcItem.top + pItem->rcStateIcon.top;
					rcStateIcon.bottom = rcStateIcon.top + pItem->rcStateIcon.Height();
					rcStateIcon.left = xColLeft + pItem->rcStateIcon.left;
					rcStateIcon.right = rcStateIcon.left + pItem->rcStateIcon.Width();

					// Icon rect
					rcIcon.top = rcItem.top + pItem->rcIcon.top;
					rcIcon.bottom = rcIcon.top + pItem->rcIcon.Height();
					rcIcon.left = xColLeft + pItem->rcIcon.left;
					rcIcon.right = rcIcon.left + pItem->rcIcon.Width();

					if (rcText.PtInRect(pt))
						*pFlags |= LVHT_ONITEMLABEL;
					else if (rcIcon.PtInRect(pt))
						*pFlags |= LVHT_ONITEMICON;
					else if (rcStateIcon.PtInRect(pt))
						*pFlags |= LVHT_ONITEMSTATEICON;
					else if (pt.x < rcText.left)
						*pFlags |= LVHT_ONITEMLEFT;	
					else if (pt.x >= rcText.right)
						*pFlags |= LVHT_ONITEMRIGHT;
				}
				break;
			}
			else
				rcItem.top = rcItem.bottom;
		}
	}

	if (*pFlags == 0)
		*pFlags = LVHT_NOWHERE;

	return nHitItem;
}


void SEC_LISTCLASS::ClearAllCy()
{
	Item* pItem = NULL;
	int i, c;

	c = GetItemCount();
	for (i=0; i<c; i++)
	{
		pItem = GetItemAt(i);
		if ( pItem )
		{
			pItem->m_bDirty = TRUE;
			pItem->m_cy = LVXC_UNINITIALIZED;
		}
	}
}

void SEC_LISTCLASS::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (GetFocus() != this)
		SetFocus();

	// lets see whats been hit
    UINT iHitFlags;
	int nHitIndex;
	int nHitCol;
    nHitIndex = HitTest( point, &iHitFlags, nHitCol );

	m_nActiveColumn = nHitCol;

	// Assume the click cannot be used to start dragging until we determine otherwise.
	m_bLookForBeginDrag = FALSE;

	BOOL bSelChange = FALSE;

	int nMaxDragColumn = GetColumnCount();
	if(GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE)
		--nMaxDragColumn;

	if (iHitFlags & LVHT_COLUMNSEP && 
		(nHitIndex+1) <= nMaxDragColumn)
	{
		Column* pCol = GetColumnAt(nHitIndex);
		int nDelta = TrackColumnResize(point, nHitIndex);
		if ( pCol )
			pCol->cx += nDelta;

		if (pCol && (GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE ) )
		{
			// Since we disallow resizing the very last column
			// via its right edge, there is no way the hit
			// column index could be equal to the last column.
			
			pCol = GetColumnAt(nHitIndex+1);
			pCol->cx -= nDelta;

			// Make sure the columns really fit.
			int i;
			int cxTotal = 0;
			for (i=0; i<GetColumnCount(); i++)
			{
				pCol = GetColumnAt(i);
				pCol->cx = otmax(pCol->cx, 5);
				cxTotal += pCol->cx;
			}

			CRect rcInside;
			GetInsideRect(rcInside);
			if (cxTotal > rcInside.Width())
			{
				pCol = GetColumnAt(0);
				pCol->cx -= (cxTotal - rcInside.Width());
			}
		}

		if(IsWordWrapEnabled())
			ClearAllCy();

		ReMeasureAllItems();
		Invalidate();
	}

	//Alright...didn't hit a column separator, so lets see if I hit and item.
	// This conditional, also lets me select an item if I am in full row select just by clicking
	// anywhere in the item's bounding rectangle
	else
	{
		ListHitTestEx( point, nHitIndex, iHitFlags, m_bLookForBeginDrag, bSelChange );
	}

	//If button header is enabled then draw the depressed header
	if( (!(GetListCtrlStyle() & LVS_NOSORTHEADER)) && (iHitFlags & LVHT_COLUMNHEADING) ) 
	{
		m_nBtnHdr = nHitIndex;
		DrawBtnHeader(nHitIndex,TRUE);

		//Capture the mouse so that we will know when the mouse leaves the control
		SetCapture();
	}

	// change the selection
	if (bSelChange)
	{
   		ProcessSelectChange(nHitIndex,iHitFlags,nFlags,ByMouse);
   		ExecuteSelectChange(nHitIndex, ByMouse);
	}

	// Remember lbutton down point for drag&drop logic in OnMouseMove
    m_ptButtonDown = point;

	// Redraw the tooltip
	m_pToolTipWnd->Refresh();
	
	// reset the dragging flag because if this is an OLE drag drop you
	// never get an OnLButtonUp
	if ( m_bDragging || m_bHdrDragging)		
	{
		m_bDragging = FALSE;
		m_bHdrDragging = FALSE;
	}

	m_nPendingSelect = -1;

	SEC_LISTBASE::OnLButtonDown(nFlags, point);
}


void SEC_LISTCLASS::ListHitTestEx( const CPoint& point, const int nHitIndex, 
												const UINT iHitFlags, 
												BOOL& bLookForBeginDrag, BOOL& bSelChange) 
{
	if( iHitFlags & (LVHT_ONITEMICON|LVHT_ONITEMLABEL) )
	{
		// Exact hit on the icon or label.
		bSelChange = TRUE;
		bLookForBeginDrag = TRUE;
	}
	else if( (GetListCtrlStyleEx() & LVS_EX_HEADERDRAGDROP) && (iHitFlags & LVHT_COLUMNHEADING) )
	{
		//We've hit a column header other than the first one; set drag-ready state
		if(nHitIndex > 0)
			bLookForBeginDrag = TRUE;
	}
	else if( (iHitFlags & LVHT_INSIDEITEMBOUNDS) &&  !( iHitFlags & LVHT_ONITEMSTATEICON) )
	{
		bSelChange = (GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) != 0;
  
		// User may have clicked between the icon and text.  
		// We want this to count as a selection click (and also as a valid drag point)!
		if( !bSelChange && (iHitFlags & LVHT_ONITEMLEFT) )
		{
			CRect  rcIcon;
 		    bSelChange = GetItemRect(nHitIndex, &rcIcon, LVIR_ICON) 
                  &&  point.x >= rcIcon.right;			
		} 
		bLookForBeginDrag = bSelChange;
	}
}


#define CX_MIN_BEGINDRAG 4
#define CY_MIN_BEGINDRAG 4

void SEC_LISTCLASS::OnMouseMove(UINT nFlags, CPoint point) 
{
    //If we are dragging...we want to eliminate all mouse move messages...
	// we do that by returning out of here...
	if( m_bDragging && !m_bHdrDragging)
		return;

	UINT iHitFlags;
	int nHitIndex;
	int nHitCol;

    nHitIndex = HitTest( point, &iHitFlags, nHitCol );

	int nMaxDragColumn = GetColumnCount();
	if(GetListCtrlStyleEx() & LVXS_FITCOLUMNSONSIZE)
		--nMaxDragColumn;

	if ((iHitFlags & LVHT_COLUMNSEP) && (nHitIndex < nMaxDragColumn))
		m_bColSepHit = TRUE;
	else
		m_bColSepHit = FALSE;

	//If header is in 'pressed' state and cursor is elsewhere then reset the header state
	if( (!(GetListCtrlStyle() & LVS_NOSORTHEADER)) && (!(iHitFlags & LVHT_COLUMNHEADING)) && 
		(m_nBtnHdr >= 0) && !m_bHdrDragging) 
	{	
		//Release the mouse capture
		if(GetCapture() == this)
			ReleaseCapture();		

		DrawBtnHeader(m_nBtnHdr,FALSE);

		if(m_bLookForBeginDrag)
			m_bLookForBeginDrag = FALSE;
		m_nBtnHdr = -1;
	}

	int nFocusIndex;

	nFocusIndex = GetFocusIndex();

	int nDragIndex = GetFocusIndex();

	if( (nDragIndex != -1) && m_bLookForBeginDrag && (!(iHitFlags&LVHT_COLUMNHEADING || m_bColSepHit))
			&& (nFlags&MK_LBUTTON || nFlags&MK_RBUTTON) )
	{
		//check the distance between 2 CPoint's
		CSize size = m_ptButtonDown -  point;
		if(     size.cx >=  CX_MIN_BEGINDRAG
			  ||  size.cy >=  CY_MIN_BEGINDRAG
			||  size.cx <= -CX_MIN_BEGINDRAG
			||  size.cy <= -CY_MIN_BEGINDRAG         )
		{
			// It's time to start dragging!
			// Don't send the notification just yet, though...
			m_bDragging = TRUE;
		    m_bLookForBeginDrag = FALSE;
		}
	}

	// Are we initiating a header drag operation? If so, then create and populate an image list with the 
	// header bitmap and invoke CImageList's dragging support
	if( (m_nBtnHdr >= 1) && m_bLookForBeginDrag && (nFlags&MK_LBUTTON) )
	{
		//check the distance between 2 CPoint's
		CSize size = m_ptButtonDown -  point;
		if(     size.cx >=  2
			  ||  size.cy >=  2
			||  size.cx <= -2
			||  size.cy <= -2         )
		{
			// Start dragging...
			m_bHdrDragging = TRUE;
			m_bLookForBeginDrag = FALSE;
			if(m_pHdrDragImageList == NULL)
			{				
				m_pHdrDragImageList = CreateHeaderDragImage(m_nBtnHdr);
				ASSERT_VALID(m_pHdrDragImageList);
				m_pHdrDragImageList->DragShowNolock(TRUE);

				int nX = point.x - GetColumnLeft(m_nBtnHdr);
				m_pHdrDragImageList->BeginDrag( 0, CPoint(nX, 0) );
			}			
		}
	}

	// If we are in a header drag operation then display the drag image so long as the cursor 
	// is within the header rect + a suitable offset(30 pixels)
	if( m_bHdrDragging && (nFlags & MK_LBUTTON) )
	{
		ASSERT_VALID(m_pHdrDragImageList);
		
		CRect rcHdr(0,0,0,0);
		GetInsideRect(rcHdr);
		rcHdr.left = 0;
		rcHdr.top = 0;
		rcHdr.bottom = m_cyHeader + 30;			

		int nInsertTemp =  m_nColInsert;
		if( rcHdr.PtInRect(point) )
		{
			m_pHdrDragImageList->DragEnter(this, CPoint(point.x,0));
			m_pHdrDragImageList->DragMove(CPoint(point.x,0));
			DrawHdrInsertionPoint(point, nHitCol);			

			if(nInsertTemp != m_nColInsert)
			{
				// Temporarily hide the drag image so that the window update is unlocked allowing the insert
				// point to be erased.
				m_pHdrDragImageList->DragLeave(this);
				InternalErasePrevInsert(nInsertTemp);
				m_pHdrDragImageList->DragEnter(this, CPoint(point.x,0));
			}
		}
		else  //the cursor has moved outside the previously set boundary. Hide the image.
			if( m_pHdrDragImageList->DragLeave(this) )			
				InternalErasePrevInsert(nInsertTemp);
	}


	if ( 
		 (GetListCtrlStyleEx() & LVXS_FLYBYTOOLTIPS) && 
		 (GetListCtrlStyle() & LVS_NOLABELWRAP)
	   )
	{
		if( m_bDragging || m_bHdrDragging)
		{
			// Always nuke the tooltip when dragging is about to begin.
			// Otherwise, it hangs around and obscures potential drop targets
			// in neighboring windows!
			m_pToolTipWnd->Hide();
		}
		else if (nHitIndex != m_nEditItem )
			m_pToolTipWnd->HookMouseMove(iHitFlags, nHitIndex, nHitCol, point);
		else
			m_pToolTipWnd->Hide();
	}

	// Okay, NOW we can send the Begin Drag notifications if we need to.
	if ( m_bDragging )
	{
			if (nFlags & MK_LBUTTON)
				NotifyBeginDrag(nDragIndex, m_ptButtonDown);
			else
				NotifyBeginRDrag(nDragIndex, m_ptButtonDown);
	}


	SEC_LISTBASE::OnMouseMove(nFlags, point);
}

void SEC_LISTCLASS::DrawHdrInsertionPoint(CPoint point, int nHitCol) 
{
	// If the cursor is over the column overhang, then set nHitCol to be index of the last column
	if(nHitCol == -1)
		nHitCol = GetColumnCount()-1;
	else if(nHitCol == 0)
		nHitCol = 1;

	CRect rcColHit(0,0,0,0);
	rcColHit.bottom = m_cyHeader;
	rcColHit.left = GetColumnLeft(nHitCol);
	rcColHit.right = GetColumnRight(nHitCol);
	CPoint ptCenter = rcColHit.CenterPoint();

	CClientDC dc(this);

	// The insertion point has to be on either side of nHitCol. Determine the closest column
	// separator and make this our insertion point
	if(point.x < ptCenter.x)		// Insertion point is on nHitCol's left separator.
	{
		// An initial check that determines if insertion rect is for the first column. If TRUE, 
		// then do not draw the insertion rect as repositioning column 0 does not make sense 
		// in the SECTreeCtrl's context.
		if(point.x >= GetColumnWidth(0)/2)	
		{
			dc.FillSolidRect( CRect(rcColHit.left,0,rcColHit.left+1,rcColHit.bottom), RGB(0,0,255) );
			m_nColInsert = nHitCol;
		}
	}
	else				//Insertion point is on nHitCol's right border
	{
		dc.FillSolidRect( CRect(rcColHit.right,0,rcColHit.right+1,rcColHit.bottom), RGB(0,0,255) );
		m_nColInsert = nHitCol+1;	
	}
}


void SEC_LISTCLASS::InternalErasePrevInsert(int nInsertTemp)
{	
	CClientDC dc(this);
	if( nInsertTemp >= GetColumnCount() )
	{
		CRect rcClient, rcColOver;
		GetInsideRect(&rcClient);

		rcColOver.left = GetColumnRight(nInsertTemp-1);
		rcColOver.top = 0;
		rcColOver.right = rcClient.right+2;
		rcColOver.bottom = m_cyHeader;

		CScrollBar* pVertSB = GetScrollBarCtrl(SB_VERT);
		if(pVertSB)
		{
			CRect rcScroll;
			pVertSB->GetWindowRect(&rcScroll);
			ScreenToClient(&rcScroll);
			CRect rcIntSect = rcColOver & rcScroll;
			if(!rcIntSect.IsRectEmpty())
				dc.ExcludeClipRect(&rcScroll);
		}
	
		// Draw the overhanging portion of the header that is present after the last column.
		dc.DrawFrameControl(&rcColOver, DFC_BUTTON, DFCS_BUTTONPUSH );

		return;
	}

	if( (nInsertTemp > 0) && (nInsertTemp < GetColumnCount()) )
	{
		if(nInsertTemp == m_nBtnHdr)
			DrawBtnHeader(m_nBtnHdr, TRUE);
		else
			DrawBtnHeader(nInsertTemp, FALSE);
	}
}


void SEC_LISTCLASS::OnLButtonUp(UINT nFlags, CPoint point) 
{
    //If we were dragging...just stop and return...we don't want to select or
	// anything like that...
	m_bLookForBeginDrag = FALSE;

	if(m_bDragging)
	{
		m_bDragging = FALSE;
		return;
	}

	if(m_bHdrDragging )
	{
		m_bHdrDragging = FALSE;

		if(m_pHdrDragImageList)
		{
			m_pHdrDragImageList->EndDrag();
			delete m_pHdrDragImageList;
			m_pHdrDragImageList = NULL;
			//Delete any insertion rect traces that may have been left over
			InternalErasePrevInsert(m_nColInsert);
		}

		//The header is pressed. Release the mouse and redraw the header in it's normal state.
		if(GetCapture() == this)
			ReleaseCapture();
		DrawBtnHeader(m_nBtnHdr, FALSE);

		// The drag operation is complete. Verify whether the insertion point is on either side of the 
		// column being dragged and, if found otherwise, perform the 'drop'.
		if( (m_nColInsert != m_nBtnHdr) && (m_nColInsert != m_nBtnHdr+1) )
			RepositionColumn(m_nBtnHdr, m_nColInsert, TRUE);		

		m_nBtnHdr = -1;
		return;
	}

	// If header is currently depressed then undo this effect
	if( (!(GetListCtrlStyle() & LVS_NOSORTHEADER)) && (m_nBtnHdr >= 0) ) 
	{
		//We have the mouse captured. Release it as this is no longer needed
		if(GetCapture() == this)
			ReleaseCapture();
		DrawBtnHeader(m_nBtnHdr, FALSE);

		//Send a NM_COLUMNCLICK notification to the target window 
		NotifyColumnClick(m_nBtnHdr);

		m_nBtnHdr = -1;		
	}

	UINT iHitFlags;
    int nHitIndex;
	int nHitCol;
    nHitIndex = HitTest( point, &iHitFlags, nHitCol );

	// check for multiple selection. Behavior should be that a left button up
	// on a selected item will single select that item
	if(IsSelected(nHitIndex) && 
		(GetSelectedCount()>1) && 
		(nFlags & (MK_CONTROL|MK_SHIFT)) == 0 )
	{
		DeselectAllItems();
   		ProcessSelectChange(nHitIndex,iHitFlags,nFlags,ByMouse);
	}
	// NEW -  if Control key pressed, toggle selection
	if(nFlags & MK_CONTROL)
	{
		ProcessSelectChange(nHitIndex, iHitFlags, nFlags, ByMouse);
	}
	//  END NEW

	BOOL  bAffectsSelection = FALSE;

	if( iHitFlags & (LVHT_ONITEMICON|LVHT_ONITEMLABEL) )
	{
		// Exact hit on the icon or label.
		bAffectsSelection = TRUE;
 	}
	else if( iHitFlags & LVHT_INSIDEITEMBOUNDS )
	{
		bAffectsSelection = (GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) != 0;
  
		// User may have released between the icon and text.  
		// We want this to count as a selection release!
		if( ! bAffectsSelection && (iHitFlags & LVHT_ONITEMLEFT) )
		{
		CRect  rcIcon;
	    bAffectsSelection =   GetItemRect(nHitIndex, &rcIcon, LVIR_ICON) 
                      &&  point.x >= rcIcon.right;
		} // if <maybe between icon and text>
	}

	if( bAffectsSelection )
	{

		if (m_nPendingEdit != -1 && m_nPendingEdit == nHitIndex)
		{
			if( m_idEditLabelTimer )
				KillTimer( m_idEditLabelTimer );
			m_idEditLabelTimer = SetTimer( 1, 500, NULL );
		}
	}

	
	if( (GetListCtrlStyle() & LVS_EDITLABELS)	&&
		(IsSelected(nHitIndex))	&&
		( (iHitFlags & LVHT_ONITEMLABEL) ||
		((GetListCtrlStyleEx() & LVXS_HILIGHTSUBITEMS) && (iHitFlags & LVHT_INSIDEITEMBOUNDS)
		 && (m_nActiveColumn > 0) && (nHitCol > 0) ) )
	 )
	{
		m_nPendingEdit = nHitIndex;
	}
	else
		m_nPendingEdit = -1;


	SEC_LISTBASE::OnLButtonUp(nFlags, point);


	NotifyNMHDR( NM_CLICK );

}


//@doc SEC_LISTCLASS
//@mfunc Reorders columns 1 to n; where '1' is the index of the first non-root column and 'n' is the total number of columns in the tree/list.
//@rdesc TRUE if successful; otherwise FALSE.
//@parm  int | nColIndexOld | The current sub-index of the column to be reordered.
//@parm  int | nColIndexNew | The new index for the column.
//@parm  BOOL | bUpdate | If TRUE then the control will be invalidated. Default is TRUE.
BOOL SEC_LISTCLASS::RepositionColumn(int nColIndexOld, int nColIndexNew, BOOL bUpdate)		
{
	ASSERT_VALID(this);
	if( (nColIndexOld <= 0) || (nColIndexNew <= 0) )
	{
		// Current SECTreeCtrl design does not allow the root column to be moved. We can remove this constraint
		// when the tree control is improved or the SECListCtrl becomes a separate entity. 
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bFlag = FALSE;
	if( nColIndexNew >= GetColumnCount() )
	{
		nColIndexNew = GetColumnCount()-1;
		bFlag = TRUE;
	}

    // Get column to reinsert
	Column* pColDrop = GetColumnAt(nColIndexOld);
	if ( !pColDrop )
	{
		TRACE0("Invalid column. Drop operation unsuccessful.\n");
		return FALSE;
	}

	int nOldIndex = nColIndexOld;	
	
	// Reinsert the column at the new position
	m_arrColumns.RemoveAt(nColIndexOld);
	if( (nColIndexNew > nOldIndex) && (bFlag != TRUE) )
	{
		m_arrColumns.InsertAt(nColIndexNew-1, pColDrop);
		pColDrop->iSubItem = nColIndexNew-1;
	}
	else
	{
		m_arrColumns.InsertAt(nColIndexNew, pColDrop);
		pColDrop->iSubItem = nColIndexNew;
	}
	// Run through all the columns and reorder the sub-item index to match the new positions 
	Column* pCol = NULL;
	for ( int nIndex = 1; nIndex < GetColumnCount(); nIndex++ )
	{
		pCol = GetColumnAt(nIndex);
		if (pCol)
			pCol->iSubItem = nIndex;
	}
	
	//If sub-item storage is enabled, then provide the SECTreeCtrl a chance to update it's internal buffer 
	InternalUpdateColumnText(nOldIndex, pColDrop->iSubItem );

	if(bUpdate)
	{
		ReMeasureAllItems();
		Invalidate();
	}

	return TRUE;
}


void SEC_LISTCLASS::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_nPendingEdit != -1)
	{
		m_nPendingEdit = -1;
		if( m_idEditLabelTimer )
			KillTimer( m_idEditLabelTimer );
		m_idEditLabelTimer = 0;
	}
    SEC_LISTBASE::OnLButtonDblClk(nFlags, point);
	NotifyNMHDR( NM_DBLCLK );
}

#ifndef UNDER_CE
void SEC_LISTCLASS::OnRButtonDown(UINT nFlags, CPoint point)
{

    if(m_pToolTipWnd->GetSafeHwnd() && m_pToolTipWnd->IsWindowVisible())
    {
        CWnd* pWndTop = GetParentFrame();
        SendMessage(WM_MOUSEACTIVATE, WPARAM(pWndTop->m_hWnd), MAKELPARAM(HTCLIENT, WM_RBUTTONDOWN));
    }	

    if( GetFocus() != this )
		SetFocus();

	// lets see whats been hit
    UINT iHitFlags;
	int nHitIndex;
	int nHitCol;
    nHitIndex = HitTest( point, &iHitFlags, nHitCol );

	// Assume the click cannot be used to start dragging until we determine otherwise.
	m_bLookForBeginDrag = FALSE;

	BOOL bSelChange = FALSE;

	if (!(iHitFlags & LVHT_COLUMNSEP))
	{
		ListHitTestEx( point, nHitIndex, iHitFlags, m_bLookForBeginDrag, bSelChange );
	}

	// change the selection
	if (bSelChange)
	{
   		ProcessSelectChange(nHitIndex,iHitFlags,nFlags,ByMouse);
   		ExecuteSelectChange(nHitIndex, ByMouse);
	}


	// Remember lbutton down point for drag&drop logic in OnMouseMove
    m_ptButtonDown = point;

	// Redraw the tooltip
	m_pToolTipWnd->Refresh();
	
	// reset the dragging flag because if this is an OLE drag drop you
	// never get an OnLButtonUp
	if ( m_bDragging )
		m_bDragging = FALSE;
	

	SEC_LISTBASE::OnRButtonDown(nFlags, point);
}


void SEC_LISTCLASS::OnRButtonUp( UINT nFlags, CPoint point )
{
	//If we are dragging we don't want to do anything but stop dragging
	m_bLookForBeginDrag = FALSE;
	if( m_bDragging )
	{
		m_bDragging = FALSE;
		return;
	}

	m_pToolTipWnd->Refresh();
	SEC_LISTBASE::OnRButtonUp(nFlags, point);
	NotifyNMHDR( NM_RCLICK );
}
#endif

BOOL SEC_LISTCLASS::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT && pWnd == this && m_bColSepHit)
	{
		::SetCursor(m_hcurSplit);
		return FALSE;
	}
	else	
		return SEC_LISTBASE::OnSetCursor(pWnd, nHitTest, message);
}

#define MIN_COL_WIDTH 10


int SEC_LISTCLASS::TrackColumnResize(CPoint pt, int nCol)
{
	int xMouseDown;

	// If there's no column header, this function shouldn't be called.
	ASSERT(IsHeaderCtrlEnabled() == TRUE);

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	// set capture to the window which received this message
	SetCapture();
	ASSERT(CWnd::GetCapture() == this);
	UpdateWindow();

	xMouseDown = pt.x;

	AfxLockTempMaps();  // protect maps while looping

	CClientDC dc(this);
	CRect rcInside;
	CRect rcDrag;
	CRect rcDragLast;
	CSize szDrag(1,1);
	CSize szLog;
	int xColLeft;

	GetInsideRect(rcInside);
	rcDrag.top = 0;
	rcDrag.bottom = rcInside.bottom;
	szLog = GetLogicalSize();
	xColLeft = rcDrag.left = GetColumnLeft(nCol);
	rcDrag.right = rcDrag.left + 2;
	rcDragLast.SetRectEmpty();

	// Drag left, static resize rect
	if (nCol > 0) {
		dc.DrawDragRect(rcDrag, szDrag, NULL, szDrag);
		rcDragLast=rcDrag;
		}

	// Draw right, moving resize rect
	rcDrag.left += GetColumnWidth(nCol);
	rcDrag.right = rcDrag.left + 2;
	dc.DrawDragRect(rcDrag, szDrag, rcDragLast, szDrag);
	rcDragLast = rcDrag;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
			pt.x = (int)(short)LOWORD(msg.lParam);
			goto ExitLoop;

		case WM_MOUSEMOVE:
			pt.x = (int)(short)LOWORD(msg.lParam);
			if (pt.x < (xColLeft + MIN_COL_WIDTH))
				pt.x = xColLeft + MIN_COL_WIDTH;
			rcDrag.left = pt.x;
			rcDrag.right = rcDrag.left + 2;
			dc.DrawDragRect(rcDrag, szDrag, rcDragLast, szDrag);
			rcDragLast = rcDrag;
			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			pt.x = xMouseDown;
			goto ExitLoop;

			// just dispatch rest of the messages
		default:
			::DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	ReleaseCapture();

#if (_MFC_VER > 0x0420)
	AfxUnlockTempMaps(FALSE);
#else
	AfxUnlockTempMaps();
#endif

	if (pt.x < (xColLeft + MIN_COL_WIDTH))
		pt.x = xColLeft + MIN_COL_WIDTH;

	return pt.x - xMouseDown;
}

//////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Keyboard Handling
//


UINT SEC_LISTCLASS::OnGetDlgCode()
{
	//
	// This function tells windows that we want to handle all keyboard
	// events.
	//
	return DLGC_WANTARROWS|DLGC_WANTCHARS|DLGC_WANTTAB|DLGC_WANTALLKEYS;
}

void SEC_LISTCLASS::OnSpaceKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nSubItem;  // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	UINT iKeyFlags = 0;
	iKeyFlags |= (bShift ? MK_SHIFT : 0 ) | ( bCtrl ? MK_CONTROL : 0 );

	if ( ProcessSelectChange( nItem, TVHT_ONITEM, iKeyFlags, ByKeyboard ) )
	{
		ExecuteSelectChange( nItem, ByKeyboard );
		EnsureVisible( nItem );
	}
}

void SEC_LISTCLASS::OnUpKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nSubItem;  // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED


	if ( nItem > 0 )
	{
		nItem--;
		UINT iKeyFlags = 0;
		iKeyFlags |= (bShift ? MK_SHIFT : 0 ) | ( bCtrl ? MK_CONTROL : 0 );

		if ( ProcessSelectChange( nItem, TVHT_ONITEM, iKeyFlags, ByKeyboard ) )
		{
			ExecuteSelectChange( nItem, ByKeyboard );
			EnsureVisible( nItem );
		}
	}



}


void SEC_LISTCLASS::OnDownKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nSubItem;  // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	if (nItem < (GetItemCount()-1))
	{
		nItem++;
		UINT iKeyFlags = 0;
		iKeyFlags |= (bShift ? MK_SHIFT : 0 ) | ( bCtrl ? MK_CONTROL : 0 );

		if ( ProcessSelectChange( nItem, TVHT_ONITEM, iKeyFlags, ByKeyboard ) )
		{
			ExecuteSelectChange( nItem, ByKeyboard );
			EnsureVisible( nItem );
		}

	}


}


void SEC_LISTCLASS::ExecuteSelectChange(int nItem, SelectAction sa)
{
	nItem;
	sa;
}



void SEC_LISTCLASS::OnF2KeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	if (-1==nItem)
	{	// How can we edit a non-existent item?
		return;
	}
	if( GetListCtrlStyle() & LVS_EDITLABELS )
		EditLabel(nItem, m_nActiveColumn);
}


void SEC_LISTCLASS::OnReturnKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	NotifyNMHDR( NM_RETURN );
}


void SEC_LISTCLASS::OnHomeKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	nSubItem;  // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	UINT iFlags = 0;
	if ( bShift )
		iFlags |= MK_SHIFT;
	if ( bCtrl )
		iFlags |= MK_CONTROL;


	if ( GetItemCount() > 0 )
	{
		// scroll
		BOOL bVScroll = !!(m_dwScrollStyle & WS_VSCROLL);
		if ( bVScroll )
		{
			CScrollBar* pBar = GetScrollBarCtrl( SB_VERT );
			if ( pBar )
			{
				SendMessage( WM_VSCROLL, MAKEWPARAM( SB_TOP, 0), (LPARAM)pBar->m_hWnd );
				Invalidate();
			}
		}

		int newItem = 0;
		ProcessSelectChange( newItem, TVHT_ONITEM, iFlags, ByKeyboard );
		ExecuteSelectChange( newItem, ByKeyboard );
		Invalidate();
	}
}


void SEC_LISTCLASS::OnPgUpKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	BOOL bVScroll = !!(m_dwScrollStyle & WS_VSCROLL);
	
	UINT iFlags = 0;
	if ( bShift )
		iFlags |= MK_SHIFT;
	if ( bCtrl )
		iFlags |= MK_CONTROL;



	if ( !bVScroll )
		OnHomeKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
	else
	{
		CScrollBar* pBar = GetScrollBarCtrl( SB_VERT );
		// scroll up if the top item is selected
		if ( pBar && (nItem == GetTopIndex()) )
			SendMessage( WM_VSCROLL, MAKEWPARAM( SB_PAGEUP, 0), (LPARAM)pBar->m_hWnd );

		int newItem = GetTopIndex();
		ProcessSelectChange( newItem, TVHT_ONITEM, iFlags, ByKeyboard );
		ExecuteSelectChange( newItem, ByKeyboard );
		Invalidate();
	}
}


void SEC_LISTCLASS::OnPgDnKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	BOOL bVScroll = !!(m_dwScrollStyle & WS_VSCROLL);

	UINT iFlags = 0;
	if ( bShift )
		iFlags |= MK_SHIFT;
	if ( bCtrl )
		iFlags |= MK_CONTROL;

	if ( !bVScroll )
		OnEndKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
	else
	{
		CScrollBar* pBar = GetScrollBarCtrl( SB_VERT );
		// scroll down one page if the last visible item is selected
		if ( pBar && (nItem == GetLastFullyVisibleItem()) )
			SendMessage( WM_VSCROLL, MAKEWPARAM( SB_PAGEDOWN, 0), (LPARAM)pBar->m_hWnd );

		int newItem = GetLastFullyVisibleItem();
		ProcessSelectChange( newItem, TVHT_ONITEM, iFlags, ByKeyboard );
		ExecuteSelectChange( newItem, ByKeyboard );
		Invalidate();
	}

		
}



void SEC_LISTCLASS::OnEndKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	nSubItem;  // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

	UINT iFlags = 0;
	if ( bShift )
		iFlags |= MK_SHIFT;
	if ( bCtrl )
		iFlags |= MK_CONTROL;


	if (GetItemCount() > 0)
	{
		// scroll
		BOOL bVScroll = !!(m_dwScrollStyle & WS_VSCROLL);
		if ( bVScroll )
		{
			CScrollBar* pBar = GetScrollBarCtrl( SB_VERT );
			if ( pBar )
			{
				SendMessage( WM_VSCROLL, MAKEWPARAM( SB_BOTTOM, 0), (LPARAM)pBar->m_hWnd );
				Invalidate();
			}
		}

		int newItem = GetItemCount() - 1;;
		ProcessSelectChange( newItem, TVHT_ONITEM, iFlags, ByKeyboard );
		ExecuteSelectChange( newItem, ByKeyboard );
		Invalidate();


	}
}


void SEC_LISTCLASS::OnLeftKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

}


void SEC_LISTCLASS::OnBackKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

}


void SEC_LISTCLASS::OnRightKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

}


void SEC_LISTCLASS::OnMultKeyPressed(int nItem, int nSubItem, BOOL bShift, BOOL bCtrl, UINT nRepCnt, UINT nFlags)
{
	nItem; // UNUSED
	bShift; // UNUSED
	nSubItem;  // UNUSED
	bCtrl; // UNUSED
	nRepCnt; // UNUSED
	nFlags; // UNUSED

}

void SEC_LISTCLASS::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_uiAutoKeySelectionMode & AutoKeySelectionDisable || nChar == VK_ESCAPE 
		|| nChar == VK_RETURN || nChar == VK_SPACE || GetItemCount() <= 0)
	{
		SEC_LISTBASE::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	// User entered an invalid key so keep the mode on but ignore further keys.
	if( m_uiAutoKeySelectionMode & AutoKeySelectionIgnore )
	{
		m_uiAutoKeySelectionMode |= AutoKeySelecting;
		return;
	}
	// If this is the first key, turn on the mode
	if(!(m_uiAutoKeySelectionMode & AutoKeySelectionOn ))
	{
		m_uiAutoKeySelectionMode |= AutoKeySelectionOn;
		m_strAutoKeySelectString.Empty();
		// Start Timer
		if( m_idAutoKeySelectTimer )
			KillTimer( m_idAutoKeySelectTimer );
		m_idAutoKeySelectTimer = SetTimer( 3, m_nAutoKeySelectionTimerInterval, NULL );
	}
	m_strAutoKeySelectString += (TCHAR)nChar;
	int nCurItem = GetFocusIndex();
	if(m_strAutoKeySelectString.GetLength()<=1)		// If first key
	{
		// Calculate the item from which to begin the search
		nCurItem = GetNextItem(nCurItem, LVNI_BELOW);
		if(nCurItem == -1)		// Loop back to the beginning
			nCurItem = GetNextItem(-1, LVNI_BELOW);
	}
	if(!TryMovingTo(nCurItem, m_strAutoKeySelectString))
	{
		// Failed to find match; 
		MessageBeep((UINT) -1);
		// Keep selection mode on but ignore further keys
		m_uiAutoKeySelectionMode |= AutoKeySelectionIgnore;	
		m_uiAutoKeySelectionMode |= AutoKeySelecting;
	}
	else
		m_uiAutoKeySelectionMode |= AutoKeySelecting;
}

BOOL SEC_LISTCLASS::TryMovingTo(int nFromItem, CString strLike)
{
	int nItem = GetNextMatchingItem(nFromItem, m_strAutoKeySelectString);
	if (nItem != -1)	// Match found
	{
		if ( ProcessSelectChange( nItem, TVHT_ONITEM, 0, ByKeyboard ) )
		{
			ExecuteSelectChange( nItem, ByKeyboard );
			EnsureVisible( nItem );
		}
		return TRUE;
	}
	return FALSE;
}

int SEC_LISTCLASS::GetNextMatchingItem(int nFromItem, CString strLike)
{
	strLike.MakeLower();
	int nItem = nFromItem;
	while(TRUE)
	{
		CString strText = GetItemText(nItem, 0);
		strText.MakeLower();
		if(strText.Find(strLike) == 0)	// Does the left most chars match?
			break;
		// Get next item to check.
		nItem = GetNextItem(nItem, LVNI_BELOW);
		if(nItem == -1)		// Loop back to the beginning
			nItem = GetNextItem(-1, LVNI_BELOW);
		if(nItem == nFromItem)	// Checked all the nodes, no match found.
			return -1;
	}
	// match found
	return nItem;
}

void SEC_LISTCLASS::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	NotifyKeyDown(nChar);

	Column* pCol = 0;
	int nSubItem = 0;

	BOOL bShift = GetKeyState(VK_SHIFT) < 0;
	BOOL bCtrl = GetKeyState(VK_CONTROL) < 0;
	int nItem = GetFocusIndex();

	pCol = GetColumnAt(m_nActiveColumn);
	if ( pCol )
		nSubItem = pCol->iSubItem;

	CWnd* pWnd = 0;
	switch(nChar)
	{
	case VK_ESCAPE:	// do some work normally done by the dialog
		pWnd = GetParent();
		if ( pWnd )
			pWnd->PostMessage( WM_COMMAND, (WPARAM)IDCANCEL, 0);
		break;
	case VK_MULTIPLY:
		OnMultKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_SPACE:
		OnSpaceKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_TAB:
		pWnd = GetParent();
		if ( pWnd )
			pWnd->PostMessage( WM_NEXTDLGCTL, (WPARAM)bShift, (LPARAM)FALSE );
		break;
	case VK_UP:
		OnUpKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_DOWN:
		OnDownKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_F2:
		OnF2KeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_RETURN:
		OnReturnKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_HOME:
		OnHomeKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_END:
		OnEndKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_LEFT:
		OnLeftKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_BACK:
		OnBackKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_RIGHT:
		OnRightKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case 0xBB:
	case VK_ADD:
		OnRightKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case 0xBD:
	case VK_SUBTRACT:
		OnLeftKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break ;
	case VK_PRIOR:
		OnPgUpKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	case VK_NEXT:
		OnPgDnKeyPressed(nItem, nSubItem, bShift, bCtrl, nRepCnt, nFlags);
		break;
	default:
		break;
	}
	SEC_LISTBASE::OnKeyDown(nChar,nRepCnt,nFlags);
}


void SEC_LISTCLASS::OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	NotifyKeyDown(nChar);
	SEC_LISTBASE::OnSysKeyDown( nChar, nRepCnt, nFlags );
}


//////////////////////////////////////////////////////////////////////////
//
// SEC_LISTCLASS Notifications
//


void SEC_LISTCLASS::NotifyBeginDrag(int nDragItem, POINT ptDrag)
{
	nDragItem; // UNUSED
	ptDrag; // UNUSED
}


BOOL SEC_LISTCLASS::NotifyBeginLabelEdit(int nEditItem, int iSubItem) const
{
	LV_DISPINFO lvdi;

	// Validate params
	ASSERT(nEditItem >= 0 && nEditItem < GetItemCount());

	// fill a NMHDR
	lvdi.hdr.hwndFrom = GetSafeHwnd();
	lvdi.hdr.idFrom   = GetDlgCtrlID();
	lvdi.hdr.code	  = LVN_BEGINLABELEDIT;
	
	Item* pItem = GetItemAt( nEditItem );

	lvdi.item.iItem = nEditItem;
	lvdi.item.iSubItem = iSubItem;
	lvdi.item.mask |= LVIF_TEXT;
	if ( pItem )
		lvdi.item.lParam = GetUserParam( pItem );


	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	return (BOOL)(pwndParent->SendMessage( WM_NOTIFY,
							 GetDlgCtrlID(), //LVN_BEGINLABELEDIT
							 (LPARAM)&lvdi ));
}


void SEC_LISTCLASS::NotifyBeginRDrag(int nDragItem, POINT ptDrag)
{
	nDragItem; // UNUSED
	ptDrag; // UNUSED
}


void SEC_LISTCLASS::NotifyDeleteItem(int nDelItem) const
{
	NM_LISTVIEW nmlv;

	// fill a NMHDR
	nmlv.hdr.hwndFrom = GetSafeHwnd();
	nmlv.hdr.idFrom   = GetDlgCtrlID();
	nmlv.hdr.code	  = LVN_DELETEITEM;
	
	//fill out specific to a treectrl
	nmlv.iItem = nDelItem;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //LVN_DELETEITEM
							 (LPARAM)&nmlv);
}


BOOL SEC_LISTCLASS::NotifyEndLabelEdit(int nEditItem, int iSubItem, CString& strEdit, BOOL bCommit) const
{
	LV_DISPINFO lvdi;
	::ZeroMemory( &lvdi, sizeof(LV_DISPINFO));
	// Validate params
	ASSERT(nEditItem >= 0 && nEditItem < GetItemCount());

	// fill a NMHDR
	lvdi.hdr.hwndFrom = GetSafeHwnd();
	lvdi.hdr.idFrom   = GetDlgCtrlID();
	lvdi.hdr.code	  = LVN_ENDLABELEDIT;

	Item* pItem = GetItemAt( nEditItem );

	lvdi.item.iItem = nEditItem;
	lvdi.item.iSubItem = iSubItem;
	lvdi.item.mask |= LVIF_TEXT;
	if ( pItem )
		lvdi.item.lParam = GetUserParam( pItem );
	

	LPTSTR pszInternalBuffer = NULL;
	// Make a copy of the edited string so the handler can alter it too
	if (bCommit)
	{
		pszInternalBuffer = new TCHAR[LVXC_INTERNALBUFFERSIZE];
		_tcscpy(pszInternalBuffer, (LPCTSTR)strEdit);
		lvdi.item.pszText = pszInternalBuffer;
	}
	else
		lvdi.item.pszText = NULL;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	LRESULT result = pwndParent->SendMessage( WM_NOTIFY,
							 GetDlgCtrlID(), //LVN_ENDLABELEDIT
							 (LPARAM)&lvdi );

	// Store changes to edited string made by handler
	BOOL bRet;
	if (result != 0 && bCommit)
	{
		strEdit = lvdi.item.pszText;
		bRet = TRUE;
	}
	else
		bRet = FALSE;

	if ( pszInternalBuffer )
		delete [] pszInternalBuffer;

	return bRet;
}

void SEC_LISTCLASS::NotifyEndLabelEditChangeComplete(int nEditItem,int iSubItem) const {
	nEditItem; iSubItem;	// UNUSED
}

void SEC_LISTCLASS::NotifySelChanged(int nNewItem, int nOldItem, SelectAction sa) const
{
	nNewItem; // UNUSED
	nOldItem; // UNUSED
	sa; // UNUSED
}


BOOL SEC_LISTCLASS::NotifySelChanging(int nNewItem, int nOldItem, SelectAction sa) const
{
	nNewItem; // UNUSED
	nOldItem; // UNUSED
	sa; // UNUSED
	return FALSE;
}


void SEC_LISTCLASS::NotifySetDispInfo(const LV_ITEM* lvi)
{
	lvi; // UNUSED
}


void SEC_LISTCLASS::NotifySetDispInfoEx(const LV_ITEM_EX* lvix) const
{
	lvix; // UNUSED
}


void SEC_LISTCLASS::NotifyNMHDR( UINT wNotify )
{
    CWnd *pwndParent = NotifyWnd();

    if( !pwndParent )
		return;

	NMHDR       nmhdr;

	// fill a NM_TREEVIEW
	memset( &nmhdr, 0, sizeof(NMHDR) );

	// fill a NMHDR
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom   = GetDlgCtrlID();
	nmhdr.code	   = wNotify;

	//Send notification message to parent window
	m_lastNotifyResult = pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), 
							 (LPARAM)&nmhdr );
}


LPARAM SEC_LISTCLASS::GetUserParam(Item* pItem) const
{
	return pItem->lParam;
}


void SEC_LISTCLASS::NotifyGetDispInfo(LV_ITEM* lvi) const
{
	LV_DISPINFO lvdi;

	// fill a NMHDR
	lvdi.hdr.hwndFrom = GetSafeHwnd();
	lvdi.hdr.idFrom   = GetDlgCtrlID();
	lvdi.hdr.code	  = LVN_GETDISPINFO;
	
	//fill out specific to a treectrl
	lvdi.item = *lvi;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //LVN_GETDISPINFO
							 (LPARAM)&lvdi );

	*lvi = lvdi.item;
}


void SEC_LISTCLASS::NotifyGetDispInfoEx(LV_ITEM_EX* lvix) const
{
	lvix; // UNUSED
}


void SEC_LISTCLASS::NotifyInsertItem(int nItem) const
{
	NM_LISTVIEW nmlv;

	// fill a NMHDR
	nmlv.hdr.hwndFrom = GetSafeHwnd();
	nmlv.hdr.idFrom   = GetDlgCtrlID();
	nmlv.hdr.code	  = LVN_INSERTITEM;
	
	//fill out specific to a treectrl
	nmlv.iItem = nItem;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //LVN_INSERTITEM
							 (LPARAM)&nmlv);
}


void SEC_LISTCLASS::NotifyKeyDown(UINT nChar) const
{
	LV_KEYDOWN lvkd;

	// fill a NMHDR
	lvkd.hdr.hwndFrom = GetSafeHwnd();
	lvkd.hdr.idFrom   = GetDlgCtrlID();
	lvkd.hdr.code	  = LVN_KEYDOWN;
	
	//fill out specific to a treectrl
	lvkd.wVKey = (WORD)nChar;
	lvkd.flags = 0;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //LVN_KEYDOWN
							 (LPARAM)&lvkd );
}


void SEC_LISTCLASS::NotifyColumnClick(int nColHit) const
{
	NMLISTVIEW nmlv;
	memset(&nmlv,0,sizeof(NMLISTVIEW));
	
	//The column index is the only usable value
	nmlv.hdr.hwndFrom = GetSafeHwnd();
	nmlv.hdr.idFrom = GetDlgCtrlID();
	nmlv.hdr.code = LVN_COLUMNCLICK;

	nmlv.iItem = -1;
	nmlv.iSubItem = nColHit;

	//Send notification message to parent window
	CWnd *pwndParent = NotifyWnd();
	pwndParent->SendMessage( WM_NOTIFY,
	                         GetDlgCtrlID(), //LVN_COLUMNCLICK
							 (LPARAM)&nmlv );
}


//Since it is possible to have a NULL notification window
// I have a special function which I use that will return a valid
// CWnd pointer to use.

CWnd* SEC_LISTCLASS::NotifyWnd() const
{
	CWnd* pNotifyWnd = GetNotifyWnd();
	
	if( pNotifyWnd == NULL )
		pNotifyWnd = GetParent();
	
	ASSERT_VALID(pNotifyWnd);

	return pNotifyWnd;
}


BOOL SEC_LISTCLASS::ProcessSelectChange
	(
		int nHitIndex,
		UINT iHitFlags,
		UINT nFlags,
		SelectAction sa
	) 
{
	iHitFlags;

	m_nPendingSelect = -1;
	
	int nPrevSel = GetFocusIndex();


	// Single select, or multi select without control or shift
	if( 
		( GetListCtrlStyle() & LVS_SINGLESEL ) || 
		( (nFlags & (MK_CONTROL|MK_SHIFT)) == 0 ) 
	  )
	{
		// no change
		if (nPrevSel == nHitIndex && IsSelected(nHitIndex))
			return TRUE;

		// if right click on selected item, just move the focus
		if ( ( nFlags & MK_RBUTTON ) && IsSelected( nHitIndex ) )
		{
			m_pToolTipWnd->Hide();
			int nOldAnchor = m_nAnchor;	// don't change the multi-select anchor
			SetFocusItem( nHitIndex );
			m_nAnchor = nOldAnchor;
			return TRUE;
		}

		// Was selection change cancelled? Abort if it was
		m_bAbortPendingSelect = NotifySelChanging(nHitIndex, nPrevSel,sa);
		if(m_bAbortPendingSelect)
			return FALSE;

		m_pToolTipWnd->Hide();

		DeselectAllItems(nHitIndex);
		SelectFocusItem(nHitIndex);
		NotifySelChanged(nHitIndex, nPrevSel,sa);
	}
	// multi-select with shift or control
	else if ( ! (GetListCtrlStyle() & LVS_SINGLESEL) )
	{
		if( ! m_bAbortPendingSelect )
		{
			// Note: must query MK_SHIFT before MK_CONTROL to properly support
			// ctrl-shift multiple selection
			if (nFlags & MK_SHIFT)
			{
				BOOL shifSel = FALSE;

				if( (sa == ByKeyboard) && (nPrevSel == nHitIndex) )
				{//We are pressing VK_SPACE
					shifSel = TRUE;
				}

				if (m_nAnchor != -1 && (nPrevSel != nHitIndex))
				{
					shifSel = TRUE;
				}

				if(shifSel)
				{
					m_bAbortPendingSelect = NotifySelChanging(nHitIndex, nPrevSel,sa);
					if(m_bAbortPendingSelect)
						return FALSE;
					int nOldAnchor = m_nAnchor;
					m_pToolTipWnd->Hide();
					DeselectAllItems();
					SelectItemRange(m_nAnchor, nHitIndex);
					SelectFocusItem(nHitIndex);
					// restore anchor, we don't change it when shift is pressed
					m_nAnchor = nOldAnchor;
					RedrawItems(m_nAnchor, nHitIndex);
					NotifySelChanged(nHitIndex, nPrevSel,sa);
				}
			}
			else if (nFlags & MK_CONTROL)
			{
				BOOL ctrlSel = FALSE;
				if ( sa == ByKeyboard )// no change to selection, just focus
				{
					int nOldAnchor = m_nAnchor;
					m_pToolTipWnd->Hide();
					SetFocusItem( nHitIndex );
					m_nAnchor = nOldAnchor;

					if(nHitIndex == nPrevSel)//We are pressing VK_SPACE, so act accordingly.
					{
						ctrlSel = TRUE;
					}

				}
				else
					ctrlSel = TRUE;

				if (ctrlSel)
				{
					m_pToolTipWnd->Hide();
					if (!m_bLookForBeginDrag)
					{
						m_bAbortPendingSelect = NotifySelChanging(nHitIndex, nPrevSel,sa);
						if(m_bAbortPendingSelect)
							return FALSE;
						SelectFocusItem(nHitIndex, !IsSelected(nHitIndex));
						NotifySelChanged(nHitIndex, nPrevSel,sa);
					}
				}
			}
		}
	}
	return TRUE;
}   


void SEC_LISTCLASS::CopyImageList
	(
		CImageList *  pImlFrom,
		CImageList *& pImlTo
	)
{
	ASSERT_VALID( pImlFrom );
#ifndef UNDER_CE
	//Write out the original images to an archive...
	CMemFile memFile;
	CArchive arOut( &memFile, CArchive::store );
	VERIFY(pImlFrom->Write(&arOut));
	arOut.Flush();
	memFile.SeekToBegin();

	//Read in the images from an archive...
	pImlTo = NULL;
	pImlTo = new CImageList;
	CArchive arIn(&memFile, CArchive::load);
	VERIFY(pImlTo->Read(&arIn));
	ASSERT(pImlFrom->GetImageCount() == pImlTo->GetImageCount());

	arIn.Close();
	arOut.Close();
	memFile.Close();
#else
	HIMAGELIST hNewList = ImageList_Duplicate(pImlFrom->m_hImageList);
	if(hNewList != NULL)
		pImlTo->Attach(hNewList);
#endif //UNDER_CE
}


BOOL SEC_LISTCLASS::CreateMemDC
	(
		CDC *  pScreenDC,
		CDC *& pMemDC,
		const CRect & rectBitmap
	) const
{
	ASSERT_VALID( pScreenDC );
	ASSERT( pMemDC == NULL );

	if( m_bDragging || m_bHdrDragging)//|| IsHeaderCtrlEnabled() )
	{
		pMemDC = pScreenDC;
		return FALSE;
	}

	BOOL bRet = TRUE;
#ifndef UNDER_CE
	try
	{
		pMemDC = new stingray::foundation::SECMemDC( pScreenDC, rectBitmap );
	}
	catch( CResourceException * pResEx )
	{
		pResEx->Delete();
		pMemDC = pScreenDC;
		bRet = FALSE;
	}
#else
	pMemDC = new stingray::foundation::SECMemDC( pScreenDC, rectBitmap );
#endif //UNDER_CE
	
	return bRet;
}


void SEC_LISTCLASS::OnCancelMode( )
{
	m_pToolTipWnd->Hide();	
}

#ifndef UNDER_CE
BOOL SEC_LISTCLASS::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	nFlags; // not used
	pt;		// not used

#if (_MFC_VER < 0x0421) 

	zDelta; // not used

#else

	m_iDeltaAccum += zDelta;

	int iActions = m_iDeltaAccum / WHEEL_DELTA;	// per SDK docs.
	if ( iActions )
	{
		m_pToolTipWnd->Hide();

		if ( m_dwScrollStyle & WS_VSCROLL)
		{
			CScrollBar* pVertSB = GetScrollBarCtrl(SB_VERT);
			if ( pVertSB )
			{
				SCROLLINFO sbi;
				pVertSB->GetScrollInfo(&sbi);
		
				// scroll up
				if ( (iActions > 0) && pVertSB && (sbi.nPos > 0) )
					ScrollVert(-iActions);
				// scroll down
				else if ( (iActions < 0) && pVertSB &&
					((sbi.nPos + (int)sbi.nPage) < (sbi.nMax+1)) ) 
					ScrollVert(-iActions);
			}
		
		}
		else if ( m_dwScrollStyle & WS_HSCROLL )
			ScrollHorz(iActions * 15);

		m_iDeltaAccum = 0;
	}

#endif // _MFC_VER >= 0x0421

	return 1;
}
#endif //UNDER_CE
//@doc SEC_LISTCLASS
//@mfunc Returns the number of visible items.
UINT SEC_LISTCLASS::GetVisibleCount() const
{
	return (UINT)(const_cast<SEC_LISTCLASS*>(this)->ItemsThisPage());
}


void SEC_LISTCLASS::OnSysColorChange( )
{
	if ( m_bTrackSystemColors )
	{
		m_rgbBk =			::GetSysColor(COLOR_WINDOW);
		m_rgbText =			::GetSysColor( COLOR_WINDOWTEXT );
		m_rgbDisabledText = ::GetSysColor( COLOR_GRAYTEXT );
		m_rgbTextBk =		::GetSysColor( COLOR_WINDOW  );
		m_rgbIconBk =		::GetSysColor( COLOR_WINDOW );
		m_rgbSelText =		::GetSysColor( COLOR_HIGHLIGHTTEXT );
		m_rgbSelTextBk =	::GetSysColor( COLOR_HIGHLIGHT );
		m_rgbSelIconBk =	::GetSysColor( COLOR_WINDOW );
		m_rgbToolTipBk =	::GetSysColor( COLOR_INFOBK );
		m_rgbToolTipText =	::GetSysColor( COLOR_INFOTEXT );
		
		// statics
		rgbNoFocusBk =		::GetSysColor(COLOR_MENU);
		rgbNoFocusText =	::GetSysColor(COLOR_MENUTEXT);
		rgbBtnFace =		::GetSysColor(COLOR_BTNFACE);
	}

}


#ifdef _DEBUG
	#undef new
#endif
	

#endif // defined(WIN32)

